/*++ATROPE2.C
has to do with the kinetics of man pulling on rope.
The 2 main functions are ExecuteRopeMoves() and NewSettleWeight()
which handle (respectively) the man/rope relation and the settling
(on ledge and rope).
--*/
#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include "atdefs.g"

/************************PULLING PHYSICS*********************************/

ExecuteRopeMoves(int *dX, int *dY, MAN *man, BELAY belay[],ROPE *rope,int mindex, int direction,int ledgesupport)
{
LIMB *limb,*oldlimb;
TIEPOINT *t,*otherbod;

   otherbod= &belay[(mindex+1)%2].b;
   
   t        = &belay[mindex].l;                  /*FOR THE LEFT HAND*/
   limb     = &man->lh;
   oldlimb  = &man->Olh;

   if( CheckLimbCentering(t,limb,ledgesupport ) )
      LimbRopeMove(limb,oldlimb,&belay[mindex],rope,t,otherbod,dX,dY,direction);


   t        = &belay[mindex].r;                  /*FOR THE RIGHT HAND*/
   limb     = &man->rh;
   oldlimb  = &man->Orh;
   if( CheckLimbCentering(t,limb,ledgesupport )  )
      LimbRopeMove(limb,oldlimb,&belay[mindex],rope,t,otherbod,dX,dY,direction);

   return(0);
}

CheckLimbCentering(TIEPOINT *t,LIMB *hand,int ledgesupport)
{
   if( t->status!=T_ON_MAN )
      return(0);
   if( (ledgesupport==0 && hand->X==0 )
   ||   ledgesupport>0                 )
      return(1);
   else
      return(0);
}

LimbRopeMove(LIMB *limb,LIMB *oldlimb,BELAY *belay,ROPE *rope,TIEPOINT *t,
                          TIEPOINT *otherbod,int *dX,int *dY,int direction)
{
int tempX=0,tempY=0;
int a,b,lift;
int inpiv,outpiv,inpull,outpull;

   if( t->status!=T_ON_MAN )                       /*if belayed to rope and*/
      return(0);

   swingBod(&tempX,&tempY,limb,oldlimb,direction); 

   if( tempX || tempY )             /*and arm motion is opposed to direction*/
   {

      inpull  = TestPull(rope,belay,otherbod,&a,&b,INROPE,&lift);
      inpiv   = isPivotForward(a,b,t->x,t->y,direction);
      outpull = TestPull(rope,belay,otherbod,&a,&b,OUTROPE,&lift);
      outpiv  = isPivotForward(a,b,t->x,t->y,direction);

      if( inpiv )                    /*if you are pulling on a tiepoint*/
      {
         if( inpull )                /*if fixed, move man*/
         {
            *dX = tempX;            
            *dY = tempY;
         }
         else                       /*if loose pull in some rope*/
            AdjustRopeEnds(rope,abs(tempX-tempY),END0,-1);
            
         if( outpull==0 && belay->b.status!=T_ON_MAN ) 
               AdjustRopeEnds(rope,abs(tempX-tempY),ENDL,1);
      }

      if( outpiv )
      {
         if( outpull )
         {
            *dX = tempX;
            *dY = tempY;
         }
         else
            AdjustRopeEnds(rope,abs(tempX-tempY),ENDL,-1);
            
         if( inpull==0 && belay->b.status!=T_ON_MAN ) 
               AdjustRopeEnds(rope,abs(tempX-tempY),END0,1);
      }
   }
   return(0);
}

