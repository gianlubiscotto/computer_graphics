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
	int fullview_texture;
  bool captured;        // true if the mouse is captured
  int cx, cy;           // client position of the cursor
  bool	fullscreen;	    // Fullscreen Flag 
	bool suono_giusto,suono_sbagliato;
	//riddles' stuff...
	int indice;
	char answer[ans_size];
	char matrix_solution[7] = { 'B','I','N','A','R','Y','\0' };
	char hogwarts_solution[7];

  
	CLabR *Maze;		// maze - labirinto
  int ldx, ldz;		// dimensions of the maze
  bool fullview,gameover_fullview;	// top view of the whole maze
	bool riddle_fullview, hogwarts_fullview,matrix_fullview,cancello_fullview,qr_fullview; //riddle dialog box
  bool StartScreen;
  bool Vinto,matrix_vinto,hogwarts_vinto,cancello_vinto, qr_vinto;
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

  int frames;           // n. of frames
  double frameTime;     // for fps computation
  double fps;

  //  model data
  std::vector<Vertex> floor;        // a cell floor
  std::vector<Vertex> ceil;        // a cell ceil
  std::vector<Vertex> wall;         // a cell wall
  // elapsed time in seconds from the beginning of the program
  

  GLuint	texture[28];			// Storage For 28 Textures!
  GLuint	base;				// Base Display List For The Font Set
public:
  //  methods
  MyModel(): hDC(NULL), hRC (NULL), hWnd (NULL), active (true),
    fullscreen(false), frames(0), fps(0), cursor(true), captured(false),
  fullview(true),gameover_fullview(false), riddle_fullview(false),matrix_fullview(false), hogwarts_fullview(false),cancello_fullview(false),qr_fullview(false), angle(0.0), angley(0.0), px (0.5), pz(0.5), NoWalls(false),
  StartScreen (true), Vinto (false), qr_vinto(false), matrix_vinto(false), hogwarts_vinto(false), cancello_vinto(false), illumin (true), suono_giusto(false), suono_sbagliato(false),
  timeleft(100000),timeout(false){
		
		indice = 0;

		ldx = 10; ldz = 8;
    Maze = new CLabR(ldx, ldz);
    Maze->Init_Perfect0();
	Maze->cancellaMuri();
	Maze->mettiMuri();
	//int startc = Maze->GetIn(Maze->xs, Maze->ys);
	px = 0.5 + 4.5;
	pz = 0.5 + 3.5;
	//Maze->L[startc].floorTexture = 1;
	//Maze->L[startc].ceilTexture = 2;
	// int endc = Maze->GetIn(Maze->xu, Maze->yu);
	 //Maze->L[endc].floorTexture = 3;
//	Maze->L[endc].ceilTexture = 2;

	  // floor and wall init. 
	  // GEOMETRY: the 'y' axis is the vertical one. The maze is defined in x and z so that
	  // 0 < x < ldx and 0 < z < ldy
	  //	A floor cell is defined in y=0, 0 < x < 1 and 0 < z < 1
	  //	A wall is defined in z=0, -0.5 < x < 0.5 and 0 < y < 1
	  //	Each element is splitted in small parts to allow good lightning... (5x5 parts)

	  //	FLOOR ----------------------------------------------------
	  float Nx, Ny, Nz;	// normals
	  Nx = Nz = 0; Ny = 1.0f;
	  int parts = 5;
	  Vertex V;
	  V.SetColor(0.3f,0.3f,0.3f);				// all vertex colors are equals
	  V.SetN(Nx, Ny, Nz);					      // all vertex normals are equals
    // One floor cell: y = 0, x in [0,1], z in [0,1]
	  float d = 1.0f / ((float) parts);
	  for(int ix = 0; ix < parts; ix++) {
		  float x = ix * d;
		  for(int iz = 0; iz < parts; iz++) {
			  float z = iz * d;
			  V.SetP(x,0.0f,z);       V.SetTexture(x,z);      floor.push_back(V);
        V.SetP(x,0.0f,z+d);     V.SetTexture(x,z+d);    floor.push_back(V);

			  V.SetP(x+d,0.0f,z+d);   V.SetTexture(x+d,z+d);  floor.push_back(V);
			  V.SetP(x+d,0.0f,z);     V.SetTexture(x+d,z);    floor.push_back(V);
			  
		  }
	  }

	  

	  //	WALL ----------------------------------------------------
	  Nx = Ny = 0; Nz = -1.0f;
	  parts = 5;
	  V.SetColor(0.8f,0.8f,0.8f);				// all vertex colors are equals
	  V.SetN(Nx, Ny, Nz);					      // all vertex normals are equals
    // One wall cell: z = 0, x in [-0.5, 0.5], y in [0,1]
	  for(int ix = 0; ix < parts; ix++) {
		  float x = ix * d - 0.5f;
      float xt = x + 0.5f;
		  for(int iy = 0; iy < parts; iy++) {
			  float y = iy * d;
			  V.SetP(x, y, 0.0f);       V.SetTexture(xt,y);      wall.push_back(V);
			  V.SetP(x, y+d, 0.0f);     V.SetTexture(xt,y+d);    wall.push_back(V);
			  
			  V.SetP(x+d, y+d, 0.0f);   V.SetTexture(xt+d,y+d);  wall.push_back(V);
        V.SetP(x+d, y, 0.0f);     V.SetTexture(xt+d,y);    wall.push_back(V);
		  }
	  }

    // timing
    this->Tstart = this->Tstamp = clock();
    this->Full_elapsed = 0;
    this->frameTime = 0;
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
  void DrawCeilText();
  void DrawWallsText(bool transparency);
  void DrawWallsFullview();
	void DrawRiddleFullview();
  bool LoadGLTextures(void);
  bool Load_a_texture(const char *name, int id);
  void BuildFont(void);
  void KillFont(void);
  /*
  //  Conversion from pixel distance to float X and Y distance
  inline float PixToCoord_X(int pix)
    { return ( 2.0f * (float) pix * (float) plx ) / (float) Wwidth; }
  inline float PixToCoord_Y(int pix)
    { return ( 2.0f * (float) pix * (float) ply ) / (float) Wheight; }
  //  conversion from client coordinates to 3d world coordinates
  inline float ClientX2World(int x) {
    return ( (2.0f * float(plx) * float(x) / float(Wwidth)) - float(plx));
  }
  inline float ClientY2World(int y) {
    return ( float(ply) - (2.0f * float(ply) * float(y) / float(Wheight)));
  }*/

};

extern class MyModel Data;