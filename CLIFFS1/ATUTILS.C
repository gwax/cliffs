/*++
ATUTILS.C contains fragments and proc's and exec's.
--*/

#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include "atdefs.g"

#include "cxldef.h"
#include "sound.g"

/***********************************PROCS******************************/
key_proc(U_DATA *keys)
{
int c=0;
   if( kbhit() )
   {
      keys->i++;
      if( keys->i==MX_KEYS )
         keys->i = 0;

      c = getch(); 
      if( c==0 )
         c = getch();

      keys->data[keys->i]  = c;
      keys->status = HOT;
   }
   else
      keys->status = READY;

   return( keys->status );
}


tm_proc(TM_DATA *tm , double TimeLimit )
{
int start,hstart,stop, hstop;
double diff;

   switch( tm->status )
   {
      case READY:
      case DONE:
         break;
      case START:
         _dos_gettime( &(tm->begin) );
         tm->status = BUSY;
         break;
      case BUSY:
         _dos_gettime( &(tm->end) );
         start = tm->begin.second;
         hstart = tm->begin.hsecond;
         stop  = tm->end.second;
         hstop = tm->end.hsecond;
         diff =  (double)(stop-start) +
                 (double)(hstop-hstart)/100.0;
         if( diff > TimeLimit )
            tm->status = DONE;
         break;
   }
   return(tm->status);
}


move_proc(U_DATA *moves, int c)
{
int i;
   if( moves->status!=M_HOT )
      return(0);
   
   i = moves->i;
   moves->data[i] = c;

   i++;
   if( i==MX_MOVES )
      i = 0;

   moves->i = i;
   moves->status = READY;
   return(0);
}

clock_proc(TM_DATA *clock)
{
static struct dostime_t temp;
int clickpause=9;
int t;
   if( clock->status==START )
   {
      _dos_gettime(&clock->begin);
      _dos_gettime(&clock->end);
      _dos_gettime(&temp);
      clock->status = READY;
   }
   else
   {
      _dos_gettime(&clock->end);
      if( (t=TimeDiff(&clock->end,&temp)) > clickpause )
      {
         _dos_gettime(&temp);
         clock->status = HOT;
      }
      else
         clock->status = READY;
   }
   return(0);
}

/******************************UTILS***************************************/

InitMan(MAN *man,int x,int y)
{
   man->X = x;
   man->Y = y;

   man->rh.X = MX_HEXT;
   man->rh.Y = 0;
   man->rh.LR =  1;
   man->rh.DU =  1;

   man->lh.X = MX_HEXT;          /*limb extensions*/
   man->lh.Y = 0;
   man->lh.LR = -1;
   man->lh.DU =  1;

   man->rf.X = MX_HEXT;
   man->rf.Y = MX_VEXT;
   man->rf.LR =  1;
   man->rf.DU = -1;

   man->lf.X = MX_HEXT;
   man->lf.Y = MX_VEXT;
   man->lf.LR = -1;
   man->lf.DU = -1;

   CopyAllLimbs(man);

   return(0);
}

DisplayLedges(LEDGE ledge[],int n)
{
int i;
   _setcolor(LEDGECOLOR);
   for( i=0; i<n; i++)
   {
      _moveto(ledge[i].leftend,ledge[i].height);
      _lineto(ledge[i].rightend,ledge[i].height);
      _moveto(ledge[i].leftend,ledge[i].height-1);
      _lineto(ledge[i].rightend,ledge[i].height-1);
   }
   return(0);
}

int DisplayMan(MAN *man,int color)
{
int a = man->X,b=man->Y-5, b2=man->Y;   /*subtracted 5 from both y coords*/
	_setcolor(color);
                                                            
	_moveto(a,b);                                            /*body*/
	_lineto(a,b2);
                            
	_moveto(a,b);                                            /*left hand*/
	_lineto(a + man->lh.LR*man->lh.X, b2 -man->lh.DU*man->lh.Y);
	_moveto(a,b);                                            /*right hand*/
	_lineto(a + man->rh.LR*man->rh.X, b2 -man->rh.DU*man->rh.Y);
	_moveto(a,b2);                                           /*left foot*/
	_lineto(a + man->lf.LR*man->lf.X, b2 -man->lf.DU*man->lf.Y);
	_moveto(a,b2);                                           /*right foot*/
	_lineto(a + man->rf.LR*man->rf.X, b2 -man->rf.DU*man->rf.Y);

	_ellipse(_GBORDER,a-2,b-8,a+2,b-3);                      /*head*/

   return(0);
}

