///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Model.h"
#include "SOIL.h"

int tex;
// All Setup For OpenGL Goes Here
bool MyModel::InitGL(void)
{
	if (!this->LoadGLTextures())				// Jump To Texture Loading Routine
  {	return false; }							// If Texture Didn't Load Return FALSE
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
  this->BuildFont();
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  //glDisable(GL_CULL_FACE);
  // eye    (0,0,0)
  // center (0,0,-1)
  // up     (0,1,0)
  gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);

  //  Material and light0
  //  Set the material - start
  GLfloat MatAmbient[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat MatDiffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
  GLfloat MatSpecular[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat MatEmmision[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat MatShininess[] = {3.0f};

  glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpecular);
  glMaterialfv(GL_FRONT, GL_EMISSION, MatEmmision);
  glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess);
  //  Set the material - end

  //  Set the light source - start
  GLfloat LightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
  GLfloat LightDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
  GLfloat LightSpec[] = {0.17f, 0.1f, 0.1f, 1.0f};
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpec);
  GLfloat LightPosition[] = {3.0f, 2.0f, 1.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,0.01f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,0.5f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION,0.5f);
  
  glEnable(GL_LIGHT0);
  //  Set the light source - end

	
  return true;										// Initialization Went OK
}


void MyModel::ReSizeGLScene(int width, int height)
{
	if (height==0) height=1;					// Prevent A Divide By Zero By
	if (width==0) width=1;					// Prevent A Divide By Zero By

	this->Wwidth = width;
	this->Wheight = height;
	
	glViewport(0,0,width,height);						// Reset The Current Viewport

	//  saving the window width and height in pixels
  this->Wheight = height;
  this->Wwidth = width;

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

//  Load one texture
bool MyModel::Load_a_texture(const char *name, int id)
{
  texture[id] = SOIL_load_OGL_texture
		( name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y );
	if(texture[id] == 0) return false;

 	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[id]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;										// Return Success
}

// Load Bitmaps And Convert To Textures
bool MyModel::LoadGLTextures(void)
{
	// Floor textures
	if (!this->Load_a_texture("../Data/floor2.png", 0)) return false;
	//soffitto
	if (!this->Load_a_texture("../Data/ceil.jpg", 2)) return false;
	// Walls textures
	if (!this->Load_a_texture("../Data/wallpaper3.jpg", 4)) return false;
	if (!this->Load_a_texture("../Data/wall.png", 5)) return false;
	if (!this->Load_a_texture("../Data/wallpaper.jpg", 6)) return false;
	if (!this->Load_a_texture("../Data/bookshelf.jpg", 7)) return false;
	//porta
	if (!this->Load_a_texture("../Data/gate1.png", 8)) return false;
	if (!this->Load_a_texture("../Data/matrixtexture-1.png", 10)) return false;
	//muro matrix
	if (!this->Load_a_texture("../Data/matrix5_1.jpg", 11)) return false;
	//matrix textures
	if (!this->Load_a_texture("../Data/matrix1_1.jpg", 27)) return false;
	if (!this->Load_a_texture("../Data/matrix2_1.jpg", 28)) return false;
	if (!this->Load_a_texture("../Data/matrix3_1.jpg", 29)) return false;
	if (!this->Load_a_texture("../Data/matrix4_1.jpg", 30)) return false;

	//indovinello
	if (!this->Load_a_texture("../Data/question.jpg", 9)) return false;
	//cancello fullview
	if (!this->Load_a_texture("../Data/gatefullview.jpg", 12)) return false;
	if (!this->Load_a_texture("../Data/gameover.jpg", 15)) return false;
	if (!this->Load_a_texture("../Data/murooltrepassabile.png", 16)) return false;
	//qr
	if (!this->Load_a_texture("../Data/frame.png", 17)) return false;
	if (!this->Load_a_texture("../Data/qrtexture.jpg", 18)) return false;
	//qr textures
	if (!this->Load_a_texture("../Data/qr1.png", 31)) return false;
	if (!this->Load_a_texture("../Data/qr2.png", 32)) return false;
	if (!this->Load_a_texture("../Data/qr3.png", 33)) return false;
	if (!this->Load_a_texture("../Data/qr4.png", 34)) return false;
	if (!this->Load_a_texture("../Data/qr5.png", 35)) return false;
	if (!this->Load_a_texture("../Data/qr6.png", 36)) return false;


	//sheldon-hogwarts
	if (!this->Load_a_texture("../Data/bazinga.jpg", 19)) return false;
	//solved
	if (!this->Load_a_texture("../Data/solved.jpg", 20)) return false;
	//math textures
	if (!this->Load_a_texture("../Data/math1.jpg", 21)) return false;
	if (!this->Load_a_texture("../Data/math2_1.jpg", 22)) return false;
	if (!this->Load_a_texture("../Data/math3.jpg", 23)) return false;
	if (!this->Load_a_texture("../Data/math4_1.jpg", 24)) return false;
	if (!this->Load_a_texture("../Data/math2.jpg", 25)) return false;
	if (!this->Load_a_texture("../Data/math4_2.jpg", 26)) return false;


	return true;										// Return Success
}

void MyModel::SetProjection()
{
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	if( this->fullview || this->riddle_fullview || this->gameover_fullview || this->solved_fullview) {
    double a = 22.5 * Wwidth / ((double) Wheight);
    double hx;  // minimum y to see all the maze in the x direction
    hx = (((double) this->ldx+0.5)/2 ) / atan(a* PI /180.0);
    double hz;  // minimum y to see all the maze in the z direction
    hz = (((double) this->ldz+0.5)/2 ) / atan(22.5* PI /180.0);
    if( hx < hz ) hx = hz;
		// eye
		double ex = this->ldx / 2.0, ez = this->ldz / 2.0;
    gluPerspective(45.0 ,(GLfloat)Wwidth/(GLfloat)Wheight,0.1,1.1*hx);
		gluLookAt(ex, hx, ez,  ex,0.0,ez,  0.0, 0.0, 1.0);
	}
	
	else {
    double Far = 2.0 * ldx;
    if( ldz > ldx ) Far = 2.0* ldz;
    gluPerspective(55.0 ,(GLfloat)Wwidth/(GLfloat)Wheight,0.001,Far);
    double ex, ey = 0.5, ez;
    ex = this->px; ez = this->pz;
    double cx, cy, cz;
    cy = ey; cx = 5.0; cz = ez;
    cx = ex + cos(angle);
    cz = ez + sin(angle);
		cy = ey + sin(angley);
		gluLookAt(ex, ey, ez,  cx, cy, cz,  0.0, 1.0, 0.0);
	}
}

//  DRAW THE FLOOR - TEXTURE - NO ILLUMINATION
void MyModel::DrawFloorText()
{
  glEnable(GL_TEXTURE_2D);
  for(int i=0; i < this->Maze->L.size(); i++) {
    glLoadIdentity();
    int ix, iy;
    this->Maze->GetXY(i,ix,iy);
    int it = this->Maze->L[i].floorTexture;
    glBindTexture(GL_TEXTURE_2D, texture[it]);
    glTranslatef((float) ix, 0, (float) iy);

    glBegin(GL_QUADS);
    if( it != 3) {
      for(int i = 0; i < this->floor.size(); i++) {
        glTexCoord2f(floor[i].u, floor[i].v);
        glNormal3f(floor[i].Nx, floor[i].Ny, floor[i].Nz);
        glColor3f(floor[i].r, floor[i].g, floor[i].b);
        glVertex3f(floor[i].x, floor[i].y,  floor[i].z);
      }
    } else {
      for(int i = 0; i < this->floor.size(); i++) {
        glTexCoord2f(floor[i].u, floor[i].v);
        glNormal3f(floor[i].Nx, floor[i].Ny, floor[i].Nz);
        glColor3f(0.9f,0.9f,0.9f);
        glVertex3f(floor[i].x, floor[i].y,  floor[i].z);
      }
    }
    glEnd();
  }
}

void MyModel::DrawCeilText()
{
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < this->Maze->L.size(); i++) {
		glLoadIdentity();
		int ix, iy;
		this->Maze->GetXY(i, ix, iy);
		int it = this->Maze->L[i].ceilTexture;
		glBindTexture(GL_TEXTURE_2D, texture[it]);
		glTranslatef((float)ix, 1, (float)iy);

		glBegin(GL_QUADS);
		
			for (int i = this->floor.size()-1; i >=0; i--) {
				glTexCoord2f(floor[i].u, floor[i].v);
				glNormal3f(floor[i].Nx, floor[i].Ny, floor[i].Nz);
				//glColor3f(floor[i].r, floor[i].g, floor[i].b);
				glColor3f(1,1,1);
				glVertex3f(floor[i].x, floor[i].y, floor[i].z);
			}
		
		glEnd();
	}
}

