#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <process.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define HIBA_00 TEXT("Error:Program initialisation process.")
#define TIMER1 113
HINSTANCE hInstGlob;
int SajatiCmdShow;
char szClassName[] = "WindowsApp";
HWND Form1;
LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);

//******************************
//dupla puffereléshez
//******************************
RECT rc;
HDC hdcMem;
HBITMAP hbmMem, hbmOld;
HBRUSH hbrBkGnd;
HDC hdc;
PAINTSTRUCT ps;

//******************************
//segédváltozók rajzoláshoz, animációhoz
//******************************
int rajzolas_folyamatban = 0;
int jatekallapot;//0-aktiv,1-vege
float sebesseg_faktor = 0.0;
float x_sebesseg = 10.0;
int x_mozgas_iranya = 0;//0-semmi,1-jobbra,2-balra
float vadasz_x = 700;
float vadasz_lebeges = 0;
float lebeges_faktor = 0.0;
float vadasz_width = 150;
boolean vadasz_visible = true;
boolean game_over_visible = false;
int lovedek_statusz = 1;
float laser_toltottseg = 10;

//******************************
//a 3D megjelenítéshez
//******************************
#define MAX_OBJ_NUM 100000
float global_nyers_csucspontok[MAX_OBJ_NUM][3];
int global_nyers_csucspontok_index[MAX_OBJ_NUM];
int global_nyers_csucspontok_tipus[MAX_OBJ_NUM];
int global_nyers_csucspontok_offset[MAX_OBJ_NUM];
int global_nyers_csucspontok_szinek[MAX_OBJ_NUM];
float global_forgatott_csucspontok[MAX_OBJ_NUM][3];
float global_lekepezett_csucspontok[MAX_OBJ_NUM][3];
float global_zorder[MAX_OBJ_NUM][2];
int global_nyers_csucspontok_length;
int global_nyers_csucspontok_index_length;
int global_nyers_csucspontok_tipus_length;
int global_nyers_csucspontok_offset_length;
int global_nyers_csucspontok_szinek_length;
int global_forgatott_csucspontok_length;
int global_lekepezett_csucspontok_length;
int global_zorder_length;
int global_nezopont = -1000;
float global_fszog_x_rad;
float global_fszog_y_rad;
float global_fszog_z_rad;
float global_fszog_x = 345;
float global_fszog_y = 345;
float global_fszog_z = 345;
int global_global_offset = 0;

//******************************
//meteorok egyéni forgatási adatai
//******************************
int meteor_statusz[MAX_OBJ_NUM];//0-aktiv,1-kilove
int meteor_sebesseg = 2;
float global_akt_forg_szog[MAX_OBJ_NUM];

//******************************
//ûrhajó megjelenítéséhez
//******************************
float urhajo_nyers_csucspontok[MAX_OBJ_NUM][3];
int urhajo_nyers_csucspontok_index[MAX_OBJ_NUM];
int urhajo_nyers_csucspontok_tipus[MAX_OBJ_NUM];
int urhajo_nyers_csucspontok_offset[MAX_OBJ_NUM];
int urhajo_nyers_csucspontok_szinek[MAX_OBJ_NUM];
float urhajo_forgatott_csucspontok[MAX_OBJ_NUM][3];
float urhajo_lekepezett_csucspontok[MAX_OBJ_NUM][3];
float urhajo_zorder[MAX_OBJ_NUM][2];
int urhajo_nyers_csucspontok_length;
int urhajo_nyers_csucspontok_index_length;
int urhajo_nyers_csucspontok_tipus_length;
int urhajo_nyers_csucspontok_offset_length;
int urhajo_nyers_csucspontok_szinek_length;
int urhajo_forgatott_csucspontok_length;
int urhajo_lekepezett_csucspontok_length;
int urhajo_zorder_length;
int urhajo_nezopont = -1000;
float urhajo_fszog_x_rad;
float urhajo_fszog_y_rad;
float urhajo_fszog_z_rad;
float urhajo_fszog_x = 10;
float urhajo_fszog_y = 180;
float urhajo_fszog_z;
int urhajo_global_offset = 0;

//******************************
//meteor megjelenítéséhez
//******************************
float meteor_nyers_csucspontok[MAX_OBJ_NUM][3];
int meteor_nyers_csucspontok_index[MAX_OBJ_NUM];
int meteor_nyers_csucspontok_tipus[MAX_OBJ_NUM];
int meteor_nyers_csucspontok_offset[MAX_OBJ_NUM];
int meteor_nyers_csucspontok_szinek[MAX_OBJ_NUM];
float meteor_forgatott_csucspontok[MAX_OBJ_NUM][3];
float meteor_lekepezett_csucspontok[MAX_OBJ_NUM][3];
float meteor_zorder[MAX_OBJ_NUM][2];
int meteor_nyers_csucspontok_length;
int meteor_nyers_csucspontok_index_length;
int meteor_nyers_csucspontok_tipus_length;
int meteor_nyers_csucspontok_offset_length;
int meteor_nyers_csucspontok_szinek_length;
int meteor_forgatott_csucspontok_length;
int meteor_lekepezett_csucspontok_length;
int meteor_zorder_length;
int meteor_nezopont = -1000;
float meteor_fszog_x_rad;
float meteor_fszog_y_rad;
float meteor_fszog_z_rad;
float meteor_fszog_x = 345;
float meteor_fszog_y = 345;
float meteor_fszog_z = 345;
int meteor_global_offset = 0;

//******************************
//egyéb segédváltozók
//******************************
float Math_PI = 3.14159265358979323846;
int stage_stageWidth = 1400;
int stage_stageHeight = 900;

//******************************
//háttéranimációhoz
//******************************
float pontx[100];
float ponty[100];
float pontx_leptek[100];
float ponty_leptek[100];
int kp_x = 400;
int kp_y = 300;