fall(MAN *man)
{
int j,i;
int start = man->Y;
   for(j=start;j<470;j++)
   {
      for(i=0;i<100;i++)
         ;
		DisplayMan(man,BACKGROUND);
      man->Y++;
		DisplayMan(man,HOTMAN);
   }

_settextcolor(HOTTEXT);
_settextposition(1,1);
_outtext("Your climbing companion watches in shocked horror");
_settextposition(2,1);
_outtext("as your body slides down a short icefield, then ");
_settextposition(3,1);
_outtext("cartwheels out over the 3,000 foot abyss...");
getch();
   return(0);
}

Congratulations()
{
   _settextcolor(HOTTEXT);
   _settextposition(1,1);
   _outtext("It is amazing! You made it to the top!");
   _settextposition(2,1);
   _outtext("The view of the surrounding peaks is spectacular");
   _settextposition(3,1);
   _outtext("Although tired, you feel an overwhelming sense of joy.");
   _settextposition(4,1);
   _outtext("You hardly notice the frostbite beginning to numb your hands.");
   getch();

   _clearscreen(_GCLEARSCREEN);
   putpik("ev6",120,0);
   getch();
   _settextposition(1,1);
   _outtext("    ");
   _settextposition(1,1);
   _outtext("    ");

   return(0);
}

Retreat()
{
_settextcolor(HOTTEXT);
_settextposition(1,1);
_outtext("You decide to retreat for today but");
_settextposition(2,1);
_outtext("are happy knowing you'll be back.");
getch();
   return(0);
}

int GameInstruct(void)
{
   _settextcolor(WHITE);
   _clearscreen(_GCLEARSCREEN);
   _settextposition(1,1);
_outtext("                 ************  ATTACK THE CLIFFS  ************ \n");
_outtext("                   ***********  QUICK REFERENCE  ***********\n");
_outtext("      \n");
_outtext("                       CopyRite(C) 1992 Peter Waksman\n");
_outtext("    \n");
_outtext("        <esc>    quit\n");
_outtext("        <F1>     select PeterB      \n");
_outtext("        <F2>     select DougalH\n");
_outtext("        <arrow>  select direction\n");
_outtext("\n");
_outtext("        To move use:\n");
_outtext("\n");
_outtext("             left      <q>                          right      <p>\n");
_outtext("             hand        <a>                        hand     <l>\n");
_outtext("                 left      <s>                 right       <k>\n");
_outtext("                 foot        <x>               foot      <m>\n");
_outtext("\n");
_outtext("\n --HIT ANY KEY--\n");
_outtext("\n");
   tune();
getch();
_outtext("        To belay rope to man use:\n");
_outtext("\n");
_outtext("                left     <w>                   right    <o>\n");
_outtext("                hand                           hand\n");
_outtext("                               body  <b>\n");
_outtext("                                        \n");
_outtext("        To belay rope to ledge use:\n");
_outtext("\n");
_outtext("                left     <e>                   right    <i>\n");
_outtext("                hand                           hand\n");
_outtext("\n");
_outtext("        Note1: To be supported the man must have either the rope attached\n");
_outtext("        or a leg and some other limb. For example 2 arms and no rope will\n");
_outtext("        not support.\n");
_outtext("\n");
_outtext("        Note2: The color of a belay dot determines if the attachment is a \n");
_outtext("        fixed knot or a sliding one:  yellow=fixed  blue=sliding.\n");
_outtext("\n");
_outtext("        Note3: You can only climb up the rope when your arms are centered.\n");
_outtext("                                                                             \n");
_outtext("        Note4: You can hit movement keys simultaneously for moving limbs\n");
_outtext("        simultaneously; also you can hit keys quickly in sucession to make\n");
_outtext("        complex movements.\n");
_outtext("  \n");
_outtext("\n --HIT ANY KEY--\n");
   getch();

   _clearscreen(_GCLEARSCREEN);
   _settextposition(10,20);
   printf("Use <enter> to continue");
   _settextposition(12,20);
   printf("Use <esc> to quit (a wise decision)");

   return(0);

}


BelayKey(int manflag)
{
   if( manflag==1)
   {
      _settextcolor(HOTTEXT);
      _settextposition(29,15);
      _outtext("DougalH:");
      _settextcolor(COLDTEXT);
      _settextposition(29,1);
      _outtext("PeterB:");
   }
   else if( manflag==2 )
   {
      _settextcolor(COLDTEXT);
      _settextposition(29,15);
      _outtext("DougalH:");
      _settextcolor(HOTTEXT);
      _settextposition(29,1);
      _outtext("PeterB:");
   }

   return(0);
}

