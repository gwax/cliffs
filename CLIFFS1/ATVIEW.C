/*++ATVIEW.C is the file with "eye of the climber" displays
--*/

#include <graph.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "atdefs.g"


#define  EXT   MX_HEXT 
#define  HEXT  20.0          /* 2*EXT  as a double*/
#define  VIEW  150.0
#define  HORIZON 300.0

int Perspective(int x,int y,int *a, int *b);


/*In displaying each ledge the (x,y) coordinates of the ledges are
re-mapped into (a,b) coordinates. The basic formulas (which get applied
to the ledge endpoints) are:
      a = a(x,y) = a0 + (x-manX)*f(y)
      b = b(x,y) = b0 + (y-manY)*g(x)
where (a0,b0) is the place the man's center will map to. The functions f(y)
and g(x) are functions varying with the movement direction.
   The display is clipped to within a pre-selected window (a1,b1)-(a2,b2).
*/

I_DisplayLedges(LEDGE ledges[],int N,MAN *man,int direction,
                  int a1,int b1, int a2, int b2,int color)
{
int i;
int a0,b0,a,b;
int x,y;
int manX=man->X,manY=man->Y;

   _setcolor(BACKGROUND);                              /*erase window*/
   _rectangle(_GFILLINTERIOR,a1,b1,a2,b2);
   _setcolor(GREEN);
   _rectangle(_GBORDER,a1-2,b1-2,a2+2,b2+2);

   _setcolor(color);
   _setcliprgn(a1,b1,a2,b2);

   switch( direction )
   {
      case UP:
         a0 = (a1+a2)/2;
         b0 = b2-2;
         for(i=0;i<N;i++)
         {
            y = ledges[i].height-manY;

            x = ledges[i].leftend-manX;
            Perspective(-y,x,&b,&a);
            a = a0 + a;
            b = b0 - b;
            _moveto(a,b);

            x = ledges[i].rightend-manX;
            Perspective(-y,x,&b,&a);
            a = a0 + a;
            b = b0 - b;
            _lineto(a,b);
         }

         _setcolor(YELLOW);
         _moveto(a0,b0-5);
         _lineto(a0,b0);
         _ellipse(_GBORDER,a0-2,b0-13,a0+2,b0-8);

         x = man->rh.X;
         y = -man->rh.Y;
         Perspective(-y,x,&b,&a);
         _moveto(a0+a,b0-b);
         _lineto(a0,b0-5);

         x = man->rf.X;
         y = man->rf.Y;
         Perspective(-y,x,&b,&a);
         _moveto(a0+a,b0-b);
         _lineto(a0,b0);

         x = -man->lh.X;
         y = -man->lh.Y;
         Perspective(-y,x,&b,&a);
         _moveto(a0+a,b0-b);
         _lineto(a0,b0-5);

         x = -man->lf.X;
         y = man->lf.Y;
         Perspective(-y,x,&b,&a);
         _moveto(a0+a,b0-b);
         _lineto(a0,b0);

         break;
      case DOWN:
         a0 = (a1+a2)/2;
         b0 = b1+2;
         for(i=0;i<N;i++)
         {
            y = ledges[i].height-manY;

            x = ledges[i].leftend-manX;
            Perspective(y,x,&b,&a);
            a = a0 + a;
            b = b0 + b;
            _moveto(a,b);

            x = ledges[i].rightend-manX;
            Perspective(y,x,&b,&a);
            a = a0 + a;
            b = b0 + b;
            _lineto(a,b);
         }

         _setcolor(YELLOW);
         _moveto(a0,b0-5);
         _lineto(a0,b0);
         _ellipse(_GBORDER,a0-2,b0-13,a0+2,b0-8);

         x = man->rh.X;
         y = -man->rh.Y;
         Perspective(y,x,&b,&a);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0-5);

         x = man->rf.X;
         y = man->rf.Y;
         Perspective(y,x,&b,&a);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0);

         x = -man->lh.X;
         y = -man->lh.Y;
         Perspective(y,x,&b,&a);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0-5);

         x = -man->lf.X;
         y = man->lf.Y;
         Perspective(y,x,&b,&a);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0);

         break;
      case LEFT:
         b0 = (b1+b2)/2;
         a0 = a2-2;
         for(i=0;i<N;i++)
         {
            y = ledges[i].height-manY;

            x = ledges[i].leftend-manX;
            Perspective(-x,y,&a,&b);
            a = a0 - a;
            b = b0 + b;
            _moveto(a,b);

            x = ledges[i].rightend-manX;
            Perspective(-x,y,&a,&b);
            a = a0 - a;
            b = b0 + b;
            _lineto(a,b);
         }

         _setcolor(YELLOW);
         _moveto(a0,b0-5);
         _lineto(a0,b0);
         _ellipse(_GBORDER,a0-2,b0-13,a0+2,b0-8);

         x = man->rh.X;
         y = -man->rh.Y;
         Perspective(-x,y,&a,&b);
         _moveto(a0-a,b0+b);
         _lineto(a0,b0-5);

         x = man->rf.X;
         y = man->rf.Y;
         Perspective(-x,y,&a,&b);
         _moveto(a0-a,b0+b);
         _lineto(a0,b0);

         x = -man->lh.X;
         y = -man->lh.Y;
         Perspective(-x,y,&a,&b);
         _moveto(a0-a,b0+b);
         _lineto(a0,b0-5);

         x = -man->lf.X;
         y = man->lf.Y;
         Perspective(-x,y,&a,&b);
         _moveto(a0-a,b0+b);
         _lineto(a0,b0);

         break;
      case RIGHT:
         b0 = (b1+b2)/2;
         a0 = a1+2;
         for(i=0;i<N;i++)
         {
            y = ledges[i].height-manY;

            x = ledges[i].leftend-manX;
            Perspective(x,y,&a,&b);
            a = a0 + a;
            b = b0 + b;
            _moveto(a,b);

            x  = ledges[i].rightend-manX;
            Perspective(x,y,&a,&b);
            a = a0 + a;
            b = b0 + b;
            _lineto(a,b);
         }

         _setcolor(YELLOW);
         _moveto(a0,b0-5);
         _lineto(a0,b0);
         _ellipse(_GBORDER,a0-2,b0-13,a0+2,b0-8);

         x = man->rh.X;
         y = -man->rh.Y;
         Perspective(x,y,&a,&b);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0-5);

         x = man->rf.X;
         y = man->rf.Y;
         Perspective(x,y,&a,&b);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0);

         x = -man->lh.X;
         y = -man->lh.Y;
         Perspective(x,y,&a,&b);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0-5);

         x = -man->lf.X;
         y = man->lf.Y;
         Perspective(x,y,&a,&b);
         _moveto(a0+a,b0+b);
         _lineto(a0,b0);

         break;
   }
//   _setcolor(YELLOW);
//   _ellipse(_GFILLINTERIOR,a0-2,b0-2,a0+2,b0+2);

   _setcliprgn(0,0,639,479);
   return(0);
}


Perspective(int x,int y,int *a, int *b)
{
double f,t;
   if( x< -EXT )
      x = -EXT;
   t = (double)x;
   f = 1.0/(HEXT+t);

   *a = (int)(t*f*VIEW);
   *b = (int)((double)y*f*HEXT);

   return(0);
}

/*
Perspective(int x,int y,int *a, int *b)
{
double t=(double)x,s;
   if( t>HORIZON )
      t = HORIZON;
   s = 1.0 - t/(2.0*HORIZON);
   *a = (int)(t*s);
   *b = (int)((double)y*s);
   return(0);
}
*/
