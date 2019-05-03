/*
Progammed in pure C by piccahoe aka near
For Multipile systems -- Linux,PSP and Windows

============= Compiling: ==================
Extract the folders and files.

Linux (compiled on Ubuntu)
1. You must have OpenGL, SDL, SDL_TIFF and SDL_mix (you can do an apt-get if you have unbuntu)
2. Open the folder you extract thats named blockheadU
3. Open the folder named linux
4. Click on gcc_compile to compile it in C or g++_compile to compile it in C+++
5. if #ifdef _SDL is comment out, uncomment it and if #ifdef _psp is not commented out then comment it out
6. in main.c if #ifdef _SDL is comment out, uncomment it and if #ifdef _psp is not commented out then comment it out

Windows (compile on XP with Dev C++)
1. You must have Dev C++, SDL (with SDL Font and SDL_mixer)
2. Open up dev C++ and open up the project in the folder /blockheadU/windows
3. Add the ../mysdl/timer.h ../mysdl/timer.cpp ../mysdl/sdls.h ../mysdl/sdls.cpp -lmingw32 -lSDLmain -lSDL -lSDL_ttf (goto project, then project option, then go to parameter)
4. if you see main.o with main.c, delete it
5. in main.c if #ifdef _SDL is comment out, uncomment it and if #ifdef _psp is not commented out then comment it out
(You might be able to compile it in cywin also)

PSP
1. You must have the pspsdk and png installed. 
2. In main.c, look for #ifdef _PSP and if its commented out uncomment it and if #ifdef _SDL is not commented out then commented it out. 
3. Go to the prompt go to the psp folder /blockheadU/PSP and type make.


======================= Things to do: ===========================
1. Fix normal -> wide in game option (psp) 
2. Fix Text position (psp)
3. Add music and sound (psp)
4. Add Top scoreboard

*/

//#define _nds
//#define _psp
#define _SDL


#include <string.h>
#include <malloc.h>
#ifdef _psp
	#include <pspdisplay.h>
	#include <pspctrl.h>
	#include <pspkernel.h>
	#include <pspdebug.h>
	#include <pspgu.h>
	#include <png.h>
	#include "PSP/graphics.h"
#endif

#ifdef _SDL
	#include "SDL.h"
	#include "SDL_ttf.h"
	#include "SDL_mixer.h"
	#include <time.h>
	#include "mysdl/sdls.h"
#endif

#ifdef _nds
	#include <nds.h>
	#include <stdio.h>
#endif


#ifdef _linux
	#include <pthread.h>
#endif

#ifdef _psp
#define printf pspDebugScreenPrintf
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define RGB(r, g, b) ((r)|((g)<<8)|((b)<<16))

PSP_MODULE_INFO("Block Head 1.0", 0, 1, 1);

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
          sceKernelExitGame();
          return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
}

#endif

struct piece
{
	int size[5][5];	
	int x;
	int y;
	int mapx;
	int mapy;
	int isdown;
	int ishitright;
	int ishitleft;
	int bottom;
	int top;
	int right;
	int left;

	int r;
	int g;
	int b;
	int num;
};

/*varibles*/

#ifdef _psp
int MAPWIDTH=11;
int MAPHEIGHT=26;
int Map[30][27];
#endif

#ifdef _SDL
//Timer fps;

/*==============For timer ===== */
int startTicks;
int pausedTicks;
int paused;
int started;


const int FRAMES_PER_SECOND = 25;
const int SCREEN_BPP = 32;
int MAPWIDTH=9;
int MAPHEIGHT=26;
const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 400;
int Map[30][26];
#endif

#ifdef _nds
int MAPWIDTH=12;
int MAPHEIGHT=22;
int Map[32][22];
#endif


char xytxt1[255];
int quit = 0;
int gamestarted= 0;
int frame = 0;
int gamestate=1;
int isdown=0;
int canmove = 1;
int level=1;
int tamez=0;
int j=0;
int temp[4][4];
enum { title_screen,gameplay,gameover,options,pauseit,aboutscr };
struct piece spiece; 
struct piece npiece;
int downtime=0;
int isdowntime=0;
int lvltme=0;
int islvltme=0;
int isrotin = 0;
int isunder=0;
int blkdwn=0;
int islrspeed=0;
int lrspeed=0;
int score=0;
int levelup=0;
#ifdef _psp 
int blockspeed=30;
#endif
#ifdef _SDL
int blockspeed=15;
#endif
#ifdef _nds
int blockspeed=30;
#endif


#ifdef _psp
int lrspeedtotal=8;
#endif
#ifdef _SDL
int lrspeedtotal=2;
#endif
#ifdef _nds
int lrspeedtotal=8;
#endif


int nextblock=-1;
int gmeovrtmr=0;

#ifdef _psp
int ttldwntme = 35;
#endif
#ifdef _SDL
int ttldwntme = 15;
#endif
#ifdef _nds
int ttldwntme = 35;
#endif

#ifdef _psp
int blocksize=10; //shrink the blocks to fit the psp screen
int pblocksize=8;
#endif
#ifdef _SDL
int blocksize=16;
int pblocksize=14;
#endif
#ifdef _nds
int blocksize=8;
int pblocksize=8;
#endif

int menutimer=0;
#ifdef _SDL
int ttlmnutme=3;
#endif
#ifdef _psp
int ttlmnutme=10;
#endif
#ifdef _nds
int ttlmnutme=10;
#endif


int ismenutt=0;

#ifdef _SDL
int menucursy=165;
#endif
#ifdef _psp
int menucursy=100;
#endif
#ifdef _nds
int menucursy=100;
#endif

int omenutimer=0;

#ifdef _SDL
int ottlmnutme=3;
#endif
#ifdef _psp
int ottlmnutme=10;
#endif
#ifdef _nds
int ottlmnutme=10;
#endif

int oismenutt=0;
int omenucursy=125;

enum { normal,wide };
int fldsze=normal;
int ispause=0;

int ispauset=0;	
int gpausec=0;
#ifdef _SDL
int gpausett=3;
#endif
#ifdef _psp
int gpausett=10;
#endif
#ifdef _nds
int gpausett=10;
#endif

#ifdef _SDL
int gmnuy=145;
#endif
#ifdef _psp
int gmnuy=60;
#endif
#ifdef _nds
int gmnuy=60;
#endif

#ifdef _SDL
int gmnux=80;
#endif
#ifdef _psp
int gmnux=160;
#endif	
#ifdef _nds
int gmnux=160;
#endif	


int mispause=0;	
int mpausec=0;
#ifdef _SDL
int mpausett=3;
#endif
#ifdef _psp
int mpausett=10;
#endif
#ifdef _nds
int mpausett=10;
#endif

int ispause2=0;

int ispauset2=0;	
int gpausec2=0;
#ifdef _SDL
int gpausett2=3;
#endif
#ifdef _psp
int gpausett2=10;
#endif
#ifdef _nds
int gpausett2=10;
#endif

#ifdef _SDL
int gmnuy2=145;
#endif
#ifdef _psp
int gmnuy2=100;
#endif
#ifdef _nds
int gmnuy2=100;
#endif

#ifdef _SDL
int gmnux2=80;
#endif
#ifdef _psp
int gmnux2=160;
#endif	
#ifdef _nds
int gmnux2=160;
#endif


int mispause2=0;	
int mpausec2=0;

#ifdef _SDL
int mpausett2=3;
#endif
#ifdef _psp
int mpausett2=10;
#endif
#ifdef _nds
int mpausett2=10;
#endif


