/*Scans the sym table and checks
each [C:/Kernel/KernelA.HPP.Z,648] HTT_DEFINE_STR entry to see if
it only occurs once in files.

It's a brute force solution, but
gets the job done... slowly.

[C:/Adam/Utils/Find.CPP.Z,145] Find() returns a count of matches.

[C:/Adam/Utils/Find.CPP.Z,182] FileOcc() is [C:/Adam/Utils/Find.CPP.Z,145] Find() with
output [C:/Kernel/KMisc.CPP.Z,223] Silent().
*/

U0 UnusedDefineScan()
{
  CDoc *old_put_doc,*old_display_doc,*doc;
  I64 i,cnt=0;
  CHashTable *table;
  CHash *temph;
  CDocEntry *doc_e,*doc_e2;
  Bool old_silent=IsSilent;

  //Technically, we might want to
  //turn-off preemption in case the
  //table were modified.
  try {
    table=Fs->hash_table;
    while (table) {
      for (i=0;i<=table->mask;i++) {
	temph=table->body[i];
	while (temph) {
	  if (temph->type&HTT_DEFINE_STR)
	    cnt++;
	  temph=temph->next;
	}
      }
      table=table->next;
    }

    progress1=0;
    progress1_max=cnt;
    StrCpy(progress1_desc,"Define Scan");

    table=Fs->hash_table;
    while (table) {
      for (i=0;i<=table->mask;i++) {
	temph=table->body[i];
	while (temph) {
	  if (temph->type&HTT_DEFINE_STR) {
	    progress1++;
	    if (FileOcc(temph->str,"/*","+l-i+T")==1) {
	      doc=DocNew;
	      old_put_doc=DocPut;
	      old_display_doc=DocDisplay;
	      Fs->put_doc=Fs->display_doc=doc;
	      Find(temph->str,"/*","+l-i+T");
	      Fs->put_doc=old_put_doc;
	      Fs->display_doc=old_display_doc;
	      doc_e=doc->root.next;
	      while (doc_e!=doc) {
		if (doc_e->type_u8==DOCT_LINK) {
		  "%s ",temph->str;
		  doc_e2=DocEntryCopy(doc,doc_e);
		  DocInsEntry(old_put_doc,doc_e2);
		  '\n';
		}
		doc_e=doc_e->next;
	      }
	      DocDel(doc);
	    }
	  }
	  temph=temph->next;
	}
      }
      table=table->next;
    }
  } catch
    PutExcept;

  Silent(old_silent);
  '\n';
  ProgressBarsRst;
}

UnusedDefineScan;
