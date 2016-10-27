/*++
ATMOVE.C is the file for man's movements
--*/

#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>
#include "atdefs.g"


man_exec(int *manflag, int *manindex, MAN man[], MAN **currentMan)
{
   if( *manflag==0 )
      return(0);
   else if( *manflag==1 )                    /*DougalH is moving*/
   {
      *manindex = 0;
      *currentMan    = &man[0];
      DisplayMan(&man[0],HOTMAN); 
      DisplayMan(&man[1],COLDMAN);          /*and PeterB is freezing*/
      BelayKey(1);
   }
   else if( *manflag==2)                     /*or PeterB is moving*/
   {
      *manindex = 1;
      *currentMan    = &man[1];
      DisplayMan(&man[1],HOTMAN);  
      DisplayMan(&man[0],COLDMAN);         /*and DougalH is freezing*/
      BelayKey(2);
   }

   *manflag=0;
   return(0);
}

MoveMan(MAN *man, U_DATA *moves,int direction, LEDGE ledges[],int N,
                                ROPE *rope, BELAY belay[],int manindex)
{
int dX=0,dY=0;
int ret=0;
int ledgesupport;
                       /*move limbs and calculate body change rel ledges*/
   ExecuteMoves(&dX, &dY, man, moves, direction, ledges, N);

                                       /*or calculate pull rel rope*/
   if( dX==0 && dY==0 )
   {
      ledgesupport = LedgeSupport(man,ledges,N);
      ExecuteRopeMoves(&dX,&dY,man,belay,rope,manindex,direction,ledgesupport);
   }
   man->X -=dX;                        /*adjust body position if appropriate*/
   man->Y +=dY;

   CopyAllLimbs(man);                             /*and update limb history*/
   UpdateBelay(&belay[manindex],man);             /*and update belay*/
   DangleEnds(rope);                              /*and update rope ends*/

   ret = NewSettleWeight(man,belay,ledges,N,rope,manindex);

/*   ret = settleWeight(man,ledges,N);  SAVE FOR ROPELESS CODE*/
                            
   if( ret==-1 )
      moves->status = DONE;

   return(ret);
}

ExecuteMoves(int *dX, int *dY, MAN *man, U_DATA *moves,int direction,
                                                      LEDGE ledges[],int N)
{
int i;
int rhdX=0,rhdY=0,lhdX=0,lhdY=0,rfdX=0,rfdY=0,lfdX=0,lfdY=0;

   for(i=0;i<moves->i;i++)
      MoveLimbKey(moves->data[i],man,direction);

   if( isAttached( man, &man->Orh, ledges, N ) )
      swingBod( &rhdX, &rhdY, &man->rh, &man->Orh ,direction); 
   if( isAttached( man, &man->Olh, ledges, N ) )
      swingBod( &lhdX, &lhdY, &man->lh, &man->Olh , direction); 
   if( isAttached( man, &man->Orf, ledges, N ) )
      swingBod( &rfdX, &rfdY, &man->rf, &man->Orf , direction);
   if( isAttached( man, &man->Olf, ledges, N ) )
      swingBod( &lfdX, &lfdY, &man->lf, &man->Olf , direction);

   if( rhdX )
      *dX = rhdX;
   else if( lhdX )
      *dX = lhdX;
   else if( rfdX )
      *dX = rfdX;
   else if( lfdX )
      *dX = lfdX;
   else
      *dX = 0;

   if( rhdY )
      *dY = rhdY;
   else if( lhdY )
      *dY = lhdY;
   else if( rfdY )
      *dY = rfdY;
   else if( lfdY )
      *dY = lfdY;
   else
      *dY = 0;

   return(0);
}