#ifdef _nds
u16* bkblk; 
u16* gfx;
u16* gfxSub;
uint16 keysPressed;
uint16 specialKeysPressed;
int blockid=0;
int screen = 1;
#endif


int (*fun2exe)();
int bonus=0;
int ttlbnus=0;
int isbnustrig=0;

int tbispause=0;	
int tbpausec=0;
int tbpausett=30;
char ttlbnuss[255];
int isabout=0;
int ismusic=1;
int titlex=0,titley=0;
int blksptx=0,blkspty=0;
int sgamex=0,sgamey=0;	
int ogamex=0,ogamey=0;
int agamex=0,agamey=0;
int egamex=0,egamey=0;
int gameopx=0;
int gameopy=0;
int gameopsndx=0;
int gameopsndy=0;
int gameopmusx=0;
int gameopmusy=0;
int gameopex = 0;
int gameopey=0;
int gameopftx=0;
int gameopfty=0;
	
int recx=0;
int recy=0;
int recw=0;
int rech=0;
	
int sndonx=0;
int sndony=0;

int fldszex=0;
int fldszey=0;

int sndeffx=0;
int sndeffy=0;
int gmepx=0;
int gmepy=0;


int gpconx=0;
int gpcony=0;

int gpopx=0;
int gpopy=0;	

int gpexy=0;
int gpexx=0;

int olvlsrtx=0;
int olvlsrty=0;

int blockids=0;

enum { gmepmenu, gmepomenu };
int gameoption=gmepmenu;

/*
	Graphic engine varibles
*/
#ifdef _psp
SceCtrlData pad;
#endif

#ifdef _nds
uint16 keysPressed;
uint16 specialKeysPressed;
int bg;
u16 colorMask;
u16* backBuffer;
uint16* back = VRAM_A;
uint16* front = VRAM_B;
#endif

#ifdef _SDL
SDL_Event event;
SDL_Surface *screen = NULL;

TTF_Font *font = NULL;
TTF_Font *titlefont = NULL;
SDL_Rect tf;
SDL_Rect paintbg;
SDL_Rect object;
SDL_Color txtcolor = { 255,255,255 };
SDL_Surface *xytetris=NULL;
Uint32 bgcolor;
Uint32 black;
Uint32 objcolor;
Uint8 *keystates;
Mix_Music *music = NULL;
Mix_Chunk *gmeovrsnd = NULL;
#endif

/* functions */
void drawfield();
void clearmap();
void Newblock();
int cotecleft();
int rotatecheckL();
int cotecright();
int rotatecheckD();
int checkdown();
int rstdwntme();
int clrtmp();
int chklne();
int ablock(struct piece* spiece,int newblock);
int chkblkdwn();
int movblkdwn();
int isgmeovr();
int fillmap();
int inigame();
int loadfonts();
int pntscrblk();
int titlescr();
int gameoverscr();
int gp_speed_down();
int gp_rotateblock();
int gp_rotateblock2();
int gp_moveblockl();
int gp_moveblockr();
int gp_lrtimer();
int drawpieces();
int drawmap();
int drawnext();
int printscore();
#ifdef _SDL
int free_();
#endif
int clear_pieces();
int restgame();
int optionscr();
int gooptions();
int ch2gmeop();
int drwnm2wde();
int resetvar();
int aboutbh();

#ifdef _SDL
void start();
void stop();
void pause();
void unpause();
#endif

#ifdef _nds
int drawrec(int id,void *gfxSub,int width,int height, int pos_x,int pos_y, int r,int g,int b);
#endif
    
    //Gets the timer's time
int get_ticks();
    
//Checks the status of the timer
int is_started();
int is_paused(); 
int ini_timer();


//graphic engine functions
int iniGraphic();
int framerate();
int frameflip();
int frameloop();
int getkeystate();
int keydownpressed();
int krotateL();
int krotateR();
int keyuppressed();
int keyleftpressed();
int keyrightpressed();
int keyenterpressed();
int keypausepressed();
int g_print(char* msg,int x,int y);
int FillRec(int width,int height,int x, int y,int r, int b, int g);


