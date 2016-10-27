#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdlib.h>
#include "atdefs.g"

int main(void);
int EditInstruct(void);
int DisplayLedge(LEDGE *ledge, int color);
int EditLedges(LEDGE ledges[],int nledges);

main()
{
FILE *fp;
LEDGE *ledges;
int nledges,nledges0,i;
char string[20];
int c,d;

   ledges = (LEDGE *)malloc(6*MX_LEDGES);

   _setvideomode(_VRES16COLOR);
   _settextposition(4,1);
   _outtext("Edit New or Old File <n/o>?");
   d = getch();
   _outtext("\nEnter name of ledge file:");
   scanf("%s",string);

   if( d=='o' )
      nledges0 = ReadLedges(ledges,string);
   else
      nledges0 = 0;

   nledges = EditLedges(ledges,nledges0);

   _clearscreen(_GCLEARSCREEN);
   _settextposition(5,1);
   _outtext("Save File <y/n>?");
   c = getch();
   if( c=='y' )
   {
      fp = fopen( string, "w" );
if( d=='o' )
      for(i=0;i<nledges;i++)
      {
         itoa( ledges[i].height , string , 10);
         fprintf( fp , "%s  ," , string );
         itoa( ledges[i].leftend , string , 10);
         fprintf( fp , "%s  ," , string );
         itoa( ledges[i].rightend , string , 10);
         fprintf( fp , "%s  ," , string );
         fprintf( fp , "\n");
      }
else
      for(i=nledges-1;i>=0;i--)
      {
         itoa( ledges[i].height , string , 10);
         fprintf( fp , "%s  ," , string );
         itoa( ledges[i].leftend , string , 10);
         fprintf( fp , "%s  ," , string );
         itoa( ledges[i].rightend , string , 10);
         fprintf( fp , "%s  ," , string );
         fprintf( fp , "\n");
      }
      fclose( fp );
   }
   _setvideomode(_DEFAULTMODE);

   return(0);
}



EditLedges(LEDGE ledges[],int nledges)
{
int i,j;
int c;
int nled = nledges;
int flag = 'm';
   _clearscreen(_GCLEARSCREEN);
   EditInstruct();

   if( nled==0 )
   {
      nled = 1;
      ledges[0].height   = 270;
      ledges[0].leftend  = 20;
      ledges[0].rightend = 600;
   }

   i = 0;
   DisplayLedges(ledges,nled);
   DisplayLedge(&ledges[i],14);

   while( (c=getkey()) != '' )
   {
      switch( c )
      {
         case 'n':
            i++;
            if( i==nled )
               i = 0;
            flag = 'm';
            break;
         case 'm':
            flag = 'm';
            break;
         case 's':
            flag = 's';
            break;
         case 'a':
            if( nled < MX_LEDGES )
            {
               nled++;
               i=nled-1;
               ledges[i].height = 270;
               ledges[i].leftend = 260;
               ledges[i].rightend =280;
               DisplayLedge(&ledges[i], 8 );
            }
            else
               printf("\b\a"); /*beep*/
            flag = 'm';
            break;
         case  75:         /*left*/
            DisplayLedge(&ledges[i], 0 );
            if( flag=='m' )
            {
               if( ledges[i].leftend - 10 > 0 )  
               {
                  ledges[i].leftend -= 10;
                  ledges[i].rightend -= 10;
               }
            }
            else
            {
               if( ledges[i].leftend + 10 < ledges[i].rightend )
                  ledges[i].rightend -= 10;
            }
            break;
         case  77:         /*right*/
            DisplayLedge(&ledges[i], 0 );
            if( flag=='m' )
            {
               if( ledges[i].rightend + 10 < 640)  
               {
                  ledges[i].leftend += 10;
                  ledges[i].rightend += 10;
               }
            }
            else
               if( ledges[i].rightend + 10 < 640)  
                  ledges[i].rightend += 10;
            break;
         case  72:         /*up*/
            DisplayLedge(&ledges[i], 0 );
            if( flag=='m' )
               if( ledges[i].leftend - 10 > 0 )  
                  ledges[i].height -= 10;
            break;
         case  80:         /*down*/
            DisplayLedge(&ledges[i], 0 );
            if( flag=='m' )
               if( ledges[i].leftend + 10 < 480 )  
                  ledges[i].height += 10;
            break;
         default:
            flag = 'm';
            break;
      }  /*ends switch*/
      DisplayLedges(ledges,nled);
      DisplayLedge(&ledges[i],14);
   }


   return(nled);
}

int EditInstruct(void)
{
   _settextposition(1,1);
   _outtext("<n>=next ledge");
   _outtext("\n<m>=move ledge");
   _outtext("\n<s>=size ledge");
   _outtext("\n<a>= add ledge");
   _outtext("\nUse arrow keys");
}

int getkey(void)
{
int c = getch();
   if( c==0 )
      c = getch();
   return(c);
}

int DisplayLedge(LEDGE *ledge, int color)
{
   _setcolor(color);
   _moveto(ledge->leftend,ledge->height);
   _lineto(ledge->rightend,ledge->height);
   _moveto(ledge->leftend,ledge->height-1);
   _lineto(ledge->rightend,ledge->height-1);
   return(0);
}
