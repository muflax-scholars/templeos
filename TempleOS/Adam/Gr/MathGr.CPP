#help_index "Graphics/Math"

public I64 gr_x_offsets[8]={-1, 0, 1,-1,1,-1,0,1},
	   gr_y_offsets[8]={-1,-1,-1, 0,0, 1,1,1},
	  gr_x_offsets2[4]={ 0,-1, 1, 0},
	  gr_y_offsets2[4]={-1, 0, 0, 1};

public I64 Line(U8 *aux_data,
		 I64 x1,I64 y1,I64 z1,
		 I64 x2,I64 y2,I64 z2,
		 Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),
		 I64 step=1,I64 start=0)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 i,j,d,dx=x2-x1,dy=y2-y1,dz=z2-z1,_x,_y,_z,
      adx=AbsI64(dx),ady=AbsI64(dy),adz=AbsI64(dz),result=0;
  Bool first=TRUE;
  if (adx>=ady) {
    if (adx>=adz) {
      if (d=adx) {
	if (dx>=0)
	  dx=0x100000000;
	else
	  dx=-0x100000000;
	dy=dy<<32/d;
	dz=dz<<32/d;
      }
    } else {
      if (d=adz) {
	dx=dx<<32/d;
	dy=dy<<32/d;
	if (dz>=0)
	  dz=0x100000000;
	else
	  dz=-0x100000000;
      }
    }
  } else {
    if (ady>=adz) {
      if (d=ady) {
	dx=dx<<32/d;
	if (dy>=0)
	  dy=0x100000000;
	else
	  dy=-0x100000000;
	dz=dz<<32/d;
      }
    } else {
      if (d=adz) {
	dx=dx<<32/d;
	dy=dy<<32/d;
	if (dz>=0)
	  dz=0x100000000;
	else
	  dz=-0x100000000;
      }
    }
  }
  x1<<=32; y1<<=32; z1<<=32;
  for (j=0;j<start;j++) {
      x1+=dx; y1+=dy; z1+=dz;
  }
  if (step!=1 && step!=0) {
    dx*=step;
    dy*=step;
    dz*=step;
    d/=step;
  }
  for (i=start;i<=d;i++) {
    if (_x!=x1.i32[1] || _y!=y1.i32[1] || _z!=z1.i32[1] || first) {
      if ((*fp_plot)(aux_data,x1.i32[1],y1.i32[1],z1.i32[1]))
	result++;
    }
    first=FALSE;
    _x=x1.i32[1]; _y=y1.i32[1]; _z=z1.i32[1];
    x1+=dx; y1+=dy; z1+=dz;
  }
  if (step==1 && (_x!=x2||_y!=y2||_z!=z2)) {
    if ((*fp_plot)(aux_data,x2,y2,z2))
      result++;
  }
  return result;
}

public Bool LOSLine(U8 *aux_data,
		 I64 x1,I64 y1,I64 z1,
		 I64 x2,I64 y2,I64 z2,
		 Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),
		 I64 step=1,I64 start=0)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 i,j,d,dx=x2-x1,dy=y2-y1,dz=z2-z1,_x,_y,_z,
      adx=AbsI64(dx),ady=AbsI64(dy),adz=AbsI64(dz);
  Bool first=TRUE;
  if (adx>=ady) {
    if (adx>=adz) {
      if (d=adx) {
	if (dx>=0)
	  dx=0x100000000;
	else
	  dx=-0x100000000;
	dy=dy<<32/d;
	dz=dz<<32/d;
      }
    } else {
      if (d=adz) {
	dx=dx<<32/d;
	dy=dy<<32/d;
	if (dz>=0)
	  dz=0x100000000;
	else
	  dz=-0x100000000;
      }
    }
  } else {
    if (ady>=adz) {
      if (d=ady) {
	dx=dx<<32/d;
	if (dy>=0)
	  dy=0x100000000;
	else
	  dy=-0x100000000;
	dz=dz<<32/d;
      }
    } else {
      if (d=adz) {
	dx=dx<<32/d;
	dy=dy<<32/d;
	if (dz>=0)
	  dz=0x100000000;
	else
	  dz=-0x100000000;
      }
    }
  }
  x1<<=32; y1<<=32; z1<<=32;
  for (j=0;j<start;j++) {
      x1+=dx; y1+=dy; z1+=dz;
  }
  if (step!=1 && step!=0) {
    dx*=step;
    dy*=step;
    dz*=step;
    d/=step;
  }
  for (i=start;i<=d;i++) {
    if (_x!=x1.i32[1] || _y!=y1.i32[1] || _z!=z1.i32[1] || first) {
      if (!(*fp_plot)(aux_data,x1.i32[1],y1.i32[1],z1.i32[1]))
	return FALSE;
    }
    first=FALSE;
    _x=x1.i32[1]; _y=y1.i32[1]; _z=z1.i32[1];
    x1+=dx; y1+=dy; z1+=dz;
  }
  if (step==1 && (_x!=x2||_y!=y2||_z!=z2)) {
    if (!(*fp_plot)(aux_data,x2,y2,z2))
      return FALSE;
  }
  return TRUE;
}

