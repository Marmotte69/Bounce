#ifndef __include_niveau2_c__
	#define __include_niveau2_c__
	#include <stdlib.h>
	#include "../header.h"
	
	// Function prototypes
	void placageObjet(int, int, int x, int y, uint8_t, uint8_t** map);
	struct Doublet retourneDoublet(float x, float y);

	extern void initNiveau2(struct World *World){
		short unsigned int i, j, X=145, Y=22;
		
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

		placageObjet(111, 1, 0,   7, BRIQUE,        (*World).map);
		placageObjet(1,   6, 0,   1, BRIQUE,        (*World).map);
	}
#endif /* #ifndef __include_niveau2_c__ */
