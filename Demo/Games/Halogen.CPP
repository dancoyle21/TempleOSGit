#define NUM_ROAD	512
#define ROAD_WIDTH_BY_2	200
#define CAR_WIDTH_BY_2	100

I64 road_x[NUM_ROAD],road_trend;
F64 speed,distance;
I64 road_ptr=0,car_x;
F64 t_last;
Bool crash;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 w=task->pix_width,h=task->pix_height;
  I64 i,x,y,xx;
  Bool old_preempt=Preempt;
  xx=w>>1-car_x+road_x[road_ptr&(NUM_ROAD-1)];

  dc->color=LTGRAY;
  for (i=0;i<NUM_ROAD;i++) {
    x=w>>1-car_x+road_x[(i+road_ptr)&(NUM_ROAD-1)];
    y=h-0.5*i;
    if (y<(h+FONT_HEIGHT)>>1) break;
    GrPlot(dc,x+ROAD_WIDTH_BY_2-0.4*i,y);
    GrPlot(dc,x-ROAD_WIDTH_BY_2+0.4*i,y);
  }
  dc->color=WHITE;

  x=w>>1-CAR_WIDTH_BY_2;
  if (x<xx-ROAD_WIDTH_BY_2)
    crash=TRUE;
  GrLine(dc,x-10,h,x-40,h-100);
  GrLine(dc,x+10,h,x+40,h-100);

  x=w>>1+CAR_WIDTH_BY_2;
  if (x>xx+ROAD_WIDTH_BY_2)
    crash=TRUE;
  GrLine(dc,x-10,h,x-40,h-100);
  GrLine(dc,x+10,h,x+40,h-100);

  dc->color=YELLOW;
  if (crash)
    GrPrint(dc,w>>1-FONT_WIDTH*4,(h-FONT_HEIGHT)>>1,"Game Over");
  Preempt(old_preempt);
}

U0 UpdateRoad()
{
  F64 t0=tS;
  distance+=speed*(t0-t_last);
  t_last=t0;
  while (distance>1.0) {
    road_trend=ClampI64(road_trend+SignI64(RandU16%3-1),-5,5);
    road_x[road_ptr&(NUM_ROAD-1)]=
	  road_x[(road_ptr-1)&(NUM_ROAD-1)]+=road_trend/3;
    road_ptr++;
    distance-=1.0;
  }
}

U0 Init()
{
  I64 i,x=0;
  DocClear;
  "$BG,DKGRAY$%h*c",(TEXT_ROWS-1)/2,'\n';
  speed=0;
  distance=0;
  road_trend=0;
  road_ptr=0;
  car_x=0;
  for (i=0;i<NUM_ROAD;i++) {
    road_x[i]=x;
    road_trend=ClampI64(road_trend+SignI64(RandU16%3-1),-5,5);
    x+=road_trend/3;
  }
  t_last=tS;
  crash=FALSE;
}

U0 Halogen()
{
  I64 a1,a2,ch=0,sc=0;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=BLACK<<4+WHITE;
  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Accelerate(,,SC_CURSOR_UP);"
	"  Deccellerate(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);
  Preempt;
  AutoComplete;
  WinMax;
  Init;
  DocCursor;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (ScanMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  ch=a1;sc=a2;
	  switch (ch) {
	    case '\n':
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto ha_done;
	  }
	  break;
	case MSG_KEY_UP:
	  ch=a1;sc=a2;
	  if (!ch)
	    switch (sc.u8[0]) {
	      case SC_CURSOR_RIGHT:
	      case SC_CURSOR_LEFT:
	      case SC_CURSOR_UP:
	      case SC_CURSOR_DOWN:
		sc=0;
		break;
	    }
	  break;
      }
      switch (sc.u8[0]) {
	case SC_CURSOR_RIGHT:
	  car_x++;
	  break;
	case SC_CURSOR_LEFT:
	  car_x--;
	  break;
	case SC_CURSOR_UP:
	  if (++speed>200) speed=200;
	  break;
	case SC_CURSOR_DOWN:
	  if (--speed<0) speed=0;
	  break;
      }
      if (crash)
	Snd(0);
      else {
	if (speed)
	  Snd(speed+10);
	else
	  Snd(0);
	UpdateRoad;
      }
      Sleep(10);
    }
ha_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  MenuPop;
  DocClear;
  SettingsPop;
}

Halogen;