public I64 Rect(U8 *aux_data,
		I64 x1,I64 y1,I64 z1,
		I64 x2,I64 y2,I64 z2,
		I64 x3,I64 y3,I64 z3,
		Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z))
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 i,j,result=0,
     d1,dx1=x2-x1,dy1=y2-y1,dz1=z2-z1,
     d2,dx2=x3-x1,dy2=y3-y1,dz2=z3-z1,
     adx1=AbsI64(dx1),ady1=AbsI64(dy1),adz1=AbsI64(dz1),
     adx2=AbsI64(dx2),ady2=AbsI64(dy2),adz2=AbsI64(dz2),
     last_x,last_y,last_z;
  Bool first=TRUE;

  if (adx1>=ady1) {
    if (adx1>=adz1)
      d1=adx1;
    else
      d1=adz1;
  } else {
    if (ady1>=adz1)
      d1=ady1;
    else
      d1=adz1;
  }
  if (adx2>=ady2) {
    if (adx2>=adz2)
      d2=adx2;
    else
      d2=adz2;
  } else {
    if (ady2>=adz2)
      d2=ady2;
    else
      d2=adz2;
  }
  d1<<=1;
  d2<<=1;
  if (d1) {
    dx1=dx1<<32/d1;
    dy1=dy1<<32/d1;
    dz1=dz1<<32/d1;
  }

  if (d2) {
    dx2=dx2<<32/d2;
    dy2=dy2<<32/d2;
    dz2=dz2<<32/d2;
  }

  x1<<=32; y1<<=32; z1<<=32;
  for (j=0;j<=d1;j++) {
    x2=x1; y2=y1; z2=z1;
    for (i=0;i<=d2;i++) {
      if ((x2.i32[1]!=last_x || y2.i32[1]!=last_y || z2.i32[1]!=last_z ||first) &&
	  (*fp_plot)(aux_data,x2.i32[1],y2.i32[1],z2.i32[1]))
	result++;
      first=FALSE;
      last_x=x2.i32[1]; last_y=y2.i32[1]; last_z=z2.i32[1];
      x2+=dx2; y2+=dy2; z2+=dz2;
    }
    x1+=dx1; y1+=dy1; z1+=dz1;
  }
  return result;
}

#help_index "Graphics/Math/3D Transformation"
#help_file "::/TempleOS/Doc/Transform"
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
I64 gr_scale=MAX_U32+1;

public U0 GrRot(I64 r[4][4],I64 *_x,I64 *_y,I64 *_z)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 x1,y1,z1,xx=*_x,yy=*_y,zz=*_z;
  x1=(r[0][0]*xx+r[0][1]*yy+r[0][2]*zz+r[0][3])>>32;
  y1=(r[1][0]*xx+r[1][1]*yy+r[1][2]*zz+r[1][3])>>32;
  z1=(r[2][0]*xx+r[2][1]*yy+r[2][2]*zz+r[2][3])>>32;
  *_x=x1;*_y=y1;*_z=z1;
}

public U0 GrTransform(CDC *dc,I64 *_x,I64 *_y,I64 *_z)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  GrRot(dc->r,_x,_y,_z);
  *_x+=dc->x;
  *_y+=dc->y;
  *_z+=dc->z;
}

public I64 *GrRotX(F64 �,CTask *mem_task=NULL)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 *r=CAlloc(sizeof(I64)*16,mem_task);

  r[5]=my_cos;
  r[10]=my_cos;
  r[9]=my_sin;
  r[6]=-my_sin;
  r[0]=gr_scale;
  r[15]=gr_scale;
  return r;
}

public I64 *GrRotY(F64 �,CTask *mem_task=NULL)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 *r=CAlloc(sizeof(I64)*16,mem_task);

  r[0]=my_cos;
  r[10]=my_cos;
  r[8]=-my_sin;
  r[2]=my_sin;
  r[5]=gr_scale;
  r[15]=gr_scale;
  return r;
}

public I64 *GrRotZ(F64 �,CTask *mem_task=NULL)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 *r=CAlloc(sizeof(I64)*16,mem_task);

  r[0]=my_cos;
  r[5]=my_cos;
  r[4]=my_sin;
  r[1]=-my_sin;
  r[10]=gr_scale;
  r[15]=gr_scale;
  return r;
}

public I64 *GrScaleMat(I64 *m1,F64 s,CTask *mem_task=NULL)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 i,*r=CAlloc(sizeof(I64)*16,mem_task);
  for (i=0;i<16;i++)
    r[i]=m1[i]*s;
  return r;
}

public I64 *GrMulMat(I64 *m1,I64 *m2,CTask *mem_task=NULL)
{
  //This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
 //Conceptually, the transform m1 is applied after m2
  I64 i,j,k,*r=CAlloc(sizeof(I64)*16,mem_task);
  F64 d;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      for (k=0;k<4;k++) {
	d=m1[k+4*j];
	d*=m2[i+4*k]; //TODO!  Possible overflow if norm > one.
	d/=gr_scale;
	r[i+4*j]+=d;
      }
    }
  }
  return r;
}