//******************************
//segédváltozók az .OBJ fájl beolvasásához
//******************************
FILE* fajl;
FILE* fajl2;
unsigned char sor1[1024], sor2[1024], sor3[1024], sor4[1024];
int s1, s2, elemszam, maxsorszelesseg = 250;
float tomb_vertices[300000][3];
int tomb_faces[300000][500];
int tomb_vertices_length = 0, tomb_faces_length = 0;

//************************
//dupla puffereléshez függvények
//************************
void DB_prepare_puffer(void);
void DB_start_drawing(void);
void DB_end_drawing(void);

//************************
//játékfüggvények
//************************
void init(void);
void restart_game(void);
void timerhandler(void);
void update_laser(void);
void draw_shoot(void);
void cleanup_matrices(void);
void kepernyo_frissit(void);
void trans_matrix_create(float nyers_pontok[MAX_OBJ_NUM][3], int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int* forgatott_tomb_length);
void perspektivikus_projekcio(int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int forgatott_tomb_length, float lekepezes_tomb[MAX_OBJ_NUM][3], int* lekepezes_tomb_length, float nezopont);
void rajzolas(void);
void kepernyo_clear(void);
void z_ordering(float order_tomb[MAX_OBJ_NUM][2], int* order_tomb_length, int nyers_index_tomb[MAX_OBJ_NUM], int nyers_index_tomb_length, int nyers_offset_tomb[MAX_OBJ_NUM], float lekepezes_tomb[MAX_OBJ_NUM][3]);
void rendezes_shell(float order_tomb[MAX_OBJ_NUM][2], int order_tomb_length);
void forgatas(float nyers_pontok[MAX_OBJ_NUM][3], int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int forgatott_tomb_length, float Xszog, float Yszog, float Zszog);
void draw_3d_objects(int tipus, int pozx, int pozy,
	float order_tomb[MAX_OBJ_NUM][2], int order_tomb_length,
	int nyers_index_tomb[MAX_OBJ_NUM],
	int nyers_offset_tomb[MAX_OBJ_NUM],
	int tipus_tomb[MAX_OBJ_NUM],
	float lekepezes_tomb[MAX_OBJ_NUM][3], int szin_tomb[MAX_OBJ_NUM]);
void scale(float arany, float lekepezes_tomb[MAX_OBJ_NUM][3], int lekepezes_tomb_length);
void set_pos(float pozx, float pozy, float lekepezes_tomb[MAX_OBJ_NUM][3], int lekepezes_tomb_length);
void setup_meteors(void);
void update_meteors(void);
void move_stars(void);
void setup_stars(void);
void vadasz_mozgatasa(void);
void game_over(void);

//************************
//.OBJ kezelõ függvények
//************************
int getelementcount(unsigned char csv_szoveg[]);
void getelement(unsigned char csv_szoveg[], unsigned int hanyadik, unsigned char csv_szoveg2[]);
void obj_loader(char fajlnev[], int* offset,
	float nyers_pontok[MAX_OBJ_NUM][3], int* nyers_pontok_tomb_length,
	int nyers_index_tomb[MAX_OBJ_NUM], int* nyers_index_tomb_length,
	int nyers_offset_tomb[MAX_OBJ_NUM], int* nyers_offset_tomb_length,
	int nyers_csucspontok_tipus[MAX_OBJ_NUM], int* nyers_csucspontok_tipus_length,
	int nyers_tipus_tomb[MAX_OBJ_NUM], int* nyers_tipus_tomb_length);


//******************
//Változók képekhez
//******************
struct kepinfo {
	BITMAPFILEHEADER* pbmfh;
	BITMAPINFO* pbmi;
	BYTE* pBits;
	int Width, Height, x, y;
	BOOLEAN visible;
};
kepinfo bg;

//**************
//Képfüggvények
//**************
BITMAPFILEHEADER* DibLoadImage(PTSTR pstrFileName);
void BMP_betoltes(kepinfo* bitkep, const char kepfajl[]);
void ScaleBMP2(HDC hdc, BITMAPINFO* pbmi, BYTE* pBits, int xkoo, int ykoo, int width, int height, int kep_width, int kep_height);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("StdWinClassName");
	MSG msg;
	WNDCLASS wndclass0;
	SajatiCmdShow = iCmdShow;
	hInstGlob = hInstance;

	wndclass0.style = CS_HREDRAW | CS_VREDRAW;
	wndclass0.lpfnWndProc = WndProc0;
	wndclass0.cbClsExtra = 0;
	wndclass0.cbWndExtra = 0;
	wndclass0.hInstance = hInstance;
	wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass0.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass0.lpszMenuName = NULL;
	wndclass0.lpszClassName = TEXT("WIN0");

	if (!RegisterClass(&wndclass0))
	{
		MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR); return 0;
	}

	Form1 = CreateWindow(TEXT("WIN0"),
		TEXT("Priso SpaceFight v1.2"),
		(WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZE),
		0,
		0,
		1920,
		1050,
		NULL,
		NULL,
		hInstance,
		NULL);

	//******************************
	//dupla pufferelés és az idõzítõ elõkészítése
	//******************************
	DB_prepare_puffer();
	SetTimer(Form1, TIMER1, 50, (TIMERPROC)NULL);

	ShowWindow(Form1, SajatiCmdShow);
	UpdateWindow(Form1);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc_lokal;
	PAINTSTRUCT ps_lokal;

	switch (message) {
	case WM_CREATE:
		
		//******************************
		//alkalamzás inicializálása
		//******************************
		init();
		break;
	case WM_ERASEBKGND:
		return (LRESULT)1; // Say we handled it.
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER1:
			//******************************
			//game loop
			//******************************
			if (rajzolas_folyamatban == 1) break;
			rajzolas_folyamatban = 1;
			timerhandler();
			rajzolas_folyamatban = 0;
			break;
		}
	case WM_CHAR:
		//******************************
		//billentyûk leütése
		//******************************
		switch (wParam)
		{
		case 100:
			x_mozgas_iranya = 1;
			x_sebesseg = 25;
			sebesseg_faktor = 0;
			urhajo_fszog_y -= 2;

			if (urhajo_fszog_y <= 170) urhajo_fszog_y = 172;
			break;
		case 97:
			x_mozgas_iranya = 2;
			x_sebesseg = 25;
			sebesseg_faktor = 0;
			urhajo_fszog_y += 2;
			if (urhajo_fszog_y >= 190) urhajo_fszog_y = 188;
			break;
		case 32:
			if (jatekallapot == 1)
			{
				restart_game();
				vadasz_visible = true;
				game_over_visible = false;
				break;
			}
			else
			{
				if (laser_toltottseg < 10);
				else
				{
					PlaySoundA("loves.wav", NULL, SND_FILENAME | SND_ASYNC);
					lovedek_statusz = 0; laser_toltottseg = 0;
				}
			}
			break;
		}
	case WM_PAINT:
		hdc_lokal = BeginPaint(hwnd, &ps_lokal);
		EndPaint(hwnd, &ps_lokal);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}
	return NULL;

}

