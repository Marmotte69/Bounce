#ifndef __include_header_c__
	#include <tice.h>
	#include "niveaux/niveaux.h"

	#define __include_header_c__
	#define Uh   						30							// 240/8 -> Hauteur fenêtre/8 -> Hauteur d'un cube en px;
	#define Ul   						30							// (240/8)/135.0*130 -> Hauteur cube*ratio -> Largeur d'un cube en px;
	#define COEFFICIENT_FRICTION_BRIQUE	0.6
	#define G 							retourneDoublet(0, 1)
	//#define MAP_MAX_X					250
	//#define MAP_MAX_Y					250
	#define MAX_VITESSE_X				16.4
	#define MAX_VITESSE_Y				16.4
	#define LIMITE_REBOND				 2.0
	#define BLOCAGE_CERCEAU				12.0
	#define ONE_SECOND         			32768

	typedef enum Objet Objet;
	typedef enum Direction Direction;
	typedef enum Axe Axe;
	enum Objet{
		VIDE, BRIQUE, BRIQUE_BLEU, BARRE_JAUNE, LOSANGE, FLECHE, BULLE, PETIT_CERCEAU, GRAND_CERCEAU, PETIT_GRIS, GRAND_GRIS, PORTE_FERMEE, PORTE_OUVERTE, GONFLEUR, DEGONFLEUR, BRIQUE_TRIANGULAIRE_BD, BRIQUE_TRIANGULAIRE_BG
	};
	enum Direction{
		NON, GAUCHE, DROITE, DEGAT, SORTIE
	};
	enum Axe{
		HORIZONTAL, VERTICAL
	};

	struct Doublet{ 	// Stocke des coordonnées et des normes de vecteurs dans R²
		float x, y;
	};
	struct Brique{
		struct Doublet bg;
		float hauteur, largeur;
	};
	struct Balle{
		struct Doublet	Coo;
		struct Doublet	vitesse;
		struct Doublet	force;
		struct Brique 	balleSave;
		float			theta;
		uint8_t			rayon;
		uint8_t			bonus;
		bool			collision;
		bool			degat;
		bool			possibiliteSaut;
	};
	/*struct BriqueTriangulaire{
		struct Doublet triangle[3]; //0 : Angle droit | 1 et 2 : segment de collision avec 1.y>2.y
		struct Brique bloc;
		bool emerge;       // true : cache de la couleur du fond | false : cache de la couleur de l'eau
	};
	struct BriqueOrientable{
			struct Brique hitbox;
			struct Brique image;
			float orientation;
	};
	struct Rond{
		struct Brique bord[2];
		struct Doublet bg;
		bool etat;
		float hauteur, largeur, rotation;
	};
	/*struct ObjetModifiable{
		struct Brique cadre;
		bool etat;
	};*/
	struct Ennemi{
		struct Brique cadre;
		Axe axe;
		int min, max;
		bool direction;
	};
	struct BarreAvancement{
		struct Brique taille;
		float dureeMax, tempsRestant;
	};
	struct World{
		struct Balle boule;
		//.bg pour le point de sauvegarde et .largeur pour le rayon au moment de la sauvegarde
		/*struct Brique mer[2];
		struct Brique briqueBleu[4];
		struct Brique bonus2[1];
		struct Brique bonus3[1];
		struct BriqueOrientable bonus1[3];
		struct BriqueOrientable degonfleurs[20];*/
		struct Ennemi* ennemis;
		struct Doublet HG;
		struct Doublet BD;
		struct Doublet CooPorte;
		struct BarreAvancement barreBonus;
		uint24_t score;
		int8_t  nbVie;
		int8_t  nbCerceau;
		uint8_t nbEnnemis;
		uint8_t** map;
	};

#endif /* #ifndef __include_header_c__ */
