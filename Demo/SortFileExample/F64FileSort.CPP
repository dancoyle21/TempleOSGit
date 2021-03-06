//This takes an input file of doubles and makes a sorted output file.
//[D:/Demo/SortFileExample/UnsortedF64.TXT.Z,1]
//[D:/Demo/SortFileExample/SortedF64.TXT.Z,1]

Cd(__DIR__);;

I64 Compare(F64 e1,F64 e2)
{
  return Sign(e1-e2);
}

U0 F64FileSortTXT(U8 *in_name,U8 *out_name)
{
  CCmpCtrl *cc;
  I64 i,n;
  F64 *a;
  CDoc *doc;

  //Pass1: Count the number of F64s.
  n=0;
  cc=CmpCtrlNew(MStrPrint("#include \"%s\"",in_name));
  while (Lex(cc))
    if (cc->token==TK_F64)
      n++;
  CmpCtrlDel(cc);

  a=MAlloc(n*sizeof(F64));

  //Pass2: Read F64s.
  i=0;
  cc=CmpCtrlNew(MStrPrint("#include \"%s\"",in_name));
  while (Lex(cc))
    if (cc->token==TK_F64)
      a[i++]=cc->cur_f64;
  CmpCtrlDel(cc);

  QSortI64(a,n,&Compare); //Sort 64-bit sized values

  //Save
  doc=DocNew(out_name);
  DocPrint(doc,"//This is sorted.\n");
  for (i=0;i<n;i++)
    DocPrint(doc,"%12.6f\n",a[i]);
  DocWrite(doc);
  DocDel(doc);

  Free(a);
}
F64FileSortTXT("UnsortedF64.TXT.Z","SortedF64.TXT.Z");
Type("UnsortedF64.TXT.Z");
Type("SortedF64.TXT.Z");

U0 F64FileSortDAT(U8 *in_name,U8 *out_name)
{//[File/CFile,1] File/CFile, [C:/Demo/Dsk/DataBase.CPP.Z,1]
  I64 size;
  F64 *a=FileRead(in_name,&size);
  QSortI64(a,size/sizeof(F64),&Compare); //Sort 64-bit sized values
  FileWrite(out_name,a,size);
  Free(a);
}
F64FileSortDAT("UnsortedF64.DAT.Z","SortedF64.DAT.Z");

U0 F64FileDumpDAT(U8 *in_name)
{//[File/CFile,1] File/CFile, [C:/Demo/Dsk/DataBase.CPP.Z,1]
  I64 i,n,size;
  F64 *a=FileRead(in_name,&size);
  n=size/sizeof(F64);
  "%s\n",in_name;
  for (i=0;i<n;i++)
    "%12.6f\n",a[i];
  Free(a);
}
F64FileDumpDAT("UnsortedF64.DAT.Z");
F64FileDumpDAT("SortedF64.DAT.Z");