public I64 *GrRotXEqu(I64 *m1,F64 �)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 r[16],*r2;
  MemSet(r,0,sizeof(r));
  r[5]=my_cos;
  r[10]=my_cos;
  r[9]=my_sin;
  r[6]=-my_sin;
  r[0]=gr_scale;
  r[15]=gr_scale;

  r2=GrMulMat(r,m1);
  MemCpy(m1,r2,sizeof(I64)*16);
  Free(r2);
  return m1;
}

public I64 *GrRotYEqu(I64 *m1,F64 �)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 r[16],*r2;
  MemSet(r,0,sizeof(r));
  r[0]=my_cos;
  r[10]=my_cos;
  r[8]=-my_sin;
  r[2]=my_sin;
  r[5]=gr_scale;
  r[15]=gr_scale;

  r2=GrMulMat(r,m1);
  MemCpy(m1,r2,sizeof(I64)*16);
  Free(r2);
  return m1;
}

public I64 *GrRotZEqu(I64 *m1,F64 �)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  F64 my_cos=Cos(�)*gr_scale,
     my_sin=Sin(�)*gr_scale;
  I64 r[16],*r2;
  MemSet(r,0,sizeof(r));
  r[0]=my_cos;
  r[5]=my_cos;
  r[4]=my_sin;
  r[1]=-my_sin;
  r[10]=gr_scale;
  r[15]=gr_scale;

  r2=GrMulMat(r,m1);
  MemCpy(m1,r2,sizeof(I64)*16);
  Free(r2);
  return m1;
}

public I64 *GrScaleMatEqu(I64 *m1,F64 s)
{
  I64 i;
  for (i=0;i<16;i++)
    m1[i]*=s;
  return m1;
}

public U0 GrSetRotMat(CDC *dc,I64 r[4][4])
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
// (1.0/Sqrt(3))*65536=37837.22
  I64 xx=37838,yy=37837,zz=37837;
  dc->r=r;
  GrRot(r,&xx,&yy,&zz);
  //scaled 32 bits
  dc->r_norm=Sqrt(xx*xx+yy*yy+zz*zz)*65536;
}

public U0 GrScalePenWidth(CDC *dc)
{
  I64 d;
  if (dc->flags&DCF_TRANSFORMATION) {
    if (dc->pen_width) {
      d=dc->pen_width*dc->r_norm+0x80000000;
      dc->pen_width=d.u32[1];
      if (dc->pen_width<1)
	dc->pen_width=1;
    }
  }
}

public I64 *GrIdent(CTask *mem_task=NULL)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 *r=CAlloc(sizeof(I64)*16,mem_task);
  r[0] .i32[1]=1;
  r[5] .i32[1]=1;
  r[10].i32[1]=1;
  r[15].i32[1]=1;
  return r;
}

public I64 *GrIdentEqu(I64 r[4][4])
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  MemSet(r,0,sizeof(I64)*16);
  r[0][0].i32[1]=1;
  r[1][1].i32[1]=1;
  r[2][2].i32[1]=1;
  r[3][3].i32[1]=1;
  return r;
}

public I64 *GrSetTranslation(I64 r[4][4],I64 x,I64 y,I64 z)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  r[0][3]=x<<32;
  r[1][3]=y<<32;
  r[2][3]=z<<32;
  r[3][3]=gr_scale;
  return r;
}

public I64 *GrAddTranslation(I64 r[4][4],I64 x,I64 y,I64 z)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  r[0][3]+=x<<32;
  r[1][3]+=y<<32;
  r[2][3]+=z<<32;
  r[3][3]=gr_scale;
  return r;
}

public Bool GrSetSymmetry(CDC *dc,I64 x1,I64 y1,I64 x2,I64 y2)
{
  F64 d;
  if (y1==y2 && x1==x2)
    return FALSE;
  dc->sym.snx=y2-y1;
  dc->sym.sny=x1-x2;
  dc->sym.snz=0;
  if (d=Sqrt(SqrI64(dc->sym.snx)+
	     SqrI64(dc->sym.sny)+
	     SqrI64(dc->sym.snz))) {
    d=gr_scale/d;
    dc->sym.snx *= d;
    dc->sym.sny *= d;
    dc->sym.snz *= d;
  }
  dc->sym.sx=x1;
  dc->sym.sy=y1;
  dc->sym.sz=0;
  return TRUE;
}

