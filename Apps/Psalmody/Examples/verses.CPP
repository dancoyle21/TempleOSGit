//8 special
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "$WW,1$This shows a random passage based on "
	  "the time at the moment this song was run.  "
	  "Talk to God as an offering, then run AfterEgypt
`Cd("::/Apps/AfterEgypt");;#include "Run"\n`
 if you "
	  "want God to talk some more.  The [C:/Adam/God/HSNotes.TXT.Z,1] Holy Spirit can "
	  "puppet you.\n");
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    "$WW+H,1$";
    DocRecalc(DocPut(task)); //Reformat word-wrap with new window size.
    while (!ScanKey) {
      FifoU8Flush(god.fifo);
      GodBitsIns(GOD_GOOD_BITS,KbdMouseEvtTime>>GOD_BAD_BITS);
      GodPassage(5);
      Play("3qAeABCBqCetABBqCetAAREE2G");
      Play("3qAeABCBqCetABBqCetAAREE2G");
      Play("3eDRRRqGReRRGGGGqE");
      Play("eDRRRqGReRRGGGGqE");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
