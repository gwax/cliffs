/*++ ATDEFS.G is the file defining constants, structures and function 
prototypes.
--*/

#define START     0
#define READY     1
#define BUSY      2
#define HOT       3     
#define DONE      4
#define M_HOT     5
#define C_HOT     6


#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4

#define INROPE    1
#define OUTROPE  -1
#define END0      1
#define ENDL     -1

#define T_OFF                   0
#define T_ON_MAN                1
#define T_ON_MAN_SLIDING        2        /*used for body but not hands*/
#define T_ON_LEDGE              3
#define T_ON_LEDGE_SLIDING      4

#define R_OFF                    0
#define R_ON                     1
#define R_SLIDING                2

#define MX_TIES   20
#define MX_PITONS 20
#define MX_KEYS   80
#define MX_MOVES  16
#define MX_LEDGES 500
#define MX_VEXT         20        /*max vertical extension*/
#define MX_HEXT         10      /*max horizontal extension*/
#define DEXT            10        

#define KEYRATE         0.15

/*******************COLORS*****************/

#define BLACK   0
#define WHITE   15
#define YELLOW  14
#define LBLUE   11
#define RED     12
#define GREY    8
#define BLUE    1
#define GREEN   2
#define ORANGE  4
#define HOTMAN   YELLOW
#define COLDMAN  LBLUE
#define HOTTEXT  YELLOW
#define COLDTEXT LBLUE
#define LEDGECOLOR GREY
#define BACKGROUND BLACK
#define ROPECOLOR  RED
#define FIXEDCOLOR   YELLOW
#define SLIDINGCOLOR LBLUE
#define ARROWCOLOR   YELLOW
/**************************************/

typedef struct
{
int status;
struct dostime_t begin;
struct dostime_t end;
}TM_DATA;

typedef struct
{
int status;
int i;           /*the "current" index*/
int *data;
}U_DATA;        /*after mallocing *(data + i) should mean something*/


typedef struct
{
int LR;           /*left right sign*/
int DU;           /*down up sign*/
int X, Y;   /*limb extensions*/
}LIMB ;

typedef struct
{
int X,Y;
LIMB lh,rh,lf,rf;
LIMB Olh,Orh,Olf,Orf;
}MAN;

typedef struct
{
int height,leftend,rightend;
}LEDGE;


typedef struct tie_struct
{
int status;
int *x,*y;
struct tie_struct *pre,*post;
}TIE;

typedef struct
{
int x0,y0,xL,yL;
int end0,endL;
TIE   tie[MX_TIES];
}ROPE;

typedef struct
{
int status;
int x,y;
TIE *tie;
}TIEPOINT;

typedef struct
{
TIEPOINT l,b,r;
}BELAY;


int main(void);
int putpik(char *pikname, int x, int y);
int sub(void);
int MoveMan(MAN *man, U_DATA *moves,int direction, LEDGE ledges[],int N, ROPE *rope, BELAY belay[],int manindex);
int ExecuteMoves(int *dX, int *dY, MAN *man, U_DATA *moves,int direction, LEDGE ledges[],int N);
int MoveLimb(LIMB *limb, int sign, int direction );
int main_exec(U_DATA *keys, U_DATA *moves, TM_DATA *tm, int *turnstatus, int *manflag, int *belaykey, int *pitonkey);
int FlipDirection(int *direction, int c);
int DisplayArrow(int x, int y, int direction,int color);
int move_proc(U_DATA *moves, int c);
int CopyAllLimbs(MAN *man);
int isAttached(MAN *man, LIMB *limb,LEDGE ledges[],int N);
int limbCopy(LIMB *source,LIMB *dest);
int key_proc(U_DATA *keys);
int tm_proc(TM_DATA *tm , double TimeLimit );
int InitMan(MAN *man,int x, int y);
int DisplayLedges(LEDGE ledges[],int n);
int DisplayMan(MAN *man,int color);
int settleWeight(MAN *man,LEDGE ledges[],int N);
int swingBod(int *dX, int *dY, LIMB *new, LIMB *old,int direction);
int fall(MAN *man);
int Congratulations(void);
int Retreat(void);
int ReadLedges(LEDGE ledges[], char string[]);  /*returns the number of ledges*/
int GameInstruct(void);
int MiniKey(void);
int man_exec(int *manflag, int *manindex, MAN man[], MAN **currentMan);
int UpdataMovingMan(MAN *currentMan,U_DATA *moves,int direction,LEDGE ledges[],int nledges);
int MoveLimbKey(int move, MAN *man, int direction);
int LedgeSupport(MAN *man,LEDGE ledges[],int N);
int turn_proc(int *direction,int key,int *turnstatus);



