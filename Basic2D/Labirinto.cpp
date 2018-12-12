///////////////////////////////////////////////////////////////////
//
//  Labirinto rettangolare
//
///////////////////////////////////////////////////////////////////

#include "Labirinto.h"
#include <time.h>
#include <stack>

///////////////////////////////////////////////////////////////////////
//  class CLabR

//  Init con creazione labirinto "tutto chiuso"
//  Entrano le dimensioni
//  Init to a "full closed maze"
//  nrows and ncols are the dimensions
CLabR::CLabR( int nrows, int ncols ): dx(nrows), dy(ncols),
  xg(0), yg(0), xs(0), ys(0), xu(0), yu(0), vinto(false)
{
  CellaR C;
  for(int i=0; i < dx*dy; i++) L.push_back(C);
}

//  random number init
void CLabR::InitRand()
{
/*
  int toglimidiqui;
  srand(3);
  return;
  //*/

  //  init rand
	static time_t tlast;
	static bool never = true;

	time_t tt = 13;		//sempre lo stesso labirinto
	if( never ) { never = false; tlast = tt; }
	else {
		if( tt > tlast ) tlast = tt;
		else return;
	}
  srand( (unsigned) tlast );
}

//  Da labirinto tutto chiuso a labirinto
//  Versione zero
//  Depth-First Search
//  A perfect maze: a maze which has one and only one path from
//  any point in the maze to any other point.
void CLabR::Init_Perfect0()
{
	this->InitRand();

  //  Posizione iniziale
  xs = 0; ys = rand() % this->dy;

  //  Posizione finale
  xu = this->dx-1;  yu = rand() % this->dy;

  //  Posizione giocatore
  xg = xs; yg = ys;

  int visited = 1, total = dx*dy;
  std::stack<int> St;
  int V[4];
  int current = rand() % total;
  while( visited < total ) {
    int nv = this->GetViciniChiusi(current,V);
    if( nv > 0 ) {
      int ch = V[0];
      if( nv > 1 ) { ch = rand() % nv; ch = V[ch]; }
      AbbattiMuro(current,ch);
      St.push(current); current = ch; visited++;
    } else {
      if( St.size() == 0 ) 
        return;  // non puo`....
      current = St.top(); St.pop();
    }
  }

  this->QuasiRisolvi();
}




//  Init con caselle nere casuali
//  Same as Init_Perfect0 with holes (unreachable rooms)
void CLabR::Init_Perfect0_parcroc()
{
	this->InitRand();

  //  Posizione iniziale
  xs = 0; ys = rand() % this->dy;

  //  Posizione finale
  xu = this->dx-1;  yu = rand() % this->dy;

  //  Posizione giocatore
  xg = xs; yg = ys;

  //  Caselle nere
  {
    int i, ncn = dx*2, nn;
    int in_s = GetIn(xs, ys);
    int in_u = GetIn(xu, yu);
    int V[4], Vs[4], nvs, Vu[4], nvu;
    nvs = GetVicini(in_s,Vs);
    nvu = GetVicini(in_u,Vu);
    for(nn=i=0; i < ncn; ) {
      nn++;
      if( nn > dx*dy*2 ) break;
      int j, k = rand() % (dx*dy);
      if( L[k].special ) continue;
      if( k == in_s || k == in_u ) continue;
      int nc = this->GetVicini(k,V);
      bool meglioNo = false;
      for(j=0; j < nc; j++) {
        if( L[V[j]].special ) meglioNo = true;
      }
      for(j=0; j < nvs; j++) {
        if( L[Vs[j]].special ) meglioNo = true;
      }
      for(j=0; j < nvu; j++) {
        if( L[Vu[j]].special ) meglioNo = true;
      }
      if( meglioNo ) continue;
      i++; L[k].special = true; L[k].colore = RGB(0,0,0);
    }
  }

  //  resto
  int visited = 1, total = dx*dy;
  std::stack<int> St;
  int V[4];
  int current = rand() % total;
  while( L[current].special  ) current = rand() % total;
  while( visited < total ) {
    int nv = this->GetViciniChiusi(current,V);
    if( nv > 0 ) {
      int ch = V[0];
      if( nv > 1 ) { ch = rand() % nv; ch = V[ch]; }
      AbbattiMuro(current,ch);
      St.push(current); current = ch; visited++;
    } else {
      if( St.size() == 0 ) break;   // mah
      current = St.top(); St.pop();
    }
  }

  this->QuasiRisolvi();
}




//  crea muro fra xm e xm+1 da y0 a y1 compresi
//  se porta apre una porta a caso
void CLabR::CreaMuroVerticale(int xm, int y0, int y1, bool porta)
{
  int i;
  int nom = y0-1;
  if( porta ) nom = y0 + (rand() % (y1-y0+1));
  for(i=y0; i <= y1; i++) {
    if( i != nom ) SetMuro( GetIn(xm,i),EST,true);
  }
}


