#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

putpik(char string[],int x,int y)
{
int i,j;
unsigned int x1,y1,x2,y2;
FILE *fp;
int high,wide;
int val,val1,val2;

   strcat( string, ".pik");
   fp = fopen(string,"r");

   x1  = x;
   y1  = y;

   x2 = x1 + 400;
   y2 = y1 + 400;

   for(i=x1;i<x2;i++)
      for(j=y1;j<y2;j++)
      {
         val = fgetc(fp);
         _setcolor( val );
         _setpixel(i,j);
      }
   fclose(fp);
}

#ifdef HADPONCE
oldputpik(string,x,y)
char *string;
int x,y;
{
int i;
char val;
int xd,yd;
unsigned int size;
char far *buffer;
FILE *fp;

      strcat( argv[1], ".pik");
   fp = fopen(string,"r");
                                      /*read the first few chars*/
   buffer = (char far *)_fmalloc(5);
   for(i=0;i<4;i++)
   {
      val = (char)fgetc(fp);
      *(buffer+i) =val;
   }

   xd = 256*(int)buffer[1];
   if( (int)buffer[0]<0 )
      xd += 256+(int)buffer[0];
   else
      xd += (int)buffer[0];

   yd = 256*(int)buffer[3];
   if( (int)buffer[2]<0 )
      yd += 256+(int)buffer[2];
   else
      yd += (int)buffer[2];

/*printf("\nxd=%d yd=%d",xd,yd);*/
                                      /*to size the image*/
   size = 4 + 4*((xd+7)/8)*yd;
   _ffree(buffer);

   rewind(fp);                        /*reset to beginning of file*/
   
   buffer = (char far *)_fmalloc(size);

   for(i=0;i<=size;i++)               /*read full image*/
   {
      val = (char)fgetc(fp);
      *(buffer+i) =val;
   }

                                     /*select display location*/
/*   x = y = 100;
   dot_on(&x,&y);
*/
   _putimage(x,y,buffer,_GPSET);

   fclose(fp);
   _ffree(buffer);

   getch();
   return(0);

}

#endif
