$BG,11$
$SP,"<1>",1$





$SP,"<2>",2$




$SP,"<3>",3$

$BG$















$SP+BP,"<4>",4,"::/TempleOS/Demo/Graphics/Camp.CPP.Z,6"$ //Pointer to sprite <6> in Camp.CPP.Z file

#define NUM_QUAIL	128
#define SKY_HEIGHT	(0.6*GR_HEIGHT)
class QuailStruct
{
  F64 x,y,dx,dy,phase;
  Bool dead,pad[7];
} q[NUM_QUAIL];

F64 t0,t_last;

U0 DrawQuail(CTask *,CDC *dc)
{
  I64 i;
  U8 *inter;
  F64 tt,t1=tT-t0;
  SpritePlot3(dc,0,SKY_HEIGHT,0,$IB,"<4>",4$);
  for (i=0;i<NUM_QUAIL;i++) {
    tt=Tri(t1+q[i].phase,1.0);
    if (q[i].dead) {
      q[i].x+=(t1-t_last)*q[i].dx;
      q[i].y+=50*(t1-t_last);
      if (q[i].y>SKY_HEIGHT) {
	q[i].y=SKY_HEIGHT;
	q[i].dx=0;
      }
      SpritePlot3(dc,q[i].x,q[i].y,0,$IB,"<3>",3$);
    } else {
      q[i].x+=(t1-t_last)*q[i].dx;
      q[i].y+=(t1-t_last)*q[i].dy;
      if (!(0<q[i].y<SKY_HEIGHT-20)) {
	q[i].dy=-q[i].dy;
	q[i].y+=(t1-t_last)*q[i].dy;
      }
      inter=SpriteInterpolate($IB,"<1>",1$,$IB,"<2>",2$,tt);
      SpritePlot3(dc,q[i].x,q[i].y,0,inter);
      Free(inter);
      if (q[i].x>0 && t1-t_last>10*Rand)
	q[i].dead=TRUE;
    }
  }
  t_last=t1;
}

U0 Quail()
{
  I64 i;
  for (i=0;i<NUM_QUAIL;i++) {
    q[i].x=RandI16%GR_WIDTH;
    q[i].y=RandU16%SKY_HEIGHT;
    q[i].dx=50*Rand+10;
    q[i].dy=20*(Rand-0.5);
    q[i].phase=Rand;
    q[i].dead=FALSE;
  }
  SettingsPush(Fs,TSF_SAME_SONG); //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->text_attr=YELLOW<<4+BLUE;
  WinMax;
  DocClear;
  "$$BG,YELLOW$$";
  PutBibleVerse("Numbers,11:11",88);
  DocTop;
  PopUpOk("Scroll down to finish reading.\n"
	"$$FG,GREEN$$<SHIFT-ESC>$$FG$$ when done.");
  View;
  DocClear;
  "$$BG,LTCYAN$$"; NewLine(SKY_HEIGHT/FONT_HEIGHT);
  "$$BG,YELLOW$$"; NewLine(5);

  t0=tT;
  t_last=0;
  Fs->draw_it=&DrawQuail;
  PressAKey;
  DocClear;
  SettingsPop(Fs,TSF_SAME_SONG);
}
           e        ����   ����   ����   ����������������   ����   ����
   ����
   ����                                 ����            
      
      ����   ����   ������������   ����   ����   ����   ����   ����   ������������    ����   ��������������������                       ����               ����           e        ����   ����   ����   ����    ����       ����   ����                                          ����      ����   ����            ����   ����   ����   ����   ������������    ����������������������������   ����   ����   ����   ����   ����   ����   ����                                         �        ����������������������������    ����                   ����   ����   ����   ����   ����   ����   ������������               ����   ����������������������������������������   ����    ����   ����
   ����   ����������������������������������������������������������������      ����   ����   ����   ��������������������   ����   ��������������������   ����   �������������������� 