public Bool GrSetSymmetry3(CDC *dc,I64 x1,I64 y1,I64 z1,I64 x2,I64 y2,I64 z2,I64 x3,I64 y3,I64 z3)
{
  F64 d,x,y,z,xx,yy,zz;
  I64 xx1,yy1,zz1,xx2,yy2,zz2,*r;
  xx1=x1-x2; yy1=y1-y2; zz1=z1-z2;
  xx2=x3-x2; yy2=y3-y2; zz2=z3-z2;
  if (!xx1 && !yy1 && !zz1 ||
      !xx2 && !yy2 && !zz2 ||
      xx1==xx2 && yy1==yy2 && zz1==zz2)
    return FALSE;

  x=yy1*zz2-zz1*yy2;
  y=-xx1*zz2+zz1*xx2;
  z=xx1*yy2-yy1*xx2;
  if (dc->flags & DCF_TRANSFORMATION) {
    r=dc->r;
    xx=x*r[0]+y*r[1]+z*r[2];
    yy=x*r[4]+y*r[5]+z*r[6];
    zz=x*r[8]+y*r[9]+z*r[10];
    x=xx; y=yy; z=zz;
  }
  if (d=Sqrt(Sqr(x)+Sqr(y)+Sqr(z))) {
    d=gr_scale/d;
    dc->sym.snx = d*x;
    dc->sym.sny = d*y;
    dc->sym.snz = d*z;
  }
  if (dc->flags & DCF_TRANSFORMATION)
    (*dc->transform)(dc,&x1,&y1,&z1);
  dc->sym.sx=x1;
  dc->sym.sy=y1;
  dc->sym.sz=z1;
  return TRUE;
}

public U0 GrReflect(CDC *dc,I64 *_x,I64 *_y,I64 *_z)
{
//This uses $LK,"fixed-point","FI:::/TempleOS/Demo/Lectures/FixedPoint.CPP"$.
  I64 x=*_x<<32,y=*_y<<32,z=*_z<<32,
    xx=*_x-dc->sym.sx,yy=*_y-dc->sym.sy,zz=*_z-dc->sym.sz,
    d=(xx*dc->sym.snx+yy*dc->sym.sny+zz*dc->sym.snz)>>16,
    xn,yn,zn,xx2,yy2,zz2;
  xn=d*dc->sym.snx>>15;
  yn=d*dc->sym.sny>>15;
  zn=d*dc->sym.snz>>15;
  xx=x-xn;
  yy=y-yn;
  zz=z-zn;
  xx2=x+xn;
  yy2=y+yn;
  zz2=z+zn;
  if (SqrI64(xx>>16 -dc->sym.sx<<16)+
      SqrI64(yy>>16 -dc->sym.sy<<16)+
      SqrI64(zz>>16 -dc->sym.sz<<16)<
      SqrI64(xx2>>16-dc->sym.sx<<16)+
      SqrI64(yy2>>16-dc->sym.sy<<16)+
      SqrI64(zz2>>16-dc->sym.sz<<16)) {
    *_x=xx.i32[1]; *_y=yy.i32[1]; *_z=zz.i32[1];
  } else {
    *_x=xx2.i32[1]; *_y=yy2.i32[1]; *_z=zz2.i32[1];
  }
}

#help_index "Graphics/Mesh"
public U0 GrLighting(CDC *dc,CD3I32 *p1,CD3I32 *p2,CD3I32 *p3,CColorROPU32 color)
{
  CD3I32 v1,v2;
  I64 i,vn_x,vn_y,vn_z;
  F64 d;

  v1.x=p1->x-p2->x;
  v1.y=p1->y-p2->y;
  v1.z=p1->z-p2->z;

  v2.x=p3->x-p2->x;
  v2.y=p3->y-p2->y;
  v2.z=p3->z-p2->z;

//V1 and V2 are vects along two sides
//of the tri joined at p2.

  vn_x=v1.y*v2.z-v1.z*v2.y;
  vn_y=v1.z*v2.x-v1.x*v2.z;
  vn_z=v1.x*v2.y-v1.y*v2.x;
  if (d=Sqrt(SqrI64(vn_x)+SqrI64(vn_y)+SqrI64(vn_z)))
    d=1<<16/d;
  vn_x*=d;
  vn_y*=d;
  vn_z*=d;
//Vn is the cross product of V1 and V3
//which means it is perpendicular.It
//is the normal vect to the surface.
//It has been scaled to length 65536.

//Light source has been scaled to length 65536.
  i=(vn_x*dc->ls.x+vn_y*dc->ls.y+vn_z*dc->ls.z)>>16;
//The dot product of the light source
//vect and the surface normal
//gives an illumination number.
//65536*65536>>16=65536

//TempleOS will generate a random U16
//and compare to dither_probability and
//will pick from two colors.
//Probability dithering does not work with pen_width>1 at this time.
  if (color.c0.rop&ROPBF_TWO_SIDED) {
    color.c0.rop&=~ROPBF_TWO_SIDED;
    i=AbsI64(i)<<1;
  } else
    i+=65536;
  if (color.c0.rop&ROPBF_HALF_RANGE_COLOR) {
    color.c0.rop&=~ROPBF_HALF_RANGE_COLOR;
    i>>=1;
    if (color>=8) {
      color-=8;
      i+=65536;
    }
  }
  if (i<65536) {
    dc->color=ROPF_PROBABILITY_DITHER+color<<16+BLACK;
    dc->dither_probability=i;
  } else {
    dc->color=ROPF_PROBABILITY_DITHER+(color^8)<<16+color;
    dc->dither_probability=i-65536;
  }
}

