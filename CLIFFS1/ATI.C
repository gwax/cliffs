/*++ATI.C is the file with main() and sub() for "I" point of view.
--*/

#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "atdefs.g"

int imain_exec(U_DATA *keys, U_DATA *moves, TM_DATA *tm, int *turnstatus);
int iMoveMan(MAN *man, U_DATA *moves,int direction, LEDGE ledges[],int N);
int turn_exec(int *direction,int key,int *turnstatus,LEDGE ledges[],int N,MAN *man);

#define WX1  440
#define WY1  275
#define WX2  620
#define WY2  435


main()
{
int score;

   _setvideomode(_VRES16COLOR);

   _settextposition(1,1);
   printf(" ! A T T A C K   T H E   C L I F F S !");
   _settextposition(3,3);
   printf(" CopyRite(C) 1992 Peter Waksman ");

   GameInstruct();
/*   MiniKey(); */

   while(getch()!='' )
   {
      _clearscreen(_GCLEARSCREEN);
      _setcolor(BACKGROUND);
      _rectangle(_GFILLINTERIOR,0,0,639,479);
      score = sub();

      _clearscreen(_GCLEARSCREEN);
      _settextposition(1,1);
      printf("TOTAL NUMBER OF MOVES = %d  ",score);

      GameInstruct();
   }
   _setvideomode(_DEFAULTMODE);
   return(0);
}

int sub(void)
{
U_DATA   keys,moves;
TM_DATA  tm,clock;
LEDGE    *ledges;
MAN      man[2],*currentMan;
int      nledges;
int      direction=RIGHT;
int      movecount = 0;
int      manflag=1,manindex=0;
int      flag=1,turnstatus=READY;
int      belaykey=0,pitonkey=0;

   keys.data = (int *)malloc(2*MX_KEYS);           /*allocate memory*/
   moves.data = (int *)malloc(2*MX_MOVES);
   ledges     = (LEDGE *)malloc(6*MX_LEDGES);

   keys.i  = 0;                                    /*init indeces*/
   moves.i = 0;
   moves.data[0] = 0;

   clock.status = START;
   keys.status  = READY;                           /*init statuses*/
   moves.status = READY;
   tm.status    = READY;
   
   DisplayClock(0);

   FlipDirection(&direction,77);                   /*displays the arrow*/

   nledges = ReadLedges(ledges,"ledges");          /*read the "ledges" file*/
//   DisplayLedges(ledges,nledges);

   InitMan(&man[0],110,250);                      /*init DougalH*/
   DisplayMan(&man[0],HOTMAN);

   DisplayMoveCount(0);

   currentMan     = &man[0];

   I_DisplayLedges(ledges,nledges,currentMan,direction,WX1,WY1,WX2,WY2,LEDGECOLOR);

/**************************END OF INITIALIZATION**********************/

   while( moves.status != DONE )
   {
      key_proc(&keys);
      move_proc( &moves, keys.data[keys.i]);
      tm_proc(&tm,KEYRATE);                  /*had 0.15*/

      imain_exec( &keys, &moves, &tm, &turnstatus);

      if( tm.status==READY )                                 /*if NOT moving*/
      {
         clock_proc(&clock);
         if( clock.status==HOT )
            DisplayClock( TimeDiff(&clock.end,&clock.begin) );

         turn_exec(&direction,keys.data[keys.i],&turnstatus,
                   ledges,nledges,currentMan);
      }
      else if( tm.status==DONE )                            /*if moving*/
      {
         movecount++;
         DisplayMoveCount(movecount);

         DisplayMan(currentMan, 0);

         flag = iMoveMan(currentMan,&moves,direction,ledges,nledges);

//         DisplayLedges(ledges,nledges);
         DisplayMan(currentMan, 14);

         I_DisplayLedges(ledges,nledges,currentMan,direction,
                         WX1,WY1,WX2,WY2,LEDGECOLOR);

         tm.status    = READY;               /*re-init the moves*/
         moves.i = 0;
      }
   }

   free( keys.data );
   free( moves.data );
                                             /*ENDGAMES*/                 
   if( flag==-1 )
      fall(currentMan);
   else if(  flag==0
        &&(  currentMan->Y + currentMan->lf.Y ==ledges[0].height
          || currentMan->Y + currentMan->rf.Y ==ledges[0].height)   )
      Congratulations();
   else
      Retreat();

   return(movecount);
}


turn_exec(int *direction,int key,int *turnstatus, LEDGE ledges[],int N,MAN *man)
{
   if( *turnstatus!=HOT )
      return(0);
   FlipDirection( direction , key);

   I_DisplayLedges(ledges,N,man,*direction,WX1,WY1,WX2,WY2,LEDGECOLOR);

   *turnstatus = READY;
   return(0);
}

imain_exec(U_DATA *keys, U_DATA *moves, TM_DATA *tm, int *turnstatus)
{
   if( keys->status!=HOT )
      return(0);
   switch( keys->data[keys->i] )
   {
      case '':
         moves->status = DONE;
         break;
      case 72:
      case 80:
      case 75:
      case 77:
/*         FlipDirection( direction , keys->data[keys->i]);*/
         *turnstatus = HOT;
         break;
      case 'p':
      case 'l':
      case 'k':
      case 'm':
      case 'q':
      case 'a':
      case 's':
      case 'x':
         moves->status = M_HOT;
         tm->status    = START;
         break;
   }
   return(0);
}


iMoveMan(MAN *man, U_DATA *moves,int direction, LEDGE ledges[],int N)
{
int dX=0,dY=0;
int ret=0;
                       /*move limbs and calculate body change if attached*/
   ExecuteMoves(&dX, &dY, man, moves, direction, ledges, N);

   man->X -=dX;                        /*adjust body position if appropriate*/
   man->Y +=dY;

   CopyAllLimbs(man);                             /*and update limb history*/

   ret = settleWeight(man,ledges,N);  
                            
   if( ret==-1 )
      moves->status = DONE;

   return(ret);
}