int main( int argc, char* args[] )
{
	inigame();


    while( quit == 0 )
    {

		framerate();

		pntscrblk();

		getkeystate();
		
		switch(gamestate)
		{
			case title_screen:
			{
				titlescr();
				break;
			}
			case gameover:
			{
				gameoverscr();	
				break;
			}
			case gameplay:
			{
				if (ispause == 0)
				{
					if (isbnustrig == 1)
					{
						tbpausec++;
						sprintf(ttlbnuss,"Bonus %ix!",ttlbnus);
						#ifdef _nds
							iprintf("\x1b[12;20H%s",ttlbnuss);
						#else
							g_print(ttlbnuss,170,175);
						#endif
						if (tbpausec == tbpausett)
						{
							isbnustrig = 0;
							tbispause=0;	
							tbpausec=0;
							ttlbnus=0;
						}
					}
					blkdwn=0;

					chkblkdwn();
					if (blkdwn == 1)
					{
						isdowntime=1;
					} else {
						movblkdwn();
					}
		
					if (isdowntime == 1)
					{				
						if (checkdown() == 1)
						{
							rstdwntme();
						} else {
							downtime++;
							if (downtime == ttldwntme)
							{
								downtime=0;
								isdowntime=0;

								fillmap();
								chklne();
								if (isgmeovr() == 1)
								{
									break;
								}
								Newblock();	
							}
						}
					}

					if (keydownpressed() && islrspeed == 1)
					{
						gp_speed_down();

					}
					if (krotateL() && islrspeed == 1)
					{
						gp_rotateblock();
					
					}
					if (krotateR() && islrspeed == 1)
					{
						gp_rotateblock2();
					
					}
					if (keyleftpressed() && islrspeed == 1)
					{	
						gp_moveblockl();
					}
			
					if (keyrightpressed() && islrspeed == 1)
					{	
						gp_moveblockr();

					}
				}
				/*
				if ( keypausepressed() == 1 && islrspeed == 1)
				{
					if (ispause == 0)
					{
						ispause = 1;
						
					}
				}
				*/
				gp_lrtimer();

				drawpieces(); //draws pieces on the map

				drawmap();
				
				drawnext();
				
				printscore();
	
				if (ispause == 1)
				{
					FillRec(recw,rech,recx,recy, 0,0, 255);
					switch(gameoption)
					{
						case gmepomenu:
						{
							#ifdef _nds
								
								
							#else
								g_print("Game Options",gameopx,gameopy);
								g_print("Sound          ",gameopsndx,gameopsndy);
								g_print("Music          ",gameopmusx,gameopmusy);
								g_print("Field type    ",gameopftx,gameopfty);
								g_print("Exit",gameopex,gameopey);
								g_print("off",sndeffx,sndeffy);
							#endif
							if (ismusic==1)
							{
								#ifdef _SDL
								if( Mix_PlayingMusic() == 0 ) 
								{ 
									if( Mix_PlayMusic( music, -1 ) == -1 ) 
									{ 
										ismusic=0; 
									}
								}
								#endif

								#ifdef _nds
								#else
									g_print("On",sndonx,sndony);
								#endif
							} else {
								#ifdef _SDL	
									Mix_HaltMusic();
								#endif
								#ifdef _nds
									g_print("Off",sndonx,sndony);
								#endif
 
							}
							switch(fldsze)
							{
								case normal:
								{
									#ifdef _nds
									#else
										g_print("normal",fldszex,fldszey);
									#endif
									break;
								}
								case wide:
								{
									#ifdef _nds
								
										g_print("wide",fldszex,fldszey);
									#else
									#endif
									break;
								}	
							}
								
							#ifdef _nds
							#else
								g_print(">",gmnux2,gmnuy2);
							#endif

							if (ispauset2==0)
							{
								gpausec2++;
							} else {
								ispauset2=0;
								gpausec2=0;
							}		
							if (gpausec2 == gpausett2)
							{
								ispauset2 = 1;
							}
							if (keydownpressed() && ispauset2 == 1 && gmnuy2 !=gameopey)
							{
								gmnuy2+=20;
							}
							if (keyuppressed() && ispauset2 == 1 && gmnuy2 !=gameopsndy)
							{
								gmnuy2-=20;
							}
							if (keyleftpressed() && ispauset2 == 1)
							{
	
								if (gmnuy2 == gameopmusy)
								{
									if (ismusic == 1)
									{
										ismusic=0;
									} else {
										ismusic=1;
									}
								}
								if (gmnuy2 == gameopfty)
								{
									if (fldsze == normal)
									{
										fldsze = wide;
										#ifdef _SDL
										MAPWIDTH=12;
										#endif
										#ifdef _psp
										MAPWIDTH=14;
										#endif
										drwnm2wde();
										//clearmap();
										//drawfield();
									} else
									if (fldsze == wide)
									{
										#ifdef _SDL				
										MAPWIDTH=9;
										#endif
										#ifdef _psp
										MAPWIDTH=11;
										#endif
										fldsze = normal;
										resetvar();
										Newblock();
									}
								}

							}
							if (keyenterpressed() == 1)
							{
								if (gmnuy2 == gameopey)
								{
									#ifdef _SDL
									gmnuy2=145;
									#endif
									#ifdef _psp
									gmnuy2=100;
									#endif
									gameoption=gmepmenu;
								}
							}
							break;
						}
						case gmepmenu:
						{
							#ifdef _nds
							#else
								g_print("Game Paused!",gmepx,gmepy);
								g_print("Continue",gpconx,gpcony);
								g_print("Options",gpopx,gpopy);
								g_print("Exit",gpexx,gpexy);
								g_print(">",gmnux,gmnuy);
							#endif

							if (ispauset==0)
							{
								gpausec++;
							} else {
								ispauset=0;
								gpausec=0;
							}		
							if (gpausec == gpausett)
							{
								ispauset = 1;
							}
							if (keydownpressed() && ispauset == 1 && gmnuy != gpexy)
							{
								gmnuy+=20;
							}
							if (keyuppressed() && ispauset == 1 && gmnuy != gpcony)
							{
								gmnuy-=20;
							}
							if ( keyenterpressed() && ispauset == 1)
							{
								if (gmnuy == gpopy)
								{
									//gameoption=gmepomenu;
									gamestate=pauseit;									
									fun2exe=&ch2gmeop;
								}
								if (gmnuy == gpcony)
								{
									ispause=0;
								}			
								if (gmnuy == gpexy)
								{
									gamestate = pauseit;
									fun2exe=&restgame;
									break;
							
								}			
							}
							break;
						}
					}
				}			
				break;

			}
			case options:
			{
				optionscr();
				break;
			}
			case aboutscr:
			{
				aboutbh();
				break;
			}
			case pauseit:
			{
				if (mispause==0)
				{
					mpausec++;
				} else {
					mispause=0;
					mpausec=0;
				}		
				if (mpausec == mpausett)
				{
					mispause = 1;
					(*fun2exe)();
					
				}
				break;
			}
		}
		frame++;
		if (frameflip() == 0)
    	{
    		return 1;    
    	}
        frameloop();
 	}
	
	#ifdef _SDL
	free_();
	#endif

	#ifdef _psp
	sceKernelExitGame();	
	#endif

    return 0;    
}





void drawfield()
{
	int t=0;

	#ifdef _nds
	for (t=1;t<MAPHEIGHT;t++)
	{
    		Map[0][t] = 2;
		Map[MAPWIDTH-1][t] = 2;
 	}
	

   	for (t=0;t<MAPWIDTH-1;t++)
   	{
		Map[t][MAPHEIGHT-1] = 3;
   	}
	#endif

	#ifdef _psp	
	for (t=1;t<MAPHEIGHT;t++)
	{
    	Map[0][t] = 2;
		Map[MAPWIDTH-1][t] = 2;
 	}
	

   	for (t=0;t<MAPWIDTH-1;t++)
   	{
		Map[t][MAPHEIGHT-1] = 3;
   	}
	#endif
	#ifdef _SDL
	for (t=1;t<23;t++)
	{
    	Map[0][t] = 2;
		Map[MAPWIDTH-1][t] = 2;
 	}
	
   	for (t=0;t<MAPWIDTH;t++)
   	{
		Map[t][23] = 3;
   	}
	#endif

}

void clearmap()
{
	int xy=0;
	int xy2=0;
	for (xy=0;xy<MAPWIDTH;xy++)
	{
		for (xy2=0;xy2<MAPHEIGHT;xy2++)
		{
			Map[xy][xy2] =0;
		}
   	 }
}


void Newblock()
{
	srand(time(NULL));
	int newblock=0;
	int xwhrstr=3;


	xwhrstr=(MAPWIDTH/2)-1;

	if (nextblock == -1)
	{
		nextblock=rand() % 6;
	} 
	newblock = nextblock;
	nextblock=rand() % 6;

	if (newblock == 5 || newblock == 6) xwhrstr--;
	if (newblock == 4) xwhrstr--;
	
	spiece.x = blocksize*xwhrstr;
	spiece.y = blocksize*1;
	
	spiece.mapx =xwhrstr;		
	spiece.mapy =1;
	spiece.left=0;
	spiece.right=0;
	spiece.ishitright = 0;

	clear_pieces();

   	ablock(&spiece,newblock);
	ablock(&npiece,nextblock);	

	isdown = 0;
}

int cotecleft()
{
	int breakout = 0;				
	int y=0;
	int x=0;

	for (y=0; y <4; y++)
	{
		for (x=0;x<4;x++)
		{
			if (spiece.size[x][y] !=1)
			{
							
			} else {
	
				if (Map[spiece.mapx+x-1][spiece.mapy+y] == 2 || Map[spiece.mapx+x-1][spiece.mapy+y] == 3 || Map[spiece.mapx+x-1][spiece.mapy+y] >= 4)
				{	
					breakout = 1;
				}
				break;
			}					
		}
		if (breakout == 1)
		{

			break;
		}
	}

	if (breakout == 0)
	{
			return 0;
	}	else {
			return 1;
	}
	return 1;
}

int rotatecheckL()
{
	int breakout = 0;	
	int x=0;
	int y=0;			

	for (y=0; y <4; y++)
	{
		for (x=0;x<4;x++)
		{
			if (temp[x][y] !=1)
			{
							
			} else {
	
				if (Map[spiece.mapx+x][spiece.mapy+y] == 2)
				{	
					breakout = 1;
				}
			}					
		}
		if (breakout == 1)
		{

			break;
		}
	}

	if (breakout == 0)
	{
			return 0;
	}	else {
			return 1;
	}
	return 1;
}