#define SCALE1	24
#define SCALE2	8

#help_index "Graphics/Math"
public I64 Circle(U8 *aux_data,
		I64 cx,I64 cy,I64 cz,
		I64 radius,
		Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),
		I64 step=1,
		F64 start_radians=0,
		F64 len_radians=2*�)
{
  I64 i,j,len=Ceil(len_radians*radius),
      x,y,x1,y1,s1,s2,c,result=0;
  F64 t;
  if (radius<=0||!step) return 0;
  t=1.0/radius;
  c=1<<SCALE1*Cos(t);
  if (step<0) {
    step=-step;
    s2=1<<SCALE1*Sin(t);
    s1=-s2;
  } else {
    s1=1<<SCALE1*Sin(t);
    s2=-s1;
  }
  if (start_radians) {
    x=radius*Cos(start_radians);
    y=-radius*Sin(start_radians);
  } else {
    x=radius;
    y=0;
  }
  x<<=SCALE2;
  y<<=SCALE2;
  for (i=0;i<=len;i+=step) {
    if ((*fp_plot)(aux_data,cx+x>>SCALE2,cy+y>>SCALE2,cz))
      result++;
    for (j=0;j<step;j++) {
      x1=(c*x+s1*y)>>SCALE1;
      y1=(s2*x+c*y)>>SCALE1;
      x=x1; y=y1;
    }
  }
  return result;
}

public I64 Ellipse(U8 *aux_data,
		I64 cx,I64 cy,I64 cz,
		I64 x_radius,I64 y_radius,
		Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),
		F64 rot_angle=0,
		I64 step=1,
		F64 start_radians=0,
		F64 len_radians=2*�)
{
  I64 i,j,len,
      x,y,_x,_y,x1,y1,x2,y2, s1,s2,c, s12,s22,c2,result;
  F64 t;
  Bool first=TRUE;
  if (x_radius<=0 || y_radius<=0 || !step)
    return 0;
  result=0;
  if (x_radius>=y_radius) {
    t=1.0/x_radius;
    len=Ceil(len_radians*x_radius);
  } else {
    t=1.0/y_radius;
    len=Ceil(len_radians*y_radius);
  }

  c=1<<SCALE1*Cos(t);
  if (step<0) {
    step=-step;
    s2=1<<SCALE1*Sin(t);
    s1=-s2;
  } else {
    s1=1<<SCALE1*Sin(t);
    s2=-s1;
  }

  c2=1<<SCALE1*Cos(rot_angle);
  s12=1<<SCALE1*Sin(rot_angle);
  s22=-s12;

  if (start_radians) {
    x=x_radius*Cos(start_radians);
    y=-x_radius*Sin(start_radians);
  } else {
    x=x_radius;
    y=0;
  }
  x<<=SCALE2;
  y<<=SCALE2;
  x2=x;
  y2=y;

  y1=y2*y_radius/x_radius;
  x=(c2*x2+s12*y1)>>SCALE1;
  y=(s22*x2+c2*y1)>>SCALE1;

  for (i=0;i<=len;i+=step) {
    if (x>>SCALE2!=_x || y>>SCALE2!=_y || first) {
      if ((*fp_plot)(aux_data,cx+x>>SCALE2,cy+y>>SCALE2,cz))
	result++;
    }
    _x=x>>SCALE2; _y=y>>SCALE2; first=FALSE;
    for (j=0;j<step;j++) {
      x1=(c*x2+s1*y2)>>SCALE1;
      y1=(s2*x2+c*y2)>>SCALE1;
      x2=x1;
      y2=y1;
      y1=y1*y_radius/x_radius;
      x=(c2*x1+s12*y1)>>SCALE1;
      y=(s22*x1+c2*y1)>>SCALE1;
    }
  }
  return result;
}

public I64 RegPoly(U8 *aux_data,
		I64 cx,I64 cy,I64 cz,
		I64 x_radius,I64 y_radius,I64 sides,
		Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),
		F64 rot_angle=0,
		I64 step=1,
		F64 start_radians=0,
		F64 len_radians=2*�)
{
  I64 i,n,x,y,x1,y1,x2,y2,
    xx1,yy1,xx2,yy2,
    s1,s2,c, s12,s22,c2,result;
  F64 angle_step;

  if (sides<=0 || x_radius<=0 || y_radius<=0)
    return 0;

  result=0;

  angle_step=2*�/sides;
  n=len_radians*sides/(2*�);

  s1=1<<SCALE1*Sin(angle_step);
  s2=-s1;
  c=1<<SCALE1*Cos(angle_step);

  s12=1<<SCALE1*Sin(rot_angle);
  s22=-s12;
  c2=1<<SCALE1*Cos(rot_angle);

  if (start_radians) {
    x=x_radius*Cos(start_radians);
    y=-x_radius*Sin(start_radians);
  } else {
    x=x_radius;
    y=0;
  }
  x<<=SCALE2;
  y<<=SCALE2;
  x2=x;
  y2=y;

  y1=y2*y_radius/x_radius;
  x=(c2*x2+s12*y1)>>SCALE1;
  y=(s22*x2+c2*y1)>>SCALE1;

  xx1=cx+x>>SCALE2;
  yy1=cy+y>>SCALE2;
  for (i=0;i<n;i++) {
    x1=(c*x2+s1*y2)>>SCALE1;
    y1=(s2*x2+c*y2)>>SCALE1;
    x2=x1;
    y2=y1;
    y1=y1*y_radius/x_radius;
    x=(c2*x1+s12*y1)>>SCALE1;
    y=(s22*x1+c2*y1)>>SCALE1;
    xx2=cx+x>>SCALE2;
    yy2=cy+y>>SCALE2;
    result+=Line(aux_data,xx1,yy1,cz,xx2,yy2,cz,fp_plot,step);
    xx1=xx2; yy1=yy2;
  }
  return result;
}

