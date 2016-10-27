/*++ATROPE.C deals with the mechanics of attaching the rope to "belays"
(parts of the man: lefthand, righthand, body) and to "pitons" (attachments
to ledges).
--*/

#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>

#include "atdefs.g"

/********************************BELAYS******************************/
belay_exec(ROPE *rope, BELAY belay[], int manindex, int *belaykey)
{
int statecount;
TIEPOINT *t;
BELAY *blay;
   if( *belaykey==0 )
      return(0);
   if( *belaykey=='e' || *belaykey=='i' )
   {

      *belaykey=0;
      return(0);
   }

   blay = &belay[manindex];

   DisplayRope(rope,BACKGROUND);

   switch( *belaykey )
   {
      case 'w':                  /*left hand*/
         t = &(blay->l);
         statecount=2;
         break;
      case 'b':                  /*body*/
         t = &(blay->b);
         statecount=3;
         break;                  
      case 'o':                  /*right hand*/
         t = &(blay->r);
         statecount=2;
         break;
   }

   BelayToRope(rope, t, statecount);

   DangleEnds(rope);

   DisplayRope(rope,ROPECOLOR);
   DisplayBelayStatus(&belay[0],191,457);
   DisplayBelayStatus(&belay[1],71,457);

   *belaykey = 0;
   return(0);
}

BelayToRope(ROPE *rope, TIEPOINT *t, int statecount)
{
int i, status=t->status;
TIE *at;
   if( status==T_ON_MAN && statecount==3 )
   {
      t->status = T_ON_MAN_SLIDING;
      (t->tie)->status = T_ON_MAN_SLIDING;
      return(0);
   }
   else if( status==T_ON_MAN || status==T_ON_MAN_SLIDING )
   {
      if( DeleteTie(t->tie) )
         t->status =T_OFF;
      return(0);
   }
                           /*else status==T_OFF and we want to add a tie*/
   if( CheckRopeTouch(t->x,t->y,rope,&at) < 0 )
      return(-1);
   if( (i=GetFreeIndex(rope))==MX_TIES )
      return(-1);

   AddTiepoint(at,&(rope->tie[i]),t,T_ON_MAN);

   return(0);
}

DeleteTie(TIE *tie)
{
   if( tie->pre==NULL || tie->post==NULL )
      return(0);
   
   tie->status = T_OFF;

   (tie->post)->pre = tie->pre;
   (tie->pre)->post = tie->post;
   return(1);
}


AddTiepoint(TIE *at, TIE *tie, TIEPOINT *t,int status)
{
   if( InsertTie(at,tie) )
   {
      tie->x   = &(t->x);                   /*attach tie to tiepoint*/
      tie->y   = &(t->y);
      t->status= status;
      t->tie   = tie;                       /*attach tiepoint to tie*/
      (t->tie)->status = status;
      return(1);
   }
   else
      return(0);
}

InitRope(ROPE *rope, int x0,int y0, int xL, int yL)
{
   rope->x0 = x0;
   rope->y0 = y0;
   rope->xL = xL;
   rope->yL = yL;

   rope->end0 = 10;
   rope->endL = 10;

   rope->tie[0].x  = &(rope->x0);
   rope->tie[0].y  = &(rope->y0);
   rope->tie[1].x  = &(rope->xL);
   rope->tie[1].y  = &(rope->yL);

   rope->tie[0].status = T_OFF;
   rope->tie[1].status = T_OFF;

   rope->tie[0].pre = NULL;
   rope->tie[0].post= &(rope->tie[1]);
   rope->tie[1].pre = &(rope->tie[0]);
   rope->tie[1].post= NULL;

   return(0);
}

DangleEnds(ROPE *rope)
{
TIE *tie;
   tie = rope->tie[0].post;
   rope->x0 = *(tie->x);
   rope->y0 = (*(tie->y))+rope->end0;

   tie = rope->tie[1].pre;
   rope->xL = *(tie->x);
   rope->yL = (*(tie->y))+rope->endL;

   return(0);
}