//  crea muro fra ym e ym+1 da x0 a x1 compresi
//  se porta apre una porta a caso
void CLabR::CreaMuroOrizzontale(int ym, int x0, int x1, bool porta)
{
  int i;
  int nom = x0-1;
  if( porta ) nom = x0 + (rand() % (x1-x0+1));
  for(i=x0; i <= x1; i++) {
    if( i != nom ) SetMuro( GetIn(i,ym),SUD,true);
  }
}


//  Marca le celle per risoluzione, usando a
//  a contiene la distanza dall'uscita
//  Torna false se non si puo` risolvere
bool CLabR::QuasiRisolvi()
{
  int i, nc, nv, oa;
  nc = dx*dy;
  for(i = 0; i < nc; i++) this->L[i].a = nc;
  std::stack<int> St;
  i = this->GetIn(xu,yu);
  L[i].a = 0;
  St.push(i);
  int V[4];
  while( St.size() > 0 ) {
    i = St.top(); St.pop();
    oa = L[i].a;
    nv = this->GetViciniRaggiungibili(i,V);
    for(i=0; i < nv; i++) {
      if( L[V[i]].a > oa+1 ) {
        L[V[i]].a = oa+1; St.push(V[i]);
      }
    }
  }

  if( this->GetC(xs,xs).a < nc ) return true;

  return false;
}


//  Muove un passo verso l'uscita, a partire da dove e`
//  Vale solo dopo QuasiRisolvi()
//  Torna true se ha mosso
bool CLabR::autoMove()
{
  int V[4], nv;
  int cur = GetIn(xg,yg);
  nv = GetViciniRaggiungibili(cur,V);
  int m = -1, aa;
  for(int i=0; i < nv; i++) {
    int av = L[V[i]].a;
    if( m == -1 || av < aa ) { aa = av; m = V[i]; }
  }
  if( m != -1 && aa < L[cur].a ) {
    GetXY(m,xg,yg); L[cur].visitata++;
    if( xg == xu && yg == yu ) vinto = true;
    return true;
  }
  return false;
}


void CLabR::canctraccia()
{
  for( int i=0; i < L.size(); i++)
    L[i].visitata = 0;
}

void CLabR::nelpostopeggiore()
{
  int am = 0, i;

  for( i=0; i < L.size(); i++) {
    if( !L[i].special && L[i].a > am && GetMuri(i) < 4 ) am = L[i].a;
  }
  std::vector<int> V;
  for( i=0; i < L.size(); i++) {
    if( !L[i].special && L[i].a == am && GetMuri(i) < 4 ) V.push_back(i);
  }
  int ip = 0;
  if( V.size() == 1 ) ip = V[0];
  else {
    int t = V.size();
    int pp = rand() % t;
    ip = V[pp];
  }
  this->GetXY(ip,xg,yg);

}

//  Torna numero di vicini speciali
int CLabR::GetViciniSpeciali(int index) 
{
  int vs = 0;
  int x, y;
  y = index / this->dx; x = index % this->dx;

  if( x <= dx-1 && GetC(x+1,y).special ) vs++;
  if( x > 0 && GetC(x-1,y).special ) vs++;
  if( y <= dy-1 && GetC(x,y+1).special ) vs++;
  if( y > 0 && GetC(x,y-1).special ) vs++;

  return vs;
}

//  Torna true se la cella index ha muro in direzione d
bool CLabR::MuroDir(int index, int d)
{
  int x, y;
  y = index / this->dx; x = index % this->dx;
  if(d == EST ) {
    if( x == dx-1 ) return true;
//    if( this->GetC(x+1,y).special ) return true;
    return L[index].muroE;
  }
  else if(d == SUD ) {
    if( y == dy-1 ) return true;
//    if( this->GetC(x,y+1).special ) return true;
    return L[index].muroS;
  }
  else if(d == NORD ) {
    if( y == 0 ) return true;
//    if( this->GetC(x,y-1).special ) return true;
    return L[index-dx].muroS;
  }
  else if(d == OVEST ) {
    if( x == 0 ) return true;
//    if( this->GetC(x-1,y).special ) return true;
    return L[index-1].muroE;
  }
  return true;  // non ci passa mai, ma il compilatore da un warning
}


//  Forza il muro della cella index in direzione d a true o false
void CLabR::SetMuro(int index, int d, bool muro) 
{
  int x, y;
  y = index / this->dx; x = index % this->dx;
  if(d == EST ) {
    if( x == dx-1 ) return;
    L[index].muroE = muro;
  }
  else if(d == SUD ) {
    if( y == dy-1 ) return;
    L[index].muroS = muro;
  }
  else if(d == NORD ) {
    if( y == 0 ) return;
    L[index-dx].muroS = muro;
  }
  else if(d == OVEST ) {
    if( x == 0 ) return;
    L[index-1].muroE = muro;
  }
}