int cotecright()
{
	int breakout = 0;	
	int y=0;
	int x=0;			

	for (y=0; y <4; y++)
	{
		for (x=3;x>0;x--)
		{
			if (spiece.size[x][y] !=1)
			{
							
			} else {
	
				if (Map[spiece.mapx+x+1][spiece.mapy+y] == 2 || Map[spiece.mapx+x+1][spiece.mapy+y] == 3 || Map[spiece.mapx+x+1][spiece.mapy+y] >= 4)
				{	
					breakout = 1;
					break;
				}
			}					
		}
		if (breakout == 1)
		{
			break;
		}
	}

	if (breakout == 0)
	{
			return 0;
	}	else {
			return 1;
	}
	return 1;
}


int rotatecheckD()
{
	int y=0;
	int x=0;
	for (y=0;y<4;y++)
	{
		for (x=0;x<4;x++)
		{
			if (temp[x][y] == 1)
			{
				if (Map[spiece.mapx+x][spiece.mapy+y] == 3 || Map[spiece.mapx+x+1][spiece.mapy+y] >= 4)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

int checkdown()
{
	int g=0;
	int h=0;
	isunder=0;

	if (isdowntime == 1)
	{
		for (g=0;g<4;g++)
		{
			for (h=0;h<4;h++)
			{
				if (spiece.size[h][g] == 1)
				{
					if (Map[spiece.mapx+h][spiece.mapy+g+1] == 3 || Map[spiece.mapx+h][spiece.mapy+g+1] >= 4)
					{
						isunder = 0;
						return 0;
					} else {
						isunder=1;
					}
				}
			}
		}
		if (isunder == 1)
		{
			return 1;
		}
	}
	return 0; 
}

int rstdwntme()
{
	isdowntime=0;
	downtime=0;
	blkdwn=0;
	isunder = 0;
	islvltme=0;
	lvltme=0;
	return 0;
}

int clrtmp()
{
	int ctemp=0;
	int ctemp2=0;

	for (ctemp=0;ctemp < 4; ctemp++)
	{
		for (ctemp2=0;ctemp2<4;ctemp2++)
		{
			temp[ctemp][ctemp2]=0;
		}
	}

	return 0;
}

int chklne()
{
	int islinedfilled = 1;
	int yaxis=0;
	int xaxis=0;
	int delx=1;
	int currenty;
	int xx=0;
	int addtoscore=0;

	bonus=0;
	#ifdef _nds
	for (yaxis=0; yaxis < MAPHEIGHT-1;yaxis++)
	#endif
	#ifdef _psp
	for (yaxis=0; yaxis < MAPHEIGHT-1;yaxis++)
	#endif
	#ifdef _SDL
	for (yaxis=0; yaxis < 23;yaxis++)
	#endif
	{
		islinedfilled = 1;
		for (xaxis=1;xaxis < MAPWIDTH; xaxis++)
		{
			if (Map[xaxis][yaxis] == 0)
			{
				islinedfilled = 0;
				break;
			}
		}	
		if (islinedfilled == 1)
		{
			for (delx=1;delx < MAPWIDTH-1;delx++)
			{
				Map[delx][yaxis] = 0;
			}
			for (currenty=yaxis-1;currenty > 0;currenty--)
			{
				for (xx=0;xx<MAPWIDTH-1;xx++)
				{
					if (Map[xx][currenty] >= 4)
					{
						Map[xx][currenty+1] = Map[xx][currenty];
						Map[xx][currenty] = 0;
						
					}
				}
			}
			addtoscore+=10;
			bonus++;
			levelup+=1;
			if (levelup == 10)
			{
				if (blockspeed !=0)
				{
					blockspeed-=1;
					level++;
					levelup=0;
				}
			}
		}	
	}
	if (bonus > 1) 
	{
		isbnustrig=1;
		ttlbnus = bonus;
		score+= addtoscore*bonus;
	} else { 
		score+= addtoscore;
	}
	return 0;
}

int ablock(struct piece* spiece,int newblock)
{
	int i=0;
	int j=0;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			spiece->size[i][j] = 0;
		}
	}
	switch (newblock)
	{
		case 0:
		{
			/*
			0100
			0100
			0100
			0100
			*/
			spiece->size[1][0] = 1;
			spiece->size[1][1] = 1;
			spiece->size[1][2] = 1;
			spiece->size[1][3] = 1;
			spiece->r=153;	
			spiece->g=102;
			spiece->b=204;	
			spiece->num = 4;	
			//spiece.bottom=3;
			break;
		}
		case 1:
		{
			/*
			  0000
			  0110
			  0110
			  0000
			*/
			spiece->size[1][1] = 1;
			spiece->size[1][2] = 1;
			spiece->size[2][1] = 1;
			spiece->size[2][2] = 1;	
			spiece->r=255;	
			spiece->g=153;
			spiece->b=0;	
			spiece->num = 5;		
			break;
		}
		case 2:
		{
			/*
			   0000
			   0100
			   1110
			   0000
			*/
			spiece->r=51;	
			spiece->g=102;
			spiece->b=204;
			spiece->size[1][1] = 1;
			spiece->size[0][2] = 1;
			spiece->size[1][2] = 1;
			spiece->size[2][2] = 1;
			spiece->num = 6;	
			break;
		}
		case 3:
		{
			/*
			0000
			1100
			0110
			0000
			*/
			spiece->r=255;	
			spiece->g=255;
			spiece->b=255;
			spiece->size[0][1] = 1;
			spiece->size[1][1] = 1;
			spiece->size[1][2] = 1;
			spiece->size[2][2] = 1;	
			spiece->num = 7;
			break;
		}
		case 4:
		{
			/*
			0000
			0011
			0110
			0000
			*/
			spiece->r=255;	
			spiece->g=255;
			spiece->b=0;
			spiece->size[2][1] = 1;
			spiece->size[3][1] = 1;
			spiece->size[1][2] = 1;
			spiece->size[2][2] = 1;
			spiece->num = 8;	
			break;
		}
		case 5:
		{
			/*
			0110
			0010
			0010
			0000
			*/
			spiece->size[1][0] = 1;
			spiece->size[2][0] = 1;
			spiece->size[2][1] = 1;
			spiece->size[2][2] = 1;
			spiece->r=255;	
			spiece->g=0;
			spiece->b=0;	
			spiece->num = 9;
		
			break;
		}
		case 6:
		{
			/*
			0110
			0010
			0010
			0000
			*/
			spiece->size[2][1] = 1;
			spiece->size[1][1] = 1;
			spiece->size[1][2] = 1;
			spiece->size[1][3] = 1;
			spiece->r=153;	
			spiece->g=255;
			spiece->b=0;
			spiece->num = 10;
			break;
		}
	}
	return 0;
}

int chkblkdwn()
{
	blkdwn=0;
				/* 
					check the piece in the array
				*/
	int y1=0;
	int x1=0;

	for (y1=0;y1<4;y1++)
	{
		for (x1=0;x1<4;x1++)
		{
						/* check if theres an one in the array */
			if (spiece.size[x1][y1] == 1)
			{
							/* 
							takes the screen x and y of the tetris map add it to the current x and y in the piece array
							and add 1 to the y axes. This checks if theres a block down.							
							*/
				if (Map[spiece.mapx+x1][spiece.mapy+y1+1] == 3 || Map[spiece.mapx+x1][spiece.mapy+y1+1] >= 4)
				{
								/*
									if down then escape out of both of the loop
								*/
					blkdwn = 1;
					break;
				}
			}
		}
		if (blkdwn == 1)
		{
			break;
		}
	}
	return 0;
}

int movblkdwn()
{
	if (lvltme== blockspeed)
	{
		spiece.mapy++;
		spiece.y+=blocksize;
		lvltme = 0;
		islvltme = 1;
	} else {
		if (islvltme == 0)
		{
			lvltme++;
		} else {
			islvltme = 0;
		}						
	}
	return 0;
}

int fillmap()
{
	int y=0;
	int y2=0;

	for (y=0;y<4;y++)
	{
		for(y2=0;y2<4;y2++)
		{
			if (spiece.size[y][y2] == 1)
			{
				Map[spiece.mapx+y][spiece.mapy+y2] = spiece.num;
			}
		}
	}
	return 0;
}

int isgmeovr()
{
	if (spiece.mapy==1)
	{
		#ifdef _SDL
		if( Mix_PlayChannel( -1, gmeovrsnd, 0 ) == -1 ) 
		{ 
			return 1; 
		}
		#endif
		#ifdef _SDL
		Mix_HaltMusic();		
		#endif							
		gamestate = gameover;
		return 1;
	}
	return 0;
}

int inigame()
{
	gamestate = title_screen;

   // object.w = 16;
    //object.h = 16;
    spiece.isdown = 0;
    spiece.ishitright = 0;
    spiece.ishitleft = 0;

   	iniGraphic();   
    Newblock();
   	clearmap();
   	drawfield();
	loadfonts();
	#ifdef _SDL
	gameopx=90;
	gameopy=105;
	#endif
	#ifdef _psp
	gameopx = 180;
	gameopy = 40;
	#endif
	#ifdef _nds
	gameopx = 180;
	gameopy = 40;
	#endif

	#ifdef _SDL
	gameopsndx=90;
	gameopsndy=145;
	#endif
	#ifdef _psp
	gameopsndx = 180;
	gameopsndy = 100;
	#endif
	#ifdef _nds
	gameopsndx = 180;
	gameopsndy = 100;
	#endif

	#ifdef _SDL
	gameopmusx=90;
	gameopmusy=165;
	#endif
	#ifdef _psp
	gameopmusx = 180;
	gameopmusy = 120;
	#endif
	#ifdef _nds
	gameopmusx = 180;
	gameopmusy = 120;
	#endif

    //ft = field type
	#ifdef _SDL
	gameopftx=90;
	gameopfty=185;
	#endif
	#ifdef _psp
	gameopftx = 180;
	gameopfty = 140;
	#endif
	#ifdef _nds
	gameopftx = 180;
	gameopfty = 140;
	#endif

    //exit
	#ifdef _SDL
	gameopex=90;
	gameopey=205;
	#endif
	#ifdef _psp
	gameopex = 180;
	gameopey = 160;
	#endif
	#ifdef _nds
	gameopex = 180;
	gameopey = 160;
	#endif

	#ifdef _SDL
	recx=70;
	recy=105;
	#endif
	#ifdef _psp
	recx = 150;
	recy = 25;
	#endif
	#ifdef _nds
	recx = 150;
	recy = 25;
	#endif

	#ifdef _SDL
	recw=150;
	rech=120;
	#endif
	#ifdef _psp
	recw = 200;
	rech = 180;
	#endif
	#ifdef _nds
	recw = 200;
	rech = 180;
	#endif

	#ifdef _SDL
	sndonx=155;
	sndony=165;
	#endif
	#ifdef _psp
	sndonx = 280;
	sndony = 120;
	#endif
	#ifdef _nds
	sndonx = 280;
	sndony = 120;
	#endif

	#ifdef _SDL
	fldszex=155;
	fldszey=185;
	#endif
	#ifdef _psp
	fldszex = 280;
	fldszey = 140;
	#endif
	#ifdef _nds
	fldszex = 280;
	fldszey = 140;
	#endif

	#ifdef _SDL
	sndeffx=155;
	sndeffy=145;
	#endif
	#ifdef _psp
	sndeffx = 280;
	sndeffy = 100;
	#endif
	#ifdef _nds
	sndeffx = 280;
	sndeffy = 100;
	#endif

	#ifdef _SDL
	gmepx=90;
	gmepy=105;
	#endif
	#ifdef _psp
	gmepx = 180;
	gmepy = 40;
	#endif
	#ifdef _nds
	sndeffx = 280;
	sndeffy = 100;
	#endif

	#ifdef _SDL
	gmepx=90;
	gmepy=105;
	#endif
	#ifdef _psp
	gmepx = 180;
	gmepy = 40;
	#endif
	#ifdef _nds
	sndeffx = 280;
	sndeffy = 100;
	#endif

	#ifdef _SDL
	gpconx=90;
	gpcony=145;
	#endif
	#ifdef _psp
	gpconx = 180;
	gpcony = 60;
	#endif
	#ifdef _nds
	sndeffx = 280;
	sndeffy = 100;
	#endif

	#ifdef _SDL
	gpopx=90;
	gpopy=165;
	#endif
	#ifdef _psp
	gpopx = 180;
	gpopy = 80;
	#endif
	#ifdef _nds
	gpopx = 180;
	gpopy = 80;
	#endif

	#ifdef _SDL
	gpexx=90;
	gpexy=185;
	#endif
	#ifdef _psp
	gpexx = 180;
	gpexy = 100;
	#endif
	#ifdef _nds
	gpexx = 180;
	gpexy = 100;
	#endif

	#ifdef _SDL
    olvlsrtx=90;
    olvlsrty=125;
	#endif

	#ifdef _psp
    olvlsrtx=180;
    olvlsrty=100;
	#endif

	#ifdef _nds
    olvlsrtx=180;
    olvlsrty=100;
	#endif
	return 0;
}



int pntscrblk()
{
	/* Fill a blackscreen */
	#ifdef _psp
	FillRec(368,272,1, 1,0, 0, 0);
	#endif
	#ifdef _SDL
	FillRec(SCREEN_WIDTH,SCREEN_HEIGHT,1, 1,0, 0, 0);
	#endif	
	#ifdef _nds
	
	#endif
	return 0;
}

int titlescr()
{


	#ifdef _SDL
	titlex=80;
	titley=100;
	blksptx=209;
	blkspty=105;

	sgamex=100;
	sgamey=165;

	ogamex=100;
	ogamey=185;

	agamex=100;
	agamey=205;

	egamex=100;
	egamey=225;

	#endif
	#ifdef _psp
	titlex=180;
	titley=40;
	blksptx=296;
	blkspty=39;

	sgamex=200;
	sgamey=100;

	ogamex=200;
	ogamey=120;

	agamex=200;
	agamey=140;

	egamex=200;
	egamey=160;
	#endif

	#ifdef _nds
	//iprintf("\x1b[10;10HPress start to play");
	//gamestate = gameplay;
	//Newblock();
 	getkeystate();
	iprintf("\x1b[4;10HBLOCK HEAD v1.0");
	iprintf("\x1b[17;10HPress Start");
	if (keyenterpressed() == 1)
	{
		gamestate = gameplay;
		Newblock();
		iprintf("\x1b[2J");
	}
	#else
	g_print("BLOCKHEAD v1.0",titlex,titley);
	g_print("Start",sgamex,sgamey);
	g_print("Options",ogamex,ogamey);
	g_print("About",agamex,agamey);
	g_print("Exit",egamex,egamey);
	#endif

	#ifdef _nds
	#else
	FillRec(5,5,titlex-10,blkspty, 0,130, 5);
	FillRec(5,5,blksptx,blkspty, 0,130, 5);

	g_print(">",sgamex-20,menucursy);


	if (ismenutt==0)
	{
		menutimer++;
	} else {
		ismenutt=0;
		menutimer=0;
	}		
	if (menutimer == ttlmnutme)
	{
		ismenutt = 1;
	}
	//g_print("PRESS ENTER TO START",50,300);
	
	if (keydownpressed() == 1 && ismenutt==1)
	{
		if (menucursy != egamey) menucursy+=20;
	}

	if (keyuppressed() == 1 && ismenutt==1)
	{
		if (menucursy != sgamey) menucursy-=20;
	}
	if (keyenterpressed() == 1)
	{
		if (menucursy == sgamey) 
		{
			if (ismusic == 1)
			{
				#ifdef _SDL
				if( Mix_PlayingMusic() == 0 ) 
				{ 
					if( Mix_PlayMusic( music, -1 ) == -1 ) 
					{ 
						ismusic=0; 
					}
				} 
				#endif
			}
			gamestate = gameplay;
			Newblock();
		}	
		if (menucursy == ogamey) 
		{
			menucursy = sgamey;
			gamestate = pauseit;
			fun2exe=&gooptions;
		}

		if (menucursy == agamey)
		{
			gamestate=pauseit;									
			fun2exe=&aboutbh;
		}
		if (menucursy == egamey) 
		{
			quit=1;
		}
	}
	#endif
	return 0;
}

int optionscr()
{
	char olevelc[255];
	#ifdef _nds
	#else
	g_print("Options",gameopx,gameopy);//,60,80);
	g_print("Start Level : ",olvlsrtx,olvlsrty);//60,125);
	g_print("Sound        : ",gameopsndx,gameopsndy);//60,145);
	g_print("Music        : ",gameopmusx,gameopmusy);//60,165);
	g_print("Field type   :",gameopftx,gameopfty);//60,185);
	g_print("Back",gameopex,gameopey);//60,205);

	g_print(">",40,omenucursy);	

	if (keydownpressed() == 1 && oismenutt==1)
	{
		if (omenucursy != 205) omenucursy+=20;
	}

	if (keyuppressed() == 1 && oismenutt==1)
	{
		if (omenucursy != 125) omenucursy-=20;
	}

	if (keyrightpressed() == 1 && oismenutt==1)
	{
		if (omenucursy == 165)
		{
			if (ismusic == 1) 
			{
				ismusic = 0;
			} else {
				ismusic = 1;
			}
		}
		if (omenucursy == 125) 
		{
			if (level != 3)
			{
				blockspeed-=1;
				level++;
			}
		}
		if (omenucursy == 185)
		{
			if (fldsze == normal)
			{
				fldsze = wide;
				
				#ifdef _SDL
				MAPWIDTH=12;
				#endif
				#ifdef _psp
				MAPWIDTH=14;
				#endif
				clearmap();
				drawfield();
			} else
			if (fldsze == wide)
			{
				fldsze = normal;
				#ifdef _SDL				
				MAPWIDTH=9;
				#endif
				#ifdef _psp
				MAPWIDTH=11;
				#endif				
				clearmap();
				drawfield();
			}
		}		
	}

	if (keyenterpressed() == 1 && oismenutt==1)
	{
		if (omenucursy == 205) 
		{
			//omenucursy=125;
			gamestate = pauseit;
			fun2exe=&restgame;
		}
	}
	if (keyleftpressed() == 1 && oismenutt==1)
	{
		if (omenucursy == 165)
		{
			if (ismusic == 1) 
			{
				ismusic = 0;
			} else {
				ismusic = 1;
			}

		}
		if (omenucursy == 125) 
		{
			if (level != 1)
			{
				blockspeed+=1;
				level--;
			}
		}	

		if (omenucursy == 185)
		{
			if (fldsze == normal)
			{
				fldsze = wide;
				#ifdef _SDL
				MAPWIDTH=12;
				#endif
				#ifdef _psp
				MAPWIDTH=14;
				#endif
				clearmap();
				drawfield();
			} else
			if (fldsze == wide)
			{
				fldsze = normal;
				#ifdef _SDL				
				MAPWIDTH=9;
				#endif
				
				#ifdef _psp
				MAPWIDTH=11;
				#endif	
				clearmap();
				drawfield();
			}
		}
	}
	sprintf(olevelc,"%i",level);
	g_print(olevelc,175,125);
	g_print("off",155,145);
	if (ismusic==1)
	{
		g_print("on",155,165);
	} else {
		g_print("off",155,165);
	}
	switch(fldsze)
	{
		case normal:
		{
			g_print("normal",155,185);
			break;
		}
		case wide:
		{
			g_print("wide",155,185);
			break;
		}	
	}
	

	if (oismenutt==0)
	{
		omenutimer++;
	} else {
		oismenutt=0;
		omenutimer=0;
	}		
	if (omenutimer == ottlmnutme)
	{
		oismenutt = 1;
	}
	#endif
	return 0;
}

int gameoverscr()
{
	if (gmeovrtmr==120)
	{
        restgame();

	} else {
		gmeovrtmr++;
	}
	#ifdef _nds
		iprintf("game over");
	#else
	g_print("GAME OVER",100,200);
	#endif
	return 0;
}

int gp_speed_down()
{
	if (blkdwn == 0 && isdowntime == 0 && islvltme == 0)
	{						
		spiece.mapy++;
		spiece.y+=blocksize;
	}
	return 0;
}

int gp_rotateblock2()
{
	//rotate right
	isrotin = 1;
	clrtmp();
	int c=0;
	int d=0;
	int e=0;
	int f=0;

	for (c=0;c < 4; c++)
	{
		for (d=0;d<4;d++)
		{
			temp[d][3-c] = spiece.size[c][d];
		}
	}
	if (rotatecheckL() == 0 && rotatecheckD() == 0 && isdowntime == 0)
	{
		for (e=0;e < 4; e++)
		{
			for (f=0;f<4;f++)
			{
				spiece.size[e][f] = temp[e][f];
			}
		}
	}
			
	isrotin = 0;
	return 0;
}

int gp_rotateblock()
{
	isrotin = 1;
	clrtmp();
	int c=0;
	int d=0;
	int e=0;
	int f=0;

	for (c=0;c < 4; c++)
	{
		for (d=0;d<4;d++)
		{
			temp[3-d][c] = spiece.size[c][d];
		}
	}
	if (rotatecheckL() == 0 && rotatecheckD() == 0 && isdowntime == 0)
	{
		for (e=0;e < 4; e++)
		{
			for (f=0;f<4;f++)
			{
				spiece.size[e][f] = temp[e][f];
			}
		}
	}
			
	isrotin = 0;
	return 0;
}

int gp_moveblockl()
{
	spiece.ishitleft = 1;
	isunder=0;

	if (checkdown() == 1)
	{
		rstdwntme();
	}
	if (cotecleft() == 0) 
	{
		spiece.x-=blocksize;
		spiece.mapx--;
	}
	spiece.ishitleft = 0;
	return 0;
}

int gp_moveblockr()
{
	if (checkdown() == 1)
	{
		rstdwntme();
	}
	spiece.ishitright = 1;
	if (cotecright() == 0) 
	{
		spiece.x+=blocksize;
		spiece.mapx++;
	}
	spiece.ishitright= 0;
	return 0;
}

int gp_lrtimer()
{
	if (islrspeed == 0)
	{
		lrspeed++;
	} else
	if (islrspeed == 1)
	{
		islrspeed = 0;
		lrspeed = 0;
	}

	if (lrspeed == lrspeedtotal)
	{
		islrspeed = 1;
	}
	return 0;
}

int drawpieces()
{
	int k=0;
	int l=0;

	for (k=0;k < 4; k++)
	{
		for (l=0; l < 4; l++)
		{
			if (spiece.size[k][l] == 1)
			{
				FillRec(pblocksize,pblocksize,blocksize*k+spiece.x+10, blocksize*l+spiece.y+10,spiece.r, spiece.g, spiece.b);
			}
		}
	}
	return 0;
}

int drawmap()
{
	int i=0;
	int j=0;

	int r;
	int b;
	int g;

	#ifdef _psp
	int j2=0;
	#endif

	#ifdef _SDL
	int j2=2;
	#endif	

	#ifdef _nds
	int j2=0;
	#endif

	for (i=0;i< MAPWIDTH; i++)
	{
		for (j=j2;j < MAPHEIGHT;j++)				
		{
			if (Map[i][j] == 2 || Map[i][j] == 3)
			{
					FillRec(blocksize,blocksize,i*blocksize+10, j*blocksize+10,0, 128, 255);
	
			}
			if (Map[i][j] >= 4)
			{
					if (Map[i][j] == 4)
					{
						r=153;	
						g=102;
						b=204;
					}
					if (Map[i][j] == 5)
					{
						r=255;	
						g=153;
						b=0;
						
					}
					if (Map[i][j] == 6)
					{
						r=51;	
						g=102;
						b=204;
						
					}
					if (Map[i][j] == 7)
					{
						r=255;
						g=255;
						b=255;
					}
					if (Map[i][j] == 8)
					{
						r=255;
						g=255;
						b=0;
					}
					if (Map[i][j] == 9)
					{
						r=255;
						g=0;
						b=0;
					}
					if (Map[i][j] == 10)
					{
						r=153;
						g=255;
						b=0;
					}
					FillRec(pblocksize,pblocksize,i*blocksize+10,j*blocksize+10,r,g,b);
			}
		}
	}
	return 0;
}
int drawnext()
{
	#ifdef _nds
	iprintf("\x1b[5;15HNext Piece:");
	#else
	g_print("Next Piece:",MAPWIDTH*blocksize+30,20);
	#endif
	int k2=0;
	int l2=0;

	for (k2=0;k2 < 4; k2++)
	{
		for (l2=0; l2 < 4; l2++)
		{
			if (npiece.size[k2][l2] == 1)
			{
				FillRec(pblocksize,pblocksize,blocksize*k2+(MAPWIDTH*blocksize+30), blocksize*l2+50,npiece.r, npiece.g, npiece.b);
			}
		}
	}
	return 0;
}

int printscore()
{
	#ifdef _nds
	iprintf("\x1b[2;20HScore: %i",score);
	iprintf("\x1b[3;20Hlevel: %i",level);
	#else
	sprintf(xytxt1, "Score: %i",score);
	g_print(xytxt1,MAPWIDTH*blocksize+30,120);
	
	sprintf(xytxt1, "level: %i", level);
	g_print(xytxt1,MAPWIDTH*blocksize+30,140);
	#endif
				//sprintf(xytxt1, "X: %i Y: %i", spiece.mapx,spiece.mapy);
				//xytetris = TTF_RenderText_Blended( font, xytxt1, txtcolor);
				//apply_surface( 10, 10, xytetris, screen );
	return 0;
}

int clear_pieces()
{
	int i=0;
	int j=0;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			spiece.size[i][j] = 0;
		}
	}
	return 0;
}

