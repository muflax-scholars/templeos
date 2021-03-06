Bool MapDrv(U8 drv_let,CPrt *p)
{
  drv_let=ToUpper(drv_let);
  if ('A'<=drv_let<='Z') {
    drv_to_prt[drv_let-'A']=p;
    p->drv_let=drv_let;
    return TRUE;
  } else
    return FALSE;
}

Bool Drv(U8 drv_let)
{
  CPrt *p=Drv2Prt(drv_let);
  CBlkDev *bdev=p->bdev;
  BlkDevChk(bdev);
  if (p!=Fs->cur_prt) {
    if ( bdev->flags & BDF_REMOVABLE &&
       !(bdev->flags & BDF_INITIALIZED) )
      ChgDsk(Prt2Drv(p));
    if (bdev->type==BDT_RAM)
      BlkDevInit(bdev);
  }
  Fs->cur_prt=p;
  Free(Fs->cur_dir);
  Fs->cur_dir=StrNew("/");
  switch (p->type) {
    case PT_REDSEA:
    case PT_FAT32:
    case PT_ISO9660:
      return TRUE;
    default:
      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
      return FALSE;
  }
}

Bool Cd(U8 *dirname,Bool make_dirs=FALSE)
{//Optionally, will create the directories if they don't exist.
  I64 maxlen,cur_dir_cluster=0;
  U8 *chg_to_buf,*new_cur_dir,*buf;
  CPrt *p;
  Bool result=TRUE;
  if (!*dirname) return TRUE;
  if (dirname[1]==':') {
    if (*dirname==':') {
      if (Fs->cur_prt->drv_let!=*sys_acct)
	if (!Drv(*dirname)) return FALSE;
    } else {
      if (Fs->cur_prt!=
	Drv2Prt(*dirname))
	  if (!Drv(*dirname)) return FALSE;
    }
    dirname+=2;
  }
  if (*dirname=='/' || !*dirname || !Fs->cur_dir) {
    Free(Fs->cur_dir);
    Fs->cur_dir=StrNew("/");
    if (*dirname=='/')
      dirname++;
  }
  chg_to_buf=MStrUtil(dirname,
    SUF_REM_LEADING|SUF_REM_TRAILING|SUF_REM_CTRL_CHARS);
  maxlen=StrLen(Fs->cur_dir)+1+StrLen(chg_to_buf)+1;
  new_cur_dir=MAlloc(maxlen);
  buf=MAlloc(maxlen);
  StrCpy(new_cur_dir,Fs->cur_dir);
  while (*chg_to_buf && result) {
    StrFirstRem(chg_to_buf,"/",buf);
    if (!*buf)
      StrCpy(new_cur_dir,"/");
    else if (!StrCmp(buf,"..")) {
      StrLastRem(new_cur_dir,"/");
      if (!*new_cur_dir)
	StrCpy(new_cur_dir,"/");
    } else if (!StrCmp(buf,".")) {
      ;
    } else if (*buf) {
      if (!StrCmp(buf,"HOME")) {
	result=Cd(sys_acct);
	Free(new_cur_dir);
	new_cur_dir=MAlloc(maxlen+StrLen(sys_acct));
	StrCpy(new_cur_dir,sys_acct+2);
      } else {
	p=Fs->cur_prt;
	cur_dir_cluster=Name2DirCluster(p,new_cur_dir);
	switch (p->type) {
	  case PT_REDSEA:
	    result=RedSeaFSCd(buf,cur_dir_cluster);
	    break;
	  case PT_FAT32:
	    result=FAT32Cd(buf,cur_dir_cluster);
	    break;
	  case PT_ISO9660:
	    result=ISO1Cd(buf,cur_dir_cluster);
	    break;
	  default:
	    PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	    result=FALSE;
	}
	if (!result && make_dirs) {
	  Free(Fs->cur_dir);
	  Fs->cur_dir=StrNew(new_cur_dir);
	  result=MkDir(buf);
	}
	if (result) {
	  if (StrCmp(new_cur_dir,"/"))
	    StrCat(new_cur_dir,"/");
	  StrCat(new_cur_dir,buf);
	}
      }
    }
  }
  Free(Fs->cur_dir);
  Fs->cur_dir=StrNew(new_cur_dir);
  Free(buf);
  Free(chg_to_buf);
  Free(new_cur_dir);
  return result;
}

