#include <iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"

using namespace std;



const int LARGEUR = 1000; //largeur fenetre
const int HAUTEUR = 600;  //hauteur fenetre
//int bouton_largeur = 
int main(int argn, char* argv[]) {//ent�te impos�e
								  //ouverture de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec � l�ouverture";
		return 1;

	}

	//on cr�e la fen�tre
	SDL_Window* win = SDL_CreateWindow("Ma fenetre",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		LARGEUR, 			//largeur en pixels			
		HAUTEUR, 			//hauteur en pixels
		SDL_WINDOW_SHOWN //d�autres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Cr�ation d�un dessin associ� � la fen�tre (1 seul renderer par fenetre)
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,  //nom de la fen�tre
		-1, //par d�faut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommand�e


	SDL_Rect button;
	button.x = LARGEUR / 2 - 500;
	button.y = HAUTEUR / 2 - 300;
	button.w = 100;
	button.h = 50;
	SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);	//pinceau vert
	SDL_RenderFillRect(rendu, &button);


	SDL_Rect button2;
	button2.x = LARGEUR / 2 - 400;
	button.y = HAUTEUR / 2 - 300;
	button2.w = 100;
	button2.h = 50;
	SDL_SetRenderDrawColor(rendu, 236, 92, 61, 255);
	SDL_RenderFillRect(rendu, &button2);


	SDL_RenderPresent(rendu);

	
	

	bool continuer = true;   //bool�en fin de programme
	SDL_Event event;

	//int x, y;//gestion des �v�nements souris/clavier, 
	//				//SDL_Event est de type struct
	while (continuer)
	{
		SDL_WaitEvent(&event);//attente d�un �v�nement
		switch (event.type) //test du type d��v�nement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
					   //on peut enlever SDL_Delay
			continuer = false;
			break;


		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x >= button.x && event.button.x <= button.x + button.w) {

					if (event.button.y >= button.y && event.button.y <= button.y + button.h) {
						continuer = false;
						break;

					}
				}
			}
		}
		
			

			
	}


	



			

		

	//destruction du renderer � la fin
	SDL_DestroyRenderer(rendu);

	//destruction � la fin
	SDL_DestroyWindow(win);   //equivalent du delete

	//fermeture
	SDL_Quit();
	return 0;
}





