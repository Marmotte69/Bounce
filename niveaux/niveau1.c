#ifndef __include_niveau1_c__
	#define __include_niveau1_c__
	#include <stdlib.h>
	#include "../header.h"
	
	// Function prototypes
	void placageObjet(uint8_t** map, int l, int h, int x, int y, uint8_t objet);
	struct Doublet retourneDoublet(float x, float y);

	extern void initNiveau1(struct World *World){
		short unsigned int i, j, X=111, Y=8;
		
		for(i=0; i<(short int)World->BD.x/Ul; i++){ // Libération de la map précédente.
			free(World->map[i]);
		}
		free(World->map);
		if(World->ennemis!=NULL){					  // Libération des ennemis.
			free(World->ennemis);
		}

		World->boule.rayon  			= Ul/2.0;
		World->boule.balleSave.bg		= retourneDoublet(1*Ul+World->boule.rayon,  1*Uh+World->boule.rayon);
		World->boule.Coo    			= retourneDoublet(1*Ul+World->boule.rayon,  1*Uh+World->boule.rayon);
		World->boule.balleSave.largeur	= World->boule.rayon;
		World->HG						= retourneDoublet(0*Ul, 0*Uh);
		World->BD						= retourneDoublet(X*Ul, Y*Uh);
		World->nbCerceau				= 6;
		World->nbEnnemis				= 0;
		World->CooPorte					= retourneDoublet(109, 5);
		World->ennemis					= NULL;
		World->map						= (uint8_t**) malloc(X * sizeof(uint8_t*)); 		 // Allocation dynamique map
		for(i=0; i<X; i++){
			World->map[i]				= (uint8_t*) malloc(Y * sizeof(uint8_t));
		}
		for(i=0; i<X; i++){
			for(j=0; j<Y; j++){
				World->map[i][j]=VIDE;
			}
		}

		placageObjet(World->map, 2,   2, 109, 5, PORTE_FERMEE);

		placageObjet(World->map, 111, 1, 0,   7, BRIQUE);
		placageObjet(World->map, 111, 1, 0,   0, BRIQUE);
		placageObjet(World->map, 1,   6, 0,   1, BRIQUE);
		placageObjet(World->map, 6,   4, 4,   1, BRIQUE);
		placageObjet(World->map, 2,   4, 13,  3, BRIQUE);
		placageObjet(World->map, 3,   4, 20,  1, BRIQUE);
		placageObjet(World->map, 2,   3, 25,  4, BRIQUE);
		placageObjet(World->map, 2,   1, 27,  4, BRIQUE);
		placageObjet(World->map, 4,   1, 31,  4, BRIQUE);
		placageObjet(World->map, 2,   1, 32,  5, BRIQUE);
		placageObjet(World->map, 2,   1, 32,  1, BRIQUE);
		placageObjet(World->map, 4,   1, 37,  4, BRIQUE);
		placageObjet(World->map, 2,   2, 39,  5, BRIQUE);
		placageObjet(World->map, 3,   1, 43,  1, BRIQUE);
		placageObjet(World->map, 2,   3, 43,  2, BRIQUE);
		placageObjet(World->map, 3,   1, 64,  1, BRIQUE);
		placageObjet(World->map, 2,   3, 65,  2, BRIQUE);
		placageObjet(World->map, 2,   4, 69,  3, BRIQUE);
		placageObjet(World->map, 2,   4, 73,  1, BRIQUE);
		placageObjet(World->map, 2,   4, 77,  3, BRIQUE);
		placageObjet(World->map, 2,   4, 81,  1, BRIQUE);
		placageObjet(World->map, 2,   4, 85,  3, BRIQUE);
		placageObjet(World->map, 2,   2, 91,  5, BRIQUE);
		placageObjet(World->map, 1,   1, 93,  6, BRIQUE);
		placageObjet(World->map, 2,   3, 94,  4, BRIQUE);
		placageObjet(World->map, 2,   4, 98,  1, BRIQUE);
		placageObjet(World->map, 2,   3, 102, 4, BRIQUE);
		placageObjet(World->map, 1,   1, 104, 6, BRIQUE);
		placageObjet(World->map, 2,   2, 105, 5, BRIQUE);
		placageObjet(World->map, 2,   4, 109, 1, BRIQUE);

		placageObjet(World->map, 1,   1, 16,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 29,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 35,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 50,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 53,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 57,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 61,  6, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 93,  5, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 104, 5, BARRE_JAUNE);

		placageObjet(World->map, 1,   1, 55,  3, LOSANGE);
		placageObjet(World->map, 1,   1, 98,  5, LOSANGE);

		placageObjet(World->map, 1,   1, 77,  1, BULLE);

		placageObjet(World->map, 1,   2, 8,   5, PETIT_CERCEAU); // Verticaux
		placageObjet(World->map, 1,   2, 32,  2, PETIT_CERCEAU);
		placageObjet(World->map, 1,   2, 73,  5, PETIT_CERCEAU);
		placageObjet(World->map, 1,   2, 81,  5, PETIT_CERCEAU);
		placageObjet(World->map, 2,   1, 96,  4, PETIT_CERCEAU); // Horizontaux
		placageObjet(World->map, 2,   1, 100, 4, PETIT_CERCEAU);
	}
#endif /* #ifndef __include_niveau1_c__ */