U0 FileAccessDel(CFileAccess *fa)
{
  CBlkDev *bdev;
  if (!fa) return;
  bdev=fa->old_prt->bdev;
  if (!(bdev->flags & BDF_INIT_IN_PROGRESS)) {
    if (fa->old_dir) {
      Drv(Prt2Drv(fa->old_prt));
      Cd(fa->old_dir);
    }
  } else {
    Fs->cur_prt=fa->old_prt;
    Free(Fs->cur_dir);
    Fs->cur_dir=StrNew("/");
  }
  Free(fa->old_dir);
  Free(fa->mask);
  Free(fa);
}

CFileAccess *FileAccessNew(U8 *_mask,Bool make_mask=FALSE,Bool make_dirs=FALSE)
{
  Bool valid=TRUE,old_silent;
  U8 *buf,*mask,*temp_mask;
  CFileAccess *fa=CAlloc(sizeof(CFileAccess));
  mask=MStrUtil(_mask,
    SUF_REM_LEADING|SUF_REM_TRAILING|SUF_REM_CTRL_CHARS);
  temp_mask=mask;
  fa->old_dir=StrNew(Fs->cur_dir);
  fa->old_prt=Fs->cur_prt;
  if (*mask && mask[1]==':') {
    if (Fs->cur_prt!=Drv2Prt(*mask))
      if (!Drv(*mask)) valid=FALSE;
    mask+=2;
  }
  if (StrStr(mask,"HOME") && Fs->cur_prt!=Drv2Prt(*sys_acct))
    Drv(*sys_acct);
  fa->p=Fs->cur_prt;
  PrtChk(fa->p);
  buf=MAlloc(StrLen(mask)+2);
  StrCpy(buf,mask);
  fa->mask=MAlloc(StrLen(mask)+2);
  StrLastRem(buf,"/",fa->mask);
  if (*mask=='/' && !*buf)
    StrCpy(buf,"/");
  //If began with Dir, change to Dir.
  if (*buf && !Cd(buf,make_dirs))
    valid=FALSE;
  if (valid && make_mask) {
    if (!*fa->mask) {
      Free(fa->mask);
      fa->mask=StrNew("*");
    } else {
      if (!make_dirs || FileNameChk(fa->mask)) {
	old_silent=Silent(ON);
	//Try mask to see if Dir. If Dir, change to dir and set to "*".
	if (Cd(fa->mask,make_dirs)) {
	  Free(fa->mask);
	  fa->mask=StrNew("*");
	}
	Silent(old_silent);
      }
    }
  }
  Free(buf);
  Free(temp_mask);
  if (!valid) {
    FileAccessDel(fa);
    fa=NULL;
//    throw(EXCEPT_FILE);
  }
  return fa;
}

