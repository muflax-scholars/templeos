//7 has graphics


$SP,"<1>",1$

$SP,"<2>",2$





F64 t0=0.0;

U0 DrawIt(CTask *,CDC *dc)
{
  SpritePlot3(dc,20,8,0,$IB,"<1>",1$);
  if (t0) {
    SpritePlot3(dc,20,24,0,$IB,"<2>",2$);
    Snd((tM-t0)*100);
    if (tM>t0+0.25)
      t0=0.0;
  }
}

U0 Song()
{
  SettingsPush; //See $LK,"SettingsPush","MN:SettingsPush"$
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    while (!ScanKey) {
      Play("3eBCsAGAGeAAetGAAsBBBBeCDqDeEA",
	  "Now,\0I\0su\0\0u\0\0ure\0\0\0\0won't\0do\0o\0o\0o\0that\0a\0gain!\n\0\0\0");

      t0=tM;
      Play("BCsAGAGeAAetGAAsBBBBeCDqDeEA",
	  "Now,\0I\0su\0\0u\0\0ure\0\0\0\0won't\0do\0o\0o\0o\0that\0a\0gain!\n\0\0\0");
      Play("2qG3etAAGBB2G3eEFCDqFeAGqC",
	  "\0\0\0\0\0\0\0Learn\0ing\0from\0\0my\0\0mis\0takes.\n\0");
      Play("2G3etAAGBB2G3eEFCDqFeAGqC",
	  "\0\0\0\0\0\0\0Learn\0ing\0from\0\0my\0\0mis\0takes.\n\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
           ;         %      !      !      %      %      "   "              :      �         []۶m�$�(I���]��I�4IR)1�0lض͵m�$I�dD�I�$ٶm۶�>O�$�@�m۶m��m�l�6O�$�$I�m۶mk]۶��$I�$I2Q�e۶m۶�<O�$I�$�dsm�&O�$I�a�$I�m�f��O�$E1I ۶��m�$I�I�؆m۶m۶m��m���$I���¶m�6�ۼ�um�$ϓ$I���d0l�m۶m۶m��m^m��I�$I�$&L�lۼ�&I�$I�$I�$IRIn[�v�M�$I�d�$ٶm۶mۺ�uM�$I�$�$� �ۆm۶m�ڵ�m�'I�$�$I�m�g�<I�$I�$I�T�$�6l۶m۶��l��I�$I�$I�$)RI�$���m�$M�'I�$)&I�m۶m��'I2I�`ٶmkm��I�I�ضmضm�:�'y�$�$L�mضm۶m[�m��m�O�$I�%�6�뼮-I�&I�$I&ذmضm۶m[W��I�4I�$I�$�d�F�$I�$I�$I�$ɓ$`۶m۶m۶m۶m�f*
�d۶n���&O�"�0R�$۶mۼu�n��խ=�$I�$I1	�m۶m�L�$I�$I�$�$I���:�5 