int restgame()
{
	ispause=0;
	resetvar();
	gamestate = title_screen;
	return 0;
}

int gooptions()
{
	gamestate = options;
	return 0;
}

int ch2gmeop()
{
	gamestate=gameplay;
	gameoption=gmepomenu;	
	return 0;
}

int drwnm2wde()
{	
	int x,y,oldw=0;
	
	//#ifdef _SDL
	oldw = 4;
	//#endif

	for (y=0;y<MAPHEIGHT-2;y++)
	{
		Map[MAPWIDTH-oldw][y] = 0;
		Map[MAPWIDTH-1][y] = 2;
	}
	
	for (x=MAPWIDTH-oldw;x<MAPWIDTH-oldw+3;x++)
	{	
		Map[x][MAPHEIGHT-3] = 3;
	}
	//MAPWIDTH+=1;
	
	return 0;
}

int resetvar()
{
	clearmap();
	drawfield();
	lrspeed=0;
	downtime=0;
	score=0;
	#ifdef _SDL
	gmnuy=145;
	#endif
	#ifdef _psp
	gmnuy=60;
	#endif
	#ifdef _psp
	blockspeed=30;
	#endif
	#ifdef _SDL
	blockspeed=15;
	#endif
	#ifdef _psp
	lrspeedtotal=8;
	#endif
	downtime=0;
	score=0;
	levelup=0;
	nextblock=-1;
	isdowntime=0;
	islvltme=0;
	level=1;
	gmeovrtmr=0;
	blkdwn=0;
	#ifdef _psp	
	menucursy=100;
	#endif
	#ifdef _SDL	
	Mix_HaltMusic();
	#endif
	return 0;
}