//draw the matrix dialog box
void MyModel::DrawRiddleFullview() {
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();
	//tex viene modificata in move or collide con la texture corrispondente
	glBindTexture(GL_TEXTURE_2D, texture[this->fullview_texture]);
		glBegin(GL_QUADS);
			glColor3f(1, 1, 1);	
			glTexCoord2f(0.0f, 0.0f); glVertex3f(11,0.5,-.5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1,0.5,-.5);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, .5, 8.5);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f(11, .5, 8.5);
		glEnd();

	// Position The Text On The Screen
	//glDisable(GL_TEXTURE_2D);
	glColor3f(0.2f, 1.0f, 0.2f);
	//risposta dell'utente
	glRasterPos3f(8.0f, 0.7f, 2.3f);
	if (!this->cancello_fullview && !this->gameover_fullview && !this->solved_fullview) {
		Data.glPrint(this->answer);
	}
}

//  Draw the walls: special case for the map
void MyModel::DrawWallsFullview()
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  glColor3f(1.0f, 0.1f,0.0f);
  glLineWidth(2.2f);
  float zz = 0.1f;
  glLoadIdentity();
  glBegin(GL_LINES);
  for(int i=0; i < this->Maze->L.size(); i++) {
    int ix, iy;
    this->Maze->GetXY(i,ix,iy);
    if( this->Maze->L[i].muroE ) {
      glVertex3f((float) ix + 1, zz, (float) iy);
      glVertex3f((float) ix + 1, zz, (float) iy + 1);
    }
    if( this->Maze->L[i].muroS ) {
      glVertex3f((float) ix, zz, (float) iy+1);
      glVertex3f((float) ix + 1, zz, (float) iy+1);
    }
  }
  glVertex3f(0.0f, zz, 0.0f );
  glVertex3f((float) this->ldx, zz, 0.0f );
  glVertex3f(0.0f, zz, 0.0f );
  glVertex3f(0.0f, zz, (float) this->ldz );

  // player
  glColor3f(0.2f, 0.9f,0.33f);
  float plx = (float) this->px;
  float plz = (float) this->pz;
  float pdx = (float) (this->px + 0.4*cos(this->angle));
  float pdz = (float) (this->pz + 0.4*sin(this->angle));
	float pdy = (float)(this->px + 0.4*cos(this->angley));

  float p = 0.04f;
  glVertex3f(plx-p, zz, plz+p );
  glVertex3f(plx+p, zz, plz-p );
  glVertex3f(plx+p, zz, plz+p );
  glVertex3f(plx-p, zz, plz-p );

  glColor3f(0.99f, 0.9f,0.1f);
  glVertex3f(plx, zz, plz );
  glVertex3f(pdx, zz, pdz );

  // remove
  glColor3f(0.2f, 0.3f,0.9f);
  zz += 0.01f;
  glVertex3f(0.0f, zz, 0.0f );
  glVertex3f(0.5f, zz, 0.0f );


  glEnd();
}

