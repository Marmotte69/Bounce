#ifndef __include_niveau1_c__
	#define __include_niveau1_c__
	#include <stdlib.h>
	#include "../header.h"
	
	// Function prototypes
	void placageObjet(int, int, int x, int y, uint8_t, uint8_t** map);
	struct Doublet retourneDoublet(float x, float y);

	extern void initNiveau1(struct World *World){
		short unsigned int i, j, X=111, Y=8;
		
		for(i=0; i<(short int)(*World).BD.x/Ul; i++){
			free((*World).map[i]);
		}
		free((*World).map);
		(*World).boule.rayon  		= Ul/2.0;
		(*World).boule.balleSave.bg	= retourneDoublet(1*Ul+(*World).boule.rayon, 1*Uh+(*World).boule.rayon);
		(*World).boule.Coo    		= retourneDoublet(1*Ul+(*World).boule.rayon, 1*Uh+(*World).boule.rayon);
		(*World).boule.balleSave.largeur	= (*World).boule.rayon;
		(*World).HG					= retourneDoublet(0*Ul, 0*Uh);
		(*World).BD					= retourneDoublet(X*Ul, Y*Uh);
		(*World).nbCerceau			= 6;
		(*World).CooPorte			= retourneDoublet(109, 5);
		(*World).map				= (uint8_t**) malloc(X * sizeof(uint8_t*));
		for(i=0; i<X; i++){
			(*World).map[i] = (uint8_t*) malloc(Y * sizeof(uint8_t));
		}
		for(i=0; i<X; i++){
			for(j=0; j<Y; j++){
				(*World).map[i][j]=VIDE;
			}
		}

		placageObjet(2,   2, 109, 5, PORTE_FERMEE,  (*World).map);

		placageObjet(111, 1, 0,   7, BRIQUE,        (*World).map);
		placageObjet(111, 1, 0,   0, BRIQUE,        (*World).map);
		placageObjet(1,   6, 0,   1, BRIQUE,        (*World).map);
		placageObjet(6,   4, 4,   1, BRIQUE,        (*World).map);
		placageObjet(2,   4, 13,  3, BRIQUE,        (*World).map);
		placageObjet(3,   4, 20,  1, BRIQUE,        (*World).map);
		placageObjet(2,   3, 25,  4, BRIQUE,        (*World).map);
		placageObjet(2,   1, 27,  4, BRIQUE,        (*World).map);
		placageObjet(4,   1, 31,  4, BRIQUE,        (*World).map);
		placageObjet(2,   1, 32,  5, BRIQUE,        (*World).map);
		placageObjet(2,   1, 32,  1, BRIQUE,        (*World).map);
		placageObjet(4,   1, 37,  4, BRIQUE,        (*World).map);
		placageObjet(2,   2, 39,  5, BRIQUE,        (*World).map);
		placageObjet(3,   1, 43,  1, BRIQUE,        (*World).map);
		placageObjet(2,   3, 43,  2, BRIQUE,        (*World).map);
		placageObjet(3,   1, 64,  1, BRIQUE,        (*World).map);
		placageObjet(2,   3, 65,  2, BRIQUE,        (*World).map);
		placageObjet(2,   4, 69,  3, BRIQUE,        (*World).map);
		placageObjet(2,   4, 73,  1, BRIQUE,        (*World).map);
		placageObjet(2,   4, 77,  3, BRIQUE,        (*World).map);
		placageObjet(2,   4, 81,  1, BRIQUE,        (*World).map);
		placageObjet(2,   4, 85,  3, BRIQUE,        (*World).map);
		placageObjet(2,   2, 91,  5, BRIQUE,        (*World).map);
		placageObjet(1,   1, 93,  6, BRIQUE,        (*World).map);
		placageObjet(2,   3, 94,  4, BRIQUE,        (*World).map);
		placageObjet(2,   4, 98,  1, BRIQUE,        (*World).map);
		placageObjet(2,   3, 102, 4, BRIQUE,        (*World).map);
		placageObjet(1,   1, 104, 6, BRIQUE,        (*World).map);
		placageObjet(2,   2, 105, 5, BRIQUE,        (*World).map);
		placageObjet(2,   4, 109, 1, BRIQUE,        (*World).map);

		placageObjet(1,   1, 16,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 29,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 35,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 50,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 53,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 57,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 61,  6, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 93,  5, BARRE_JAUNE,   (*World).map);
		placageObjet(1,   1, 104, 5, BARRE_JAUNE,   (*World).map);

		placageObjet(1,   1, 55,  3, LOSANGE,       (*World).map);
		placageObjet(1,   1, 98,  5, LOSANGE,       (*World).map);

		placageObjet(1,   1, 77,  1, BULLE,         (*World).map);

		placageObjet(1,   2, 8,   5, PETIT_CERCEAU, (*World).map); // Verticaux
		placageObjet(1,   2, 32,  2, PETIT_CERCEAU, (*World).map);
		placageObjet(1,   2, 73,  5, PETIT_CERCEAU, (*World).map);
		placageObjet(1,   2, 81,  5, PETIT_CERCEAU, (*World).map);
		placageObjet(2,   1, 96,  4, PETIT_CERCEAU, (*World).map); // Horizontaux
		placageObjet(2,   1, 100, 4, PETIT_CERCEAU, (*World).map);
	}
#endif /* #ifndef __include_niveau1_c__ */