int roll(int min, int max)
{
	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	// [0,1[ * (max - min) + min is in [min,max[
	int that = min + static_cast<int>(x * (max - min));

	return that;
}

//******************************
//inicializáció
//******************************
void init(void)
{
	BMP_betoltes(&bg, "bg.bmp");
	lebeges_faktor = -4;
	restart_game();
}

//******************************
//játék alapbeállításai
//******************************
void restart_game(void)
{
	int i;

	jatekallapot = 0;
	x_mozgas_iranya = 0;
	meteor_sebesseg = 2;
	setup_stars();
	setup_meteors();
	cleanup_matrices();
	obj_loader((char*)"meteor.obj", &meteor_global_offset,
		meteor_nyers_csucspontok, &meteor_nyers_csucspontok_length,
		meteor_nyers_csucspontok_index, &meteor_nyers_csucspontok_index_length,
		meteor_nyers_csucspontok_offset, &meteor_nyers_csucspontok_offset_length,
		meteor_nyers_csucspontok_tipus, &meteor_nyers_csucspontok_tipus_length,
		meteor_nyers_csucspontok_szinek, &meteor_nyers_csucspontok_szinek_length);
	obj_loader((char*)"ship.obj", &urhajo_global_offset,
		urhajo_nyers_csucspontok, &urhajo_nyers_csucspontok_length,
		urhajo_nyers_csucspontok_index, &urhajo_nyers_csucspontok_index_length,
		urhajo_nyers_csucspontok_offset, &urhajo_nyers_csucspontok_offset_length,
		urhajo_nyers_csucspontok_tipus, &urhajo_nyers_csucspontok_tipus_length,
		urhajo_nyers_csucspontok_szinek, &urhajo_nyers_csucspontok_szinek_length);
	trans_matrix_create(global_nyers_csucspontok, global_nyers_csucspontok_length, global_forgatott_csucspontok, &global_forgatott_csucspontok_length);
	trans_matrix_create(meteor_nyers_csucspontok, meteor_nyers_csucspontok_length, meteor_forgatott_csucspontok, &meteor_forgatott_csucspontok_length);
	trans_matrix_create(urhajo_nyers_csucspontok, urhajo_nyers_csucspontok_length, urhajo_forgatott_csucspontok, &urhajo_forgatott_csucspontok_length);

	for (i = 0; i < meteor_nyers_csucspontok_szinek_length; i++)
	{

		 meteor_nyers_csucspontok_szinek[i] = RGB(97, 5, 0);

	}
}

//******************************
//game loop
//******************************
void timerhandler(void)
{
	int i;
	update_meteors();

	if (jatekallapot == 1) meteor_sebesseg = 0;

	for (i = 0; i < global_nyers_csucspontok_length; ++i)
	{
		global_nyers_csucspontok[i][2] += meteor_sebesseg;
		global_akt_forg_szog[i] += 8;//forgatas!!!
		if (global_akt_forg_szog[i] >= 360) global_akt_forg_szog[i] = 0;
	}
	vadasz_mozgatasa();
	kepernyo_frissit();
}

//******************************
//lézer töltöttség kirajzolása
//******************************
void update_laser(void)
{
	HDC hdc;
	HPEN hPen1;
	RECT rect;
	HBRUSH hBrush;

	laser_toltottseg += 1.0; if (laser_toltottseg > 10) laser_toltottseg = 10;

	hdc = hdcMem;
	hPen1 = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, hPen1);
	SelectObject(hdc, hBrush);
	SetRect(&rect, 10, 10, 10 + laser_toltottseg * 20, 20);
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hPen1);
	DeleteObject(hBrush);
}

//******************************
//lövéás kirajzolása
//******************************
void draw_shoot(void)
{
	HDC hdc;
	HPEN hPen1;

	laser_toltottseg += 1.0; if (laser_toltottseg > 10) laser_toltottseg = 10;
	hdc = hdcMem;
	hPen1 = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	SelectObject(hdc, hPen1);
	MoveToEx(hdc, vadasz_x - 100, 700 + vadasz_lebeges, NULL);
	LineTo(hdc, vadasz_x - 10, 700 + vadasz_lebeges - 350);
	MoveToEx(hdc, vadasz_x + 100, 700 + vadasz_lebeges, NULL);
	LineTo(hdc, vadasz_x + 10, 700 + vadasz_lebeges - 350);
	DeleteObject(hPen1);
}

