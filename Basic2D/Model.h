///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

//  world geometry
// eye    (0,0,0)
// center (0,0,-1)
// up     (0,1,0)
// left -1, right 1
//  top & bottom centered on Y to match the window aspect ratio
// clipping planes: 1, 5.1

#pragma warning(disable: 4996)

#include <vector>
#include <time.h>
#include <math.h>
#include "Labirinto.h"

#define PI 3.141592654
#define ans_size 40

// A class for storing vertices
//  Vertex
class Vertex {
public:

  float x, y, z;        // 3d coordinate of the vertex
  float Nx, Ny, Nz;     // Normal to the surface (if present)
	float u, v;			// texture coordinates
	float r, g, b;		// Color (0.0 -  1.0)
  Vertex() {}
	Vertex(float x, float y, float z): x(x), y(y), z(z) { }
	Vertex(float x, float y, float z, float u, float v): x(x), y(y), z(z),
  u(u), v(v) { }
  inline void SetP(float x,float y,float z) { this->x = x; this->y = y;
  this->z = z; }
  inline void SetN(float x,float y,float z) { Nx = x; Ny = y; Nz = z; }
  inline void SetTexture(float uu,float vv) { u = uu; v = vv; }
  inline void SetColor(float rr,float gg,float bb) { r = rr; g = gg; b = bb; }
};


class MyModel {
public:
  //  general data for window and input controls
  HDC			hDC;		        // Private GDI Device Context
  HGLRC		hRC;		        // Permanent Rendering Context
  HWND		hWnd;		        // Holds Our Window Handle
  HINSTANCE	hInstance;		// Holds The Instance Of The Application

  bool	keys[256];			// Array Used For The Keyboard Routine
  bool	active;		      // Window Active Flag Set To TRUE By Default
  bool cursor;          // true if visible
  bool captured;        // true if the mouse is captured
  int cx, cy;           // client position of the cursor
  bool	fullscreen;	    // Fullscreen Flag 
	int height;						//altezza soffitto
	//riddles' stuff...
	int indice;
	char answer[ans_size];
	char matrix_solution[7] = { 'B','I','N','A','R','Y','\0' };
	char hogwarts_solution[3] = {'P','I','\0'};
	char qr_solution[18] = { 'W','E',' ','F','I','N','I','S','H','E','D',' ','I','D','E','A','S','\0' };
	int fullview_texture;	//texture for riddle_fullview
	bool suono_giusto, suono_sbagliato;
	int solved_riddles;
	bool fullview, gameover_fullview, solved_fullview;	// top view of the whole maze
	bool riddle_fullview, hogwarts_fullview, matrix_fullview, cancello_fullview, qr_fullview; //riddle dialog box
	bool Vinto, matrix_vinto, hogwarts_vinto, cancello_vinto, qr_vinto;

  
	CLabR *Maze;		// maze - labirinto
  int ldx, ldz;		// dimensions of the maze
  bool StartScreen;
  double px, pz;  // player position
  double angle;   // and orientation
	double angley;
  bool NoWalls;           // if true i can walk across walls
  bool illumin;
  double timeleft;
	bool timeout;
	clock_t Tstamp, Tstart;
	double Full_elapsed;

private:
  int Wheight, Wwidth;  // window dimensions in pixels

  //  model data
  std::vector<Vertex> floor;        // a cell floor
  std::vector<Vertex> ceil;        // a cell ceil
  std::vector<Vertex> wall;         // a cell wall
	void walls_height();

  GLuint	texture[45];			// Storage For 40 Textures!
  GLuint	base;				// Base Display List For The Font Set
public:
  //  methods
  MyModel(): hDC(NULL), hRC (NULL), hWnd (NULL), active (true),fullscreen(false), cursor(true), captured(false), illumin(true),
		fullview(true), gameover_fullview(false), riddle_fullview(false),matrix_fullview(false),hogwarts_fullview(false),cancello_fullview(false),qr_fullview(false),
		angle(0.0), angley(0.0), px (0.5), pz(0.5), NoWalls(false), StartScreen (true), 
		Vinto (false), qr_vinto(false), matrix_vinto(false), hogwarts_vinto(false), cancello_vinto(false), suono_giusto(false), suono_sbagliato(false),
		timeleft(100000),timeout(false),solved_fullview(false){
		
		indice = 0;
		solved_riddles = 0;

		ldx = 10; ldz = 8;
    Maze = new CLabR(ldx, ldz);
    Maze->Init_Perfect0();
		Maze->cancellaMuri();
		Maze->mettiMuri();
		height = 5; //height of sky
		px = 0.5 + 4.5;
		pz = 0.5 + 3.5;
		
		walls_height();

    // timing
    this->Tstart = this->Tstamp = clock();
    this->Full_elapsed = 0;
  }

  ~MyModel() {
    this->KillFont();
  }
  inline bool IsInClient(int x, int y) {
    if( x >= 0 && x < Wwidth && y >= 0 && y < Wheight ) return true;
    return false;
  }
  bool DrawGLScene(void);
  bool MoveOrCollide(double npx, double npz, double enpx, double enpz);
  void SetProjection();
  bool InitGL(void);
  void ReSizeGLScene(int width, int height);
  void glPrint(const char *fmt, ...);			// Custom GL "Print" Routine
	bool verifica_risposta(char* answer);

private:
  void DrawFloorText();
  void DrawFloorFullview();
  void DrawCeilText();
  void DrawWallsText(bool transparency);
  void DrawWallsFullview();
	void DrawRiddleFullview();
  bool LoadGLTextures(void);
  bool Load_a_texture(const char *name, int id);
  void BuildFont(void);
	void KillFont(void);

};

extern class MyModel Data;