#help_index "Graphics/CD3I32;Math/CD3I32"
public F64 P3I32Dist(CD3I32 *p1,CD3I32 *p2)
{
  return Sqrt(SqrI64(p1->x-p2->x)+SqrI64(p1->y-p2->y)+SqrI64(p1->z-p2->z));
}

public F64 P3I32Norm(CD3I32 *p)
{
  return Sqrt(SqrI64(p->x)+SqrI64(p->y)+SqrI64(p->z));
}

public I64 P3I32SqrNorm(CD3I32 *p)
{
  return SqrI64(p->x)+SqrI64(p->y)+SqrI64(p->z);
}

#help_index "Graphics/Math"
public I64 Bezier2(U8 *aux_data,CD3I32 *ctrl,Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),Bool first=TRUE)
{//2nd order
  I64 x,y,z,xx,yy,zz,dx,dy,dz,d_max,result=0;
  F64 x0=ctrl[0].x,y0=ctrl[0].y,z0=ctrl[0].z,
      x1=ctrl[1].x-x0,y1=ctrl[1].y-y0,z1=ctrl[1].z-z0,
      x2=ctrl[2].x-x0,y2=ctrl[2].y-y0,z2=ctrl[2].z-z0,
      t,d=P3I32Dist(&ctrl[0],&ctrl[1])+
	     P3I32Dist(&ctrl[1],&ctrl[2])+
	     P3I32Dist(&ctrl[2],&ctrl[0]),
	 s=0.5/d,t1,t2;
  xx=x0; yy=y0; zz=z0;
  if (first && (*fp_plot)(aux_data,xx,yy,zz))
    result++;
  for (t=0.0;t<=1.0;t+=s) {
    t1=t*(1.0-t);
    t2=t*t;
    x=x0+x1*t1+x2*t2;
    y=y0+y1*t1+y2*t2;
    z=z0+z1*t1+z2*t2;
    dx=AbsI64(x-xx);
    dy=AbsI64(y-yy);
    dz=AbsI64(z-zz);
    if (dx>dy)
      d_max=dx;
    else
      d_max=dy;
    if (dz>d_max)
      d_max=dz;
    if (!d_max)
      s*=1.1;
    else {
      s*=0.9;
      if ((*fp_plot)(aux_data,x,y,z))
	result++;
      xx=x;yy=y;zz=z;
    }
  }
  x=ctrl[2].x; y=ctrl[2].y; z=ctrl[2].z;
  if ((xx!=x || yy!=y || zz!=z) &&
      (*fp_plot)(aux_data,x,y,z))
    result++;
  return result;
}

public I64 Bezier3(U8 *aux_data,CD3I32 *ctrl,Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),Bool first=TRUE)
{ //3rd order
  I64 x,y,z,xx,yy,zz,dx,dy,dz,d_max,result=0;
  F64 x0=ctrl[0].x,y0=ctrl[0].y,z0=ctrl[0].z,
      x1=ctrl[1].x-x0,y1=ctrl[1].y-y0,z1=ctrl[1].z-z0,
      x2=ctrl[2].x-x0,y2=ctrl[2].y-y0,z2=ctrl[2].z-z0,
      x3=ctrl[3].x-x0,y3=ctrl[3].y-y0,z3=ctrl[3].z-z0,
      t,d=P3I32Dist(&ctrl[0],&ctrl[1])+
	     P3I32Dist(&ctrl[1],&ctrl[2])+
	     P3I32Dist(&ctrl[2],&ctrl[3])+
	     P3I32Dist(&ctrl[3],&ctrl[0]),
	 s=0.5/d,nt,t1,t2,t3;
  xx=x0; yy=y0; zz=z0;
  if (first && (*fp_plot)(aux_data,xx,yy,zz))
    result++;
  for (t=0.0;t<=1.0;t+=s) {
    nt=1.0-t;
    t1=t*nt*nt;
    t2=t*t*nt;
    t3=t*t*t;
    x=x0+x1*t1+x2*t2+x3*t3;
    y=y0+y1*t1+y2*t2+y3*t3;
    z=z0+z1*t1+z2*t2+z3*t3;
    dx=AbsI64(x-xx);
    dy=AbsI64(y-yy);
    dz=AbsI64(z-zz);
    if (dx>dy)
      d_max=dx;
    else
      d_max=dy;
    if (dz>d_max)
      d_max=dz;
    if (!d_max)
      s*=1.1;
    else {
      s*=0.9;
      if ((*fp_plot)(aux_data,x,y,z))
	result++;
      xx=x;yy=y;zz=z;
    }
  }
  x=ctrl[3].x; y=ctrl[3].y; z=ctrl[3].z;
  if ((xx!=x || yy!=y || zz!=z) &&
      (*fp_plot)(aux_data,x,y,z))
    result++;
  return result;
}