//******************************
//a forgatott_tomb elõkészítése
//******************************
void trans_matrix_create(float nyers_pontok[MAX_OBJ_NUM][3], int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int* forgatott_tomb_length)
{
	int i;

	*forgatott_tomb_length = 0;
	for (i = 0; i < nyers_pontok_length; ++i)
	{
		forgatott_tomb[i][0] = nyers_pontok[i][0];
		forgatott_tomb[i][1] = nyers_pontok[i][1];
		forgatott_tomb[i][2] = nyers_pontok[i][2];
		++* forgatott_tomb_length;
	}
}

//******************************
//mátrixok alaphelyzetbe állítása
//******************************
void cleanup_matrices(void)
{
	urhajo_nyers_csucspontok_szinek_length = urhajo_nyers_csucspontok_tipus_length = urhajo_nyers_csucspontok_index_length = urhajo_nyers_csucspontok_length = 0;
	meteor_nyers_csucspontok_szinek_length = meteor_nyers_csucspontok_tipus_length = meteor_nyers_csucspontok_index_length = meteor_nyers_csucspontok_length = 0;
	urhajo_global_offset = urhajo_nyers_csucspontok_offset_length = urhajo_forgatott_csucspontok_length = urhajo_lekepezett_csucspontok_length = urhajo_zorder_length = 0;
	meteor_global_offset = meteor_nyers_csucspontok_offset_length = meteor_forgatott_csucspontok_length = meteor_lekepezett_csucspontok_length = meteor_zorder_length = 0;
}

//******************************
//forgatási szügek és a képernyõ frissítése
//******************************
void kepernyo_frissit(void)
{
	urhajo_fszog_x_rad = urhajo_fszog_x * Math_PI / 180;
	urhajo_fszog_y_rad = urhajo_fszog_y * Math_PI / 180;
	urhajo_fszog_z_rad = urhajo_fszog_z * Math_PI / 180;
	meteor_fszog_x_rad = meteor_fszog_x * Math_PI / 180;
	meteor_fszog_y_rad = meteor_fszog_y * Math_PI / 180;
	meteor_fszog_z_rad = meteor_fszog_z * Math_PI / 180;
	global_fszog_x_rad = global_fszog_x * Math_PI / 180;
	global_fszog_y_rad = global_fszog_y * Math_PI / 180;
	global_fszog_z_rad = global_fszog_z * Math_PI / 180;
	rajzolas();
}

//******************************
//egy képkocka kirajzolása
//******************************
void rajzolas(void)
{
	int i;
	int akt_index;
	int sorszam;

	DB_start_drawing();

	kepernyo_clear();
	move_stars();

	forgatas(global_nyers_csucspontok, global_nyers_csucspontok_length,
		global_forgatott_csucspontok, global_forgatott_csucspontok_length,
		global_fszog_x_rad, global_fszog_y_rad, global_fszog_z_rad);
	perspektivikus_projekcio(global_nyers_csucspontok_length, global_forgatott_csucspontok, global_forgatott_csucspontok_length,
		global_lekepezett_csucspontok, &global_lekepezett_csucspontok_length, global_nezopont);
	set_pos(stage_stageWidth / 2, stage_stageHeight / 3, global_lekepezett_csucspontok, global_lekepezett_csucspontok_length);
	z_ordering(global_zorder, &global_zorder_length, global_nyers_csucspontok_index, global_nyers_csucspontok_index_length, global_nyers_csucspontok_offset, global_lekepezett_csucspontok);

	for (i = global_zorder_length - 1; i > -1; --i)
	{
		sorszam = global_zorder[i][1];//poligon sorszama
		akt_index = global_nyers_csucspontok_offset[sorszam];
		if (meteor_statusz[akt_index] == 0)
		{
			meteor_fszog_y = global_akt_forg_szog[akt_index];
			meteor_fszog_y_rad = meteor_fszog_y * Math_PI / 180;
			forgatas(meteor_nyers_csucspontok, meteor_nyers_csucspontok_length,
				meteor_forgatott_csucspontok, meteor_forgatott_csucspontok_length,
				meteor_fszog_x_rad, meteor_fszog_y_rad, meteor_fszog_z_rad);
			perspektivikus_projekcio(meteor_nyers_csucspontok_length,
				meteor_forgatott_csucspontok, meteor_forgatott_csucspontok_length,
				meteor_lekepezett_csucspontok, &meteor_lekepezett_csucspontok_length, meteor_nezopont);
			scale(global_lekepezett_csucspontok[akt_index][2] / 500, meteor_lekepezett_csucspontok, meteor_lekepezett_csucspontok_length);
			set_pos(global_lekepezett_csucspontok[akt_index][0], global_lekepezett_csucspontok[akt_index][1], meteor_lekepezett_csucspontok, meteor_lekepezett_csucspontok_length);
			z_ordering(meteor_zorder, &meteor_zorder_length, meteor_nyers_csucspontok_index, meteor_nyers_csucspontok_index_length, meteor_nyers_csucspontok_offset, meteor_lekepezett_csucspontok);
			if (lovedek_statusz == 0 && global_nyers_csucspontok[akt_index][0] > ((vadasz_x / 3) - 400) && global_nyers_csucspontok[akt_index][0] < ((vadasz_x / 3) + 50))
			{
				meteor_statusz[akt_index] = 1;
				break;
			}
			else
			{
				draw_3d_objects(1, global_lekepezett_csucspontok[akt_index][0], global_lekepezett_csucspontok[akt_index][1],
					meteor_zorder, meteor_zorder_length,
					meteor_nyers_csucspontok_index, meteor_nyers_csucspontok_offset,
					meteor_nyers_csucspontok_tipus, meteor_lekepezett_csucspontok, meteor_nyers_csucspontok_szinek);

			}
			if (global_nyers_csucspontok[akt_index][0] > ((vadasz_x / 3) - 250) && global_nyers_csucspontok[akt_index][0] < ((vadasz_x / 3) - 150) && global_nyers_csucspontok[akt_index][2] > 500)
			{
				vadasz_visible = false;
				game_over_visible = true;
				jatekallapot = 1;
			}
		}
	}

	if (jatekallapot == 1) game_over();

	if (vadasz_visible == true)
	{
		forgatas(urhajo_nyers_csucspontok, urhajo_nyers_csucspontok_length,
			urhajo_forgatott_csucspontok, urhajo_forgatott_csucspontok_length,
			urhajo_fszog_x_rad, urhajo_fszog_y_rad, urhajo_fszog_z_rad);
		perspektivikus_projekcio(urhajo_nyers_csucspontok_length,
			urhajo_forgatott_csucspontok, urhajo_forgatott_csucspontok_length,
			urhajo_lekepezett_csucspontok, &urhajo_lekepezett_csucspontok_length, urhajo_nezopont);
		scale(1.0, urhajo_lekepezett_csucspontok, urhajo_lekepezett_csucspontok_length);
		set_pos(vadasz_x, 700 + vadasz_lebeges, urhajo_lekepezett_csucspontok, urhajo_lekepezett_csucspontok_length);
		z_ordering(urhajo_zorder, &urhajo_zorder_length, urhajo_nyers_csucspontok_index, urhajo_nyers_csucspontok_index_length, urhajo_nyers_csucspontok_offset, urhajo_lekepezett_csucspontok);
		draw_3d_objects(1, 0, 0,
			urhajo_zorder, urhajo_zorder_length,
			urhajo_nyers_csucspontok_index, urhajo_nyers_csucspontok_offset,
			urhajo_nyers_csucspontok_tipus, urhajo_lekepezett_csucspontok, urhajo_nyers_csucspontok_szinek);

		if (lovedek_statusz == 0)
		{
			draw_shoot();
			lovedek_statusz = 1;
		}
	}

	update_laser();

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
}

