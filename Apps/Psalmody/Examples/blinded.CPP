//7 has words
F64 t0=Beat;

U0 DrawIt(CTask *task,CDC *dc)
{
  static I64 i=0;
  F64 dt=Beat-t0;
  if (dt>16.0 && i++&1) {
    dc->color=WHITE;
    GrRect(dc,0,0,task->pix_width,task->pix_height);
  }
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try {
    task=PopUpViewPrint("[C:/Misc/Bible.TXT.Z,87803]\n");
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      t0=Beat;
      Play("3qEeACA2G3qCEC2G3E",
	    "Now... \0 \0 \0 \0 \0I \0see \0the \0light.\n\0 \0");
      Play("EeACA2G3qCEC2G3E",
	    "Now... \0 \0 \0 \0 \0I \0see \0the \0light.\n\0 \0");
      Play("GCGBEeDFCAqD",
	    "It's \0 \0quite \0 \0bright.\n\0 \0 \0 \0 \0 \0");
      Play("GCGBEeDFCAqD",
	    "It's \0 \0quite \0 \0bright.\n\0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
