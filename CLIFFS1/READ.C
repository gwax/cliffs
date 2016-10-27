#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <graph.h>

#include "newdefs.g"

                                   /*reads the file named "ledges"*/
                                   /*returns the number of ledges*/
int ReadLedges(LEDGE ledges[],char filename[]); 

int ReadLedges(LEDGE ledges[], char filename[])
{
FILE *fp;
char string[120],temp[20];
int i = 0;
int k,top;
int c;


	fp = fopen(filename,"r");
	
	do
	{
		if( NULL == fgets( string,120, fp ) )
			break;

                                                 /*get ledge height*/
		for(k=0;k<20;k++)
		{
			if( (c=(int)string[k])==',' )
				break;
			else
				temp[k] = string[k];
		}
		top = k+1;
		temp[k+1] = 0;

		ledges[i].height = atoi( temp );

		for(k=0;k<20;k++)                            /*get ledge left end*/
		{
			if( (c=(int)string[k+top])==',' )
				break;
			else
				temp[k] = string[k+top];
		}
		top = k+1+top;
		temp[k+1] = 0;

		ledges[i].leftend = atoi( temp );

		for(k=0;k<20;k++)                          /*get ledge right end*/
		{
			if( (c=(int)string[k+top])==',' )
				break;
			else
				temp[k] = string[k+top];
		}
		top = k+1+top;
		temp[k+1] = 0;

		ledges[i].rightend = atoi( temp );

/*printf("\ni=%d height=%d left=%d right=%d",i,ledges[i].height, ledges[i].leftend, ledges[i].rightend);*/

		i++;

	}while( i<MX_LEDGES);


	fclose( fp );

	return(i);

}