//******************************
//képernyõ törlése
//******************************
void kepernyo_clear(void)
{
	HBRUSH hBrush;
	RECT rect;
	HDC hdc;
	PAINTSTRUCT  ps;
	hdc = hdcMem;
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);
	SetRect(&rect, 0, 0, 1500, 900);
	FillRect(hdc, &rect, hBrush);
	ScaleBMP2(hdc, bg.pbmi, bg.pBits, bg.x, bg.y, 1920, 1050, bg.Width, bg.Height);
	DeleteObject(hBrush);
}

//******************************
//mélységi rendezés
//******************************
void z_ordering(float order_tomb[MAX_OBJ_NUM][2], int* order_tomb_length, int nyers_index_tomb[MAX_OBJ_NUM], int nyers_index_tomb_length, int nyers_offset_tomb[MAX_OBJ_NUM], float lekepezes_tomb[MAX_OBJ_NUM][3])
{
	int i;
	int j = 0;
	int s, x_minusz_hatar = -100, y_minusz_hatar = -100, x_max_hatar = stage_stageWidth + 100, y_max_hatar = stage_stageHeight + 100;
	int kilog;
	float tavolsag;

	*order_tomb_length = 0;
	for (i = 0; i < nyers_index_tomb_length; ++i)
	{
		tavolsag = 999999;
		kilog = 0;
		for (s = 0; s < nyers_index_tomb[i]; j = nyers_offset_tomb[i] + s, ++s)
		{
			if (lekepezes_tomb[j][0] < x_minusz_hatar || lekepezes_tomb[j][0] > x_max_hatar) kilog = 1;
			if (lekepezes_tomb[j][1] < y_minusz_hatar || lekepezes_tomb[j][1] > y_max_hatar) kilog = 1;
			if (lekepezes_tomb[j][2] < tavolsag)
			{
				tavolsag = lekepezes_tomb[j][2];
			}
		}
		if (kilog != 0) continue;
		if (tavolsag > 650) continue;

		order_tomb[*order_tomb_length][0] = tavolsag;
		order_tomb[*order_tomb_length][1] = i;
		++* order_tomb_length;
	}
	rendezes_shell(order_tomb, *order_tomb_length);
}

//******************************
//rendezõ algoritmus
//******************************
void rendezes_shell(float order_tomb[MAX_OBJ_NUM][2], int order_tomb_length)
{
	int i;
	int j;
	int t;
	int k;
	float csere0;
	float csere1;

	for (k = order_tomb_length / 2; k > 0; k = k / 2)
		for (i = k; i < order_tomb_length; ++i)
			for (j = i - k; (j >= 0) && (order_tomb[j][0] > order_tomb[j + k][0]); j = j - k)
			{
				csere0 = order_tomb[j][0];
				csere1 = order_tomb[j][1];
				order_tomb[j][0] = order_tomb[j + k][0];
				order_tomb[j][1] = order_tomb[j + k][1];
				order_tomb[j + k][0] = csere0;
				order_tomb[j + k][1] = csere1;
			}
}

//******************************
//3D forgatás
//******************************
void forgatas(float nyers_pontok[MAX_OBJ_NUM][3], int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int forgatott_tomb_length, float Xszog, float Yszog, float Zszog)
{
	int i;
	float szog_sin = sin(Xszog);
	float szog_cos = cos(Xszog);
	float t0, t2, t1;

	for (i = 0; i < nyers_pontok_length; ++i)
	{
		forgatott_tomb[i][0] = nyers_pontok[i][0];
		forgatott_tomb[i][1] = (nyers_pontok[i][1] * szog_cos) - (nyers_pontok[i][2] * szog_sin);
		forgatott_tomb[i][2] = nyers_pontok[i][1] * szog_sin + nyers_pontok[i][2] * szog_cos;
	}

	szog_sin = sin(Yszog);
	szog_cos = cos(Yszog);

	for (i = 0; i < forgatott_tomb_length; ++i)
	{
		t0 = forgatott_tomb[i][0];
		t2 = forgatott_tomb[i][2];
		forgatott_tomb[i][0] = t0 * szog_cos + t2 * szog_sin;
		forgatott_tomb[i][2] = t0 * szog_sin - t2 * szog_cos;
	}

	szog_sin = sin(Zszog);
	szog_cos = cos(Zszog);

	for (i = 0; i < forgatott_tomb_length; ++i)
	{
		t0 = forgatott_tomb[i][0];
		t1 = forgatott_tomb[i][1];
		forgatott_tomb[i][0] = t0 * szog_cos - t1 * szog_sin;
		forgatott_tomb[i][1] = t0 * szog_sin + t1 * szog_cos;

	}
}