//  Torna nel nome il numero di muri della cella index, in
//  M true o false (tanti true come il numero che torna)
int CLabR::GetMuri(int index, bool M[4])
{
  int i, muri = 0;
  for(i=0; i < 4; i++) {
    if( MuroDir(index,i) ) { muri++; M[i] = true; }
    else M[i] = false;
  }
  return muri;
}


//  come sopra, solo numero muri
int CLabR::GetMuri(int index) 
{
  int i, muri = 0;
  for(i=0; i < 4; i++) { if( MuroDir(index,i) ) muri++;  }
  return muri;
}


//  Torna i vicini della cella index. Ovvero 4 (ma sui bordi
//  3 o 2)
int CLabR::GetVicini( int index, int V[4] )
{
  int nv=0;
  int x, y;
  this->GetXY(index, x, y);
  if( y > 0 ) { V[nv] = index-dx; nv++; }
  if( x < dx-1 ) { V[nv] = index+1; nv++; }
  if( y < dy-1 ) { V[nv] = index+dx; nv++; }
  if( x > 0 ) { V[nv] = index-1; nv++; }
  return nv;
}

//  Torna numero e indici dei vicini con tutti i muri intatti e
//  i loro indici in V (da 0 a 4 indici, a seconda di cosa torna)
int CLabR::GetViciniChiusi( int index, int V[4] )
{
  int nv=0;
  int x, y;
  this->GetXY(index, x, y);
  if( y > 0 && GetMuri(index-dx) == 4 ) { 
    if( !L[index-dx].special ) { V[nv] = index-dx; nv++; }
  }
  if( x < dx-1 && GetMuri(index+1) == 4 ) {
    if( !L[index+1].special ) { V[nv] = index+1; nv++; }
    }
  if( y < dy-1 && GetMuri(index+dx) == 4 ) {
      if( !L[index+dx].special ) { V[nv] = index+dx; nv++; }
    }
  if( x > 0 && GetMuri(index-1) == 4 ) {
    if( !L[index-1].special ) { V[nv] = index-1; nv++; }
    }
  return nv;
}


//  Torna numero e indici dei vicini raggiungibili e
//  i loro indici in V (da 0 a 4 indici, a seconda di cosa torna)
int CLabR::GetViciniRaggiungibili(int index, int V[4])
{
	int nv = 0;


	if (!MuroDir(index, NORD)) { V[nv] = index - dx; nv++; }
	if (!MuroDir(index, EST)) { V[nv] = index + 1;  nv++; }
	if (!MuroDir(index, SUD)) { V[nv] = index + dx; nv++; }
	//muro da oltrepassare
	if (index == 60) {
		V[nv] = index + dx; nv++;
	}
	if (index == 70) {	//e ritorno
		V[nv] = index - dx; nv++;
	}

	if (!MuroDir(index, OVEST)) { V[nv] = index - 1; nv++; }
	return nv;
}


//  Se ha senso abbatte il muro fra le due caselle passate
void CLabR::AbbattiMuro(int i1, int i2)
{
  if( i2 == i1-dx ) SetMuro(i1,NORD,false);
  if( i2 == i1+1  ) SetMuro(i1,EST,false);
  if( i2 == i1+dx ) SetMuro(i1,SUD,false);
  if( i2 == i1-1  ) SetMuro(i1,OVEST,false);
}

void CLabR::cancellaMuri() {
	for (int i = 0; i < L.size(); i++) {
		L[i].muroE = false;
		L[i].muroS = false;
	}
}

void CLabR::mettiMuri() {

	//rimetto muro esterno a lato
	for (int i = 9; i < L.size(); i = i + 10) {
		L[i].muroE = true;
	}

	//rimetto muro esterno sopra
	for (int i = 70; i < L.size(); i++) {
		L[i].muroS = true;
	}

	//creo labirinto come voglio
	std::vector<int> muriE = { 0, 3, 6, 12, 14, 15, 17, 18, 20, 21, 22, 23, 24, 26, 27, 28, 30, 35, 37, 43, 46, 48, 50, 51, 54, 55, 60, 61, 62, 63, 64, 66, 68, 71, 72, 73, 75 };
	for (int i = 0; i < size(muriE); i++) {
		L[muriE[i]].muroE = true;
	}
	std::vector<int> muriS = {1,2,4,6,8,11,13,16,21,23,25,27,28,31,32,33,36,37,38,41,42,43,45,47,48,53,56,57,58,60,65,67,68};
	for (int i = 0; i < size(muriS); i++) {
		L[muriS[i]].muroS = true;
	}

	//modifico le texture
	L[0].WallsTexture[0] = 16; //9 e 3/4

	L[37].WallsTexture[1] = 8; //porta
	L[38].WallsTexture[3] = 8; //porta dall'altro lato
	L[37].WallsTransparency[1] = true;

	L[51].WallsTexture[0] = 9;	
	L[55].WallsTexture[0] = 11;	//matrix wall
	L[6].WallsTexture[1] = 9;	

}