MoveLimbKey(int move, MAN *man, int direction)
{
LIMB *limb;
int sign;
   switch( move )
   {
      case 'p':                          /*RIGHT HAND*/
         limb = &man->rh;
         sign = 1;
         break;
      case 'l':
         limb = &man->rh;
         sign = -1;
         break;
      case 'q':                           /*LEFT HAND*/
         limb = &man->lh;
         sign = 1;
         break;
      case 'a':
         limb = &man->lh;
         sign = -1;
         break;
   
      case 'k':                           /*RIGHT FOOT*/
         limb = &man->rf;
         if( direction==UP || direction==DOWN )
            sign = -1;
         else
            sign = 1;
         break;
      case 'm':
         limb = &man->rf;
         if( direction==UP || direction==DOWN )
            sign = 1;
         else
            sign = -1;
         break;
      case 's':                         /*LEFT FOOT*/
         limb = &man->lf;
         if( direction==UP || direction==DOWN )
            sign = -1;
         else
            sign = 1;
         break;
      case 'x':
         limb = &man->lf;
         if( direction==UP || direction==DOWN )
            sign = 1;
         else
            sign = -1;
         break;
   }

   MoveLimb( limb, sign, direction );

   return(0);
}


MoveLimb(LIMB *limb, int sign, int direction )
{
int a;
   if( direction==LEFT || direction==RIGHT )
   {
      a = limb->X;
      
      a += sign*DEXT;
      if( a<0 )
         a = 0;
      if( a>MX_HEXT )
         a = MX_HEXT;

      limb->X = a;
   }
   else /*if UP or DOWN*/
   {
      a = limb->Y;
      
      a += sign*DEXT;
      if( a<0 )
         a = 0;
      if( a>MX_VEXT )
         a = MX_VEXT;

      limb->Y = a;
   }
   return(0);
}

CopyAllLimbs(MAN *man)
{
   limbCopy(&man->rh,&man->Orh);
   limbCopy(&man->lh,&man->Olh);
   limbCopy(&man->lf,&man->Olf);
   limbCopy(&man->rf,&man->Orf);
   return(0);
}
limbCopy(LIMB *source,LIMB *dest)
{            
   dest->LR     = source->LR;   
   dest->DU     = source->DU;   
   dest->X      = source->X;
   dest->Y      = source->Y;

   return(0);
}

isAttached(MAN *man, LIMB *limb,LEDGE ledges[],int N)
{
int i;
int x,y;
   x = man->X + limb->LR * limb->X;
   y = man->Y - limb->DU * limb->Y;
   for(i=0;i<N;i++)
   {
      if(  y == ledges[i].height  
      &&   x >= ledges[i].leftend 
      &&   x <= ledges[i].rightend )
         return(1);
   }
   return(0);
}

swingBod(int *dX, int *dY, LIMB *new, LIMB *old,int direction)
{

int   d = new->X - old->X;
int   e = new->Y - old->Y;

   d *= new->LR;     /*same as old->LR*/
   e *= new->DU;

   if(  ( d>0 && direction==LEFT  )
     || ( d<0 && direction==RIGHT )
     )
      *dX = d;
   if(  ( e>0 && direction==DOWN  )
     || ( e<0 && direction==UP    )
     )
      *dY= e;   

   return(0);
}

LedgeSupport(MAN *man,LEDGE ledges[],int N)
{
int attcount = 0;
   if( isAttached(man,&man->rh, ledges, N) )
      attcount +=1;
   if( isAttached(man,&man->lh, ledges, N) )
      attcount +=1;
   if( isAttached(man,&man->lf, ledges, N) )
      attcount +=2;
   if( isAttached(man,&man->rf, ledges, N) )
      attcount +=2;

   return(attcount);
}

settleWeight(MAN *man,LEDGE ledges[],int N)
{
int attcount;
   while(1)
   {
      DisplayMan(man,HOTMAN);

      attcount = LedgeSupport(man,ledges, N);
      
      if( attcount>2 )
         break;

      if( man->Y > 350 )
         return(-1);

      DisplayMan(man,BACKGROUND);

      (man->Y)++;
   }
   return(0);
}