//******************************
//projekció
//******************************
void perspektivikus_projekcio(int nyers_pontok_length, float forgatott_tomb[MAX_OBJ_NUM][3], int forgatott_tomb_length, float lekepezes_tomb[MAX_OBJ_NUM][3], int* lekepezes_tomb_length, float nezopont)
{
	int i;
	float sultra = stage_stageHeight / 2;

	*lekepezes_tomb_length = 0;

	for (i = 0; i < nyers_pontok_length; ++i)
	{
		sultra = nezopont / (nezopont - forgatott_tomb[i][2]);
		lekepezes_tomb[i][0] = (forgatott_tomb[i][0] * (sultra));
		lekepezes_tomb[i][1] = (forgatott_tomb[i][1] * (sultra));
		lekepezes_tomb[i][2] = forgatott_tomb[i][2];
		++* lekepezes_tomb_length;
	}
}

//******************************
//objektumok kirajzolása
//******************************
void draw_3d_objects(int tipus, int pozx, int pozy,
	float order_tomb[MAX_OBJ_NUM][2], int order_tomb_length,
	int nyers_index_tomb[MAX_OBJ_NUM],
	int nyers_offset_tomb[MAX_OBJ_NUM],
	int tipus_tomb[MAX_OBJ_NUM],
	float lekepezes_tomb[MAX_OBJ_NUM][3], int szin_tomb[MAX_OBJ_NUM])
{
	int sorszam;
	int i;
	int j;
	int k;
	int akt_index;

	HBRUSH hBrush;
	HDC hdc;
	HPEN hPen1;
	POINT vektor_parancsok[60000];
	int vektor_poz = 0;

	hdc = hdcMem;
	hPen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));


	hBrush = CreateSolidBrush(szin_tomb[1]);

	SelectObject(hdc, hPen1);
	SelectObject(hdc, hBrush);

	for (i = order_tomb_length - 1; i > -1; --i)
	{


		sorszam = order_tomb[i][1];//poligon sorszama
		akt_index = nyers_offset_tomb[sorszam];
		j = akt_index;

		if (tipus == 1 && tipus_tomb[sorszam] == tipus)
		{
			vektor_poz = 0;
			for (j = akt_index, k = 0; k < nyers_index_tomb[sorszam]; ++j, ++k)
			{
				vektor_parancsok[vektor_poz].x = lekepezes_tomb[j][0];
				vektor_parancsok[vektor_poz].y = lekepezes_tomb[j][1];
				++vektor_poz;
			}
			Polygon(hdc, vektor_parancsok, vektor_poz);
		}
	}

	DeleteObject(hBrush);
	DeleteObject(hPen1);
}

//******************************
//objektumok 3D nagyítása
//******************************
void scale(float arany, float lekepezes_tomb[MAX_OBJ_NUM][3], int lekepezes_tomb_length)
{
	int i;
	for (i = 0; i < lekepezes_tomb_length; ++i)
	{
		lekepezes_tomb[i][0] *= arany;
		lekepezes_tomb[i][1] *= arany;
		lekepezes_tomb[i][2] *= arany;
	}
}

//******************************
//objektumok 2D pozícionálása
//******************************
void set_pos(float pozx, float pozy, float lekepezes_tomb[MAX_OBJ_NUM][3], int lekepezes_tomb_length)
{
	int i;
	for (i = 0; i < lekepezes_tomb_length; ++i)
	{
		lekepezes_tomb[i][0] += pozx;
		lekepezes_tomb[i][1] += pozy;
	}
}

//******************************
//.OBJ feldolgozó segédfüggvény
//******************************
int getelementcount(unsigned char csv_szoveg[])
{
	int s1, s2;
	for (s1 = s2 = 0; s1 < strlen((const char*)csv_szoveg); ++s1)
	{
		if (csv_szoveg[s1] == 10) break;
		else if (csv_szoveg[s1] == 32) ++s2;
	}
	return s2;
}

//******************************
//.OBJ feldolgozó segédfüggvény
//******************************
void getelement(unsigned char csv_szoveg[], unsigned int hanyadik, unsigned char csv_szoveg2[])
{
	int s1, s2, s3, s4 = 0;
	for (s1 = 0, s2 = 0; s1 < strlen((const char*)csv_szoveg); ++s1)
	{
		if (csv_szoveg[s1] == 32)
		{
			++s2;
			if (s2 == hanyadik)
			{
				for (s3 = s1 + 1; s3 < strlen((const char*)csv_szoveg); ++s3)
				{
					if (csv_szoveg[s3] == 32 || csv_szoveg[s3] == 10)
					{
						csv_szoveg2[s4] = 0;
						return;
					}
					else csv_szoveg2[s4++] = csv_szoveg[s3];
				}
			}
		}
	}
}