int aboutbh()
{
	#ifdef _nds
	#else
	if (isabout == 0)
	{
		isabout = 1;
		gamestate = aboutscr;
		
	} else {
		g_print("ABOUT BLOCKHEAD",80,100);
		g_print("Programmed by Piccahoe a.k.a near",20,140);
		g_print("In Game music:",20,180);
		g_print(" Marches Militaires OP 51 No. 1",20,200);
		g_print("by Schubert;",20,220);
		g_print("Played by Malcolm Bilson & Robert Levin",20,240); 
		//g_print("Controls:",40,180);
		//g_print("Move: Left/right arrows",40,200);
		//g_print("Rotate: Z,X",40,220);
		g_print("Press enter",100,280);
		if (keyenterpressed() == 1)
		{
			isabout=0;
			//omenucursy=125;
		
			gamestate = pauseit;
			fun2exe=&restgame;
		}
	}
	#endif
	return 0;
}

//timer options
#ifdef _SDL
int ini_timer()
{
  	startTicks = 0;
    pausedTicks = 0;
    paused = 0;
    started = 0;
	return 0;
}

void start()
{
    //Start the timer
    started = 1;
    
    //Unpause the timer
    paused = 0;
    
    //Get the current clock time
    startTicks = SDL_GetTicks();    
}
void stop()
{
    //Stop the timer
    started = 0;
    
    //Unpause the timer
    paused = 0;    
}