int EraseScene(MAN *man,ROPE *rope);
int ReDisplayScene(int manindex,MAN man[],ROPE *rope,LEDGE ledges[],int nledges);

int belay_exec(ROPE *rope, BELAY *belay, int manindex, int *belaykey);
int InitRope(ROPE *rope,int x0, int y0, int xL, int yL);
int DisplayRope(ROPE *rope, int color);
int DeleteTie(TIE *tie);
int InsertTie(TIE *at, TIE *tie);
int UpdateBelay(BELAY *belay, MAN *man);
int InitBelay(BELAY *belay,MAN *man);
int DisplayBelayStatus(BELAY *belay, int Ox, int Oy);
int BelayToRope(ROPE *rope, TIEPOINT *t, int statecount);
int GetFreeIndex(ROPE *rope);
int CheckRopeTouch(int x, int y, ROPE *rope,TIE **at);
int BelayKey(int manflag);
int isNearRope(int a, int b, int x1, int y1, int x2, int y2);
int DisplayMoveCount(int count);
int AddTiepoint(TIE *at, TIE *tie, TIEPOINT *t,int status);
int DangleEnds(ROPE *rope);

int piton_exec(int *pitonkey,TIEPOINT pitons[],MAN *man,ROPE *rope, LEDGE ledges[],int nledges);
int InitPitons(TIEPOINT pitons[]);
int DisplayPitons(TIEPOINT pitons[]);
int GetPitonIndex(TIEPOINT pitons[], int x, int y);
int PitonToRope(TIEPOINT *piton,ROPE *rope,TIE *at,int x,int y);
int UpdateRopePitonStatus(ROPE *rope,TIEPOINT pitons[]);

/*int rope_exec(ROPE *rope,int *ropekey, BELAY belay[],TIEPOINT pitons[],int manindex);*/

#ifdef WAIT
int ExecuteRopeMoves(int *dX, int *dY, ROPE *rope,BELAY *belay, MAN *man, int direction, int manindex);
int isPulling( TIEPOINT *t, LIMB *limb, LIMB *oldlimb, int direction,int *dX, int *dY);
int TestRopePull(TIEPOINT *t, int direction, int *in, int *out);
int DoPull(TIEPOINT *t,TIEPOINT *b,TIEPOINT *ob,ROPE *rope,int dir,int *dX,int *dY,int hX,int hY);
int LedgeSupport(MAN *man,LEDGE ledges[],int N);
int RopeSupport(BELAY *belay,int *pivX,int *pivY);
int tmpSetPivot(TIEPOINT *t,int *tin,int *tout,int *x,int *y,int in,int out);
int SetPivot(int *pivX,int *pivY,int xl,int yl,int xr,int yr,int xb,int yb);
int TestBootstrap(TIEPOINT *t,TIE *u,TIE *v,int in,int out,int *rflag);
#endif

int NewSettleWeight(MAN *man,BELAY belay[],LEDGE ledges[],int N,ROPE *rope,int manindex);
int TestPull(ROPE *rope,BELAY *belay,TIEPOINT *otherbod,int *a,int *b,int dir,int *lift);
int NextTie(TIE **tie,int dir);
int HangDrop(MAN *man, BELAY *belay,TIEPOINT *otherbod,LEDGE  ledges[],int N,ROPE *rope);
int ReplaceMan(MAN *man,ROPE *rope,BELAY *belay,int x,int y);
int isPivotForward(int a, int b , int handX, int handY, int direction);
int AdjustRopeEnds(ROPE *rope,int length,int end,int sign);
int Circle(int x,int a,int b,double r);
int SwingBaby(int a,int b,int ilift, int olift, MAN *man,BELAY *belay,ROPE *rope, LEDGE ledges[],int N);
int OnBelay(BELAY *belay);
int StartFall(MAN *man,BELAY *belay,LEDGE ledges[],int N,ROPE *rope);
int DoPull(int test,int a,int b,int direction,TIEPOINT *t, ROPE *rope,int *hX, int *hY, int sign);
int ExecuteRopeMoves(int *dX, int *dY, MAN *man, BELAY belay[],ROPE *rope,int manindex, int direction,int ledgesupport);
int LimbRopeMove(LIMB *limb,LIMB *oldlimb,BELAY *belay,ROPE *rope,TIEPOINT *t, TIEPOINT *otherbod,int *dX,int *dY,int direction);
int CheckLimbCentering(TIEPOINT *t,LIMB *hand,int ledgesupport);

int clock_proc(TM_DATA *clock);
int DisplayClock(int time);
int TimeDiff(struct dostime_t *new,struct dostime_t *old);

int I_DisplayLedges(LEDGE ledges[],int N,MAN *man,int direction,int a1,int b1, int a2, int b2,int color);
