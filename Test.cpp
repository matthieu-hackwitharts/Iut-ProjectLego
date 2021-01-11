#include<SDL.h>
#include<SDL_ttf.h>
#include"config_sdl.h"
#include<iostream>
#include <fstream>
#include <ctime>

using namespace std;

enum Forme { cercle, rectangle };
enum Instr { AV, TD, TG };

struct obstacle
{
	Forme forme;
	SDL_Point centre;
	SDL_Color couleur;
	int largeur;
	int hauteur;
};

struct instruction
{
	Instr type;
	int valeur;
};

typedef struct obstacle obstacle;
typedef struct instruction instruction;

int pyth(int x, int y, int x2, int y2);
bool testCollision(obstacle objet, SDL_Point pt, int rayon);
void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);
instruction* ajInstr(int& n, instruction* tabInstr, instruction instr);
instruction* supprInstr(int& n, instruction* tabInstr, int ind);
void depInstr(int& n, instruction* tabInstr, int ind, int ord);

int main(int argn, char* argv[]) {

	srand(time(NULL));

	SDL_DisplayMode* Bureau = new SDL_DisplayMode;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}

	SDL_GetCurrentDisplayMode(0, Bureau);

	SDL_Window* win = SDL_CreateWindow("Ma fenetre",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		Bureau->w,
		Bureau->h,
		SDL_WINDOW_SHOWN
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED);

	SDL_RenderPresent(rendu);

	SDL_Event event;

	bool continuer = true;

	instruction* tabInstr = NULL;
	int nbInstr = 0;

	instruction aAj = { AV,100 };
	instruction aAj2 = { AV,200 };

	tabInstr = ajInstr(nbInstr, tabInstr, aAj);
	tabInstr = ajInstr(nbInstr, tabInstr, aAj2);

	cout << tabInstr[0].type << " " << tabInstr[0].valeur << endl;
	cout << tabInstr[1].type << " " << tabInstr[1].valeur << endl;

	depInstr(nbInstr, tabInstr, 0, 1);

	cout << tabInstr[0].type << " " << tabInstr[0].valeur << endl;
	cout << tabInstr[1].type << " " << tabInstr[1].valeur << endl;

	tabInstr = supprInstr(nbInstr, tabInstr, 0);

	cout << tabInstr[0].type << " " << tabInstr[0].valeur << endl;

	obstacle ob[2];

	ob[0] = { cercle,{Bureau->w/2, Bureau->h/2}, {255,255,255,255}, 50, 50 };
	ob[1] = { rectangle,{Bureau->w/2 - 150, Bureau->h/2}, {255,255,255,255}, 100, 100 };
	SDL_SetRenderDrawColor(rendu, ob[0].couleur.r, ob[0].couleur.g, ob[0].couleur.b, ob[0].couleur.a);
	if (ob[0].forme == cercle)
		DrawCircle(rendu, ob[0].centre.x, ob[0].centre.y, ob[0].largeur);
	else {
		SDL_Rect rect = { ob[0].centre.x - ob[0].largeur / 2, ob[0].centre.y - ob[0].hauteur / 2, ob[0].largeur , ob[0].hauteur };
		SDL_RenderFillRect(rendu, & rect);
	}
	SDL_SetRenderDrawColor(rendu, ob[0].couleur.r, ob[0].couleur.g, ob[0].couleur.b, ob[0].couleur.a);
	if (ob[1].forme == cercle)
		DrawCircle(rendu, ob[1].centre.x, ob[1].centre.y, ob[1].largeur);
	else {
		SDL_Rect rect = { ob[1].centre.x - ob[1].largeur / 2, ob[1].centre.y - ob[1].hauteur / 2, ob[1].largeur , ob[1].hauteur };
		SDL_RenderFillRect(rendu, &rect);
	}
	SDL_RenderPresent(rendu);

	if (testCollision(ob[1], { 0,0 }, 50))       
		cout << "Coucou";
	else
		cout << "Non";

	while (continuer)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				continuer = false;
				break;
			}
		}
	}

	if(tabInstr != NULL)
		delete[] tabInstr;

	return 0;
}

int pyth(int x, int y, int x2, int y2) {
	return sqrt(pow(abs(x - x2), 2) + pow(abs(y - y2), 2));
}

