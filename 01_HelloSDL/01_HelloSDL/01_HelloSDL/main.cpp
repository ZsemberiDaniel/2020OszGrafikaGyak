#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	// std::cin.get();
}

int main(int argc, char* args[])
{
	// állítsuk be, hogy kilépés elõtt hívja meg a rendszer az exitProgram() függvényt
	// atexit( exitProgram );

	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjünk ki
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
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
	// 3. lépés: töröljük az ablak háttérszínét és várjunk 2 másodpercet
	//
	SDL_Color colors[] = {
		{ 255, 0, 0, 255 },
		{ 0, 255, 0, 255 },
		{ 0, 0, 255, 255 }
	};

	for (int i = 0; i < 3; i++) {
		// aktuális rajzolási szín legyen fekete és töröljük az aktuális rajzolási színnel az ablak kliensterületét
		SDL_SetRenderDrawColor(ren,	// melyik renderelõnek állítjuk be az aktuális rajzolási színét
			colors[i].r,	// vörös intenzitás - 8 bites elõjel nélküli egész szám
			colors[i].g,		// zöld intenzitás - 8 bites elõjel nélküli egész szám
			colors[i].b,		// kék intenzitás - 8 bites elõjel nélküli egész szám
			colors[i].a);	// átlátszóság - 8 bites elõjel nélküli egész szám

		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
			0,		// piros
			255,	// zöld
			0,		// kék
			255);	// átlátszatlanság
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			10, 10, // vonal kezdõpontjának (x,y) koordinátái
			10, 30);// vonal végpontjának (x,y) koordinátái

		//S -- fekete
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			10, 10, // vonal kezdõpontjának (x,y) koordinátái
			30, 10);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			10, 10, // vonal kezdõpontjának (x,y) koordinátái
			10, 30);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			10, 30, // vonal kezdõpontjának (x,y) koordinátái
			30, 30);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			30, 30, // vonal kezdõpontjának (x,y) koordinátái
			30, 60);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			30, 60, // vonal kezdõpontjának (x,y) koordinátái
			10, 60);

		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			50, 10, // vonal kezdõpontjának (x,y) koordinátái
			50, 60);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			50, 10, // vonal kezdõpontjának (x,y) koordinátái
			70, 30);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			70, 30, // vonal kezdõpontjának (x,y) koordinátái
			90, 10);
		SDL_RenderDrawLine(ren,	// renderer címe, ahová vonalat akarunk rajzolni
			90, 10, // vonal kezdõpontjának (x,y) koordinátái
			90, 60);

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);

		// várjunk 2 másodpercet
		SDL_Delay(2000);
	}

	// 3. feladat: 2 másodpercenként változzon a háttér színe! Elõször legyen piros, aztán zöld és végül kék,
	// majd lépjen ki a program!

	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}