//******************************
//.OBJ fájlok beolvasása
//******************************
void obj_loader(char fajlnev[], int* offset,
	float nyers_pontok[MAX_OBJ_NUM][3], int* nyers_pontok_tomb_length,
	int nyers_index_tomb[MAX_OBJ_NUM], int* nyers_index_tomb_length,
	int nyers_offset_tomb[MAX_OBJ_NUM], int* nyers_offset_tomb_length,
	int nyers_csucspontok_tipus[MAX_OBJ_NUM], int* nyers_csucspontok_tipus_length,
	int nyers_szinek_tomb[MAX_OBJ_NUM], int* nyers_szinek_tomb_length)
{
	int i, j;
	float adat1, adat2, adat3, adat4;
	fajl = fopen(fajlnev, "rt"); if (fajl == NULL) { printf("file io error"); return; }

	tomb_faces_length = tomb_vertices_length = 0;

	while (!feof(fajl))
	{
		fgets((char*)sor1, maxsorszelesseg, fajl);

		if (sor1[0] == 118 && sor1[1] == 32) //*** 'v '
		{
			getelement(sor1, 1, sor2); adat1 = atof((const char*)sor2);
			getelement(sor1, 2, sor2); adat2 = atof((const char*)sor2);
			getelement(sor1, 3, sor2); adat3 = atof((const char*)sor2);
			tomb_vertices[tomb_vertices_length][0] = adat1;
			tomb_vertices[tomb_vertices_length][1] = adat2;
			tomb_vertices[tomb_vertices_length++][2] = adat3;
		}
		else if (sor1[0] == 102 && sor1[1] == 32) //*** 'f '
		{
			elemszam = getelementcount(sor1);

			tomb_faces[tomb_faces_length][0] = elemszam;
			for (i = 1; i < elemszam + 1; ++i)
			{
				getelement(sor1, i, sor2); adat1 = atof((const char*)sor2);
				tomb_faces[tomb_faces_length][i] = adat1 - 1;
			}
			++tomb_faces_length;
		}
	}
	fclose(fajl);

	for (i = 0; i < tomb_faces_length; ++i)
	{
		for (j = 1; j < tomb_faces[i][0] + 1; ++j)
		{
			nyers_pontok[*nyers_pontok_tomb_length][0] = tomb_vertices[tomb_faces[i][j]][0];
			nyers_pontok[*nyers_pontok_tomb_length][1] = tomb_vertices[tomb_faces[i][j]][1];
			nyers_pontok[*nyers_pontok_tomb_length][2] = tomb_vertices[tomb_faces[i][j]][2];
			++* nyers_pontok_tomb_length;
		}
		nyers_index_tomb[*nyers_index_tomb_length] = tomb_faces[i][0];
		++* nyers_index_tomb_length;
		nyers_csucspontok_tipus[*nyers_csucspontok_tipus_length] = 1;
		++* nyers_csucspontok_tipus_length;
		nyers_offset_tomb[*nyers_offset_tomb_length] = *offset;
		++* nyers_offset_tomb_length;
		*offset += tomb_faces[i][0];
		nyers_szinek_tomb[*nyers_szinek_tomb_length] = RGB(200, 222, 200);
		++* nyers_szinek_tomb_length;
	}
}

//******************************
//meterorok inicializálása
//******************************
void setup_meteors(void)
{
	int i;
	float billeno = 1;

	global_global_offset = global_nyers_csucspontok_length = global_nyers_csucspontok_index_length = global_nyers_csucspontok_tipus_length = global_nyers_csucspontok_offset_length = 0;
	global_forgatott_csucspontok_length = global_lekepezett_csucspontok_length = global_zorder_length = 0;

	for (i = 0; i < 10; ++i)
	{
		meteor_statusz[i] = 0;
		if (rand() % 3 < 2) billeno = rand() % 250;
		else billeno = -1 * rand() % 250;
		global_nyers_csucspontok[global_nyers_csucspontok_length][0] = billeno;
		global_nyers_csucspontok[global_nyers_csucspontok_length][1] = 0;
		global_nyers_csucspontok[global_nyers_csucspontok_length][2] = rand() % 350;
		++global_nyers_csucspontok_length;
		global_akt_forg_szog[i] = rand() % 359;
		global_nyers_csucspontok_index[global_nyers_csucspontok_index_length++] = 1;
		global_nyers_csucspontok_tipus[global_nyers_csucspontok_tipus_length++] = 1;
		global_nyers_csucspontok_offset[global_nyers_csucspontok_offset_length++] = global_global_offset;
		++global_global_offset;
	}
}

//******************************
//meteorok frissítése
//******************************
void update_meteors(void)
{
	int i;
	float billeno = 1;
	for (i = 0; i < global_nyers_csucspontok_length; ++i)
	{
		if (global_nyers_csucspontok[i][2] > 500)
		{
			if (rand() % 3 < 2) billeno = rand() % 250;
			else billeno = -1 * (rand() % 250);
			global_nyers_csucspontok[i][0] = billeno;
			global_nyers_csucspontok[i][2] = 0;
			global_akt_forg_szog[i] = 0;
			meteor_statusz[i] = 0;
		}
	}
}

//******************************
//háttér inicializálása
//******************************
void setup_stars(void)
{
	int i;
	kp_x = stage_stageWidth / 2;
	kp_y = stage_stageHeight / 2;
	for (i = 0; i < 100; ++i)
	{
		pontx[i] = rand() % stage_stageWidth;
		ponty[i] = rand() % stage_stageHeight;
		pontx_leptek[i] = (pontx[i] - kp_x) / kp_x * 15;
		ponty_leptek[i] = (ponty[i] - kp_y) / kp_y * 3;
	}
}

//******************************
//csillagok mozgatása
//******************************
void move_stars(void)
{
	HDC hdc;
	HPEN hPen1;
	int i;

	hdc = hdcMem;
	hPen1 = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, hPen1);

	kp_x = stage_stageWidth / 2;
	kp_y = stage_stageHeight / 2;
	for (i = 0; i < 100; ++i)
	{
		Ellipse(hdc, pontx[i], ponty[i], pontx[i] + 2, ponty[i] + 2);

		pontx[i] += pontx_leptek[i];
		ponty[i] += ponty_leptek[i];
		if (pontx[i] > (kp_x * 2) || ponty[i] > (kp_y * 2) || pontx[i] < 0 || ponty[i] < 0)
		{
			pontx[i] = rand() % (kp_x * 2);
			ponty[i] = rand() % (kp_y * 2);
			pontx_leptek[i] = (pontx[i] - kp_x) / kp_x * 15;
			ponty_leptek[i] = (ponty[i] - kp_y) / kp_y * 3;
		}
	}
	DeleteObject(hPen1);
}