InsertTie(TIE *at, TIE *tie)    
{
   if( at->post !=NULL )          /*insert "tie" after "at" */
   {
      tie->pre  = at;                  /*link tie between at and at->post*/
      tie->post = at->post;

      (at->post)->pre = tie;           /*fix the "up" link of at->post*/
      at->post  = tie;                 /*and the "down" link of at*/
      return(1);
   }
   else if( at->pre!=NULL )       /*or insert "tie" before "at" */
   {
      tie->post = at;
      tie->pre  = at->pre;
      (at->pre)->post = tie;
      at->pre   = tie;
      return(1);
   }
   return(0);
}

CheckRopeTouch(int x, int y, ROPE *rope, TIE **at)
{
TIE *tie,*sie;
   tie = &( rope->tie[0] );
   sie = tie->post;
   while( sie!=NULL )    /*looping through rope with adjacent ties*/
   {
      if( isNearRope(x,y,*tie->x,*tie->y,*sie->x,*sie->y)  )
      {
         *at = tie;
         return(0);
      }
      tie = sie;
      sie = sie->post;
   }
   return(-1);
}

isNearRope(int a, int b, int x1, int y1, int x2, int y2)
{
int p, tol=4;
   if( x1==x2 )               /*rope is vertical*/
   {
      if( a!=x1 )             /*test x-coordinate*/
         return(0);
      if( (b-y1)*(b-y2)>0 )   /*test between-ness of y-coords*/
         return(0);
      return(1);
   }

   if( (a-x1)*(a-x2)>0 )      /*test between-ness of x-coords*/
      return(0);
                              /*find point on sloping rope*/
   p = y1 + (int)( (double)(a-x1) * (double)(y2-y1)/(double)(x2-x1) );

   if( p-tol<=b && p+tol>=b )
      return(1);
   else
      return(0);
}


InitBelay(BELAY *belay, MAN *man)
{
   belay->l.status = T_OFF;
   belay->b.status = T_OFF;
   belay->r.status = T_OFF;

   UpdateBelay(belay,man);

   return(0);
}

UpdateBelay(BELAY *belay, MAN *man)
{
int x=man->X,y=man->Y;
   (belay->b).x = x;
   (belay->b).y = y;
   (belay->l).x = x - (man->lh.X);
   (belay->l).y = y - (man->lh.Y);
   (belay->r).x = x + (man->rh.X);
   (belay->r).y = y - (man->rh.Y);
   return(0);
}

GetFreeIndex(ROPE *rope)
{
int i;
TIE *tie,*test;
   for(i=0;i<MX_TIES;i++)
   {
      test= &(rope->tie[i]);
      tie = &(rope->tie[0]);
      while(tie!=NULL)                  /*loop through all ties on rope*/
      {
         if( tie==test )
            break;
         tie = tie->post;
      }
                       /*if you went through rope without seeing "tie[i]"*/
      if( tie==NULL )   
         break;
   }
   return(i);
}


DisplayRope(ROPE *rope,int color)
{
TIE   *tie;

   _setcolor(color);

   tie = &(rope->tie[0]);
   _moveto( *tie->x, *tie->y);
   tie = tie->post;

   while( tie!= NULL )
   {
      _lineto( *tie->x, *tie->y);
      tie = tie->post;
   }
   return(0);
}



/************************PITONS**************************************/
piton_exec(int *pitonkey,TIEPOINT pitons[],MAN *man,ROPE *rope,
                                                  LEDGE ledges[],int nledges)
{
int x,y,i;
TIE *at;
   if( *pitonkey==0 )
      return(0);
   switch( *pitonkey )
   {
      case 'i':
         if( isAttached(man,&(man->rh),ledges,nledges)  )
         {                                           /*if touching ledges*/
            x = man->X + (man->rh).X;
            y = man->Y - (man->rh).Y;
            if( CheckRopeTouch(x,y,rope,&at)<0 )    /*and touching rope*/
               return(-1);
                                                 /*get first piton at (x,y)*/
                                               /*or index of first available*/
            if( (i = GetPitonIndex(pitons,x,y)) == MX_PITONS )
               return(-1);

            if( PitonToRope(&pitons[i],rope,at,x,y)==-1 )
               return(-1);
         }
         break;
      case 'e':
         if( isAttached(man,&(man->lh),ledges,nledges) )
         {
            x = man->X - (man->lh).X;
            y = man->Y - (man->lh).Y;
            if( CheckRopeTouch(x,y,rope,&at)<0 )     /*and touching rope*/
               return(-1);
                                          /*get first piton at (x,y)*/
                                          /*or index of first available*/
            if( (i = GetPitonIndex(pitons,x,y)) == MX_PITONS )
               return(-1);
            
            if( PitonToRope(&pitons[i],rope,at,x,y)==-1 )
               return(-1);
         }
         break;
   }

   DangleEnds(rope);

   DisplayMan(man,HOTMAN);
   DisplayRope(rope,ROPECOLOR);
   DisplayPitons(pitons);
   *pitonkey = 0;

   return(0);
}

