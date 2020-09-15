#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// atexit( exitProgram );
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == nullptr)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

	SDL_Rect movinRect;
	movinRect.x = 0;
	movinRect.y = 0;
	movinRect.w = 20;
	movinRect.h = 20;

	SDL_Color color = {255, 0, 0, 255};

	while (!quit)
	{
		// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				// egérgomb felengedésének eseménye; a felengedett gomb a ev.button.button -ban található
				// a lehetséges gombok a következõek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
				//    ha a jobb egérgombot, akkor kékre
				if (ev.button.button == SDL_BUTTON_LEFT) {
					color = {255, 0, 0, 255};
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					color = { 0, 0, 255, 255 };
				}
				break;
			}
		}

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
			color.r,		// piros
			color.g,	// zöld
			color.b,		// kék
			255);	// átlátszatlanság

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt négyszög
		// sajat feladat:  szelesseg, 10 -> 30, 3mp alatt
		float time = SDL_GetTicks() / 1000.0f;
		float width = abs(20.0f * cosf(time / 6.0 * M_PI)) + 10;

		// 20x20-as négyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - width / 2;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = width;
		cursor_rect.h = 20;
		// legyen a kitöltési szín piros
		SDL_SetRenderDrawColor( ren, color.r, color.g, color.b, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 0. feladat: mozogjon egy negyzet jobbra 2 mp-et, fel 2 mp-et es aztan alljon meg
		int timeTicks = SDL_GetTicks();
		if (timeTicks < 2000) {
			movinRect.x += 1;
		}
		else if (timeTicks < 4000) {
			movinRect.y += 1;
		}

		// legyen a kitöltési szín piros
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderFillRect(ren, &movinRect);

		// 3. feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t
		const int N = 80;
		float radius = 50.0f;
		SDL_Point points[N + 1];
		for (int i = 0; i <= N; i++) {
			float rad = i * 2.0f * M_PI / N;
			points[i].x = radius * cosf(rad) + mouseX;
			points[i].y = radius * sinf(rad) + mouseY;
		}

		SDL_RenderDrawLines(ren, points, N + 1);
		
		// projektalt kocka
		int windowWidth = 640;
		int windowHeight = 480;
		int cubeSize = 200;
		float offset = 30.0;

		// [0;1] - 0.5 => [-.5;0.5] * 2.0 => [-1;1]
		SDL_Point offsetCoord = {
			offset * 2.0f * (mouseX / (float) windowWidth - 0.5f), offset * 2.0f * (mouseY / (float) windowHeight - 0.5f)
		};

		SDL_Point middle = {
			windowWidth / 2,
			windowHeight / 2
		};

		SDL_Rect rect1;
		rect1.x = middle.x - cubeSize / 2 + offsetCoord.x;
		rect1.y = middle.y - cubeSize / 2 + offsetCoord.y;
		rect1.w = cubeSize;
		rect1.h = cubeSize;

		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawRect(ren, &rect1);


		SDL_Rect rect2;
		rect2.x = middle.x - cubeSize / 2 - offsetCoord.x;
		rect2.y = middle.y - cubeSize / 2 - offsetCoord.y;
		rect2.w = cubeSize;
		rect2.h = cubeSize;

		SDL_RenderDrawRect(ren, &rect2);
		SDL_RenderDrawLine(ren, rect1.x, rect1.y, rect2.x, rect2.y);
		SDL_RenderDrawLine(ren, rect1.x + rect1.w, rect1.y, rect2.x + rect2.w, rect2.y);
		SDL_RenderDrawLine(ren, rect1.x, rect1.y + rect1.h, rect2.x, rect2.y + rect2.h);
		SDL_RenderDrawLine(ren, rect1.x + rect1.w, rect1.y + rect1.h, rect2.x + rect2.w, rect2.y + rect2.h);
		

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}

	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}