int MiniKey(void)
{
int start = 23;
   _settextcolor(HOTTEXT);

   _settextposition(start,1);
   _outtext("<q>");
   _settextposition(start+1,2);
   _outtext("<a>");
   _settextposition(start+3,1);
   _outtext("<s>");
  _settextposition(start+4,2);
   _outtext("<x>");


   _settextposition(start,7);
   _outtext("<p>");
   _settextposition(start+1,6);
   _outtext("<l>");
   _settextposition(start+3,7);
   _outtext("<k>");
  _settextposition(start+4,6);
   _outtext("<m>");

   return(0);
}

FlipDirection(int *direction, int c)
{
int Ox=535,Oy=445;
   switch( c )
   {
      case 75:   
         DisplayArrow(Ox,Oy,*direction,BACKGROUND);
         *direction = LEFT;
         DisplayArrow(Ox,Oy,LEFT,ARROWCOLOR);
         break;
      case 77:
         DisplayArrow(Ox,Oy,*direction,BACKGROUND);
         *direction = RIGHT;
         DisplayArrow(Ox,Oy,RIGHT,ARROWCOLOR);
         break;
      case 72:
         DisplayArrow(Ox,Oy,*direction,BACKGROUND);
         *direction = UP;
         DisplayArrow(Ox,Oy,UP,ARROWCOLOR);
         break;
      case 80:
         DisplayArrow(Ox,Oy,*direction,BACKGROUND);
         *direction = DOWN;
         DisplayArrow(Ox,Oy,DOWN,ARROWCOLOR);
         break;
   }
   return(0);
}

DisplayArrow(int x, int y, int direction,int color)
{
int tipX,tipY;
   _setcolor(color);
   switch( direction )
   {
      case UP:
         tipX = x + 10;
         tipY = y + 3;
         _moveto(tipX,tipY);          /*the shaft*/
         _lineto(tipX,y+20);
         _moveto(tipX+1,tipY);
         _lineto(tipX+1,y+20);

         _moveto(tipX,tipY);
         _lineto(x+3,y+10);

         _moveto(tipX,tipY);
         _lineto(x+17,y+10);

         break;
      case DOWN:
         tipX = x + 10;
         tipY = y + 17;
         _moveto(tipX,tipY);          /*the shaft*/
         _lineto(tipX,y);
         _moveto(tipX+1,tipY);
         _lineto(tipX+1,y);

         _moveto(tipX,tipY);
         _lineto(x+3,y+10);

         _moveto(tipX,tipY);
         _lineto(x+17,y+10);

         break;
      case LEFT:  
         tipX = x + 3;
         tipY = y + 10;
         _moveto(tipX,tipY);          /*the shaft*/
         _lineto(x+20,tipY);
         _moveto(tipX,tipY+1);
         _lineto(x+20,tipY+1);

         _moveto(tipX,tipY);
         _lineto(x+10,y+3);

         _moveto(tipX,tipY);
         _lineto(x+10,y+17);

         break;
      case RIGHT:
         tipX = x + 17;
         tipY = y + 10;
         _moveto(tipX,tipY);          /*the shaft*/
         _lineto(x,tipY);
         _moveto(tipX,tipY+1);
         _lineto(x,tipY+1);

         _moveto(tipX,tipY);
         _lineto(x+10,y+3);

         _moveto(tipX,tipY);
         _lineto(x+10,y+17);

         break;
   }
   return(0);
}

DisplayBelayStatus(BELAY *belay, int Ox, int Oy)
{
int x = Ox;
int y = Oy;
int u;

   if( belay->l.status==T_OFF )
   {
      _setcolor(BACKGROUND);
      u = _GFILLINTERIOR;
      _ellipse(u,x-2,y-2,x+2,y+2);
      _setcolor(YELLOW);
      u = _GBORDER;
      _ellipse(u,x-2,y-2,x+2,y+2);
   }
   else
   {
      _setcolor(FIXEDCOLOR);
      u = _GFILLINTERIOR;
      _ellipse(u,x-2,y-2,x+2,y+2);
   }

   x += 7;
   if( belay->b.status==T_OFF )
   {
      _setcolor(BACKGROUND);
      u = _GFILLINTERIOR;
      _ellipse(u,x-2,y-2,x+2,y+2);
      _setcolor(YELLOW);
      u = _GBORDER;
      _ellipse(u,x-2,y-2,x+2,y+2);
   }
   else if( belay->b.status==T_ON_MAN_SLIDING )
   {
      u = _GFILLINTERIOR;
      _setcolor(SLIDINGCOLOR);
      _ellipse(u,x-2,y-2,x+2,y+2);
   }
   else if( belay->b.status==T_ON_MAN )
   {
      u = _GFILLINTERIOR;
      _setcolor(FIXEDCOLOR);
      _ellipse(u,x-2,y-2,x+2,y+2);
   }

   x += 7;
   if( belay->r.status==T_OFF )
   {
      _setcolor(BACKGROUND);
      u = _GFILLINTERIOR;
      _ellipse(u,x-2,y-2,x+2,y+2);
      _setcolor(YELLOW);
      u = _GBORDER;
      _ellipse(u,x-2,y-2,x+2,y+2);
   }
   else
   {
      u = _GFILLINTERIOR;
      _setcolor(FIXEDCOLOR);
      _ellipse(u,x-2,y-2,x+2,y+2);
   }

   return(0);
}

