//7 has graphics
//Melody by Terry A. Davis






$SP,"<1>",1$







$SP,"<2>",2$









$SP,"<3>",3$






F64 t0=tMBeat;

#define PHASE1	9.5
#define PHASE2	10.0
#define PHASE3	10.5
#define PHASE4	16.0
#define PHASE5	16.5

#define MAN_X	250
#define MAN_Y	30

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=tMBeat-t0;
  U8 *inter;
	 if (dt<PHASE1)
    SpritePlot3(dc,MAN_X,MAN_Y,0,$IB,"<1>",1$);
  else if (dt<PHASE2) {
    inter=SpriteInterpolate($IB,"<1>",1$,$IB,"<2>",2$,(dt-PHASE1)/(PHASE2-PHASE1));
    SpritePlot3(dc,MAN_X,MAN_Y,0,inter);
    Free(inter);
  } else if (dt<PHASE3) {
    inter=SpriteInterpolate($IB,"<2>",2$,$IB,"<3>",3$,(dt-PHASE2)/(PHASE3-PHASE2));
    SpritePlot3(dc,MAN_X,MAN_Y,0,inter);
    Free(inter);
  } else if (dt<PHASE4)
    SpritePlot3(dc,MAN_X,MAN_Y,0,$IB,"<3>",3$);
  else if (dt<PHASE5) {
    inter=SpriteInterpolate($IB,"<3>",3$,$IB,"<1>",1$,(dt-PHASE4)/(PHASE5-PHASE4));
    SpritePlot3(dc,MAN_X-100*(dt-PHASE4),MAN_Y-5*(dt-PHASE4),0,inter);
    Free(inter);
  } else
    SpritePlot3(dc,MAN_X-100*(PHASE5-PHASE4),MAN_Y-5*(PHASE5-PHASE4),0,$IB,"<1>",1$);
}

U0 Song()
{
  CTask *task;
  MusicSettingsRst;
  music_tempo= 2.350;
  music_stacatto_factor= 0.900;
  try {
    task=PopUpViewStr(
    "$LK,"Ecclesiastes,2:24","BF:Ecclesiastes,2:24"$\n"
	"$LK,"Ecclesiastes,3:13","BF:Ecclesiastes,3:13"$\n"
	);
    TaskWait(task);
    SetWinHorz(Fs->win_left,Fs->win_right,task);
    SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
    Fs->draw_it=&DrawIt;
    Fs->task_end_cb=&SndTaskEndCB;
    while (!ScanKey) {
      t0=tMBeat;
      Play("M5/43qEAeDDCCqBEAeDD",
	  "Up\0a\0bove\0He\0gen\0tly\0smiles\n\0as\0we\0meet\0and\0");
      Play("CCqBFBeEEDDqCF",
	  "con\0quer\0trials.\n\0$$FG,RED$$You\0can\0do\0it\0lit\0tle\0one.\n\0With\0");
      Play("BeEEDDqC",
	  "out\0work\0there\0is\0no\0fun.\n$FG$\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
           �         U      T      T      X       X       Z      Z      ]      W      \   ����X   ����U   ����X   ����U   ����[   ����]   ����]   ����[       U   ����T   ����T   ����R   ����           �         U      O      O      X       X       [      [      `      W      U   ����X   ����O   ����R   ����M   ����U   ����S   ����S   ����Q      O   ����O   ����O   ����K                 �         U      K      K      Q      R      U      U      `      R      J   ����Q      D   ����F   ����=   ����J   ����L   
   L   
   F      D   ����F      F      >       