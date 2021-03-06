/*
This demo uses gr_dc2, so you must add a
draw_it routine because gr_dc2 is not
persistent.  (It gets erased each update by
the system.)
*/








      $SP,"<1>",1$







CDC *dc;

U0 DrawIt(CTask *,CDC *)
{
//We can change the pen width because it was not
//define in <CTRL-R>.
  dc->pen_width=2;
  DCDepthBufRst(dc);
  SpritePlot3(dc,0,0,0,$IB,"<1>",1$);
}

U0 SpritePlot3D()
{
  F64 �=0,�=0;
  dc=DCAlias(gr_dc2,Fs);

  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->draw_it=&DrawIt;
  Preempt(OFF); //We don't want update during matrix change
  dc->x=200;
  dc->y=200;
  dc->z=GR_Z_ALL;
  dc->flags|=DCF_TRANSFORMATION;
  DCAllocDepthBuf(dc);
  while (!ScanChar) {
    GrIdentEqu(dc->r);
    GrRotXEqu(dc->r,�);
    GrRotZEqu(dc->r,�);
    GrSetRotMat(dc,dc->r);

    Sleep(40);

    �+=2*�/100;
    �+=2*�/130;
  }
  SettingsPop;
  DCDel(dc);
}

SpritePlot3D;
           �            ��������    2   ����        2       2   ����2   ��������2       2   2                                                                                                       