//  DRAW THE WALLS - TEXTURE - NO ILLUMINATION

void MyModel::DrawWallsText(bool transparency)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		//  Loop on the maze cells
	for (int i = 0; i < this->Maze->L.size(); i++) {
		int ix, iy;
		this->Maze->GetXY(i, ix, iy);
		// EST wall?
		if (this->Maze->L[i].muroE && this->Maze->L[i].WallsTransparency[1]==transparency) {
			int it = this->Maze->L[i].WallsTexture[EST];
			glBindTexture(GL_TEXTURE_2D, texture[it]);
			glLoadIdentity();
			glTranslatef((float)ix + 1, 0, (float)iy + 0.5f);
			glRotatef(90.0f, 0, 1.0f, 0);
			glBegin(GL_QUADS);
			for (int j = 0; j < this->wall.size(); j++) {
				glTexCoord2f(wall[j].u, wall[j].v);
				glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
				glColor3f(wall[j].r, wall[j].g, wall[j].b);
				glVertex3f(wall[j].x, wall[j].y, wall[j].z);
			}
			glEnd();

			// An now... the WEST(OVEST) rear wall!
			
			if (i < this->Maze->L.size() - 1) {
				it = this->Maze->L[i + 1].WallsTexture[OVEST];
				glBindTexture(GL_TEXTURE_2D, texture[it]);
				glLoadIdentity();
				glTranslatef((float)ix + 1, 0, (float)iy + 0.5f);
				glRotatef(-90.0f, 0, 1.0f, 0);
				glBegin(GL_QUADS);
				for (int j = 0; j < this->wall.size(); j++) {
					glTexCoord2f(wall[j].u, wall[j].v);
					glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
					glColor3f(wall[j].r, wall[j].g, wall[j].b);
					glVertex3f(wall[j].x, wall[j].y, wall[j].z);
				}
				glEnd();
			}
			
		}

		// SOUTH wall?
		if (this->Maze->L[i].muroS && this->Maze->L[i].WallsTransparency[0] == transparency) {
			int it = this->Maze->L[i].WallsTexture[SUD];
			glBindTexture(GL_TEXTURE_2D, texture[it]);
			glLoadIdentity();
			glTranslatef((float)ix + 0.5f, 0, (float)iy + 1.0f);
			glBegin(GL_QUADS);
			for (int j = 0; j < this->wall.size(); j++) {
				glTexCoord2f(wall[j].u, wall[j].v);
				glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
				glColor3f(wall[j].r, wall[j].g, wall[j].b);
				glVertex3f(wall[j].x, wall[j].y, wall[j].z);
			}
			glEnd();

			// An now... the NORTH(NORD) rear wall!
			if (i < this->Maze->L.size() - this->ldx) {
				it = this->Maze->L[i + this->ldx].WallsTexture[NORD];
				glBindTexture(GL_TEXTURE_2D, texture[it]);
				glLoadIdentity();
				glTranslatef((float)ix + 0.5f, 0, (float)iy + 1.0f);
				glRotatef(180.0f, 0, 1.0f, 0);
				glBegin(GL_QUADS);
				for (int j = 0; j < this->wall.size(); j++) {
					glTexCoord2f(wall[j].u, wall[j].v);
					glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
					glColor3f(wall[j].r, wall[j].g, wall[j].b);
					glVertex3f(wall[j].x, wall[j].y, wall[j].z);
				}
				glEnd();
			}
			
		}
	}   // END Loop on the maze cells


	//  z = 0 and x = 0 boundary walls

	//  x = 0
	for (int i = 0; i < this->ldx; i++) {
		int it = this->Maze->L[i].WallsTexture[EST];
		glBindTexture(GL_TEXTURE_2D, texture[it]);
		glLoadIdentity();
		int ix = 0, iy;
		iy = i;
		glTranslatef((float)ix, 0, (float)iy + 0.5f);
		glRotatef(90.0f, 0, 1.0f, 0);

		glBegin(GL_QUADS);
		for (int j = 0; j < this->wall.size(); j++) {
			glTexCoord2f(wall[j].u, wall[j].v);
			glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
			glColor3f(wall[j].r, wall[j].g, wall[j].b);
			glVertex3f(wall[j].x, wall[j].y, wall[j].z);
		}
		glEnd();

		// An now... the WEST(OVEST) rear wall!
		it = this->Maze->L[i].WallsTexture[OVEST];
		glBindTexture(GL_TEXTURE_2D, texture[it]);
		glLoadIdentity();
		glTranslatef((float)ix, 0, (float)iy + 0.5f);
		glRotatef(-90.0f, 0, 1.0f, 0);

		glBegin(GL_QUADS);
		for (int j = 0; j < this->wall.size(); j++) {
			glTexCoord2f(wall[j].u, wall[j].v);
			glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
			glColor3f(wall[j].r, wall[j].g, wall[j].b);
			glVertex3f(wall[j].x, wall[j].y, wall[j].z);
		}
		glEnd();
	}

	// z = 0
	for (int i = 0; i < this->ldx; i++) {
		int it = this->Maze->L[i].WallsTexture[SUD];
		glBindTexture(GL_TEXTURE_2D, texture[it]);
		glLoadIdentity();
		int ix, iy = 0;
		ix = i;
		glTranslatef((float)ix + 0.5f, 0, 0);

		glBegin(GL_QUADS);
		for (int j = 0; j < this->wall.size(); j++) {
			glTexCoord2f(wall[j].u, wall[j].v);
			glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
			glColor3f(wall[j].r, wall[j].g, wall[j].b);
			glVertex3f(wall[j].x, wall[j].y, wall[j].z);
		}
		glEnd();

		// An now... the NORTH(NORD) rear wall!
		it = this->Maze->L[i].WallsTexture[NORD];
		glBindTexture(GL_TEXTURE_2D, texture[it]);
		glLoadIdentity();
		glTranslatef((float)ix + 0.5f, 0, 0);
		glRotatef(180.0f, 0, 1.0f, 0);

		glBegin(GL_QUADS);
		for (int j = 0; j < this->wall.size(); j++) {
			glTexCoord2f(wall[j].u, wall[j].v);
			glNormal3f(wall[j].Nx, wall[j].Ny, wall[j].Nz);
			glColor3f(wall[j].r, wall[j].g, wall[j].b);
			glVertex3f(wall[j].x, wall[j].y, wall[j].z);
		}
		glEnd();
	}

}