CDirEntry *FilesFind(U8 *files_find_mask,I64 fuf_flags=0)
{
/*
Flags:
 $LK,"FUF_RECURSE","MN:FUF_RECURSE"$		+r Recurse
 $LK,"FUF_FLATTEN_TREE","MN:FUF_FLATTEN_TREE"$	+f $BK,1$be sure to use this along with '+F'$BK,0$.
 $LK,"FUF_JUST_DIRS","MN:FUF_JUST_DIRS"$		+D just directories
 $LK,"FUF_JUST_FILES","MN:FUF_JUST_FILES"$		+F just files
 $LK,"FUF_JUST_TXT_FILES","MN:FUF_JUST_TXT_FILES"$	+T just text files: $LK,"FILENAME_MASK_TXT","MN:FILENAME_MASK_TXT"$
 $LK,"FUF_JUST_SRC_FILES","MN:FUF_JUST_SRC_FILES"$	+S just src files : $LK,"FILENAME_MASK_SRC","MN:FILENAME_MASK_SRC"$
See $LK,"ST_FILE_UTIL_FLAGS","MN:ST_FILE_UTIL_FLAGS"$ when used in calling program taking text flags.
*/
  CDirEntry *result;
  CFileAccess *fa;
  if (fuf_flags&~FUG_FILES_FIND)
    throw(EXCEPT_FUF,4);
  if (fa=FileAccessNew(files_find_mask,TRUE)) {
    result=SysFilesFind(fa->mask,fuf_flags);
    FileAccessDel(fa);
  } else
    result=NULL;
  return result;
}

Bool IsDir(U8 *dir_name)
{
  U8 *mask=MSPrintF("%s/*",dir_name);
  Bool result,old_silent=Silent(ON);
  CFileAccess *fa;
  if (fa=FileAccessNew(mask)) {
    FileAccessDel(fa);
    result=TRUE;
  } else
    result=FALSE;
  Free(mask);
  Silent(old_silent);
  return result;
}

U0 Dir(U8 *files_find_mask="*",Bool full=FALSE)
{
  CFileAccess *fa;
  if (fa=FileAccessNew(files_find_mask,TRUE)) {
    Dir2(fa->mask,0,full);
    FileAccessDel(fa);
  }
}


I64 Del(U8 *files_find_mask,Bool print_msg=TRUE)
{
  I64 result=0;
  CFileAccess *fa;
  if (fa=FileAccessNew(files_find_mask)) {
    switch (fa->p->type) {
      case PT_REDSEA:
	result=RedSeaFSFilesDel(fa->p,Fs->cur_dir,fa->mask,0,FALSE,print_msg);
	break;
      case PT_FAT32:
	result=FAT32FilesDel(fa->p,Fs->cur_dir,fa->mask,0,FALSE,print_msg);
	break;
      case PT_ISO9660:
	PutDefineErr("ST_NOT_WRITABLE");
	break;
      default:
	PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
    }
    FileAccessDel(fa);
  }
  return result;
}

U8 *FileRead(U8 *filename,I64 *_size=NULL,I64 *_attr=NULL,Bool raw=FALSE)
{
  CHashGeneric *temph;
  U8 *absname,*altname,*curname,*result=NULL;
  I64 i,size=0,attr=0;
  CFileAccess *fa;
  CArcCompress *ac=NULL;
  if (_attr) *_attr=0;
  absname=FileNameAbs(filename);
  altname=ToggleZorNotZ(absname);
  if (!raw && ((temph=HashFind(absname,adam_task->hash_table,HTT_FILE))||
	       (temph=HashFind(altname,adam_task->hash_table,HTT_FILE)))) {
    if (FileAttr(absname) & _ATTR_COMPRESSED) {
      ac=temph->user_data0;
      if (_size) *_size=ac->expanded_size;
      if (_attr) *_attr=FileAttr(absname,*_attr);
      result=ExpandBuf(ac);
    } else {
      result=MAlloc(temph->user_data1+1);
      MemCpy(result,temph->user_data0,temph->user_data1);
      result[temph->user_data1]=0; //Terminate
      if (_size) *_size=temph->user_data1;
    }
  } else {
    for (i=0;i<2 && !result;i++) {//Try name, then altname
      if (!i)
	curname=absname;
      else
	curname=altname;
      if (fa=FileAccessNew(curname)) {
	switch (fa->p->type) {
	  case PT_REDSEA:
	    result=RedSeaFSFileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	    break;
	  case PT_FAT32:
	    result=FAT32FileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	    break;
	  case PT_ISO9660:
	    result=ISO1FileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	    break;
	  default:
	    PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	}
	FileAccessDel(fa);
      }
    }

    //Search parent directories.
    for (i=0;i<2 && !result;i++) {//Try name, then altname
      if (!i)
	curname=absname;
      else
	curname=altname;
      if (fa=FileAccessNew(curname)) {
	while (!result && StrCmp(Fs->cur_dir,"/")) {
	  Cd("..");
	  switch (Fs->cur_prt->type) {
	    case PT_REDSEA:
	      result=RedSeaFSFileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	      break;
	    case PT_FAT32:
	      result=FAT32FileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	      break;
	    case PT_ISO9660:
	      result=ISO1FileRead(fa->p,Fs->cur_dir,fa->mask,&size,&attr,raw,&ac);
	      break;
	    default:
	      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	  }
	}
	FileAccessDel(fa);
      }
    }
    if (!result) {
      "%s ",filename;
      PutDefineErr("ST_FILE_NOT_FOUND");
    }
    if (_size) *_size=size;
    if (_attr) *_attr=attr;
    if (result && !raw && attr & _ATTR_RESIDENT)
      HashGenericAdd(curname,HTT_FILE,AMAllocIdentical(ac),size,0,adam_task);
    Free(ac);
  }
  Free(absname);
  Free(altname);
  return result;
}