void pause()
{
    //If the timer is running and isn't already paused
    if( ( started == 1 ) && ( paused == 0 ) )
    {
        //Pause the timer
        paused = 1;
    
        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void unpause()
{
    //If the timer is paused
    if( paused == 1 )
    {
        //Unpause the timer
        paused = 0;
    
        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;
        
        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int get_ticks()
{
    //If the timer is running
    if( started == 1 )
    {
        //If the timer is paused
        if( paused == 1)
        {
            //Return the number of ticks when the the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }    
    }
    
    //If the timer isn't running
    return 0;    
}

int is_started()
{
    return started;    
}

int is_paused()
{
    return paused;    
}

#endif


//graphic engine

int iniGraphic()
{
	#ifdef _nds

	touchPosition touch;

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D | 
					DISPLAY_SPR_1D_LAYOUT | 
					DISPLAY_SPR_ACTIVE | 
					DISPLAY_BG0_ACTIVE |
					DISPLAY_BG1_ACTIVE );

	consoleDemoInit();
	
	//for back buffering
	vramSetMainBanks(VRAM_A_LCD, VRAM_B_LCD, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);
	

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	

	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);
	//oamInit(&oamSub, SpriteMapping_1D_32, false);

	//for backbuffering
	bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	colorMask = 0x1F;

	backBuffer = (u16*)bgGetGfxPtr(bg) + 256*256;

	 gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	 gfxSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_Bmp);

	SPRITE_PALETTE[1] = RGB15(31,0,0);
   	SPRITE_PALETTE_SUB[1] = RGB15(0,31,0);

	 

	#endif

	#ifdef _psp
    	pspDebugScreenInit();
    	SetupCallbacks();
    	initGraphics();
	#endif

	#ifdef _SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 0;    
    }
    
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    if( screen == NULL )
    {
        return 0;    
    }
    if( TTF_Init() == -1 )
    {
        return 0;    
    }
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) 
	{ 
		return 0; 
	}
	music = Mix_LoadMUS( "music.mp3" );
	if (music == NULL)
	{	
		return 0;
	}
	gmeovrsnd = Mix_LoadWAV( "gameover.wav" );
	if (gmeovrsnd == NULL)
	{
		return  0;
	}
	ini_timer();
	#endif
	return 0;
}