bool MyModel::DrawGLScene(void)
{
  this->SetProjection();

  //  TIMING - start
  clock_t t = clock();
  
  // elapsed time in seconds from the beginning of the program
  this->Full_elapsed = double (t - Tstart) /  (double) CLOCKS_PER_SEC;
	
	//timeout
	if (this->Full_elapsed >= 15*60) {
		this->gameover_fullview = true;
		this->riddle_fullview = true;
		this->fullview_texture = 15;
		this->SetProjection();
		this->timeout = true;
	}

	if (this->solved_riddles == 3) {
		this->solved_fullview = true;
		this->riddle_fullview = true;
		this->fullview_texture = 20;
		this->SetProjection();
	}
	//timer to be displayed
	this->timeleft = double(15 * 60) - this->Full_elapsed;
  //  TIMING - end

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
  glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View
  //  In this way texture and surface color are blended
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	if (!illumin) glDisable(GL_LIGHTING);
	else {
		glEnable(GL_LIGHTING);
		GLfloat LightPosition[] = { (float)px, .8f, (float)pz, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	}

  //  floor solo se mappa dall'alto o dentro il gioco
	if (!this->riddle_fullview) {
		this->DrawWallsText(false);	//non png
		this->DrawCeilText();
		this->DrawFloorText();
		this->DrawWallsText(true);	//png files
	}
	
  //  walls
  if( this->fullview ) this->DrawWallsFullview(); //disegna le righe rosse e il pg
	//schermata indovinello
	if (this->riddle_fullview) this->DrawRiddleFullview(); 
	
	
  //  Some text in map mode
  if( this->fullview ) {
    glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
 	  glLoadIdentity();									// Reset The Current Modelview Matrix
	  glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

 	  // Color
	  glColor3f(0.4f,0.4f,0.4f);
	  // Position The Text On The Screen
		glRasterPos3f((float) ldx,1.0f, (float) ldz + 0.1f);
		this->glPrint("Time left: %6.2f sec.  ", timeleft);
	
  }

  return true;
}

bool MyModel::verifica_risposta(char* answer) {
	
	if (this->matrix_fullview) {
		//controllo se answer corrisponde alla risposta esatta
		int j = 0;
		bool flag = false;
		while (!flag && j<7) {
			if (matrix_solution[j] != answer[j]) {
				flag = true;
			}
			else j++;
		}
		this->matrix_fullview = false;
		if (flag == false) {
			this->matrix_vinto = true;
			this->cancello_vinto = true;
			for (int i = 0; i < this->Maze->L.size(); i++) {
				this->Maze->L[i].WallsTexture[0] = 27;
				this->Maze->L[i].WallsTexture[1] = 28;
				this->Maze->L[i].WallsTexture[2] = 29;
				this->Maze->L[i].WallsTexture[3] = 30;
				this->Maze->L[i].floorTexture = 27;
				this->Maze->L[i].ceilTexture = 11;
			}
			this->Maze->mettiMuri();
			return true;
		}
		else {
			return false;
		}
	}
	else if (this->qr_fullview) {
		//controllo se answer corrisponde alla risposta esatta
		int j = 0;
		bool flag = false;
		while (!flag && j < 18) {
			if (qr_solution[j] != answer[j]) {
				flag = true;
			}
			else j++;
		}
		this->qr_fullview = false;
		if (flag == false) {
			this->qr_vinto = true;
			for (int i = 0; i < this->Maze->L.size(); i++) {
				this->Maze->L[i].WallsTexture[0] = 31;
				this->Maze->L[i].WallsTexture[1] = 32;
				this->Maze->L[i].WallsTexture[2] = 33;
				this->Maze->L[i].WallsTexture[3] = 34;
				this->Maze->L[i].floorTexture = 35;
				this->Maze->L[i].ceilTexture = 36;
			}
			this->Maze->mettiMuri();
			return true;
		}
		else {
			return false;
		}
	}
	else if (this->hogwarts_fullview) {
		//controllo se answer corrisponde alla risposta esatta
		int j = 0;
		bool flag = false;
		while (!flag && j < 3) {
			if (hogwarts_solution[j] != answer[j]) {
				flag = true;
			}
			else j++;
		}
		this->hogwarts_fullview = false;
		if (flag == false) {
			this->hogwarts_vinto = true;
			for (int i = 0; i < this->Maze->L.size(); i++) {
				this->Maze->L[i].WallsTexture[0] = 21;
				this->Maze->L[i].WallsTexture[1] = 22;
				this->Maze->L[i].WallsTexture[2] = 23;
				this->Maze->L[i].WallsTexture[3] = 24;
				this->Maze->L[i].floorTexture = 25;
				this->Maze->L[i].ceilTexture = 26;
			}
			this->Maze->mettiMuri();
			return true;
		}
		else {
			return false;
		}
	}
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//  bitmap fonts
void MyModel::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-20,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void MyModel::KillFont(void)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void MyModel::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

//  Move to the new position with collision detection against walls
//  return true if move
bool MyModel::MoveOrCollide(double npx, double npz, double enpx, double enpz)
{
	int nx = (int)enpx, nz = (int)enpz;
	int ni = this->Maze->GetIn(nx, nz);

	int ox = (int)px, oz = (int)pz;
	int oi;
	oi = this->Maze->GetIn(ox, oz);

	if (this->NoWalls) goto OKMOVE;

	// maze limits
	if (enpx < 0 || enpx > ldx || enpz < 0 || enpz > ldz) return false;
	
	if (oi == ni) goto OKMOVE; // same cell

	int V[4], nv;
	nv = this->Maze->GetViciniRaggiungibili(oi, V);
	for (int i = 0; i < nv; i++)
		if (V[i] == ni) goto OKMOVE;

	return false;

OKMOVE:
	if (nx == this->Maze->xu && nz == this->Maze->yu) this->Vinto = true;
	px = npx; pz = npz;

	//hogwarts non risolto
	if (oi != ni && ni == 51 && !hogwarts_vinto) { 
		this->fullview_texture = 9; //todo texture
		this->riddle_fullview = true;
		this->hogwarts_fullview = true;
	}
	//matrix non risolto
	else if (oi != ni && ni == 55 && !matrix_vinto) {	
		this->fullview_texture = 10;
		this->riddle_fullview = true;
		this->matrix_fullview = true;
		//TODO: texture di matrix
	}
	//cancello non aperto
	else if (oi != ni && ni == 38 && !cancello_vinto) {	
		this->fullview_texture = 12;	//todo texture cancello
		this->riddle_fullview = true;
		this->cancello_fullview = true;
		//TODO: texture di matrix
	}
	//cancello che si apre
	else if (oi != ni && ni == 38 && cancello_vinto && this->Maze->L[37].muroE) {
		this->Maze->L[37].muroE = false;	//todo MURO CHE SI ALZA
	}
	//qr non risolto
	else if (oi != ni && ni == 6 && !qr_vinto) {
		this->fullview_texture = 18; //todo cambiare
		this->riddle_fullview = true;
		this->qr_fullview = true;
		//TODO: texture di matrix
	}
	return true;
}