U8 *TextFileRead(U8 *filename,I64 *_attr=NULL)
{
  return FileRead(filename,NULL,_attr);
}


Bool FileFind(U8 *filename,CDirEntry *rde=NULL,I64 fuf_flags=0)
{ //$LK,"FUF_JUST_DIRS","MN:FUF_JUST_DIRS"$, $LK,"FUF_JUST_FILES","MN:FUF_JUST_FILES"$, $LK,"FUF_Z_OR_NOT_Z","MN:FUF_Z_OR_NOT_Z"$, $LK,"FUF_SCAN_PARENTS","MN:FUF_SCAN_PARENTS"$
  //If you pass rde, you must Free(rde->full_name);
  I64 i,j,cur_dir_cluster;
  U8 *altname,*curname,*full_name=NULL;
  CDirEntry de;
  CFileAccess *fa;
  Bool result=FALSE;
  if (fuf_flags&~FUG_FILE_FIND)
    throw(EXCEPT_FUF,5);
  if (!filename) return FALSE;
  altname=ToggleZorNotZ(filename);
  if (fuf_flags&FUF_Z_OR_NOT_Z)
    j=2;
  else
    j=1;
  for (i=0;i<j && !result;i++) {
    if (!i)
      curname=filename;
    else
      curname=altname;
    if (fa=FileAccessNew(curname)) {
      cur_dir_cluster=Name2DirCluster(fa->p,Fs->cur_dir);
      switch (fa->p->type) {
	case PT_REDSEA:
	  result=RedSeaFSFileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	  break;
	case PT_FAT32:
	  result=FAT32FileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	  break;
	case PT_ISO9660:
	  result=ISO1FileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	  break;
	default:
	  PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
      }
      if (result && rde)
	full_name=MSPrintF("%C:%s/%s",Fs->cur_prt->drv_let,Fs->cur_dir,de.name);
      FileAccessDel(fa);
    }
  }
  for (i=0;i<j && !result && fuf_flags&FUF_SCAN_PARENTS;i++) {
    if (!i)
      curname=filename;
    else
      curname=altname;
    if (fa=FileAccessNew(curname)) {
      cur_dir_cluster=Name2DirCluster(fa->p,Fs->cur_dir);
      while (!result && StrCmp(Fs->cur_dir,"/")) {
	Cd("..");
	cur_dir_cluster=Name2DirCluster(fa->p,Fs->cur_dir);
	switch (fa->p->type) {
	   case PT_REDSEA:
	     result=RedSeaFSFileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	     break;
	   case PT_FAT32:
	     result=FAT32FileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	     break;
	   case PT_ISO9660:
	     result=ISO1FileFind(fa->p,cur_dir_cluster,fa->mask,&de,fuf_flags);
	     break;
	   default:
	     PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	}
      }
      if (result && rde)
	full_name=MSPrintF("%C:%s/%s",Fs->cur_prt->drv_let,Fs->cur_dir,de.name);
      FileAccessDel(fa);
    }
  }
  if (rde) {
    if (result) {
      MemCpy(rde,&de,sizeof(CDirEntry));
      rde->full_name=full_name;
    } else
      MemSet(rde,0,sizeof(CDirEntry));
  }
  Free(altname);
  return result;
}

