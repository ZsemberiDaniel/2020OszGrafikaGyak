#include "MyApp.h"
#include <math.h>

CMyApp::CMyApp(void)
{
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// törlési szín függjön a felhasznalo inputjatol
	glClearColor(backgroundColor.r / 255.0, backgroundColor.g / 255.0, backgroundColor.b / 255.0, 1.0f);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	// a "memoria" valtoztatasa
	if (key.keysym.sym == SDLK_t) {
		backgroundColor = { 0, 0, 255, 255 };
	} else if (key.keysym.sym == SDLK_r) {
		backgroundColor = { 0, 255, 0, 255 };
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{

}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w * 0.5f, _h * 0.5 );
}