#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER_ID             1
#define TIMER_ELAPSE         20
#define WNDWIDTH             720
#define WNDHEIGHT            430
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      20

//定义资源的尺寸         
#define HERO_SIZE_X          65
#define HERO_SIZE_Y          49
#define HERO_MAX_FRAME_NUM   9
#define BUILDING_SIZE_X      900
#define BUILDING_SIZE_Y      600
#define ROOF_SIZE_X          65
#define ROOF_SIZE_Y          33
#define BLOCK_SIZE_X         65
#define BLOCK_SIZE_Y         65
#define GAME_STATUS_SIZE_X   40
#define GAME_STATUS_SIZE_Y   30

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

/*地形结构体，地形方块位图、屋顶位图、位置、大小、屋顶以及方块大小、方块个数*/
typedef struct
{
	HBITMAP hBlockBmp;
	HBITMAP hRoofBmp;
	POINT	pos;
	SIZE    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
}Terrian;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
}GameStatus;

/*全局变量*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("奔跑吧！Doge!");

/*声明位图句柄*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;

/*全局函数*/
//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄人物
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建单个地形
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBlockBmp表示方块位图句柄
	hRoofBmp表示屋顶位图句柄
	roofHeight屋顶宽度和高度
	blockHeight方块宽度和高度
*************************************************/ 
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero更新
VOID HeroUpdate();
//地形更新
VOID TerrianUpdate();
//游戏状态更新
VOID GameStatusUpdate();
//判断是否点击暂停
BOOL Paused(POINT);
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);