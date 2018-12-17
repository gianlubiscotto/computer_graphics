/*
 *		EDIT: this code was edited by Gianluca Ceccoli and Chiara Leoni as Computer Graphics academic project	
 *		inside Master Degree in Computer Engineering (2018)	
 *
 *    CREDITS: this program is based on the new nehe lesson 06.
 *    Modifiyed by Aldo Grattarola for the Computer Graphics course.
 *
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Model.h"
#include "resource.h"
#include "audiere.h"
using namespace audiere;
//  LIBRERIE OPENGL e multimendia
//	OpenGL libraries
#pragma comment( lib, "opengl32.lib" )				// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )						// Search For GLu32.lib While Linking
//#pragma comment( lib, "winmm.lib" )						// Search For WinMM Library While Linking

class MyModel Data;
int i = 0;	//index for char array for user input answering riddles 
POINTS last_mouse_p;
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

//  kill the window
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (Data.fullscreen)									// Are We In Fullscreen Mode?
	{	ChangeDisplaySettings(NULL,0);			// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (Data.hRC)											// Do We Have A Rendering Context?
	{	if (!wglMakeCurrent(NULL,NULL))	{				// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
  	if (!wglDeleteContext(Data.hRC)) {			// Are We Able To Delete The RC?
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		Data.hRC=NULL;										// Set RC To NULL
	}

	if (Data.hDC && !ReleaseDC(Data.hWnd,Data.hDC)) {		// Are We Able To Release The DC
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		Data.hDC=NULL;										// Set DC To NULL
	}

	if (Data.hWnd && !DestroyWindow(Data.hWnd)) {			// Are We Able To Destroy The Window?
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		Data.hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("Basic2D",Data.hInstance))		// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		Data.hInstance=NULL;									// Set hInstance To NULL
	}
}
///////////////////////////////////////////////////////////
//  create the window
/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{

	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
	
	POINT init;
	GetCursorPos(&init);
	Data.cx = init.x;
	Data.cy = init.y;
	//Data.fullscreen=TRUE;			// Set The Global Fullscreen Flag

	Data.hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= Data.hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "Basic2D";								// Set The Class Name
	

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (Data.fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				Data.fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (Data.fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(Data.hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"Basic2D",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								Data.hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(Data.hDC=GetDC(Data.hWnd)))				// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(Data.hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(Data.hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(Data.hRC=wglCreateContext(Data.hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(Data.hDC,Data.hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(Data.hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(Data.hWnd);						// Slightly Higher Priority
	SetFocus(Data.hWnd);									// Sets Keyboard Focus To The Window
	Data.ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!Data.InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Urca Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}


///////////////////////////////////////////////////////////
// callback function
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
      { Data.active=TRUE; }					// Program Is Active
      else { Data.active=FALSE; }		// Program Is No Longer Active
			return 0;								      // Return To The Message Loop
		}

    case WM_LBUTTONUP:
      if( Data.StartScreen ) {
				Data.startscreen_fullview = false; 
				Data.StartScreen = false; 
				break;
      }
      break;

    case WM_MOUSEMOVE:
      if(!Data.fullview && !Data.riddle_fullview){	//se non sono in una schermata fullview il mouse muove la visuale
         POINTS p;
         p = MAKEPOINTS(lParam);
				 RECT rt;
				 GetClientRect(hWnd, &rt);
         if( Data.IsInClient(p.x,p.y) ) {

					 if (Data.cx > rt.right-100 || Data.cx < rt.left + 100) {
						 SetCursorPos(rt.right / 2, Data.cy);
						 p = MAKEPOINTS(lParam);
						 Data.cx = p.x; Data.cy = p.y;

					 }
					 if (Data.cy > rt.bottom - 100 || Data.cy < rt.top + 100) {
						 SetCursorPos(Data.cx, rt.bottom/2);
						 p = MAKEPOINTS(lParam);
						 Data.cx = p.x; Data.cy = p.y;

					 }
					 if (Data.cx < p.x) {
						 Data.angle += (p.x-Data.cx)/1000.0;
					 }
					 else if (Data.cx > p.x) {
						 Data.angle -= (Data.cx - p.x) / 1000.0;
					 }

					 //visuale in giù
					 if (Data.cy < p.y) {
						 if(Data.angley>-1.6){
							 Data.angley -= (p.y - Data.cy) / 1000.0;
						 }
					 }
					 //visuale in su
					 else if (Data.cy > p.y) {
						 if (Data.angley < 1.5) {
							 Data.angley += (Data.cy - p.y) / 1000.0;
						 }
					 }
					 Data.cx = p.x; Data.cy = p.y;
           if( !Data.captured ) { Data.captured = true; SetCapture(hWnd); ShowCursor(FALSE); }
         } else {
           if( Data.captured ) { Data.captured = false; ReleaseCapture(); ShowCursor(TRUE); } 
         }
      }
      break;

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{	case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{	PostQuitMessage(0);					// Send A Quit Message
			return 0;								    // Jump Back
		}

		case WM_KEYDOWN: // Is A Key Being Held Down?
		{							
			if (Data.riddle_fullview) {		
				//if in riddle view, keys corresponds to user's answer
				
				//ESC
				if (wParam == VK_ESCAPE) {	
					//all fullviews to false
					Data.riddle_fullview = false;
					Data.matrix_fullview = false;
					Data.hogwarts_fullview = false;
					Data.gate_fullview = false;
					Data.qr_fullview = false;
					//array cleaned
					Data.index = 0;
					Data.answer[Data.index] = '\0';
				}

				//ENTER, confirm answer
				else if (wParam == VK_RETURN) {
					//if not gate fullview
					if (!Data.gate_fullview) {
						//verify answer
						if (Data.verify_answer(Data.answer)) {
							//correct sound
							Data.solved_riddles += 1;
							if (Data.solved_riddles != 3) {
								Data.correct_sound = true;
							}
						}
						//if answer is wrong
						else {
							//incorrect sound
							Data.incorrect_sound = true;
						}
						//in any case
						Data.riddle_fullview = false;
						//clean the char array
						Data.index = 0;
						Data.answer[Data.index] = '\0';
					}
					//if in gate fullview
					else {
						Data.riddle_fullview = false;
						Data.gate_fullview = false;
					}
				}
				//any other key but ESC or ENTER
				else {
					if (!Data.gate_fullview) {
						if (wParam == VK_BACK) {
							//delete last char
							if (Data.index > 0) {
								Data.index--;
								Data.answer[Data.index] = '\0';
							}
						}
						else {
							//handle user input into char array
							if (Data.index < ans_size - 1 && ((wParam > 47 && wParam < 58) || (wParam > 64 && wParam < 91) || wParam == 32)) {
								Data.answer[Data.index] = static_cast<char>(wParam);
								Data.answer[Data.index + 1] = '\0';
								Data.index++;
							}
						}
					}
					
				}
			}
			else{
				if (!Data.fullview) {
					Data.keys[wParam] = TRUE;		// If So, Mark It As TRUE
				}
				return 0;// Jump Back
			}
			
		}

		case WM_KEYUP:								// Has A Key Been Released? And different from M?
		{	
			Data.keys[wParam] = FALSE;// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{	Data.ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

///////////////////////////////////////////////////////////
//  Main program
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
  // Init of the default button depending on Data.fullscreen
  unsigned long Def = MB_DEFBUTTON1;
  if( !Data.fullscreen ) Def = MB_DEFBUTTON2;
	
  if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?",
    "Start FullScreen?",MB_YESNO|MB_ICONQUESTION | Def) == IDYES)
	{
		Data.fullscreen=true;							// Windowed Mode
	}
  
	// Create Our OpenGL Window
	HWND dW=GetDesktopWindow();
	int dx, dy;
	RECT R; 
	if (GetWindowRect(dW, &R)) {
		dx = R.right; dy = R.bottom;
	}
	else {
		dx = 640; dy = 480;
	}
	if (!CreateGLWindow("Project boh",dx,dy,8,Data.fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

  //  AUDIO - start
  AudioDevicePtr device(OpenDevice());
  if (!device) {
    return 0;         // failure
  }
  OutputStreamPtr stream(OpenSound(device, "../Data/ambient.wav", true));
  if (!stream) {
    return 0;         // failure
  }
	OutputStreamPtr sttheme(OpenSound(device, "../Data/strangerthings.mp3", true));
	if (!sttheme) {
		return 0;         // failure
	}
	stream->setRepeat(true);
	stream->setVolume(0.5f); // 50% volume
	stream->play();
  
  OutputStreamPtr bell(OpenSound(device, "../Data/bell.wav", false));
  OutputStreamPtr stupid(OpenSound(device, "../Data/stupid.wav", false));
	OutputStreamPtr gameover(OpenSound(device, "../Data/gameover.wav", false));
	OutputStreamPtr win(OpenSound(device, "../Data/win.wav", false));
	OutputStreamPtr grass_footstep(OpenSound(device, "../Data/grass1.wav", false));
	OutputStreamPtr floor_footstep(OpenSound(device, "../Data/hard.wav", false));
	if (!floor_footstep) {
		return 0;
	}

  //  AUDIO - end

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((Data.active && !Data.DrawGLScene()) || Data.keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done = TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(Data.hDC);					// Swap Buffers (Double Buffering)
			}

			if (Data.keys[VK_UP]) {
				double r = 0.01;
				double npx, npz;
				npx = Data.px + r * cos(Data.angle);
				npz = Data.pz + r * sin(Data.angle);
				r += 0.051;
				double enpx = Data.px + r * cos(Data.angle);
				double enpz = Data.pz + r * sin(Data.angle);

				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles==0) {
						grass_footstep->play();
					}
					else floor_footstep->play();
				}
			}
			//go forward
			if (Data.keys['W']) {
				double r = 0.01;
				double npx, npz;
				npx = Data.px + r * cos(Data.angle);
				npz = Data.pz + r * sin(Data.angle);
				r += 0.051;
				double enpx = Data.px + r * cos(Data.angle);
				double enpz = Data.pz + r * sin(Data.angle);

				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles == 0) {
						grass_footstep->play();	
					}
					else floor_footstep->play();
				}
			}
			if (Data.keys[VK_DOWN]) {
				double r = 0.01;
				double npx, npz;
				npx = Data.px - r * cos(Data.angle);
				npz = Data.pz - r * sin(Data.angle);
				r += 0.051;
				double enpx = Data.px - r * cos(Data.angle);
				double enpz = Data.pz - r * sin(Data.angle);
				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles == 0) {
						grass_footstep->play();
					}
					else floor_footstep->play();
				}
			}
			//go backward
			if (Data.keys['S']) {
				double r = 0.01;
				double npx, npz;
				npx = Data.px - r * cos(Data.angle);
				npz = Data.pz - r * sin(Data.angle);
				r += 0.051;
				double enpx = Data.px - r * cos(Data.angle);
				double enpz = Data.pz - r * sin(Data.angle);
				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles == 0) {
						grass_footstep->play();
					}
					else floor_footstep->play();
				}
			}
			//go right
			if (Data.keys['D']) {
				double r = 0.01;
				double npx, npz;

				npx = Data.px - r * cos(Data.angle - PI / 2.0);
				npz = Data.pz - r * sin(Data.angle - PI / 2.0);
				r += 0.051;
				double enpx = Data.px - r * cos(Data.angle - PI / 2.0);
				double enpz = Data.pz - r * sin(Data.angle - PI / 2.0);
				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles == 0) {
						grass_footstep->play();
					}
					else floor_footstep->play();
				}
			}


			//go left
			if (Data.keys['A']) {
				double r = 0.01;
				double npx, npz;

				npx = Data.px - r * cos(Data.angle + PI / 2.0);
				npz = Data.pz - r * sin(Data.angle + PI / 2.0);
				r += 0.051;
				double enpx = Data.px - r * cos(Data.angle + PI / 2.0);
				double enpz = Data.pz - r * sin(Data.angle + PI / 2.0);
				if (Data.MoveOrCollide(npx, npz, enpx, enpz)) {
					if (Data.solved_riddles == 0) {
						grass_footstep->play();
					}
					else floor_footstep->play();
				}
			}
			if (Data.keys[VK_RIGHT]) {
				Data.angle += 0.01;
			}
			if (Data.keys[VK_LEFT]) {
				Data.angle -= 0.01;
			}
			if (!Data.StartScreen) {
				if (Data.keys['V']) {
					Data.fullview = true;
				}else Data.fullview = false;
			}
			if( Data.keys['N'] ) {
				Data.NoWalls = true;
			} else Data.NoWalls = false;

			if (Data.keys['L']) {
				Data.illumin = true;
			}
			if (Data.keys['O']) {
				Data.illumin = false;
			}
			if (Data.correct_sound) {
				bell->setVolume(0.5f);
				bell->play();
				Data.correct_sound = false;
			}
			if (Data.incorrect_sound) {
				stupid->setVolume(0.5f);
				stupid->play();
				Data.incorrect_sound = false;
			}

			if (Data.timeout) {
				stream->stop();
				sttheme->stop();
				gameover->play();
				while (gameover->isPlaying()) {
				}
				done = true;
			}
			//all riddles solved
			if (Data.solved_fullview) {
				stream->stop();
				sttheme->stop();
				win->play();
				while (win->isPlaying()) {
				}
				done = true;
			}
			if (Data.solved_riddles > 0) {
				stream->stop();
				sttheme->setRepeat(true);
				sttheme->setVolume(0.5f); // 50% volume
				sttheme->play();
			}
		}
	}
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program

}