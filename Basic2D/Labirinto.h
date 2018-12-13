///////////////////////////////////////////////////////////////////
//
//  A simple rectangular maze
//  Classi per gestire un labirinto rettangolare
//
///////////////////////////////////////////////////////////////////

//  blocco contro include multiplo / Avoid problems with multple inclusions
#if !defined(LABIRINTOH_INCLUSA)
#define LABIRINTOH_INCLUSA

// disable warning signed/unsigned mismatch
#pragma warning(disable: 4018)

// disable deprecation
#pragma warning(disable: 4996)

#include <windows.h>
#include <math.h>
#include <vector>

#define absv(x)         ((x) < 0 ? (-(x)) : (x))

///////////////////////////////////////////////////////////////////////
//  Cella di labirinto (rettangolare ?)
//  Rectangular maze cell
class CellaR
{
public:
  unsigned visitata;    // how many times has benn visited
  int a;                // spare int for processing
  //  Ogni cella ha solo due muri, gli altri sono delle celle
  //  vicine (l'ultima riga e l'ultima colonna hanno piu`
  //  muri del necessario)
  //  Every cell has two walls, East and South (plus all the externals
  //  walls of the maze
  bool muroE;           // true if the east wall is present
  bool muroS;           // true if the south
  //  Casella speciale colorata (non di labirinto)
  //  Special cell (solid)
  bool special;
  
  COLORREF colore;
  //  texture indices
  int floorTexture;
  int ceilTexture;
  int WallsTexture[4];  // 0-3 see NORT EST ...
  bool WallsTransparency[2]; //0 per nord/sud 1 per est/ovest
	//dare id per facciata e retro
public:
  CellaR(): visitata(0), a(0), muroE(true), muroS(true), 
    special(false), floorTexture(0), ceilTexture(4)

  { colore = RGB(0,0,0);
  WallsTexture[0] = 4;
  WallsTexture[1] = 4;
  WallsTexture[2] = 4;
  WallsTexture[3] = 4;
  WallsTransparency[0] = false;
  WallsTransparency[1] = false;
  }
};


//  Gestione muri, codice numeri      0
//  Walls codes                     3   1
//                                    2
#define NORD  0
#define EST   1
#define SUD   2
#define OVEST 3

///////////////////////////////////////////////////////////////////////
//	Classe che definisce un labirinto rettangolare
//  Class to store a rectangular maze
class CLabR
{
public:
  //  Il labirinto: rettangolo di dx x dy celle e
  int dx, dy;             // number of cells in x and y directions

  std::vector<CellaR> L;  // celle o stanze (cells or rooms)

  //  Positions of:
  int xg, yg;             // giocatore - player
  int xs, ys;             // entrata - entrance
  int xu, yu;             // uscita - exit

  bool vinto;             // flag se arrivo in fondo, true if end reached

public:
	CLabR();								// non c'e` - do not exist
	CLabR(int nrows, int ncols); // creator

  //  init labirinto
  void Init_Perfect0();
  void Init_Perfect0_parcroc();
  void CreaMuroVerticale(int xm, int y0, int y1, bool porta);
  // muro fra ym e ym+1 da x0 a x1 compresi
  void CreaMuroOrizzontale(int ym, int x0, int x1, bool porta);

  //  Utilita` per gestire
  inline int GetIn(int x, int y) {      // indice da x,y
    return (y*dx + x); }

  inline CellaR & GetC(int x, int y) {  //  Cella  x,y
    return L[GetIn(x,y)]; }

  void GetXY(int i, int &x, int &y) {   // da indice a x,y
    y = i / this->dx; x = (i % this->dx); }

  //  Torna nel nome il numero di muri della cella index, in
  //  M true o false (tanti true come il numero che torna)
  int GetMuri(int index, bool M[4]);
  //  come sopra, solo numero muri
  int GetMuri(int index);
  //  Torna true se la cella index ha muro in direzione d
  bool MuroDir(int index, int d);
  //  Forza il muro della cella index in direzione d a true o false
  void SetMuro(int index, int d, bool muro);

  int GetViciniSpeciali(int index);

  //  Torna numero e indici dei vicini con tutti i muri intatti e
  //  i loro indici in V (da 0 a 4 indici, a seconda di cosa torna)
  int GetViciniChiusi(int index, int V[4]);
  int GetVicini(int index, int V[4]);
  int GetViciniRaggiungibili(int index, int V[4]);

  void AbbattiMuro(int i1, int i2);

  void canctraccia();
  void nelpostopeggiore();

	void InitRand();

  //  Risoluzione
  bool QuasiRisolvi();
  bool autoMove();

  void cancellaMuri();
  void mettiMuri();

};



#endif		//	LABIRINTOH_INCLUSA