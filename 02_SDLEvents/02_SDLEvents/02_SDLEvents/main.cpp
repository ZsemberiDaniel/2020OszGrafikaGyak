#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// atexit( exitProgram );
	//
	// 1. l�p�s: inicializ�ljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�n ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. l�p�s: hozzuk l�tre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							640,						// ablak sz�less�ge
							480,						// �s magass�ga
							SDL_WINDOW_SHOWN);			// megjelen�t�si tulajdons�gok

	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: hozzunk l�tre egy renderel�t, rajzol�t
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendelj�k hozz� a renderert
								-1,  // melyik index� renderert inicializ�ljuka
									 // a -1 a harmadik param�terben meghat�rozott ig�nyeinknek megfelel� els� renderel�t jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az ig�nyeink, azaz
																						// hardveresen gyors�tott �s vsync-et bev�r�
    if (ren == nullptr)
	{
        std::cout << "[Renderer l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. l�p�s: ind�tsuk el a f� �zenetfeldolgoz� ciklust
	// 

	// v�get kell-e �rjen a program fut�sa?
	bool quit = false;
	// feldolgozand� �zenet ide ker�l
	SDL_Event ev;
	// eg�r X �s Y koordin�t�i
	Sint32 mouseX = 0, mouseY = 0;

	SDL_Rect movinRect;
	movinRect.x = 0;
	movinRect.y = 0;
	movinRect.w = 20;
	movinRect.h = 20;

	SDL_Color color = {255, 0, 0, 255};

	while (!quit)
	{
		// am�g van feldolgozand� �zenet dolgozzuk fel mindet:
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
				// eg�rgomb felenged�s�nek esem�nye; a felengedett gomb a ev.button.button -ban tal�lhat�
				// a lehets�ges gombok a k�vetkez�ek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				// 2. feladat: ha a user a bal eg�rgombot nyomja meg akkor a t�glalap sz�ne v�ltson pirosra,
				//    ha a jobb eg�rgombot, akkor k�kre
				if (ev.button.button == SDL_BUTTON_LEFT) {
					color = {255, 0, 0, 255};
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					color = { 0, 0, 255, 255 };
				}
				break;
			}
		}

		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktu�lis rajzol�si sz�n legyen z�ld �s rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer c�me, aminek a rajzol�si sz�n�t be akarjuk �ll�tani
			color.r,		// piros
			color.g,	// z�ld
			color.b,		// k�k
			255);	// �tl�tszatlans�g

		SDL_RenderDrawLine(	ren,	// renderer c�me, ahov� vonalat akarunk rajzolni
							0, 0, // vonal kezd�pontj�nak (x,y) koordin�t�i
							mouseX, mouseY);// vonal v�gpontj�nak (x,y) koordin�t�i

		// 1. feladat: az eltelt id� f�ggv�ny�ben periodikusan n�jj�n �s cs�kkenjen
		//    az eg�rmutat� k�z�ppontj�val kirajzolt n�gysz�g
		// sajat feladat:  szelesseg, 10 -> 30, 3mp alatt
		float time = SDL_GetTicks() / 1000.0f;
		float width = abs(20.0f * cosf(time / 6.0 * M_PI)) + 10;

		// 20x20-as n�gyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - width / 2;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = width;
		cursor_rect.h = 20;
		// legyen a kit�lt�si sz�n piros
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

		// legyen a kit�lt�si sz�n piros
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderFillRect(ren, &movinRect);

		// 3. feladat: rajzolj ki egy 50 sugar� k�rvonalat az eg�rmutat� k�r�!
		// seg�ts�g: haszn�ld a SDL_RenderDrawLines()-t
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
		

		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}

	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}