public I64 BSpline2(U8 *aux_data,CD3I32 *ctrl,I64 cnt,Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),Bool closed=FALSE)
{ //2nd order
  I64 i,j,result;
  CD3I32 *c;
  Bool first;
  if (cnt<3) return 0;
  first=TRUE;
  result=0;
  if (closed) {
    cnt++;
    c=MAlloc(sizeof(CD3I32)*(cnt*2-1));
    j=1;
    for (i=0;i<cnt-2;i++) {
      c[j].x=(ctrl[i].x+ctrl[i+1].x)/2.0;
      c[j].y=(ctrl[i].y+ctrl[i+1].y)/2.0;
      c[j].z=(ctrl[i].z+ctrl[i+1].z)/2.0;
      j+=2;
    }
    c[j].x=(ctrl[0].x+ctrl[cnt-2].x)/2.0;
    c[j].y=(ctrl[0].y+ctrl[cnt-2].y)/2.0;
    c[j].z=(ctrl[0].z+ctrl[cnt-2].z)/2.0;

    c[0].x=(c[1].x+c[j].x)/2.0;
    c[0].y=(c[1].y+c[j].y)/2.0;
    c[0].z=(c[1].z+c[j].z)/2.0;
    j=2;
    for (i=0;i<cnt-2;i++) {
      c[j].x=(c[j-1].x+c[j+1].x)/2.0;
      c[j].y=(c[j-1].y+c[j+1].y)/2.0;
      c[j].z=(c[j-1].z+c[j+1].z)/2.0;
      j+=2;
    }
    c[j].x=c[0].x;
    c[j].y=c[0].y;
    c[j].z=c[0].z;
  } else {
    c=MAlloc(sizeof(CD3I32)*(cnt*2-1));
    c[0].x=ctrl[0].x;
    c[0].y=ctrl[0].y;
    c[0].z=ctrl[0].z;
    c[cnt*2-2].x=ctrl[cnt-1].x;
    c[cnt*2-2].y=ctrl[cnt-1].y;
    c[cnt*2-2].z=ctrl[cnt-1].z;
    j=1;
    for (i=0;i<cnt-1;i++) {
      c[j].x=(ctrl[i].x+ctrl[i+1].x)/2.0;
      c[j].y=(ctrl[i].y+ctrl[i+1].y)/2.0;
      c[j].z=(ctrl[i].z+ctrl[i+1].z)/2.0;
      j+=2;
    }
    j=2;
    for (i=0;i<cnt-2;i++) {
      c[j].x=(c[j-1].x+c[j+1].x)/2.0;
      c[j].y=(c[j-1].y+c[j+1].y)/2.0;
      c[j].z=(c[j-1].z+c[j+1].z)/2.0;
      j+=2;
    }
  }
  for (i=0;i<cnt*2-2;i+=2) {
    result+=Bezier2(aux_data,&c[i],fp_plot,first);
    first=FALSE;
  }
  Free(c);
  return result;
}

