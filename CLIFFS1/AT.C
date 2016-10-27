/*++AT.C is the file with main() and sub()
--*/

#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "atdefs.g"


main()
{
int score;

   _setvideomode(_VRES16COLOR);

   _settextposition(1,1);
   printf(" ! A T T A C K   T H E   C L I F F S !");
   _settextposition(4,3);
   printf(" CopyRite(C) 1991-1992, Peter Waksman (508)371-1890");

   getch();

   GameInstruct();

   while( getch()!='' )
   {
      _clearscreen(_GCLEARSCREEN);
      _setcolor(BACKGROUND);
      _rectangle(_GFILLINTERIOR,0,0,639,479);

      score = sub();                      /*Start the program*/

      _settextposition(1,1);
      printf("TOTAL NUMBER OF MOVES = %d  ",score);
      getch();
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
ROPE     rope;
TIEPOINT *pitons;
MAN      man[2],*currentMan;
BELAY    belay[2];
int      nledges;
int      direction=RIGHT;
int      movecount = 0;
int      manflag=1,manindex=0;
int      flag=1,turnstatus=READY;
int      belaykey=0,pitonkey=0;

/*******************************INITIALIZE*********************************/

   keys.data  = (int *)malloc(2*MX_KEYS);          /*allocate memory*/
   moves.data = (int *)malloc(2*MX_MOVES);
   ledges     = (LEDGE *)malloc(6*MX_LEDGES);
   pitons     = (TIEPOINT *)malloc( sizeof(TIEPOINT)*MX_PITONS );

   keys.i  = 0;                                    /*init indeces*/
   moves.i = 0;
   moves.data[0] = 0;

   clock.status = START;
   keys.status  = READY;                           /*init statuses*/
   moves.status = READY;
   tm.status    = READY;
   
   DisplayClock(0);

   FlipDirection(&direction,77);                   /*displays the arrow*/
   BelayKey(1);                                     /*and status displays*/               

   nledges = ReadLedges(ledges,"ledges");          /*read the "ledges" file*/
   DisplayLedges(ledges,nledges);

   InitMan(&man[0],110,250);                      /*init DougalH*/
   InitBelay(&belay[0],&man[0]);
   DisplayMan(&man[0],HOTMAN);

   InitMan(&man[1],70,250);                       /*init PeterB*/
   InitBelay(&belay[1],&man[1]);    
   DisplayMan(&man[1],COLDMAN);

   InitRope(&rope,70,250,110,250);                /*init rope*/

   BelayToRope(&rope,&(belay[0].b) ,3);       /*belay DougalH's body to rope*/
   BelayToRope(&rope,&(belay[1].b) ,3);       /*belay PeterB's  body to rope*/

   DangleEnds(&rope);

   DisplayRope(&rope,ROPECOLOR);
   DisplayBelayStatus(&belay[0],191,457);
   DisplayBelayStatus(&belay[1],71,457);

   DisplayMoveCount(0);

   InitPitons(pitons);                          /*init Pitons*/

   currentMan     = &man[0];

/**************************START MAIN LOOP*****************************/


   while( moves.status != DONE )
   {
      key_proc(&keys);
      move_proc( &moves, keys.data[keys.i]);
      tm_proc(&tm,KEYRATE);     
      turn_proc(&direction,keys.data[keys.i],&turnstatus);

      main_exec( &keys, &moves, &tm, &turnstatus, &manflag, &belaykey, &pitonkey);

      if( tm.status==READY )                                 /*if NOT moving*/
      {
         clock_proc(&clock);
         if( clock.status==HOT )
            DisplayClock( TimeDiff(&clock.end,&clock.begin) );

         man_exec(&manflag, &manindex, man, &currentMan);
         belay_exec(&rope,belay,manindex,&belaykey);
         piton_exec(&pitonkey,pitons,currentMan,&rope,ledges,nledges);
      }
      else if( tm.status==DONE )                            /*if moving*/
      {
         movecount++;
         DisplayMoveCount(movecount);

         EraseScene(currentMan, &rope);

         flag = MoveMan(currentMan,&moves,direction,ledges,nledges,
                                                       &rope,belay,manindex);
         ReDisplayScene(manindex,man,&rope,ledges,nledges);

         tm.status    = READY;               /*re-init the moves*/
         moves.i = 0;
      }
   }

/****************************END THE GAME***************************/
   free( keys.data );
   free( moves.data );
   free( pitons );
                                            
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


turn_proc(int *direction,int key,int *turnstatus)
{
   if( *turnstatus!=HOT )
      return(0);
   FlipDirection( direction , key);
   *turnstatus = READY;
   return(0);
}


main_exec(U_DATA *keys, U_DATA *moves, TM_DATA *tm, int *turnstatus,
            int *manflag, int *belaykey, int *pitonkey)
{
   if( keys->status!=HOT )
      return(0);
   switch( keys->data[keys->i] )
   {
      case '':
         moves->status = DONE;
         break;
      case 'o':
         *belaykey = 'o';
         break;
      case 'w':
         *belaykey = 'w';
         break;
      case 'b':
         *belaykey = 'b';
         break;
      case 'i':
         *pitonkey = 'i';
         break;
      case 'e':
         *pitonkey = 'e';
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
      case 60:                          /*F2 key*/
         *manflag = 1;
         break;
      case 59:                          /*F1 key*/
         *manflag = 2;
         break;
   }
   return(0);
}