TestPull(ROPE *rope,BELAY *belay,TIEPOINT *otherbod,int *a,int *b,int dir,int *lift)
{
int ret = 0;
TIE *ltie,*rtie,*btie,*sie;
int btemp;
   ltie = belay->l.tie;
   rtie = belay->r.tie; 
   btie = belay->b.tie;
   if( dir==1 )
      sie = &rope->tie[0];
   else
      sie = &rope->tie[1];

   while( sie!=NULL )
   {
      if( sie==ltie
      ||  sie==rtie
      ||  sie==btie )
      {
         btemp = *(sie->y);
         NextTie(&sie,-dir);
         *a = *(sie->x);
         *b = *(sie->y);
         if( *b < btemp )
            *lift = 1;
         else
            *lift = 0;
         break;
      }

      if( (sie==otherbod->tie && sie->status==T_ON_MAN)
      ||   sie->status==T_ON_LEDGE )
         ret = 1;

      NextTie(&sie,dir);
   }

   if( sie==NULL )
      ret = 0;

   return(ret);
}

AdjustRopeEnds(ROPE *rope,int length,int end,int sign)
{
   if( end==END0 )
   {
      rope->end0 += sign*length;
      if( rope->end0 < 10 )
         rope->end0 = 10;
   }
   else if( end==ENDL )
   {
      rope->endL += sign*length;
      if( rope->endL < 10 )
         rope->endL = 10;
   }
   return(0);
}


/*************************SETTLING  PHYSICS**************************/

NewSettleWeight(MAN *man,BELAY belay[],LEDGE ledges[],int N,ROPE *rope,int manindex)
{
int ledgesupport,inpull,outpull,ilift=0,olift=0,ia,ib,oa,ob;
TIEPOINT *otherbod;

   ledgesupport = LedgeSupport(man,ledges,N);

   if( ledgesupport > 2 )    
      return(0);
   if( OnBelay(&belay[manindex]) < 2  )
      return( StartFall(man,&belay[manindex],ledges,N,rope) );

   otherbod = &(belay[(manindex+1)%2].b);

   inpull  = TestPull(rope,&belay[manindex],otherbod,&ia,&ib, 1,&ilift);
   outpull = TestPull(rope,&belay[manindex],otherbod,&oa,&ob,-1,&olift);

   if( ilift + olift + ledgesupport > 2 )    /*hold a rope supported stance*/
      return(0);

   if(      inpull==1 && outpull==0 )
      return( SwingBaby( ia, ib,ilift, olift , man , &belay[manindex],rope,ledges,N) );
   else if( inpull==0 && outpull==1 )
      return( SwingBaby( oa, ob,ilift, olift , man , &belay[manindex],rope,ledges,N) );
   else if( inpull==0 && outpull==0 )
      return( StartFall(man,&belay[manindex],ledges,N,rope) );
   else /* if( inpull==1 && outpull==1 )*/
   {
      if( ia<=man->X && man->X<=oa
      ||  oa<=man->X && man->X<=ia )
         return( HangDrop(man,&belay[manindex],otherbod,ledges,N,rope) );
      else if( abs(ia - man->X) < abs(oa - man->X) )
         return( SwingBaby( ia, ib, ilift, olift ,man , &belay[manindex],rope,ledges,N) );
      else
         return( SwingBaby( oa, ob, ilift, olift ,man , &belay[manindex],rope,ledges,N) );
   }
}

NextTie(TIE **tie,int dir)
{
  if( dir==1 )
      *tie = (*tie)->post;
   else
      *tie = (*tie)->pre;
   return(0);
}

HangDrop(MAN *man, BELAY *belay,TIEPOINT *otherbod,LEDGE ledges[],int N,ROPE *rope)
{
int ilift=0,olift=0,ia,ib,oa,ob;
int y0,y1;
   y1 = y0 = man->Y;

   while( (ilift==0 || olift==0) && (y1-y0)<100 )
   {
      man->Y++;
      y1++;
      UpdateBelay(belay,man);
      DangleEnds(rope);

      if( ilift + olift + LedgeSupport(man,ledges,N) > 2 )
         return(0);

      TestPull(rope,belay,otherbod,&ia,&ib, 1,&ilift);
      TestPull(rope,belay,otherbod,&oa,&ob,-1,&olift);
   }

   if( y1-y0==100 )
      return( -1 );
   else
   {
      man->X = 10*((man->X+5)/10);           /*round to nearest "10" spot*/
      man->Y = 10*((man->Y+5)/10);
      return( 0 );
   }
}