public I64 BSpline3(U8 *aux_data,CD3I32 *ctrl,I64 cnt,Bool (*fp_plot)(U8 *aux,I64 x,I64 y,I64 z),Bool closed=FALSE)
{ //3rd order
  I64 i,j,result;
  F64 x,y,z;
  CD3I32 *c;
  Bool first;
  if (cnt<3) return 0;
  first=TRUE;
  result=0;
  if (closed) {
    cnt++;
    c=MAlloc(sizeof(CD3I32)*(cnt*3-2));
    j=1;
    for (i=0;i<cnt-2;i++) {
      x=ctrl[i].x;
      y=ctrl[i].y;
      z=ctrl[i].z;
      c[j].x=(ctrl[i+1].x-x)/3.0+x;
      c[j].y=(ctrl[i+1].y-y)/3.0+y;
      c[j].z=(ctrl[i+1].z-z)/3.0+z;
      j++;
      c[j].x=2.0*(ctrl[i+1].x-x)/3.0+x;
      c[j].y=2.0*(ctrl[i+1].y-y)/3.0+y;
      c[j].z=2.0*(ctrl[i+1].z-z)/3.0+z;
      j+=2;
    }
    x=ctrl[cnt-2].x;
    y=ctrl[cnt-2].y;
    z=ctrl[cnt-2].z;
    c[j].x=(ctrl[0].x-x)/3.0+x;
    c[j].y=(ctrl[0].y-y)/3.0+y;
    c[j].z=(ctrl[0].z-z)/3.0+z;
    j++;
    c[j].x=2.0*(ctrl[0].x-x)/3.0+x;
    c[j].y=2.0*(ctrl[0].y-y)/3.0+y;
    c[j].z=2.0*(ctrl[0].z-z)/3.0+z;

    c[0].x=(c[1].x+c[j].x)/2.0;
    c[0].y=(c[1].y+c[j].y)/2.0;
    c[0].z=(c[1].z+c[j].z)/2.0;

    j=3;
    for (i=0;i<cnt-2;i++) {
      c[j].x=(c[j-1].x+c[j+1].x)/2.0;
      c[j].y=(c[j-1].y+c[j+1].y)/2.0;
      c[j].z=(c[j-1].z+c[j+1].z)/2.0;
      j+=3;
    }
    c[j].x=c[0].x;
    c[j].y=c[0].y;
    c[j].z=c[0].z;
  } else {
    c=MAlloc(sizeof(CD3I32)*(cnt*3-2));
    c[0].x=ctrl[0].x;
    c[0].y=ctrl[0].y;
    c[0].z=ctrl[0].z;
    c[cnt*3-3].x=ctrl[cnt-1].x;
    c[cnt*3-3].y=ctrl[cnt-1].y;
    c[cnt*3-3].z=ctrl[cnt-1].z;
    j=1;
    for (i=0;i<cnt-1;i++) {
      x=ctrl[i].x;
      y=ctrl[i].y;
      z=ctrl[i].z;
      c[j].x=(ctrl[i+1].x-x)/3.0+x;
      c[j].y=(ctrl[i+1].y-y)/3.0+y;
      c[j].z=(ctrl[i+1].z-z)/3.0+z;
      j++;
      c[j].x=2.0*(ctrl[i+1].x-x)/3.0+x;
      c[j].y=2.0*(ctrl[i+1].y-y)/3.0+y;
      c[j].z=2.0*(ctrl[i+1].z-z)/3.0+z;
      j+=2;
    }
    j=3;
    for (i=0;i<cnt-2;i++) {
      c[j].x=(c[j-1].x+c[j+1].x)/2.0;
      c[j].y=(c[j-1].y+c[j+1].y)/2.0;
      c[j].z=(c[j-1].z+c[j+1].z)/2.0;
      j+=3;
    }
  }
  for (i=0;i<cnt*3-3;i+=3) {
    result+=Bezier3(aux_data,&c[i],fp_plot,first);
    first=FALSE;
  }
  Free(c);
  return result;
}

#define CC_LEFT		1
#define CC_RIGHT	2
#define CC_TOP		4
#define CC_BOTTOM	8

public Bool ClipLine(I64 *_x1,I64 *_y1,I64 *_x2,I64 *_y2,I64 left,I64 top,I64 right,I64 bottom)
{
  I64 x,y,x1=*_x1,y1=*_y1,x2=*_x2,y2=*_y2,
      cc,cc1,cc2;
  if (y1>bottom)
    cc1=CC_BOTTOM;
  else if (y1<top)
    cc1=CC_TOP;
  else
    cc1=0;
  if (x1>right)
    cc1|=CC_RIGHT;
  else if (x1<left)
    cc1|=CC_LEFT;

  if (y2>bottom)
    cc2=CC_BOTTOM;
  else if (y2<top)
    cc2=CC_TOP;
  else
    cc2=0;
  if (x2>right)
    cc2|=CC_RIGHT;
  else if (x2<left)
    cc2|=CC_LEFT;

  while (TRUE) {
    if (!(cc1|cc2))
      return TRUE;
    if (cc1&cc2)
      return FALSE;

    if (cc1)
      cc=cc1;
    else
      cc=cc2;

    if (cc&CC_BOTTOM) {
      x=x1+(x2-x1)*(bottom-y1)/(y2-y1);
      y=bottom;
    } else if (cc&CC_TOP) {
      x=x1+(x2-x1)*(top-y1)/(y2-y1);
      y=top;
    } else if (cc&CC_RIGHT) {
      y=y1+(y2-y1)*(right-x1)/(x2-x1);
      x=right;
    } else {
      y=y1+(y2-y1)*(left-x1)/(x2-x1);
      x=left;
    }

    if (cc==cc1) {
      *_x1=x1=x;
      *_y1=y1=y;
      if (y1>bottom)
	cc1=CC_BOTTOM;
      else if (y1<top)
	cc1=CC_TOP;
      else
	cc1=0;
      if (x1>right)
	cc1|=CC_RIGHT;
      else if (x1<left)
	cc1|=CC_LEFT;
    } else {
      *_x2=x2=x;
      *_y2=y2=y;
      if (y2>bottom)
	cc2=CC_BOTTOM;
      else if (y2<top)
	cc2=CC_TOP;
      else
	cc2=0;
      if (x2>right)
	cc2|=CC_RIGHT;
      else if (x2<left)
	cc2|=CC_LEFT;
    }
  }
}

#help_index "Graphics"