EraseScene(MAN *man,ROPE *rope)
{
   DisplayMan(man,BACKGROUND);
   DisplayRope(rope,BACKGROUND);
   return(0);
}

ReDisplayScene(int manindex,MAN man[],ROPE *rope,LEDGE ledges[],int nledges)
{
   DisplayLedges(ledges,nledges);
   DisplayMan(&man[manindex],HOTMAN);
   DisplayMan(&man[(manindex+1)%2],COLDMAN);
   DisplayRope(rope,ROPECOLOR);
   return(0);
}

DisplayMoveCount(int count)
{
char string1[30],string2[30];
   _settextposition(29,40);
   _settextcolor(HOTTEXT);
   strcpy(string1,"MOVE = ");
   itoa(count,string2,10);
   strcat(string1,string2);
   _outtext(string1);
   return(0);
}


TimeDiff(struct dostime_t *new,struct dostime_t *old)
{   
int newt = (int)new->second + 60*(int)new->minute;   
int oldt = (int)old->second + 60*(int)old->minute;
   return(newt-oldt); /*returns the time difference in seconds*/
}

DisplayClock(int time)
{
char string[24];
int minute,second;
   minute = time/60;
   second = time-minute*60;
   _settextcolor(YELLOW);
   _settextposition(29,56);
   itoa(minute,string,10);
   _outtext(string);
   _outtext(":");
   itoa(second,string,10);
   _outtext(string);
   _outtext("  ");
   return(0);
}

tune()
{

int quarter=10; 
int eighth = 5; 

if(kbhit() )
return(0);
   sound_(2*D,eighth);             /*start at low D*/

if(kbhit() )
return(0);
   sound_(G,quarter);
   sound_(A,quarter);
   sound_(B,eighth);
   sound_(D,eighth);
   sound_(E,eighth);
   sound_(HI_G,eighth);



if(kbhit() )
return(0);

   sound_(D,quarter);
   sound_(B,eighth);
   sound_(C,eighth);
   sound_(D,quarter);
   sound_(B,eighth);
   sound_(D,eighth);
if(kbhit() )
return(0);
   sound_(E,quarter);
   sound_(B,quarter);
   sound_(D,3*quarter);
if(kbhit() )
return(0);
   sound_(REST,2*quarter+eighth);
//sound_(2*G,eighth);
//sound_(REST,quarter);
//sound_(G,eighth);

//sound_(2*G,eighth);
//sound_(REST,eighth);
//sound_(2*FSHARP,eighth);
   sound_(B,eighth);
if(kbhit() )
return(0);
   sound_(E,quarter);
   sound_(DSHARP,quarter);
   sound_(B,eighth);
   sound_(GSHARP,eighth);
   sound_(A,eighth);
   sound_(CSHARP,eighth);
if(kbhit() )
return(0);
   sound_(E,quarter);
   sound_(HI_GSHARP,quarter);
   sound_(B,quarter);
   sound_(REST,eighth);
   sound_(E,eighth);
if(kbhit() )
return(0);
   sound_(HI_FSHARP,2*quarter);
   sound_(E,quarter);
   sound_(REST,quarter);
if(kbhit() )
return(0);
   sound_(HI_FSHARP,2*quarter+eighth);
   sound_(E,eighth);
   sound_(C,eighth);
   sound_(FSHARP,eighth);
   sound_(G,quarter);
if(kbhit() )
return(0);


   sound_(4*G,eighth);
   sound_(2*D,eighth);
   sound_(G,eighth);
   sound_(B,quarter);
   sound_(HI_G,4*quarter);


   return(0);
}