int loadfonts()
{
 	#ifdef _SDL
        font = TTF_OpenFont( "times.ttf", 15 );
	titlefont = TTF_OpenFont( "times.ttf", 25 ); 
	#endif
	return 0;
}

int framerate()
{
	#ifdef _SDL
	//fps.start();
	start();
	while( SDL_PollEvent( &event ) )
    	{
    		if( event.type == SDL_QUIT )
        	{
        		quit = 1;
        	}
    	}	
	#endif 

	#ifdef _nds
\
	///
	#endif





	
	return 0;
}

int frameflip()
{
	#ifdef _nds
	swiWaitForVBlank();


	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	/*
	backBuffer = (u16*)bgGetGfxPtr(bg);
	swiWaitForVBlank();
	backBuffer = (u16*)bgGetGfxPtr(bg) + 256*256;
	*/
	iprintf("\x1b[2J");
	if(screen) {
		videoSetMode(MODE_FB1);
		front = VRAM_B;
		back = VRAM_A;
		screen = 0;
	} else {
		videoSetMode(MODE_FB0);	
		front = VRAM_A;
		back = VRAM_B;
		screen = 1;
	}
	blockid=0;
	#endif
	#ifdef _psp
	flipScreen();
	#endif 
	#ifdef _SDL
	if( SDL_Flip( screen ) == -1) return 0;
	#endif
	return 1;
}

int frameloop()
{
	#ifdef _psp
   int i=0;
   for(i=0; i<1; i++) 
   {
   		sceDisplayWaitVblankStart();
   }
	#endif
	#ifdef _SDL
    while( get_ticks() < 1000 / FRAMES_PER_SECOND )
    {
                   
    }
	#endif

	#ifdef _nds

		if(bgGetMapBase(bg) == 8)
		{
			bgSetMapBase(bg, 0);
		}
		else
		{
			bgSetMapBase(bg, 8);
		}
		colorMask ^= 0x3FF;
	#endif
   return 1;
}

int getkeystate()
{
	#ifdef _psp
	sceCtrlReadBufferPositive(&pad, 1);
	#endif	
	#ifdef _SDL
	keystates = SDL_GetKeyState( NULL );
	#endif
	#ifdef _nds
	scanKeys();

	keysPressed = ~(REG_KEYINPUT);
	specialKeysPressed = ~IPC->buttons;
	#endif
	return 0;
}

int keydownpressed()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_DOWN) return 1;
	#endif	
	#ifdef _SDL
	if (keystates[ SDLK_DOWN]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_DOWN) return 1;
	#endif
	return 0;
}

int krotateL()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_CROSS) return 1;
	#endif
	#ifdef _SDL
	if (keystates[ SDLK_z]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_A) return 1;
	#endif	
	return 0;
}

int keyuppressed()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_UP) return 1;
	#endif
	#ifdef _SDL
	if (keystates[ SDLK_UP]) return 1;
	#endif	
	#ifdef _nds
	if(keysPressed & KEY_UP) return 1;
	#endif
	return 0;
}

int krotateR()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_CIRCLE) return 1;
	#endif
	#ifdef _SDL
	if (keystates[ SDLK_x]) return 1;
	#endif	
	#ifdef _nds
	if(keysPressed & KEY_B) return 1;
	#endif
	return 0;
}

int keyleftpressed()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_LEFT) return 1;
	#endif	
	#ifdef _SDL
	if (keystates[ SDLK_LEFT]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_LEFT) return 1;
	#endif
	return 0;
}

int keyrightpressed()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_RIGHT) return 1;
	#endif	
	#ifdef _SDL
	if (keystates[ SDLK_RIGHT]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_RIGHT) return 1;
	#endif
	return 0;
}

int keyenterpressed()
{
	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_CROSS) return 1;
	#endif	
	#ifdef _SDL
	if (keystates[ SDLK_RETURN]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_START) return 1;
	#endif
	return 0;
}

int keypausepressed()
{

	#ifdef _psp
	if (pad.Buttons & PSP_CTRL_START) return 1;
	#endif
	#ifdef _SDL
	if (keystates[ SDLK_p ]) return 1;
	#endif
	#ifdef _nds
	if(keysPressed & KEY_START) return 1;
	#endif
	return 0;
}

int g_print(char* msg,int x,int y)
{
	#ifdef _nds
	#else
	const char* msg2 = (const char*) msg;
	#endif

	#ifdef _psp
	Color txtcolor = RGB(255, 255, 255);
	printTextScreen(x,y,msg2,txtcolor);
	#endif	
	#ifdef _SDL
	xytetris = TTF_RenderText_Blended( font, msg2, txtcolor);
	apply_surface( x, y, xytetris, screen );
	#endif
	return 0;
}

int FillRec(int width,int height,int x, int y,int r, int b, int g)
{
	#ifdef _psp
	Color tcolor = RGB(r, b, g);
	fillScreenRect(tcolor,x,y,width,height);
	#endif
	#ifdef _SDL
	objcolor = SDL_MapRGB(screen->format,r,b,g);
	object.w=width;
	object.h=height;							
	object.x=x;
	object.y=y;
	SDL_FillRect(screen, &object, objcolor);
	#endif

	#ifdef _nds
	drawrec(blockid,gfxSub,8,8, x,y, r,g,b);
	blockid++;
	#endif

	return 0;
}
#ifdef _SDL
int free_()
{
	Mix_FreeMusic( music );
	Mix_FreeChunk(gmeovrsnd);	
        TTF_CloseFont( font );
        TTF_Quit(); 
	Mix_CloseAudio();    
	SDL_Quit();
	

}
#endif

#ifdef _nds
int drawrec(int id,void *gfxSub,int width,int height, int pos_x,int pos_y, int r,int g,int b)
{

	SpriteSize thesize;

	if (width == 8 && height == 8) thesize = SpriteSize_8x8;

	if (width == 16 && height == 16) thesize = SpriteSize_16x16;
	
	if (width == 32 && height == 32) thesize = SpriteSize_32x32;

	if (width == 64 && height == 64) thesize = SpriteSize_64x64;

	dmaFillHalfWords(ARGB16(1,r,g,b),gfxSub, width*height*2);

	

	oamSet(&oamSub, 
		id, 
		pos_x, 
		pos_y, 
		0, 
		15,
		thesize,
		SpriteColorFormat_Bmp, 
		gfxSub, 
		-1, 
		false, 
		false,			
		false, false, 
		false	
	);
	return 0;
}
#endif