I64 FileSize(U8 *filename,I64 fuf_flags=0)
{ //$LK,"FUF_JUST_DIRS","MN:FUF_JUST_DIRS"$, $LK,"FUF_JUST_FILES","MN:FUF_JUST_FILES"$, $LK,"FUF_Z_OR_NOT_Z","MN:FUF_Z_OR_NOT_Z"$, $LK,"FUF_SCAN_PARENTS","MN:FUF_SCAN_PARENTS"$
  CDirEntry de;
  if (FileFind(filename,&de,fuf_flags)) {
    Free(de.full_name);
    return de.size;
  } else
    return 0;
}

I64 FileSizeExpanded(U8 *filename,I64 fuf_flags=0)
{ //$LK,"FUF_JUST_DIRS","MN:FUF_JUST_DIRS"$, $LK,"FUF_JUST_FILES","MN:FUF_JUST_FILES"$, $LK,"FUF_Z_OR_NOT_Z","MN:FUF_Z_OR_NOT_Z"$, $LK,"FUF_SCAN_PARENTS","MN:FUF_SCAN_PARENTS"$
  U8 buf[BLK_SIZE];
  CPrt *p;
  CDirEntry de;
  if (FileFind(filename,&de,fuf_flags)) {
    p=Drv2Prt(*de.full_name);
    Free(de.full_name);
    if (p->type==PT_REDSEA && de.expanded_size)
      return de.expanded_size;
    else {
      if (de.size && FileAttr(de.name) & _ATTR_COMPRESSED) {
	RBlks(p,buf,Cluster2Blk(p,de.cluster),1);
	return (&buf)(CArcCompress *)->expanded_size;
      } else
	return de.size;
    }
  } else
    return 0;
}

I64 FileWrite(U8 *filename,U8 *fbuf,
   I64 size,CDate cdt=0,I64 attr=0,Bool raw=FALSE)
{
  I64 c=0;
  CHashGeneric *temph;
  CFileAccess *fa;
  CArcCompress *ac=NULL;
  U8 *absname=FileNameAbs(filename);
  if (fa=FileAccessNew(filename,FALSE,TRUE)) {
    switch (fa->p->type) {
      case PT_REDSEA:
	c=RedSeaFSFileWrite(fa->p,Fs->cur_dir,fa->mask,
	  fbuf,size,cdt,attr,raw,&ac);
	break;
      case PT_FAT32:
	c=FAT32FileWrite(fa->p,Fs->cur_dir,fa->mask,
	  fbuf,size,cdt,attr,raw,&ac);
	break;
      case PT_ISO9660:
	PutDefineErr("ST_NOT_WRITABLE");
	break;
      default:
	PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
    }
    if (temph=HashFind(absname,adam_task->hash_table,HTT_FILE)) {
      if (!raw && ac && attr & _ATTR_RESIDENT) {
	Free(temph->user_data0);
	temph->user_data0=AMAllocIdentical(ac);
	temph->user_data1=size;
      } else
	HashRemDel(temph,adam_task->hash_table);
    } else if (!raw && ac && attr & _ATTR_RESIDENT)
      HashGenericAdd(absname,HTT_FILE,AMAllocIdentical(ac),size,0,adam_task);
    Free(ac);
    FileAccessDel(fa);
  }
  Free(absname);
  return c;
}

U0 TextFileWrite(U8 *filename,U8 *st,I64 attr=0)
{
  FileWrite(filename,st,StrLen(st),0,attr);
}