bool testCollision(obstacle objet, SDL_Point pt, int rayon) {
	int dist;
	if (objet.forme == cercle) {
		dist =  pyth(pt.x, pt.y, objet.centre.x, objet.centre.y);
		if (dist <= rayon + objet.largeur)
			return true;
		else
			return false;
	}
	else {
		SDL_Rect rect = {objet.centre.x - objet.largeur / 2,objet.centre.y - objet.hauteur / 2, objet.largeur, objet.hauteur };
		if (pt.x >= rect.x && pt.x <= rect.x + rect.w && rayon + rect.w / 2)
			return true;
		else if (pt.y >= rect.y && pt.y <= rect.y + rect.h && rayon + rect.h / 2)
			return true;
		else {
			if (pt.x < rect.x && pt.y < rect.y) {
				dist = pyth(pt.x, pt.y, rect.x, rect.y);
				if (dist <= rayon)
					return true;
				else
					return false;
			}
			else if (pt.x > rect.x && pt.y < rect.y) {
				dist = pyth(pt.x, pt.y, rect.x + rect.w, rect.y);
				if (dist <= rayon)
					return true;
				else
					return false;
			}
			else if (pt.x < rect.x && pt.y > rect.y) {
				dist = pyth(pt.x, pt.y, rect.x, rect.y + rect.h);
				if (dist <= rayon)
					return true;
				else
					return false;
			}
			else if (pt.x > rect.x && pt.y > rect.y) {
				dist = pyth(pt.x, pt.y, rect.x + rect.w, rect.y + rect.h);
				if (dist <= rayon)
					return true;
				else
					return false;
			}

		}
	}
}

void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius)
{
	const int diameter = (radius * 2);

	int x = (radius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - diameter);
	SDL_Rect rect;

	while (x >= y)
	{
		rect = { centreX - x, centreY - y, centreX + x - (centreX - x), centreY + y - (centreY - y) };
		SDL_RenderDrawRect(renderer,&rect);
		rect = { centreX - y, centreY - x, centreX + y - (centreX - y), centreY + x - (centreY - x) };
		SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX + x, centreY - y);
		//rect = { centreX, centreY, abs(centreX - (centreX + x)), abs(centreY - (centreY + y)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX + x, centreY + y);
		//rect = { centreX, centreY, abs(centreX - (centreX - x)), abs(centreY - (centreY - y)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX - x, centreY - y);
		//rect = { centreX, centreY, abs(centreX - (centreX + x)), abs(centreY - (centreY + y)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX - x, centreY + y);
		//rect = { centreX, centreY, abs(centreX - (centreX + y)), abs(centreY - (centreY - x)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX + y, centreY - x);
		//rect = { centreX, centreY, abs(centreX - (centreX + y)), abs(centreY - (centreY - x)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX + y, centreY + x);
		//rect = { centreX, centreY, abs(centreX - (centreX - y)), abs(centreY - (centreY - x)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX - y, centreY - x);
		//rect = { centreX, centreY, abs(centreX - (centreX - y)), abs(centreY - (centreY + x)) };
		//SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawLine(renderer, centreX, centreY, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

instruction* ajInstr(int& n, instruction* tabInstr, instruction instr) {
	instruction* nvTab = new instruction[++n];
	if (tabInstr != NULL) {
		for (int i = 0; i < n - 1; i++)
			nvTab[i] = tabInstr[i];
		delete[]tabInstr;
	}
	nvTab[n - 1] = instr;
	return nvTab;
}

instruction* supprInstr(int& n, instruction* tabInstr, int ind) {
	if (tabInstr != NULL && ind <= n) {
		instruction* nvTab = new instruction[--n];
		for (int i = 0, j = 0; i < n; i++, j++) {
			if (i == ind)
				j++;
			nvTab[i] = tabInstr[j];
		}
		delete[] tabInstr;
		return nvTab;
	}
	return tabInstr;
}

void depInstr(int& n, instruction* tabInstr, int ind, int ord) {
	if (ind + ord <= n) {
		instruction tmp;
		tmp = tabInstr[ind];
		tabInstr[ind] = tabInstr[ind + ord];
		tabInstr[ind + ord] = tmp;
	}
}