//******************************
//ûrhajó animálása
//******************************
void vadasz_mozgatasa(void)
{
	if (x_mozgas_iranya == 1)
	{
		if ((vadasz_x + x_sebesseg + vadasz_width) < stage_stageWidth)
		{
			vadasz_x += x_sebesseg;
			x_sebesseg -= sebesseg_faktor;
			sebesseg_faktor += 0.5;
			if (x_sebesseg <= 0) x_mozgas_iranya = 0;
		}
	}
	else if (x_mozgas_iranya == 2)
	{
		if ((vadasz_x - x_sebesseg) > 0)
		{
			vadasz_x -= x_sebesseg;
			x_sebesseg -= sebesseg_faktor;
			sebesseg_faktor += 0.5;
			if (x_sebesseg <= 0) x_mozgas_iranya = 0;
		}
	}
	if (urhajo_fszog_y > 180) --urhajo_fszog_y;
	else if (urhajo_fszog_y < 180) ++urhajo_fszog_y;
	if (vadasz_lebeges == 12) lebeges_faktor = -3;
	if (vadasz_lebeges == -12) lebeges_faktor = 3;
	vadasz_lebeges += lebeges_faktor;
	urhajo_fszog_x += lebeges_faktor / 10;
}

//************************
//dupla pufferelés segédfüggvény
//************************
void DB_prepare_puffer(void)
{
	GetClientRect(Form1, &rc);
	hdc = GetDC(Form1);
	hbmMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
}

//************************
//dupla pufferelés segédfüggvény
//************************
void DB_start_drawing(void)
{
	GetClientRect(Form1, &rc);
	hdc = GetDC(Form1);
	BeginPaint(Form1, &ps);
	hdcMem = CreateCompatibleDC(hdc);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	hbrBkGnd = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdcMem, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);
}

//************************
//dupla pufferelés segédfüggvény
//************************
void DB_end_drawing(void)
{
	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	EndPaint(Form1, &ps);
}

//************************
//játék vége állapot megjelenítése
//************************
void game_over(void)
{
	HFONT hFont;
	HDC    hdc2;
	HPEN hPen1;
	HBRUSH hBrush;
	hdc2 = hdcMem;

	hPen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	hFont = CreateFont(72, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, 5, VARIABLE_PITCH, TEXT("Bahnschrift"));
	SelectObject(hdc2, hFont);
	SetBkMode(hdc2, TRANSPARENT);
	SetTextColor(hdc2, RGB(255, 255, 255));

	RECT rect;
	GetWindowRect(Form1, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;


	int xPos = width / 2;
	int yPos = height / 2;


	SelectObject(hdc2, hPen1);
	SelectObject(hdc2, hBrush);
	SetPolyFillMode(hdc2, ALTERNATE);
	TextOut(hdc2, xPos, yPos, TEXT("Game Over"), 9);
	ReleaseDC(Form1, hdc2);
	DeleteObject(hPen1);
	DeleteObject(hBrush);
	DeleteObject(hFont);
}

//************************
//bitkép betöltéséhez
//************************
BITMAPFILEHEADER* DibLoadImage(PTSTR pstrFileName)
{
	BOOL bSuccess;
	DWORD dwFileSize, dwHighSize, dwBytesRead;
	HANDLE hFile;
	BITMAPFILEHEADER* pbmfh;

	hFile = CreateFileA((LPCSTR)pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return NULL; }
	dwFileSize = GetFileSize(hFile, &dwHighSize);

	if (dwHighSize) { CloseHandle(hFile); return NULL; }
	pbmfh = (BITMAPFILEHEADER*)malloc(dwFileSize);
	if (!pbmfh) { CloseHandle(hFile); return NULL; }
	bSuccess = ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!bSuccess || (dwBytesRead != dwFileSize)
		|| (pbmfh->bfType != *(WORD*)"BM")
		|| (pbmfh->bfSize != dwFileSize))
	{
		free(pbmfh); return NULL;
	}
	return pbmfh;
}

//************************
//bitkép betöltése
//************************
void BMP_betoltes(kepinfo* bitkep, const char kepfajl[])
{
	if (bitkep->pbmfh) { free(bitkep->pbmfh); bitkep->pbmfh = NULL; }
	bitkep->pbmfh = DibLoadImage(PTSTR(kepfajl));
	if (bitkep->pbmfh == NULL);
	else
	{
		bitkep->pbmi = (BITMAPINFO*)(bitkep->pbmfh + 1);
		bitkep->pBits = (BYTE*)bitkep->pbmfh + bitkep->pbmfh->bfOffBits;
		if (bitkep->pbmi->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
		{
			bitkep->Width = ((BITMAPCOREHEADER*)bitkep->pbmi)->bcWidth;
			bitkep->Height = ((BITMAPCOREHEADER*)bitkep->pbmi)->bcHeight;
		}
		else
		{
			bitkep->Width = bitkep->pbmi->bmiHeader.biWidth;
			bitkep->Height = abs(bitkep->pbmi->bmiHeader.biHeight);
		}
	}
}

void ScaleBMP2(HDC hdc, BITMAPINFO* pbmi, BYTE* pBits, int xkoo, int ykoo, int width, int height, int kep_width, int kep_height)
{
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchDIBits(hdc, xkoo, ykoo, width, height, 0, 0, kep_width, kep_height, pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
}