PitonToRope(TIEPOINT *piton,ROPE *rope,TIE *at,int x,int y)
{
int j;
   switch( piton->status )
   {
      case T_ON_LEDGE:
         piton->status = T_ON_LEDGE_SLIDING;
         (piton->tie)->status = T_ON_LEDGE_SLIDING;
         break;
      case T_ON_LEDGE_SLIDING:                   /*If deleting a piton*/
         DisplayRope(rope,BACKGROUND);
         DeleteTie( piton->tie );
         piton->status = T_OFF;
         _setcolor(BACKGROUND);
         _ellipse(_GFILLINTERIOR,x-2,y-2,x+2,y+2);
         _setcolor(SLIDINGCOLOR);                /*this fixes a monitor bug*/
         _ellipse(_GFILLINTERIOR,x-2,y-2,x+2,y+2);
         _setcolor(BACKGROUND);
         _ellipse(_GFILLINTERIOR,x-2,y-2,x+2,y+2);
         break;
      default:                                     /*If adding a new piton*/
         if( (j = GetFreeIndex(rope))== MX_TIES )
            return(-1);

         DisplayRope(rope,BACKGROUND);
         piton->status = T_ON_LEDGE;
         piton->x      = x;
         piton->y      = y;
         piton->tie    = at;
         AddTiepoint(at,&(rope->tie[j]),piton,T_ON_LEDGE);
         break;
   }
   return(0);
}


InitPitons(TIEPOINT pitons[])
{
int i;
   for(i=0;i<MX_PITONS;i++)
   {
      pitons[i].status = T_OFF;
      pitons[i].x      = -1;
      pitons[i].y      = -1;
      pitons[i].tie    = NULL;
   }
   return(0);
}

DisplayPitons(TIEPOINT pitons[])
{
int x,y,i,status;
   for(i=0;i<MX_PITONS;i++)
   {
      if( (status=pitons[i].status)==T_OFF )
         continue;
      else
      {
         if( status==T_ON_LEDGE )
            _setcolor(FIXEDCOLOR);
         else
            _setcolor(SLIDINGCOLOR);

         x = pitons[i].x;
         y = pitons[i].y;

         _ellipse(_GFILLINTERIOR,x-2,y-2,x+2,y+2);
      }
   }
   return(0);
}

GetPitonIndex(TIEPOINT pitons[], int x, int y)
{
int i;
   for(i=0;i<MX_PITONS;i++)
      if( x==pitons[i].x && y==pitons[i].y )
         break;
   if( i<MX_TIES )               /*if a piton was touching, return its index*/
      return(i);
   else
      for(i=0;i<MX_TIES;i++)     /*or find the next availabe piton*/
         if( T_OFF==pitons[i].status )
            break;
   return(i);
}

PrintRope(ROPE *rope)
{
TIE *tie;
int j=0;
   _settextposition(1,1);
   tie = &(rope->tie[0]);
   while( tie!=NULL )
   {
      printf("\ncounter=%d [pre,tie,post]=[%d,%d,%d] (x,y)=(%d,%d) status=%d",
                j,tie->pre,tie,tie->post,*(tie->x),*(tie->y),tie->status);
      j++;
      tie = tie->post;
   }
   printf("\n                                                              ");
   return(0);
}

