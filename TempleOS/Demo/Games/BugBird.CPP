AcctRegSetDftEntry("TempleOS/BugBird","F64 best_score=9999;\n");
AcctRegExeBranch("TempleOS/BugBird");


  $SP,"<1>",1$





  $SP,"<2>",2$




  $SP,"<3>",3$





  $SP,"<4>",4$


#define EAT_TIME	0.5

Bool flap_down,flap_up,rst;
F64 t_flap,yy,xx,eat_timeout;
F64 frame_x,percent_completed,t0,tf;

#define MAX_BUGS	32
I32 bugs_x[MAX_BUGS],bugs_y[MAX_BUGS];
Bool bugs_dead[MAX_BUGS];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,y,x,
    h=task->win_pixel_width-10,
    v=task->win_pixel_height-10;
  U8 *inter,*inter2,*inter3;
  F64 tt;
  if (h<1) h=1;
  if (v<1) v=1;
  tt=t_flap*t_flap*t_flap;
  inter=SpriteInterpolate($IB,"<1>",1$,$IB,"<2>",2$,tt);
  if (eat_timeout && tT<eat_timeout) {
    inter2=SpriteInterpolate($IB,"<3>",3$,$IB,"<4>",4$,tt);
    inter3=SpriteInterpolate(inter2,inter,1.0-(eat_timeout-tT)/EAT_TIME);
    Free(inter);
    Free(inter2);
    inter=inter3;
  }
  SpritePlot3(dc,xx,yy,0,inter);
  Free(inter);
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+5;
      y=bugs_y[i]%v+5;
      dc->color=BLACK;
      GrPlot(dc,x,y);
      GrPlot(dc,x+1,y);
      dc->color=RED;
      GrPlot(dc,x,y-1);
    }
  if (tf) {
    dc->color=RED;
    GrPutS(dc,(task->win_pixel_width-FONT_WIDTH*14)/2,
		(task->win_pixel_height-FONT_HEIGHT)/2,"Game Completed");
    tt=tf;
  } else {
    dc->color=GREEN;
    tt=tT;
  }
  GrPrintF(dc,0,0,"Bugs:%3.1f%% Time:%3.2f Best:%3.2f",
    percent_completed,tt-t0,best_score);
}

U0 CheckBugs(CTask *task,CTask *song_task)
{
  I64 i,j=0,x,y,
    h=task->win_pixel_width-10,
    v=task->win_pixel_height-10;
  if (h<1) h=1;
  if (v<1) v=1;

  if (eat_timeout && eat_timeout-tT<0.75*EAT_TIME) {
    Suspend(song_task,FALSE);
    if (tT>=eat_timeout)
      eat_timeout=0;
  }
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+5;
      y=bugs_y[i]%v+5;
      if (Abs(x-xx)<10 && Abs(y-yy)<10) {
	bugs_dead[i]=TRUE;
	eat_timeout=tT+EAT_TIME;
	Snd(1000);
	Suspend(song_task);
	j++;
      }
    } else
      j++;
  if (!tf && j==MAX_BUGS) {
    tf=tT;
    Suspend(song_task);
    Snd(0);
    if (tf-t0<best_score)
      best_score=tf-t0;
  }
  percent_completed=j*100.0/MAX_BUGS;
  frame_x-=0.1;
  if (frame_x<0)
    frame_x+=h;
}

U0 Init(CTask *song_task)
{
  I64 i;
  flap_down=FALSE;
  flap_up=TRUE;
  t_flap=0;
  yy=0;
  xx=100;
  frame_x=0;
  for (i=0;i<MAX_BUGS;i++) {
    bugs_dead[i]=FALSE;
    bugs_x[i]=RandU16;
    bugs_y[i]=RandU16;
  }
  Suspend(song_task,FALSE);
  eat_timeout=0;
  tf=0;
  t0=tT;
}


U0 SongTask(I64)
{ //Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3eBEBCBEsEFqEeBEBCBEsEF");
    Play("qEeADABADsDCqDeBEBCB");
    Play("EsEDqE");
  }
}


U0 AnimateTask(I64)
{
  F64 tt=0;
  while (TRUE) {
    if (rst) {
      Init(Fs->parent_task->song_task);
      rst=FALSE;
    }
    if (flap_down) {
      flap_down=FALSE;
      tt=-0.005;
    } else if (flap_up) {
      flap_up=FALSE;
      tt= 0.005;
    }
    if (tt<0) {
      yy+=75*tt;
      tt+=0.000015;
    } else
      yy+=0.15;
    yy=Limit(yy,10,Fs->parent_task->win_pixel_height-10);
    t_flap=Limit(t_flap+tt,0.0,1.0);
    CheckBugs(Fs->parent_task,Fs->parent_task->song_task);
    Sleep(1);
  }
}

U0 BugBird()
{
  Bool rst_space=TRUE;
  I64 p1,p2;

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,'\n');"
  "  Flap(,CH_SPACE);"
      "}"
      );

  PopUpOk("Use $$FG,GREEN$$<SPACE BAR>$$FG$$ to flap.\nHold down to glide.");

  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  WinBorder(ON);
  SetWinHorz(1,TEXT_COLS/2);
  SetWinVert(2,TEXT_ROWS/2);
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  rst=TRUE;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  while (rst)
    Yield;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (GetMsg(&p1,&p2,1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case CH_SPACE:
	      if (rst_space) {
		flap_down=TRUE;
		rst_space=FALSE;
	      }
	      break;
	    case '\n':
	      rst=TRUE;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto bl_done;
	  }
	  break;
	case MSG_KEY_UP:
	  switch (p1) {
	    case CH_SPACE:
	      flap_up=TRUE;
	      rst_space=TRUE;
	      break;
	  }
	  break;
      }
    }
bl_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
  AcctRegWriteBranch("TempleOS/BugBird","F64 best_score=%5.4f;\n",best_score);
}

BugBird;
           e        ����    ����   ����   ����������������   ����   ����
   ����
   ����                                 ����            
      
      ����   ����   ������������   ����   ����   ����   ����   ����   ������������������������������������������������                    ����               ����           e        ����   ����   ����   ����    ����       ����   ����                                          ����      ����   ����            ����   ����   ����   ����   ������������    ����������������������������   ����   ����   ����   ����   ����   ����   ����                                         e        ����    ����   ����   ����������������   ����   ����   ����   ����   ����   ����   ����   ����         ����            
      
      ����   ����   ������������   ����   ����   ����   ����   ����   ������������������������������������������������                    ����   ����   ����   ����           e        ����   ����   ����   ����    ����       ����   ����            
   ����
   ����                   ����      ����   ����            ����   ����   ����   ����   ������������    ����������������������������   ����   ����   ����   ����   ����   ����   ����            
       
             