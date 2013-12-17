#define NUM_ZEROS	2
Complex zeros[NUM_ZEROS]={{10.0,0},{-30,0}};
#define NUM_POLES	2
Complex poles[NUM_POLES]={{-20.0,-15.0},{-20.0,15.0}};

F64 scale;

Complex *PoleZeroFind(I64 x,I64 y)
{
  I64 i;
  F64 d,best_d=MAX_F64;
  Complex *result=NULL;
  for (i=0;i<NUM_POLES;i++) {
    d=Sqr(poles[i].x-x)+Sqr(poles[i].y-y);
    if (d<best_d) {
      best_d=d;
      result=&poles[i];
    }
  }
  for (i=0;i<NUM_ZEROS;i++) {
    d=Sqr(zeros[i].x-x)+Sqr(zeros[i].y-y);
    if (d<best_d) {
      best_d=d;
      result=&zeros[i];
    }
  }
  return result;
}

F64 F(Complex *x)
{
  F64 m,a;
  Complex num,denom,n1;

  CPoly(&num,  x,zeros,NUM_ZEROS);
  CPoly(&denom,x,poles,NUM_POLES);
  CDiv(&num,&denom,&n1);
 
  R2P(&m,&a,n1.x,n1.y);
  if (ip_rb)
    return pi+a;
  else
    return m;
}

F64 MPDraw(CTask *task)
{
  Complex xx;
  I64 x,y,w=task->win_pixel_width,h=task->win_pixel_height,cx=w/2,cy=h/2,
     lo=Gs->num*h/mp_cnt,hi=(Gs->num+1)*h/mp_cnt;
  F64 yy,y_total=0;
  CDC *dc=DCAlias(gr_dc,task);
  for (y=lo;y<hi;y++) {
    for (x=0;x<w;x++) {
      CEqu(&xx,x-cx,cy-y);
      yy=scale*F(&xx);
      dc->color=Limit(yy,0,14);
      y_total+=Limit(yy,-14,14);
      GrPlot(dc,x,y);
    }
    }
  DCDel(dc);
  return y_total;
}

U0 Draw()
{
  CSrvCmd *tempm[MP_MAX_PROCESSORS];
  F64 y_total,old_y_total=MAX_F64;
  I64 i,w=Fs->win_pixel_width,h=Fs->win_pixel_height,cx=w/2,cy=h/2;
  CDC *dc=DCAlias;
  while (TRUE) {
    for (i=0;i<mp_cnt;i++)
      tempm[i]=JobQue(&MPDraw,Fs,i,0);
    y_total=0;
    for (i=0;i<mp_cnt;i++)
      y_total+=GetResult(tempm[i])(F64);

    if (!y_total) break;
    scale*=7*GR_WIDTH*GR_HEIGHT/y_total;
    if (Abs(y_total-old_y_total)<1.0*GR_WIDTH*GR_HEIGHT)
      break;
    old_y_total=y_total;
  }
  dc->color=15;
  GrLine(dc,0,cy,w,cy);
  GrLine(dc,cx,0,cx,h);
  for (i=0;i<NUM_ZEROS;i++)
    GrPrint(dc,cx+zeros[i].x-FONT_WIDTH/2,cy-zeros[i].y-FONT_HEIGHT/2,"o");
  for (i=0;i<NUM_POLES;i++)
    GrPrint(dc,cx+poles[i].x-FONT_WIDTH/2,cy-poles[i].y-FONT_HEIGHT/2,"x");
  DCDel(dc);
}

U0 PoleZeros()
{
  I64 msg_code,p1,p2,p11,p22,cx,cy;
  Complex *tempc=NULL;

  PopUpOk(
	"Drag the poles and zeros with left mouse.\n"
	"Hold right mouse for phase plot.\n"
  );
  WinMgrSync;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->win_inhibit=WIG_TASK_DFT-WIF_FOCUS-WIF_BORDER;
  SetVGAPalette4(vga_palette4_gray);
  SetVGAPaletteColor(15,0xFF0000); //White is red

  WinMax;
  WordStat;
  WinBorder;

  DCFill;
  DocClear;
  scale=1.0;

  try {
    Draw;
    while (TRUE) {
      msg_code=GetMsg(&p1,&p2,1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN+1<<MSG_IP_L_UP+1<<MSG_IP_R_DOWN+1<<MSG_IP_R_UP+1<<MSG_IP_MOVE);
pz_msg:
      cx=Fs->win_pixel_width/2;
      cy=Fs->win_pixel_height/2;
      switch (msg_code) {
	case MSG_IP_L_DOWN:
	  tempc=PoleZeroFind(p1-cx,cy-p2);
	  break;
	case MSG_IP_MOVE:
	  if (tempc) {
	    p11=p1;p22=p2;
	    //get to last mouse move
	    while (msg_code=ScanMsg(&p1,&p2,1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN+1<<MSG_IP_L_UP+1<<MSG_IP_R_DOWN+1<<MSG_IP_R_UP+1<<MSG_IP_MOVE))
	      if (msg_code==MSG_IP_MOVE) {
		p11=p1;p22=p2;
	      } else
		goto pz_msg;

	    tempc->x=p11-cx;
	    tempc->y=cy-p22;
	    Draw;
	  }
	  break;
	case MSG_IP_L_UP:
	  if (tempc) {
	    tempc->x=p1-cx;
	    tempc->y=cy-p2;
	    tempc=NULL;
	    Draw;
	  }
	  break;
	case MSG_IP_R_DOWN:
	case MSG_IP_R_UP:
	  Draw;
	  break;
	case MSG_KEY_DOWN:
	  if (p1==CH_SHIFT_ESC||p1==CH_ESC)
	    goto pz_done;
      }
      WinMgrSync;
    }
pz_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  DCFill;
}

PoleZeros;