SwingBaby(int a,int b,int ilift, int olift, MAN *man,BELAY *belay,ROPE *rope,
                                                     LEDGE ledges[],int N)
{
int x=man->X,y=man->Y;
int sign,lim;
double u,v,r;
   if( x<a )
      sign = 1;
   else if( x>a )
      sign = -1;
   else
      sign = 0;

   lim = b + 20;

   while( y < lim )       /*fall until the man is a few feet below pivot*/
   {
      y+=5;
      ReplaceMan(man,rope,belay,x,y);
      if( ilift + olift + LedgeSupport(man, ledges, N) > 2 )
         return(0);
   }

   u = (double)(x-a);
   v = (double)(y-b);
   r = sqrt( u*u + v*v );

   if( sign==0 )
      return(0);

   lim = a+(a-x);                      /*find mirror position of x rel a*/
   if( sign>0 )                        /*shorten the swing slightly*/
      lim -=20;
   else
      lim +=20;

   while( sign*x < sign*lim  )              /*then swing*/
   {
      x +=sign*5;
      y = Circle(x,a,b,r);
      ReplaceMan(man,rope,belay,x,y);
      if( ilift + olift + LedgeSupport(man, ledges, N) > 2 )
         return(0);
   }
   ReplaceMan(man,rope,belay,10*((man->X+5)/10),10*((man->Y+5)/10));

   return(0);
}


ReplaceMan(MAN *man,ROPE *rope,BELAY *belay,int x,int y)
{
   DisplayMan(man,BACKGROUND);              /*erase man*/
   DisplayRope(rope,BACKGROUND);
   man->X = x;                     /*change his coords*/
   man->Y = y;
   UpdateBelay(belay,man);        /*update what depends on him*/
   DangleEnds(rope);              /*redisplay stuff*/
   DisplayMan(man,HOTMAN);
   DisplayRope(rope,ROPECOLOR);
   return(0);
}

StartFall(MAN *man,BELAY *belay,LEDGE ledges[],int N,ROPE *rope)
{
int y0,y1;
   y1 = y0 = man->Y;
   while( (y1-y0)< 100 )               /*you start to fall*/
   {
      DisplayMan(man,HOTMAN);
      DisplayRope(rope,ROPECOLOR);
      if( LedgeSupport(man,ledges,N) > 2 )
         return(0);
      else
      {
         DisplayMan(man,BACKGROUND);
         DisplayRope(rope,BACKGROUND);
         man->Y += 10;
         y1     += 10;
         UpdateBelay(belay,man);
         DangleEnds(rope);
      }
   }
   return(-1);
}

OnBelay(BELAY *belay)
{
int bcount = 0;
   if( belay->l.status==T_ON_MAN )
      bcount += 1;
   if( belay->r.status==T_ON_MAN )
      bcount += 1;
   if( belay->b.status==T_ON_MAN )  /*a sliding body belay doesn't support*/
      bcount += 2;                  /*a fixed   body belay doubly supports*/
   return(bcount);
}

Circle(int x,int a,int b,double r)
{
double arg;
   arg = r*r-(double)((x-a)*(x-a));
   if( arg>0.0 )
      return( b + (int)sqrt(arg) );
   else
      return(b);
}

isPivotForward(int a, int b , int handX, int handY, int direction)
{
int ret = 0;
   switch(direction)
   {
      case UP:
         if( handY > b )
            ret = 1;
         break;
      case DOWN:
         if( handY < b )
            ret = 1;
         break;
      case RIGHT:
         if( handX < a )
            ret = 1;
         break;
      case LEFT:
         if( handX > a )
            ret = 1;
         break;
   }
   return(ret);
}



