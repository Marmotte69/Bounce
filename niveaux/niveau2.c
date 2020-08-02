#ifndef __include_niveau2_c__
	#define __include_niveau2_c__
	#include <stdlib.h>
	#include "../header.h"
	
	// Function prototypes
	void placageObjet(uint8_t** map, int l, int h, int x, int y, uint8_t objet);
	struct Doublet retourneDoublet(float x, float y);

	extern void initNiveau2(struct World *World){
		short unsigned int i, j, X=135, Y=22;
		
		for(i=0; i<(short int)World->BD.x/Ul; i++){ // Libération de la map précédente.
			free(World->map[i]);
		}
		free(World->map);
		if(World->ennemis!=NULL){					  // Libération des ennemis.
			free(World->ennemis);
		}

		World->HG						= retourneDoublet(-4*Ul, -16*Uh); 		// [0,0] + Coordonnées du référentiel de l'écran avec balle centrée dans le référentiel du niveau
		World->BD						= retourneDoublet((X-4)*Ul, (Y-16)*Uh); // [X,Y] + "
		World->boule.rayon  			= Ul/2.0;
		World->boule.balleSave.bg		= retourneDoublet(World->HG.x+9*Ul+World->boule.rayon, World->HG.y+15*Uh+World->boule.rayon);
		World->boule.Coo    			= retourneDoublet(World->HG.x+9*Ul+World->boule.rayon, World->HG.y+15*Uh+World->boule.rayon);
		World->boule.balleSave.largeur	= World->boule.rayon;
		World->nbCerceau				= 8;
		World->nbEnnemis				= 6;
		World->CooPorte				 	= retourneDoublet(133, 5);
		World->ennemis					= malloc(World->nbEnnemis*sizeof(struct Ennemi)); // Allocation dynamique ennemis
		World->map						= (uint8_t**) malloc(X * sizeof(uint8_t*));		  // Allocation dynamique map
		for(i=0; i<X; i++){
			World->map[i] 				= (uint8_t*) malloc(Y * sizeof(uint8_t));
		}
		for(i=0; i<X; i++){
			for(j=0; j<Y; j++){
				World->map[i][j]=VIDE;
			}
		}

		placageObjet(World->map, 135,  1, 0,   21, BRIQUE);
		placageObjet(World->map, 132,  1, 0,   14, BRIQUE);
		placageObjet(World->map, 2,    6, 0,   15, BRIQUE);
		placageObjet(World->map, 2,    4, 15,  17, BRIQUE);
		placageObjet(World->map, 5,    1, 17,  17, BRIQUE);
		placageObjet(World->map, 2,    2, 22,  17, BRIQUE);
		placageObjet(World->map, 2,    4, 26,  15, BRIQUE);
		placageObjet(World->map, 2,    4, 30,  17, BRIQUE);
		placageObjet(World->map, 2,    2, 34,  15, BRIQUE);
		placageObjet(World->map, 5,    4, 41,  17, BRIQUE);
		placageObjet(World->map, 2,    4, 66,  15, BRIQUE);
		placageObjet(World->map, 6,    1, 74,  20, BRIQUE);
		placageObjet(World->map, 4,    1, 75,  19, BRIQUE);
		placageObjet(World->map, 2,    1, 76,  18, BRIQUE);
		placageObjet(World->map, 2,    4, 88,  15, BRIQUE);
		placageObjet(World->map, 2,    2, 100, 15, BRIQUE);
		placageObjet(World->map, 2,    2, 100, 19, BRIQUE);
		placageObjet(World->map, 2,    4, 111, 17, BRIQUE);
		placageObjet(World->map, 2,    4, 115, 15, BRIQUE);
		placageObjet(World->map, 2,    4, 121, 17, BRIQUE);
		placageObjet(World->map, 2,    1, 119, 18, BRIQUE);
		placageObjet(World->map, 2,    3, 126, 15, BRIQUE);
		placageObjet(World->map, 3,    1, 130, 17, BRIQUE);
		placageObjet(World->map, 2,   22, 133,  0, BRIQUE);
		placageObjet(World->map, 2,   14, 111,  0, BRIQUE);
		placageObjet(World->map, 2,    1, 118, 13, BRIQUE);
		placageObjet(World->map, 2,    1, 119, 12, BRIQUE);
		placageObjet(World->map, 2,    1, 120, 11, BRIQUE);
		placageObjet(World->map, 2,    1, 121, 10, BRIQUE);
		placageObjet(World->map, 2,    1, 128, 13, BRIQUE);
		placageObjet(World->map, 2,    1, 127, 12, BRIQUE);
		placageObjet(World->map, 2,    1, 126, 11, BRIQUE);
		placageObjet(World->map, 2,    1, 125, 10, BRIQUE);
		placageObjet(World->map, 2,    1, 113, 10, BRIQUE);
		placageObjet(World->map, 12,   1, 113,  4, BRIQUE);
		placageObjet(World->map, 17,   1, 116,  7, BRIQUE);
		placageObjet(World->map, 20,   1, 113,  0, BRIQUE);

		placageObjet(World->map, 1,   1,  13,  20, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 131,  20, BARRE_JAUNE);
		placageObjet(World->map, 1,   1, 123,  13, BARRE_JAUNE);

		placageObjet(World->map, 1,   1,  76,  15, LOSANGE);
		placageObjet(World->map, 1,   1, 113,  11, LOSANGE);
		placageObjet(World->map, 1,   1, 113,   1, LOSANGE);

		placageObjet(World->map, 1,   1,  18,  19, BULLE);
		placageObjet(World->map, 1,   1, 119,  19, BULLE);

		placageObjet(World->map, 1,   2, 111,  15, PETIT_CERCEAU); // Vertical
		placageObjet(World->map, 2,   1, 131,  14, PETIT_CERCEAU); // Horizontal
		placageObjet(World->map, 1,   2,  22,  15, GRAND_CERCEAU); // Verticaux
		placageObjet(World->map, 1,   2,  41,  15, GRAND_CERCEAU);
		placageObjet(World->map, 1,   2,  66,  19, GRAND_CERCEAU);
		placageObjet(World->map, 1,   2,  88,  19, GRAND_CERCEAU);
		placageObjet(World->map, 1,   2, 100,  17, GRAND_CERCEAU);
		placageObjet(World->map, 1,   2, 123,   2, GRAND_CERCEAU);

		placageObjet(World->map, 9,   1,  102, 21, DEGONFLEUR);
		
		placageObjet(World->map, 1,   1,   32, 20, GONFLEUR);
		
		placageObjet(World->map, 2,   2,  133,  5, PORTE_FERMEE);
		
		World->ennemis[0].cadre.bg.x    = World->HG.x+ 2*Ul;
		World->ennemis[0].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[0].cadre.hauteur = 2*Uh;
		World->ennemis[0].cadre.largeur = 2*Ul;
		World->ennemis[0].min			= World->HG.y+15*Uh;
		World->ennemis[0].max			= World->HG.y+19*Uh;
		World->ennemis[0].axe			= VERTICAL;
		World->ennemis[0].direction	  	= false; // 0000

		World->ennemis[1].cadre.bg.x    = World->HG.x+ 4*Ul;
		World->ennemis[1].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[1].cadre.hauteur = 2*Uh;
		World->ennemis[1].cadre.largeur = 2*Ul;
		World->ennemis[1].min			= World->HG.y+15*Uh;
		World->ennemis[1].max			= World->HG.y+19*Uh;
		World->ennemis[1].axe			= VERTICAL;
		World->ennemis[1].direction	  	= false;

		World->ennemis[2].cadre.bg.x    = World->HG.x+49*Ul;
		World->ennemis[2].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[2].cadre.hauteur = 2*Uh;
		World->ennemis[2].cadre.largeur = 2*Ul;
		World->ennemis[2].min			= World->HG.y+15*Uh;
		World->ennemis[2].max			= World->HG.y+19*Uh;
		World->ennemis[2].axe			= VERTICAL;
		World->ennemis[2].direction	  	= false;

		World->ennemis[3].cadre.bg.x    = World->HG.x+55*Ul;
		World->ennemis[3].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[3].cadre.hauteur = 2*Uh;
		World->ennemis[3].cadre.largeur = 2*Ul;
		World->ennemis[3].min			= World->HG.y+15*Uh;
		World->ennemis[3].max			= World->HG.y+19*Uh;
		World->ennemis[3].axe			= VERTICAL;
		World->ennemis[3].direction		= false;

		World->ennemis[4].cadre.bg.x    = World->HG.x+61*Ul;
		World->ennemis[4].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[4].cadre.hauteur = 2*Uh;
		World->ennemis[4].cadre.largeur = 2*Ul;
		World->ennemis[4].min			= World->HG.y+15*Uh;
		World->ennemis[4].max			= World->HG.y+19*Uh;
		World->ennemis[4].axe			= VERTICAL;
		World->ennemis[4].direction	  	= false;

		World->ennemis[5].cadre.bg.x    = World->HG.x+93*Ul;
		World->ennemis[5].cadre.bg.y    = World->HG.y+17*Uh;
		World->ennemis[5].cadre.hauteur = 2*Uh;
		World->ennemis[5].cadre.largeur = 2*Ul;
		World->ennemis[5].min			= World->HG.y+15*Uh;
		World->ennemis[5].max			= World->HG.y+19*Uh;
		World->ennemis[5].axe			= VERTICAL;
		World->ennemis[5].direction		= false;
	}
#endif /* #ifndef __include_niveau2_c__ */
