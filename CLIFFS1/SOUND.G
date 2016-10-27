/*The sound_(int tone,int duration) function seems to follow some rules 
like the following:

        Larger values of "tone" give lower notes, so that if
        you halve the value of "tone" then the sound goes up one octave.

        The asdcending scale is represented, starting with the first
        F# below middle C, approximately as follows (given in the form of
        a def file:
*/

#define         REST            100

#define        FSHARP          6400
#define        G               6000
#define        GSHARP          5650
#define        A               5400
#define        ASHARP          5000
#define        B               4750
#define        C               4500
#define        CSHARP          4250
#define        D               4000
#define        DSHARP          3750
#define        E               3500
#define        F               3350

#define        HI_FSHARP       3150
#define        HI_G            3000
#define        HI_GSHARP       2825
#define        HI_A            2700
#define        HI_ASHARP       2500
#define        HI_B            2375
#define        HI_C            2250
#define        HI_CSHARP       2125
//#define        HI_D            2000
#define        HI_D            2020
#define        HI_DSHARP       1875
#define        HI_E            1750
#define        HI_F            1675


#ifdef TESTBED
tune()
{

int quarter=12; 
int eighth = 6; 

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
#endif
