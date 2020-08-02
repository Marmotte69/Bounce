#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <math.h>

#include <assert.h>
#include <debug.h>

#include "header.h"

extern struct Doublet retourneDoublet(const float x, const float y){
	struct Doublet a;
	a.x=x;
	a.y=y;
	return a; 
}
extern void placageObjet(uint8_t** map, const int l, const int h, int x, const int y, const uint8_t objet){
	int H, L = x+l, j;

	for(; x<L; x++){		
		H = y+h;
		for(j=y; j<H; j++){	
			map[x][j]=objet;
		}
	}
}
void centrageFenetre(struct World* World){
	struct Doublet termes = {0, 0};
	uint8_t i;

	if((int)World->boule.Coo.x<(int)gfx_lcdWidth/2 && World->HG.x<0){ // Lorsque mouvement vers gauche
		termes = retourneDoublet(1, 0);
		while((int)World->boule.Coo.x+termes.x<(int)gfx_lcdWidth*0.4 && World->HG.x+termes.x<0){
			termes.x++;
		}
	}else if((int)World->boule.Coo.x>(int)gfx_lcdWidth/2 && World->BD.x>gfx_lcdWidth){ // Lorsque mouvement vers la droite
		termes = retourneDoublet(-1,0);
		while((int)World->boule.Coo.x+termes.x>(int)gfx_lcdWidth*0.6 && World->BD.x+termes.x>gfx_lcdWidth){
			termes.x--;
		}
	}
	if((int)World->boule.Coo.y>gfx_lcdHeight/2 && World->BD.y>gfx_lcdHeight){ // Lorsque mouvement vers le bas
		termes.y--;
		while((int)World->boule.Coo.y+termes.y>(int)gfx_lcdHeight*0.8 && World->BD.y+termes.y>gfx_lcdHeight){
			termes.y--;
		}
	}else if((int)World->boule.Coo.y<gfx_lcdHeight/2 && World->HG.y<0){ // Lorsque mouvement vers le haut
		termes.y++;
		while((int)World->boule.Coo.y+termes.y<(int)gfx_lcdHeight*0.2 && World->HG.y+termes.y<0){
			termes.y++;
		}
	}
	if(termes.x != 0 || termes.y != 0){
		World->HG.x					    +=termes.x; // Décalage point haut-gauche du niveau
		World->HG.y					    +=termes.y;
		World->BD.x					    +=termes.x; // Décalage point bas-droit du niveau
		World->BD.y					    +=termes.y;
		World->boule.Coo.x			    +=termes.x; // Décalage de la min
		World->boule.Coo.y			    +=termes.y;
		World->boule.balleSave.bg.x	    +=termes.x; // Décalage de la sauvegarde
		World->boule.balleSave.bg.y	    +=termes.y;

		for(i=0; i<World->nbEnnemis; i++){
			World->ennemis[i].cadre.bg.x+=termes.x;
			World->ennemis[i].cadre.bg.y+=termes.y;
			//if(World->ennemis[i].axe==true){ 		!! BUG [COMPILATEUR ?] SI BRANCHEMENT CONDITIONNEL DÉCOMMENTÉ !!! (NMI reset caused by writing to flash at address 0x0016 from unprivileged code.)
				World->ennemis[i].min   +=termes.y;
				World->ennemis[i].max   +=termes.y;
			/*}else if(World->ennemis[i].axe==HORIZONTAL){
				World->ennemis[i].min	+=termes.x;
				World->ennemis[i].max   +=termes.x;
			}*/
		}
		/*World->ennemis[0].cadre.bg.x+=termes.x;
		World->ennemis[0].cadre.bg.y+=termes.y;
		World->ennemis[0].min   +=termes.y;
		World->ennemis[0].max   +=termes.y;*/
	}
}

void initialisation(struct World* World){
	int i, j, X=11, Y=8;

	// Pour menu principal
	World->boule.Coo.x				= 75;
	World->boule.Coo.y				= 15;
	World->BD							= retourneDoublet(11, 8);
	World->map 						= (uint8_t**) malloc(X * sizeof(uint8_t*));
		for(i=0; i<X; i++){
		World->map[i] = (uint8_t*) malloc(Y * sizeof (uint8_t));
	}
	World->HG							= retourneDoublet(0*Ul, 0*Uh);
	World->boule.rayon				= Ul/2.0;
	World->nbCerceau					= 1;
	World->nbEnnemis					= 0;
	World->ennemis					= NULL;
	

	// Paramétres généraux
	for(i=0; i<X; i++){
		for(j=0; j<Y; j++){
			World->map[i][j]			=VIDE;
		}
	}
	World->boule.possibiliteSaut		= false;
	World->boule.degat				= false;
	World->boule.vitesse				= retourneDoublet(0, 0);
	World->boule.force				= retourneDoublet(0, 0);
	World->boule.theta				= 1;
	World->boule.bonus				= 0;
	World->boule.collision			= false;
	World->score						= 0;
	World->nbVie						= 3;
	// Barre bonus
	World->barreBonus.taille.hauteur	= 15; // Passer en macros #define 0000
	World->barreBonus.taille.largeur	= gfx_lcdWidth;
	World->barreBonus.taille.bg		= retourneDoublet(0, 0);
	World->barreBonus.dureeMax		= 10; // Passer en macros #define 0000
	World->barreBonus.tempsRestant	= 0;
}

bool collisionCercleCercle(const int x1, const int y1, const float d1, const int x2, const int y2, const float d2){
	if(d1+d2>sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))){ // Si somme des diamètres supérieure à la distance entre les 2 centres
		return true;
	}
	return false;
}
bool collisionRectanglevsRectangle(const struct Brique* box1, const struct Brique* box2){
	if((box2->bg.x >= box1->bg.x + box1->largeur)   // trop à droite
	 || (box2->bg.x + box2->largeur <= box1->bg.x)  // trop à gauche
	 || (box2->bg.y >= box1->bg.y + box1->hauteur)  // trop en bas
	 || (box2->bg.y + box2->hauteur <= box1->bg.y)){// trop en haut
		return false;
   }else{
		return true;
	}
}
bool collisionPointCercle(const struct Balle* C, const int x, const int y){
   int d2 = (x-C->Coo.x)*(x-C->Coo.x) + (y-C->Coo.y)*(y-C->Coo.y);
   if(d2>C->rayon*C->rayon){
		return false;
	}else{
		return true;
	}
}
bool collisionPointRectangle(const struct Brique* box, const int curseur_x, const int curseur_y){
	if(curseur_x>=box->bg.x && curseur_x<box->bg.x+box->largeur && curseur_y>=box->bg.y && curseur_y<box->bg.y+box->hauteur){
		return true;
	}else{
		return false;
	}
}
bool projectionSurSegment(const int Cx, const int Cy, const int Ax, const int Ay, const int Bx, const int By){
	int ACx = Cx-Ax;
	int ACy = Cy-Ay;
	int ABx = Bx-Ax;
	int ABy = By-Ay;
	int BCx = Cx-Bx;
	int BCy = Cy-By;
	int s1  = (ACx*ABx) + (ACy*ABy);
	int s2  = (BCx*ABx) + (BCy*ABy);
	if(s1*s2>0){
		return false;
	}
	return true;
}
struct Brique getBoxAutourCercle(const struct Balle* cercle){
	struct Brique carre;
	carre.bg.x = cercle->Coo.x-cercle->rayon;
	carre.bg.y = cercle->Coo.y-cercle->rayon;
	carre.hauteur = 2*cercle->rayon;
	carre.largeur = 2*cercle->rayon;
	return carre;
}
bool collisionCercleRectangle(const struct Balle* C1, const struct Brique* box1){
	struct Brique boxCercle = getBoxAutourCercle(C1);  // retourner la bounding box de l'image porteuse, ou calculer la bounding box.
	bool projvertical;
	bool projhorizontal;
	
	if(!(collisionRectanglevsRectangle(box1, &boxCercle))){
		return false;   // premier test
	}
	if(collisionPointCercle(C1, box1->bg.x, box1->bg.y) || collisionPointCercle(C1, box1->bg.x, box1->bg.y+box1->hauteur) || collisionPointCercle(C1, box1->bg.x+box1->largeur, box1->bg.y) || collisionPointCercle(C1, box1->bg.x+box1->largeur, box1->bg.y+box1->hauteur)){
		return true;   // deuxieme test
	}
	if(collisionPointRectangle(box1, C1->Coo.x, C1->Coo.y)){
		return true;   // troisieme test
	}
	projvertical   = projectionSurSegment(C1->Coo.x, C1->Coo.y, box1->bg.x, box1->bg.y, box1->bg.x, box1->bg.y+box1->hauteur);
	projhorizontal = projectionSurSegment(C1->Coo.x, C1->Coo.y, box1->bg.x, box1->bg.y, box1->bg.x+box1->largeur, box1->bg.y);
	if(projvertical || projhorizontal){
		return true;	// cas E
	}
	return false;		// cas B
}
float distanceCentreCercleDroite(const struct Balle* C, const struct Doublet* A, const struct Doublet* B){
	struct Doublet u;	//# Vecteur u
	struct Doublet AC;	//# Vecteur AC
	float numerateur;	// norme du vecteur v
	float denominateur;	// norme de u

	u.x = B->x - A->x;
	u.y = B->y - A->y;
	AC.x = C->Coo.x - A->x;
	AC.y = C->Coo.y - A->y;
	numerateur = u.x*AC.y - u.y*AC.x;
	if(numerateur <0){
		numerateur = -numerateur;   // valeur absolue ; si c'est négatif, on prend l'opposé.
	}
	denominateur = sqrt(u.x*u.x + u.y*u.y);
	return numerateur / denominateur;;
}
bool collisionDroite(const struct Balle* C, const struct Doublet* A, const struct Doublet* B){
	float CI = distanceCentreCercleDroite(C, A, B);

	if(CI<C->rayon){
		return true;
	}else{
		return false;
	}
}
bool collisionSegment(const struct Balle* C, const struct Doublet* A, const struct Doublet* B){
	struct Doublet AB, AC, BC;	//# Vecteurs
	float pscal1;				// produit scalaire
	float pscal2;				// produit scalaire

	if(!(collisionDroite(C, A, B))){
		return false;  // si on ne touche pas la droite, on ne touchera jamais le segment
	}
	AB.x = B->x - A->x;
	AB.y = B->y - A->y;
	AC.x = C->Coo.x - A->x;
	AC.y = C->Coo.y - A->y;
	BC.x = C->Coo.x - B->x;
	BC.y = C->Coo.y - B->y;
	pscal1 = AB.x*AC.x + AB.y*AC.y;
	pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;
	if(pscal1>=0 && pscal2>=0){
		return true;   // I entre A et B, ok.
	}
	// dernière possibilité, A ou B dans le cercle
	if(collisionPointCercle(C, A->x, A->y)){
		return true;
	}
	if(collisionPointCercle(C, B->x, B->y)){
		return true;
	}
	return false;
}
bool collisionLosange(const struct Balle* Boule, const struct Doublet* A, const struct Doublet* B, const struct Doublet* C, const struct Doublet* D){
		if(collisionSegment(Boule, A, B) ||
		   collisionSegment(Boule, B, C) ||
		   collisionSegment(Boule, C, D) ||
		   collisionSegment(Boule, D, A)){
			return true;
		}else{
			return false;
		}
}
bool libre(const uint8_t** map, const uint8_t i, const uint8_t j){
	if(map[i][j]!=BRIQUE && map[i][j]!=BRIQUE_BLEU && map[i][j]!=PORTE_FERMEE && map[i][j]!=PORTE_OUVERTE){
		return true;
	}
	return false;
}

void deplacement(struct Balle *balle){
	int a = 0;

	if(balle->bonus==3){
		a+=500;
	}

	if(kb_Data[7] & kb_Left){
		if(balle->vitesse.x==0){
			balle->vitesse.x = -MAX_VITESSE_X/2.0;
		}else if(balle->vitesse.x>-MAX_VITESSE_X/1.5){
			balle->vitesse.x--;
		}else{
			balle->vitesse.x = -MAX_VITESSE_X-a;
		}
		balle->collision = false;
	}else if(kb_Data[7] & kb_Right){
		if(balle->vitesse.x==0){
			balle->vitesse.x = MAX_VITESSE_X/2.0;
		}else if(balle->vitesse.x<MAX_VITESSE_X/1.5){
			balle->vitesse.x++;
		}else{
			balle->vitesse.x = MAX_VITESSE_X-a;
		}
		balle->collision = false;
	}else{
		balle->vitesse.x=0; // Mise à l'arrêt 
	}
	if(kb_Data[7] & kb_Up){
		if(balle->bonus==1 || balle->possibiliteSaut){
			balle->vitesse.y	= -MAX_VITESSE_Y;
			balle->force.y	= 0;
			balle->collision	= false;
			balle->possibiliteSaut = false;
		}
	}
}
void collisionDroite0Deg  (struct Balle* balle, const int y, float CoeffFri){
	if(balle->vitesse.y>-50){
		balle->Coo.y+=y-(balle->Coo.y-balle->rayon);
		balle->vitesse.y = 0;
	}else{
		balle->Coo.y+=2.0*(y-(balle->Coo.y-balle->rayon));
		balle->vitesse.y = -balle->vitesse.y;
		balle->vitesse.x = CoeffFri*balle->vitesse.x;
		balle->vitesse.y = CoeffFri*balle->vitesse.y;
	}
	if(abs(balle->vitesse.y)<MAX_VITESSE_Y && balle->bonus==2){
		if(kb_Data[7] & kb_Down){
			balle->force.y = MAX_VITESSE_Y;
			balle->vitesse.y = 0;
			balle->collision = false;
		}
	}
}
void collisionDroite90Deg (struct Balle* balle, const int x, float CoeffFri){
	if(CoeffFri==0){
		balle->Coo.x	   = x-balle->rayon;
		balle->vitesse.x = 0;
	}else{
		balle->Coo.x	  -= 2.0*(balle->Coo.x+balle->rayon-x);
		balle->vitesse.x = -(CoeffFri*balle->vitesse.x);
	}
}
void collisionDroite180Deg(struct Balle* balle, const int y, float CoeffFri){
	if(balle->vitesse.y<LIMITE_REBOND){
		balle->Coo.y	  -=(balle->Coo.y+balle->rayon)-y;
		balle->vitesse.y = 0;
	}else{
		balle->Coo.y	  -= 2.0*(balle->Coo.y+balle->rayon-y);
		balle->vitesse.y = -balle->vitesse.y;
		balle->vitesse.x = CoeffFri*balle->vitesse.x;
		balle->vitesse.y = CoeffFri*balle->vitesse.y;
	}
	if(abs(balle->vitesse.y)<MAX_VITESSE_Y && balle->bonus!=2){
		balle->possibiliteSaut = true;
	}
}
void collisionDroite270Deg(struct Balle* balle, const int x, float CoeffFri){
	if(CoeffFri==0){
		balle->Coo.x	   = x+balle->rayon;
		balle->vitesse.x = 0;
	}else{
		balle->Coo.x	  += 2.0*(x-(balle->Coo.x-balle->rayon));
		balle->vitesse.x = CoeffFri*balle->vitesse.x;
	}
}
void collisionDroite135Deg(struct Balle* balle, const struct Doublet* a, const struct Doublet* b, float CoeffFri){
	float distanceRebond = balle->rayon-distanceCentreCercleDroite(balle, a, b);
	float tamponVitesse = balle->vitesse.x;

	if(balle->vitesse.x>-100){
		CoeffFri = 1;
	}
	balle->Coo.x+=2.0*distanceRebond;
	balle->Coo.y+=2.0*distanceRebond;
	balle->vitesse.x = abs(balle->vitesse.y);
	balle->vitesse.y = abs(tamponVitesse);
	balle->vitesse.x = COEFFICIENT_FRICTION_BRIQUE*balle->vitesse.x;
	balle->vitesse.y = COEFFICIENT_FRICTION_BRIQUE*balle->vitesse.y;
	if(abs(balle->vitesse.y)<500 && balle->bonus!=2){
		/*if(isKeyPressed(SDLK_UP)){
			balle->force.y = 89000;
			balle->vitesse.y = 0;
			balle->collision = false;
		}*/
	}
}
void collisionDroite45Deg (struct Balle* balle, const struct Doublet* a, const struct Doublet* b, float CoeffFri){
	float distanceRebond = balle->rayon-distanceCentreCercleDroite(balle, a, b);
	float tamponVitesse  = balle->vitesse.x;

	if(balle->vitesse.x<100){
		CoeffFri=1;
	}
	balle->Coo.x	  -= 2.0*distanceRebond;
	balle->Coo.y	  += 2.0*distanceRebond;
	balle->vitesse.x = abs(balle->vitesse.y);
	balle->vitesse.y = abs(tamponVitesse);
	balle->vitesse.x = balle->vitesse.x*CoeffFri;
	balle->vitesse.y = balle->vitesse.y*CoeffFri;
	/*if(abs(balle->vitesse.y)<500 && balle->bonus!=2){
		if(isKeyPressed(SDLK_UP)){
			balle->force.y = 89000;
			balle->vitesse.y = 0;
			balle->collision = false;
		}
	}*/
}
void collisionDroite225Deg(struct Balle* balle, const struct Doublet* a, const struct Doublet* b, float CoeffFri){
	float distanceRebond = balle->rayon-distanceCentreCercleDroite(balle, a, b);
	float tamponVitesse  = balle->vitesse.x;

	balle->Coo.x+=2.0*distanceRebond;
	balle->Coo.y-=2.0*distanceRebond;
	balle->vitesse.x = abs(balle->vitesse.y);
	balle->vitesse.y = -abs(tamponVitesse);
	balle->vitesse.x = balle->vitesse.x*CoeffFri;
	balle->vitesse.y = balle->vitesse.y*CoeffFri;
}
void collisionDroite315Deg(struct Balle* balle, const struct Doublet* a, const struct Doublet* b, float CoeffFri){
	float distanceRebond = balle->rayon-distanceCentreCercleDroite(balle, a, b);
	float tamponVitesse  = balle->vitesse.x;

	balle->Coo.x+=2.0*distanceRebond;
	balle->Coo.y-=2.0*distanceRebond;
	balle->vitesse.x = -balle->vitesse.y;
	balle->vitesse.y = -tamponVitesse;
	balle->vitesse.x = balle->vitesse.x*CoeffFri;
	balle->vitesse.y = balle->vitesse.y*CoeffFri;
	
}

uint8_t collision(struct World* World){
	int i, j, a;
	uint8_t retour = NON;
	struct Doublet A, B, C, D, hd;
	//struct Balle   testBalle;
	struct Brique  testBrique = {{0, 0}, Uh, Ul};
	bool inhibeDessus;
	//testBalle.rayon			  = Ul;

	World->boule.collision = false;
	
	for(i=0; i<World->nbEnnemis; i++){ // Collision avec ennemis
		if(collisionCercleRectangle(&World->boule, &World->ennemis[i].cadre)){
			return DEGAT;
		}
	}
	
	i = (abs(World->HG.x)+World->boule.Coo.x)/Ul;
	j = (abs(World->HG.y)+World->boule.Coo.y)/Uh;
	
	switch(World->map[i][j]){     // Collision case actuelle
		case PETIT_CERCEAU:
			if((World->map[i][j+1]==PETIT_CERCEAU || World->map[i][j-1]==PETIT_CERCEAU) && World->boule.Coo.x>(i+0.3)*Ul-abs(World->HG.x) && World->boule.Coo.x<(i+0.7)*Ul-abs(World->HG.x)){ // MARGE 0000
				World->nbCerceau--;
				World->score+=500;
				World->map[i][j]=PETIT_GRIS;
				if(World->map[i][j+1]==PETIT_CERCEAU){
					World->map[i][j+1]=PETIT_GRIS;
				}else{
					World->map[i][j-1]=PETIT_GRIS;
				}
			}else if((World->map[i+1][j]==PETIT_CERCEAU || World->map[i-1][j]==PETIT_CERCEAU) && World->boule.Coo.y>(j+0.3)*Ul-abs(World->HG.y) && World->boule.Coo.y<(j+0.7)*Ul-abs(World->HG.y)){ // MARGE 0000
				World->nbCerceau--;
				World->score+=500;
				World->map[i][j]=PETIT_GRIS;
				if(World->map[i+1][j]==PETIT_CERCEAU){
					World->map[i+1][j]=PETIT_GRIS;
				}else{
					World->map[i-1][j]=PETIT_GRIS;
				}
			}
			// Pas de "break;".
		case PETIT_GRIS:
			if(World->map[i][j-1]==PETIT_CERCEAU || World->map[i][j-1]==PETIT_GRIS){
				if(World->boule.vitesse.x<0 && World->boule.vitesse.x>-MAX_VITESSE_X){
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x)+1, j*Uh-abs(World->HG.y)+15)){
						World->boule.Coo.x++;
					}
					retour = GAUCHE;
				}else if(World->boule.vitesse.x>0 && World->boule.vitesse.x<MAX_VITESSE_X){
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x)-1, j*Uh-abs(World->HG.y)+15)){
						World->boule.Coo.x--;
					}
					retour = DROITE;
				}else{
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+16)){ // Maintient sur bord intérieur
						World->boule.Coo.y--;
						//World->boule.vitesse.x*=COEFFICIENT_FRICTION_BRIQUE/3;
						World->boule.possibiliteSaut = true;
						World->boule.vitesse.y	   = 0;
					}
					if(World->boule.Coo.x<(i+0.4)*Ul-abs(World->HG.x)){ // Chute par côté adapté
						World->boule.Coo.x-=0.5;
					}else if(World->boule.Coo.x>(i+0.6)*Ul-abs(World->HG.x)){
						World->boule.Coo.x+=0.5;
					}
				}
				/*if(abs(World->boule.vitesse.y)<MAX_VITESSE_Y && World->boule.bonus!=2){
					if(kb_Data[7] & kb_Up){
						World->boule.force.y	 = -MAX_VITESSE_Y; // A mettre dans deplacement() !! 00000
						World->boule.vitesse.y = 0;
						World->boule.collision = false;
					}
				}*/
			}else if(World->map[i][j+1]==PETIT_CERCEAU || World->map[i][j+1]==PETIT_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+14)){ // Maintient sur bord intérieur
					World->boule.Coo.y++;
					World->boule.vitesse.y=0;
					//World->boule.force.y=0; (Bonus 2 ??) 0000
				}
				if(World->boule.Coo.x<(i+0.4)*Ul-abs(World->HG.x)){ // Chute par côté adapté
					World->boule.Coo.x-=0.5;
				}else if(World->boule.Coo.x>(i+0.6)*Ul-abs(World->HG.x)){
					World->boule.Coo.x+=0.5;
				}
			}else if(World->map[i-1][j]==PETIT_CERCEAU || World->map[i-1][j]==PETIT_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+15)){
					World->boule.Coo.x--;
				}
			}
			else if(World->map[i+1][j]==PETIT_CERCEAU || World->map[i+1][j]==PETIT_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+15)){
					World->boule.Coo.x++;
				}
			}
			break;
		case GRAND_CERCEAU:
			if((World->map[i][j+1]==GRAND_CERCEAU || World->map[i][j-1]==GRAND_CERCEAU) && World->boule.Coo.x>(i+0.3)*Ul-abs(World->HG.x) && World->boule.Coo.x<(i+0.7)*Ul-abs(World->HG.x)){ // MARGE 0000
				World->nbCerceau--;
				World->score+=500;
				World->map[i][j]=GRAND_GRIS;
				if(World->map[i][j+1]==GRAND_CERCEAU){
					World->map[i][j+1]=GRAND_GRIS;
				}else{
					World->map[i][j-1]=GRAND_GRIS;
				}
			}else if((World->map[i+1][j]==GRAND_CERCEAU || World->map[i-1][j]==GRAND_CERCEAU) && World->boule.Coo.y>(j+0.3)*Ul-abs(World->HG.y) && World->boule.Coo.y<(j+0.7)*Ul-abs(World->HG.y)){ // MARGE 0000
				World->nbCerceau--;
				World->score+=500;
				World->map[i][j]=GRAND_GRIS;
				if(World->map[i+1][j]==GRAND_CERCEAU){
					World->map[i+1][j]=GRAND_GRIS;
				}else{
					World->map[i-1][j]=GRAND_GRIS;
				}
			}
			// Pas de "break;".
		case GRAND_GRIS:
			if(World->map[i][j-1]==GRAND_CERCEAU || World->map[i][j-1]==GRAND_GRIS){
				if(World->boule.vitesse.x<0 && World->boule.vitesse.x>-MAX_VITESSE_X){
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x)+1, j*Uh-abs(World->HG.y)+10)){
						World->boule.Coo.x++;
					}
					retour = GAUCHE;
				}else if(World->boule.vitesse.x>0 && World->boule.vitesse.x<MAX_VITESSE_X){
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x)-1, j*Uh-abs(World->HG.y)+10)){
						World->boule.Coo.x--;
					}
					retour = DROITE;
				}else{
					while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+20)){ // Maintient sur bord intérieur
						World->boule.Coo.y--;
						//World->boule.vitesse.x*=COEFFICIENT_FRICTION_BRIQUE/3;
						World->boule.possibiliteSaut = true;
						World->boule.vitesse.y	   = 0;
					}
					if(World->boule.Coo.x<(i+0.4)*Ul-abs(World->HG.x)){ // Chute par côté adapté
						World->boule.Coo.x-=0.5;
					}else if(World->boule.Coo.x>(i+0.6)*Ul-abs(World->HG.x)){
						World->boule.Coo.x+=0.5;
					}
				}
				/*if(abs(World->boule.vitesse.y)<MAX_VITESSE_Y && World->boule.bonus!=2){
					if(kb_Data[7] & kb_Up){
						World->boule.force.y	 = -MAX_VITESSE_Y; // A mettre dans deplacement() !! 00000
						World->boule.vitesse.y = 0;
						World->boule.collision = false;
					}
				}*/
			}else if(World->map[i][j+1]==GRAND_CERCEAU || World->map[i][j+1]==GRAND_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+9)){ // Maintient sur bord intérieur
					World->boule.Coo.y++;
					World->boule.vitesse.y=0;
					//World->boule.force.y=0; (Bonus 2 ??) 0000
				}
				if(World->boule.Coo.x<(i+0.4)*Ul-abs(World->HG.x)){ // Chute par côté adapté
					World->boule.Coo.x-=0.5;
				}else if(World->boule.Coo.x>(i+0.6)*Ul-abs(World->HG.x)){
					World->boule.Coo.x+=0.5;
				}
			}else if(World->map[i-1][j]==GRAND_CERCEAU || World->map[i-1][j]==GRAND_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+10)){
					World->boule.Coo.x--;
				}
			}
			else if(World->map[i+1][j]==GRAND_CERCEAU || World->map[i+1][j]==GRAND_GRIS){
				while(collisionPointCercle(&World->boule, (i+0.5)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y)+10)){
					World->boule.Coo.x++;
				}
			}
			break;
	}
	if(World->boule.vitesse.y<0){	// Collision cases du dessus
		for(a=i-1; a<i+2; a++){
			switch(World->map[a][j-1]){
				case PORTE_OUVERTE:
					if(libre(World->map, a, j)){
						A = retourneDoublet( a   *Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
						B = retourneDoublet((a+1)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre(World->map, a, j)){
						A = retourneDoublet( a   *Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
						B = retourneDoublet((a+1)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							collisionDroite0Deg(&(World->boule), j*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case DEGONFLEUR:
					World->boule.rayon=Ul/2.0; // Pas de break !!
				case BRIQUE:
					if(libre(World->map, a, j)){
						testBrique.bg.x = a    *Ul-abs(World->HG.x);// -(abs(x)) = +x <=> x<0   00000
						testBrique.bg.y = (j-1)*Uh-abs(World->HG.y);
						if(collisionCercleRectangle(&(World->boule), &testBrique)){
							collisionDroite0Deg(&World->boule, j*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur	= Ul*0.5;
					testBrique.bg.x 	= (a+0.25)*Ul-abs(World->HG.x);// -(abs(x)) = +x <=> x<0   00000
					testBrique.bg.y		= (j-1)	  *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						return DEGAT;
					}
					break;
				case GONFLEUR:
					testBrique.largeur	= 12;
					testBrique.bg.x 	= a*Ul-abs(World->HG.x)+12;// -(abs(x)) = +x <=> x<0   00000
					testBrique.bg.y		= (j-1)	  *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						World->boule.rayon = 20;
						collisionDroite0Deg(&World->boule, j*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
					}
					break;
				case LOSANGE:
					A = retourneDoublet((a+0.5)*Ul-abs(World->HG.x),  j  *Uh-abs(World->HG.y));
					B = retourneDoublet((a+1)  *Ul-abs(World->HG.x), (j-0.5)*Uh-abs(World->HG.y));
					C = retourneDoublet((a+0.5)*Ul-abs(World->HG.x), (j-1)  *Uh-abs(World->HG.y));
					D = retourneDoublet( a     *Ul-abs(World->HG.x), (j-0.5)*Uh-abs(World->HG.y));					
					if(collisionLosange(&(World->boule), &A, &B, &C, &D)){
						World->score+=500;
						World->boule.balleSave.bg 	 = retourneDoublet((a+0.5)*Ul-abs(World->HG.x), (j-0.5)*Uh-abs(World->HG.y));
						World->boule.balleSave.largeur = World->boule.rayon;
						World->map[a][j-1]       = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle(World->boule.Coo.x, World->boule.Coo.y, World->boule.rayon, (a+0.5)*Ul-abs(World->HG.x), (j-0.5)*Uh-abs(World->HG.y), 0.5*Ul)){
						World->map[a][j-1]=VIDE;
						World->nbVie++;
						World->score+=1000;
					}
					break;
				case PETIT_GRIS: 
				case PETIT_CERCEAU:
					A = retourneDoublet((a+0.25)*Ul-abs(World->HG.x),   j*Uh-abs(World->HG.y)-7);
					B = retourneDoublet((a+0.25)*Ul-abs(World->HG.x)+5, j*Uh-abs(World->HG.y)-7);
					if(collisionSegment(&(World->boule), &A, &B) && (World->map[a][j-2]==PETIT_CERCEAU || World->map[a][j-2]==PETIT_GRIS)){
						collisionDroite0Deg(&World->boule, j*Uh-abs(World->HG.y)-7, 1);
					}
					break;
			}
		}
	}else{ 							// Collision cases du dessous
		for(a=i-1; a<i+2; a++){
			switch(World->map[a][j+1]){
				case PORTE_OUVERTE:
					if(libre(World->map, a, j)){
						A = retourneDoublet( a   *Ul-abs(World->HG.x), (j+1)*Uh-abs(World->HG.y));
						B = retourneDoublet((a+1)*Ul-abs(World->HG.x), (j+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre(World->map, a, j)){
						A = retourneDoublet( a   *Ul-abs(World->HG.x), (j+1)*Uh-abs(World->HG.y));
						B = retourneDoublet((a+1)*Ul-abs(World->HG.x), (j+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							collisionDroite180Deg(&(World->boule), (j+1)*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case DEGONFLEUR:
					World->boule.rayon=Ul/2.0; // Pas de break !!
				case BRIQUE:
					if(libre(World->map, a, j)){
						testBrique.bg.x = a    *Ul-abs(World->HG.x);
						testBrique.bg.y = (j+1)*Uh-abs(World->HG.y);
						if(collisionCercleRectangle(&(World->boule), &testBrique)){
							collisionDroite180Deg(&(World->boule), (j+1)*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur = Ul*0.5;
					testBrique.bg.x    = (a+0.25)	*Ul-abs(World->HG.x);
					testBrique.bg.y    = (j+1)		*Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						return DEGAT;
					}
					break;
				case GONFLEUR:
					testBrique.largeur	= 12;
					testBrique.bg.x 	= a*Ul-abs(World->HG.x)+9;// -(abs(x)) = +x <=> x<0   00000
					testBrique.bg.y		= (j+1)	  *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						World->boule.rayon = 20;
						collisionDroite180Deg(&(World->boule), (j+1)*Uh-abs(World->HG.y), COEFFICIENT_FRICTION_BRIQUE);
					}
					break;
				case LOSANGE:
					A = retourneDoublet((a+0.5)*Ul-abs(World->HG.x), (j+2)  *Uh-abs(World->HG.y));
					B = retourneDoublet((a+1)  *Ul-abs(World->HG.x), (j+1.5)*Uh-abs(World->HG.y));
					C = retourneDoublet((a+0.5)*Ul-abs(World->HG.x), (j+1)  *Uh-abs(World->HG.y));
					D = retourneDoublet( a     *Ul-abs(World->HG.x), (j+1.5)*Uh-abs(World->HG.y));					
					if(collisionLosange(&(World->boule), &A, &B, &C, &D)){
						World->score+=500;
						World->boule.balleSave.bg 	 = retourneDoublet((a+0.5)*Ul-abs(World->HG.x), (j+0.5)*Uh-abs(World->HG.y));
						World->boule.balleSave.largeur = World->boule.rayon;
						World->map[a][j+1]			 = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle(World->boule.Coo.x, World->boule.Coo.y, World->boule.rayon, (a+0.5)*Ul-abs(World->HG.x), (j+0.5)*Uh-abs(World->HG.y), 0.5*Ul)){
						World->map[a][j+1]= VIDE;
						World->nbVie++;
						World->score+=1000;
					}
					break;
				case PETIT_GRIS:
				case PETIT_CERCEAU:
					A = retourneDoublet((a+0.25)*Ul-abs(World->HG.x), (j+1)*Uh-abs(World->HG.y)+7);
					B = retourneDoublet((a+0.25)*Ul-abs(World->HG.x)+5, (j+1)*Uh-abs(World->HG.y)+7);
					if(collisionSegment(&(World->boule), &A, &B) && (World->map[a][j+2]==PETIT_CERCEAU || World->map[a][j+2]==PETIT_GRIS)){
						collisionDroite180Deg(&World->boule, (j+1)*Uh-abs(World->HG.y)+7, 1);
					}
					break;
			}
		}
	}
	if(World->boule.vitesse.x<0){	// Collision cases à gauche
		for(a=j-1; a<j+2; a++){
			switch(World->map[i-1][a]){
				case PORTE_OUVERTE:
					if(libre(World->map, i, a)){
						A = retourneDoublet(i*Ul-abs(World->HG.x),  a   *Uh-abs(World->HG.y));
						B = retourneDoublet(i*Ul-abs(World->HG.x), (a+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre(World->map, i, a)){
						A = retourneDoublet(i*Ul-abs(World->HG.x),  a   *Uh-abs(World->HG.y));
						B = retourneDoublet(i*Ul-abs(World->HG.x), (a+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							collisionDroite270Deg(&(World->boule), i*Ul-abs(World->HG.x), 0);
							return GAUCHE;
						}
					}
					break;
				case BRIQUE:
					if(libre(World->map, i, a)){
						testBrique.largeur = Ul;
						testBrique.bg.x    = (i-1)*Ul-abs(World->HG.x);
						testBrique.bg.y    = a    *Uh-abs(World->HG.y);
						if(collisionCercleRectangle(&(World->boule), &testBrique)){
							collisionDroite270Deg(&(World->boule), i*Ul-abs(World->HG.x), 0);
							return GAUCHE;
						}
					}
					break;
				case BARRE_JAUNE:
					if(World->map[i][a]!=BRIQUE){ // ?? 0000
						testBrique.largeur = 0.5*Ul;
						testBrique.bg.x    = (i-0.75)*Ul-abs(World->HG.x);
						testBrique.bg.y    = a       *Uh-abs(World->HG.y);
						if(collisionCercleRectangle(&(World->boule), &testBrique)){
							return DEGAT;
						}
					}
					break;
				case GONFLEUR:
					testBrique.largeur	= 12;
					testBrique.bg.x 	= i *Ul-abs(World->HG.x)-21;
					testBrique.bg.y		= a *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						World->boule.rayon = 20;
						collisionDroite270Deg(&(World->boule), i*Ul-abs(World->HG.x)-9, 0);
						return GAUCHE;
					}
					break;
				case LOSANGE:
					A = retourneDoublet((i-0.5)*Ul-abs(World->HG.x), (a+1)  *Uh-abs(World->HG.y));
					B = retourneDoublet( i     *Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));
					C = retourneDoublet((i-0.5)*Ul-abs(World->HG.x),  a     *Uh-abs(World->HG.y));
					D = retourneDoublet((i-1)  *Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));					
					if(collisionLosange(&(World->boule), &A, &B, &C, &D)){
						World->score+=500;
						World->boule.balleSave.bg 	   = retourneDoublet((i-0.5)*Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));
						World->boule.balleSave.largeur = World->boule.rayon;
						World->map[i-1][a]	   = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle(World->boule.Coo.x, World->boule.Coo.y, World->boule.rayon, (i-0.5)*Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y), 0.5*Ul)){
						World->map[i-1][a]= VIDE;
						World->nbVie++;
						World->score+=1000;
					}
					break;
			}			
		}
	}else{ 							// Collision cases à droite
		for(a=j-1; a<j+2; a++){
			switch(World->map[i+1][a]){
				case PORTE_OUVERTE:
					if(libre(World->map, i, a)){
						A = retourneDoublet((i+1)*Ul-abs(World->HG.x),  a   *Uh-abs(World->HG.y));
						B = retourneDoublet((i+1)*Ul-abs(World->HG.x), (a+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre(World->map, i, a)){
						A = retourneDoublet((i+1)*Ul-abs(World->HG.x),  a   *Uh-abs(World->HG.y));
						B = retourneDoublet((i+1)*Ul-abs(World->HG.x), (a+1)*Uh-abs(World->HG.y));
						if(collisionSegment(&(World->boule), &A, &B)){
							collisionDroite90Deg(&(World->boule), (i+1)*Ul-abs(World->HG.x), 0);
							return DROITE;
						}
					}
					break;
				case DEGONFLEUR:
					World->boule.rayon=Ul/2.0; // Pas de break !!
				case BRIQUE:
					if(libre(World->map, i, a)){
						testBrique.largeur = Ul;
						testBrique.bg.x    = (i+1)*Ul-abs(World->HG.x);
						testBrique.bg.y    = a    *Uh-abs(World->HG.y);
						if(collisionCercleRectangle(&(World->boule), &testBrique)){
							collisionDroite90Deg(&(World->boule), (i+1)*Ul-abs(World->HG.x), 0);
							return DROITE;
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur = Ul*0.5;
					testBrique.bg.x    = (i+1.25) *Ul-abs(World->HG.x);
					testBrique.bg.y    = a		  *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						return DEGAT;
					}
					break;
				case GONFLEUR:
					testBrique.largeur	= 12;
					testBrique.bg.x 	= (i+1) *Ul-abs(World->HG.x)+9;
					testBrique.bg.y		= a     *Uh-abs(World->HG.y);
					if(collisionCercleRectangle(&(World->boule), &testBrique)){
						World->boule.rayon=20;
						collisionDroite90Deg(&(World->boule), (i+1)*Ul-abs(World->HG.x)+9, 0);
						return DROITE;
					}
					break;
				case LOSANGE:
					A = retourneDoublet((i+1.5)*Ul-abs(World->HG.x), (a+1)  *Uh-abs(World->HG.y));
					B = retourneDoublet((i+2)  *Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));
					C = retourneDoublet((i+1.5)*Ul-abs(World->HG.x),  a     *Uh-abs(World->HG.y));
					D = retourneDoublet((i+1)  *Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));					
					if(collisionLosange(&(World->boule), &A, &B, &C, &D)){
						World->score+=500;
						World->boule.balleSave.bg 	   = retourneDoublet((i+0.5)*Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y));
						World->boule.balleSave.largeur = World->boule.rayon;
						World->map[i+1][a]	   = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle(World->boule.Coo.x, World->boule.Coo.y, World->boule.rayon, (i+1.5)*Ul-abs(World->HG.x), (a+0.5)*Uh-abs(World->HG.y), 0.5*Ul)){
						World->map[i+1][a]= VIDE;
						World->nbVie++;
						World->score+=1000;
					}
					break;
				/*case PETIT_CERCEAU:
					if(map[i+1][a-1]==PETIT_CERCEAU){
						A = retourneDoublet((i+1)*Ul-abs(World->HG.x),   a*Uh-abs(World->HG.y));
						B = retourneDoublet((i+1.5)*Ul-abs(World->HG.x), a*Uh-abs(World->HG.y)+15);
						while(collisionSegment(A, B, World->boule)){
							World->boule.Coo.y--;
							//collisionDroite45Deg(&World->boule, A, B, 1);
							//while(true);
						}
					}
					break;*/
			}
		}
	}
	/*for(i=0; i<MAXBRIQUEBLEU; i++){ // Collision avec briques bleues
		if(collisionCercleRectangle(World->boule, World->briqueBleu[i]) && World->boule.collision==false){
			rebond(&(World->boule), World->briqueBleu[i], COEFFICIENT_FRICTION_BRIQUE*2.0/*, true);
			World->boule.collision = true;
			return false;
		}
	}
	for(i=0; i<MAXBONUS1; i++){     // Collision avec bonus 1
		if(collisionCercleRectangle(World->boule, World->bonus1[i].hitbox) && World->boule.collision==false){
			rebond(&(World->boule), World->bonus1[i].hitbox, COEFFICIENT_FRICTION_BRIQUE);
			World->boule.bonus = 1;
			World->barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXBONUS2; i++){     // Collision avec bonus 2
		if(collisionCercleRectangle(World->boule, World->bonus2[i]) && World->boule.collision==false){
			rebond(&World->boule, World->bonus2[i], COEFFICIENT_FRICTION_BRIQUE);
			World->boule.collision = true;
			World->boule.bonus = 2;
			World->barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXBONUS3; i++){     // Collision avec bonus 3
		if(collisionCercleRectangle(World->boule, World->bonus3[i]) && World->boule.collision==false){
			rebond(&World->boule, World->bonus3[i], COEFFICIENT_FRICTION_BRIQUE);
			World->boule.bonus = 3;
			World->barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXTRIANGLE; i++){   // Collision avec briques triangulaires
		if(World->briqueAngle[i].triangle[0].x==World->briqueAngle[i].triangle[2].x && World->briqueAngle[i].triangle[1].x<World->briqueAngle[i].triangle[0].x){       // Brique de la forme |\.
			if(CollisionSegment(World->briqueAngle[i].bloc.bg, World->briqueAngle[i].triangle[2], World->boule)){     // Rebond contre dessous
				collisionDroite180Deg(&World->boule, World->briqueAngle[i].bloc.bg.y, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}
			/*else if(CollisionSegment(World->briqueAngle[i].bloc.bg, World->briqueAngle[i].triangle[1], World->boule)){   // Rebond contre gauche (Inutile dans ce niveau)
				collisionDroite90Deg(World->boule, World->briqueAngle[i].bloc.bg.x, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}
			else if(CollisionSegment(World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], World->boule)){ // Rebond en biais
				collisionDroite135Deg(&World->boule, World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}
		}else if(World->briqueAngle[i].triangle[0].x==World->briqueAngle[i].triangle[2].x && World->briqueAngle[i].triangle[1].x>World->briqueAngle[i].triangle[0].x){ // Brique de la forme /|
			/*Doublet bd = retourneDoublet(World->briqueAngle[i].bloc.bg.x+World->briqueAngle[i].bloc.largeur, World->briqueAngle[i].bloc.bg.y);
			if(CollisionSegment(World->briqueAngle[i].triangle[2], bd, World->boule)){                               // Rebond contre dessous (Inutile dans ce niveau)
				collisionDroite180Deg(World->boule, bd.y, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else if(CollisionSegment(bd, World->briqueAngle[i].triangle[1], World->boule)){                         // Rebond contre droite (Inutile dans ce niveau)
				collisionDroite270Deg(World->boule, bd.x, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else 
			if(CollisionSegment(World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], World->boule)){ // Rebond en biais
				collisionDroite45Deg(&World->boule, World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}
		}else if(World->briqueAngle[i].triangle[0].x==World->briqueAngle[i].triangle[1].x && World->briqueAngle[i].triangle[0].x>World->briqueAngle[i].triangle[2].x){ // Brique de la forme |/
			/*Doublet hg = retourneDoublet(World->briqueAngle[i].bloc.bg.x, World->briqueAngle[i].bloc.bg.y+World->briqueAngle[i].bloc.hauteur);
			if(CollisionSegment(World->briqueAngle[i].triangle[2], hg, World->boule)){                               // Rebond contre gauche (Inutile dans ce niveau)
				collisionDroite90Deg(&World->boule, hg.x, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else if(CollisionSegment(World->briqueAngle[i].triangle[1], hg, World->boule)){                         // Rebond contre haut (Inutile dans ce niveau)
				collisionDroite0Deg(&World->boule, hg.y, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else 
			if(CollisionSegment(World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], World->boule)){ // Rebond en biais
				collisionDroite225Deg(&World->boule, World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}
		}else if(World->briqueAngle[i].triangle[0].x==World->briqueAngle[i].triangle[1].x && World->briqueAngle[i].triangle[0].x<World->briqueAngle[i].triangle[2].x){ // Brique de la forme \|
			hd = retourneDoublet(World->briqueAngle[i].bloc.bg.x+World->briqueAngle[i].bloc.largeur, World->briqueAngle[i].bloc.bg.y+World->briqueAngle[i].bloc.hauteur);
			if(CollisionSegment(World->briqueAngle[i].triangle[1], hd, World->boule)){                                     // Rebond contre haut (Inutile dans ce niveau)
				collisionDroite0Deg(&World->boule, hd.y, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else if(CollisionSegment(hd, World->briqueAngle[i].triangle[2], World->boule)){                               // Rebond contre droite (Inutile dans ce niveau)
				collisionDroite270Deg(&World->boule, hd.x, COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}else if(CollisionSegment(World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], World->boule)){ // Rebond en biais
				collisionDroite315Deg(&World->boule, World->briqueAngle[i].triangle[1], World->briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				World->boule.collision = true;
				return false;
			}		
		}
	}
	for(i=0; i<MAXEAU; i++){        // Collision avec eau
		if(collisionCercleRectangle(World->boule, World->mer[i])){
			float a=pousseeArchimede(World->boule,  World->mer[i].bg.y+World->mer[i].hauteur);
			if(World->boule.vitesse.y<-100){
				World->boule.vitesse.y*=0.97;
			}else if(World->boule.vitesse.y>50){
				World->boule.vitesse.y*=0.8;
			}
			World->boule.vitesse.x=World->boule.vitesse.x*0.97;
			if(World->boule.rayon==Ul/2.0){
				a/=60.0;
			}else{
				a/=15.0;
			}
			World->boule.force.y-=a;
			i = MAXEAU;
		}
	}*/
	return retour;
}
void patternEnnemis(struct Ennemi* ennemi, double t){
	t/=500;

	if((*ennemi).axe){	// Déplacement vertical
		if((*ennemi).cadre.bg.y>(*ennemi).max){
			(*ennemi).direction=0;
			(*ennemi).cadre.bg.y-=t;
		}else if((*ennemi).cadre.bg.y<(*ennemi).min){
			(*ennemi).direction=1;
			(*ennemi).cadre.bg.y+=t;
		}else if((*ennemi).direction==1){
			(*ennemi).cadre.bg.y+=t;
		}else{
			(*ennemi).cadre.bg.y-=t;
		}
	}else{				// Déplacement horizontal
		if((*ennemi).cadre.bg.x+(*ennemi).cadre.largeur>(*ennemi).max){
			(*ennemi).direction=0;
			(*ennemi).cadre.bg.x-=t;
		}else if((*ennemi).cadre.bg.x<(*ennemi).min){
			(*ennemi).direction=1;
			(*ennemi).cadre.bg.x+=t;
		}else if((*ennemi).direction==1){
			(*ennemi).cadre.bg.x+=t;
		}else{
			(*ennemi).cadre.bg.x-=t;
		}
	}
}
bool update_balle(struct World* World, double deltaTemps){
	int8_t	b;
	uint8_t	typeCollision;
	
	for(b=0; b<World->nbEnnemis; b++){
		patternEnnemis(&World->ennemis[b], deltaTemps);
	}
	
	b=1;
	deltaTemps/=3276.0;

	if(World->boule.bonus==2){
		b=-1;
	}
	if(abs(World->boule.force.y)!= MAX_VITESSE_Y){ // Application de la gravité si pas de départ de saut.
		World->boule.force.y		 = G.y * b;
	}
	if(World->boule.degat){ // Remise de la balle au point de sauvegarde suite au "pop".
		World->boule.Coo			 = World->boule.balleSave.bg;
	}
	World->boule.degat				 = false;
	World->boule.possibiliteSaut	 = false;

	//World->boule.vitesse.x 	   = (float)deltaTemps*World->boule.force.x; // Newton 
	World->boule.vitesse.y			+= (float)deltaTemps*World->boule.force.y; //  "
	World->boule.Coo.x 				+= (float)deltaTemps*World->boule.vitesse.x;
	World->boule.Coo.y				+= (float)deltaTemps*World->boule.vitesse.y;
	typeCollision						 = collision(World);
	
	//World->boule.force.x		   = 0;
	World->boule.force.y			 	 = 0;

	if(typeCollision==SORTIE){
		return true;
	}else if(typeCollision==DEGAT){// Collision malus	
		World->boule.degat			 = true;
		World->boule.vitesse		 	 = retourneDoublet(0, 0);
		World->boule.force			 = retourneDoublet(0, 0);
		World->boule.bonus			 = 0;
		World->boule.rayon			 = World->boule.balleSave.largeur;
		World->nbVie--;
	}else{
		if(typeCollision==GAUCHE){
			World->boule.theta		-=10;
		}else if(typeCollision==DROITE){
			World->boule.theta		+=10;
		}else{
			World->boule.theta		+=asin((deltaTemps*World->boule.vitesse.x)/World->boule.rayon)*40.743*b; // *180/M_PI/360*256
		}
		while(World->boule.theta>255){ // Utilitée pour éviter overflow ?? 0000
			World->boule.theta		-=255;
		}
	}

	if(World->nbCerceau==0){
		World->map[(short unsigned int)World->CooPorte.x]  [(short unsigned int)World->CooPorte.y]   = PORTE_OUVERTE;
		World->map[(short unsigned int)World->CooPorte.x]  [(short unsigned int)World->CooPorte.y+1] = PORTE_OUVERTE;
		World->map[(short unsigned int)World->CooPorte.x+1][(short unsigned int)World->CooPorte.y]   = PORTE_OUVERTE;
		World->map[(short unsigned int)World->CooPorte.x+1][(short unsigned int)World->CooPorte.y+1] = PORTE_OUVERTE;
		World->nbCerceau--;
	}
	

	return false;
}

#include "gfx/sprite.h"
gfx_UninitedSprite(rotate_balle,        balle_width,            balle_height);
gfx_UninitedSprite(rotate_grosse_balle, grosse_balle_width,     grosse_balle_height);
gfx_UninitedSprite(GaucheJaunePetitHor, GaucheJaunePetit_width, GaucheJaunePetit_height);
gfx_UninitedSprite(DroitJaunePetitHor,  DroitJaunePetit_width,  DroitJaunePetit_height);
gfx_UninitedSprite(GaucheJauneGrandHor, GaucheJauneGrand_width, GaucheJauneGrand_height);
gfx_UninitedSprite(DroitJauneGrandHor,  DroitJauneGrand_width,  DroitJauneGrand_height);
gfx_UninitedSprite(GaucheGrisPetitHor,  GaucheGrisPetit_width,  GaucheGrisPetit_height);
gfx_UninitedSprite(DroitGrisPetitHor,   DroitGrisPetit_width,   DroitGrisPetit_height);
gfx_UninitedSprite(GaucheGrisGrandHor,  GaucheGrisGrand_width,  GaucheGrisGrand_height);
gfx_UninitedSprite(DroitGrisGrandHor,   DroitGrisGrand_width,   DroitGrisGrand_height);

void affiche_score(const struct World *World){
	uint8_t i;
	
	// Affichage vies
	gfx_SetColor(1);
	gfx_Circle(10, 10, 9);
	gfx_SetColor(2);
	gfx_FillCircle_NoClip(10, 10, 8);
	gfx_SetTextXY(22, 6);
	gfx_PrintString("x");
	gfx_SetTextXY(31, 7);
	if(World->nbVie==-1){
		gfx_PrintString("00");
	}else{
		gfx_PrintInt(World->nbVie, 2);
	}

	// Affichage cerceaux
	for(i=0; i<World->nbCerceau; i++){
		gfx_TransparentSprite_NoClip(Petitcerceau, 48+i*7, 3);
	}
	
	// Affichage score
	gfx_SetTextXY(260, 7);
	gfx_PrintInt(World->score, 7);
}
uint8_t pause(){
	uint8_t 	i;
	uint8_t 	curseur=0; // enum ?? 0000
	const char	option[4][20]={{"Reprendre"}, {"Recommencer"}, {"Quitter le niveau"}, {"Quitter le jeu"}};

	// Assombrissement
	for(i=3; i<255; i++){
		gfx_palette[i]=gfx_Darken(gfx_palette[i], 125);
	}

	while(kb_Data[1] & kb_2nd){ // Empêche sortie avant 
		kb_Scan();
	}

	// Menu
	do{
		gfx_SetColor(2);
		gfx_FillRectangle_NoClip(0.2*gfx_lcdWidth, 0.3*gfx_lcdHeight, 0.6*gfx_lcdWidth, 0.4*gfx_lcdHeight);
		gfx_SetColor(1);
		gfx_Rectangle_NoClip(0.2*gfx_lcdWidth+10, 0.3*gfx_lcdHeight+10, 0.6*gfx_lcdWidth-20, 0.4*gfx_lcdHeight-20);
		i=gfx_SetTextBGColor(2);
		gfx_PrintStringXY(" Pause ", 0.5*(gfx_lcdWidth-gfx_GetStringWidth(" Pause ")), 0.3*gfx_lcdHeight+6);	
		gfx_SetTextBGColor(i);
		for(i=0; i<4; i++){
			if(i==curseur){
				gfx_SetTextFGColor(0);
				gfx_PrintStringXY(option[i], 0.5*(gfx_lcdWidth-gfx_GetStringWidth(option[i])),  0.5*(gfx_lcdHeight-38)+10*i);
				gfx_SetTextFGColor(1);
			}else{
				gfx_PrintStringXY(option[i], 0.5*(gfx_lcdWidth-gfx_GetStringWidth(option[i])),  0.5*(gfx_lcdHeight-38)+10*i);
			}
		}
		kb_Scan();
		if(kb_Data[7] & kb_Up || kb_Data[7] & kb_Down){
			if(kb_Data[7] & kb_Up && curseur>0){
				curseur--;
			}else if(kb_Data[7] & kb_Down && curseur<3){
				curseur++;
			}
		}
		if(kb_Data[6] & kb_Enter){
			return curseur;
		}
		gfx_SwapDraw();
		delay(175); // ?? 0000
	}while(!(kb_Data[1] & kb_2nd));
	return 4;
}
void draw(const struct World *World){
	const uint8_t X_ORIGINE=abs(World->HG.x)/Ul, Y_ORIGINE=abs(World->HG.y)/Uh;
	uint8_t  i, j, c, d;
	bool cerceau=false;
	
	gfx_FillScreen(0);

	for(i=X_ORIGINE; i<(X_ORIGINE+gfx_lcdWidth/Ul)+2; i++){          // Affichage objets
		for(j=Y_ORIGINE; j<(Y_ORIGINE+gfx_lcdHeight/Uh)+1; j++){
			switch(World->map[i][j]){
				case BRIQUE:
					gfx_Sprite(brique, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case DEGONFLEUR:
					gfx_Sprite(degonfleur, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case GONFLEUR:
					gfx_Sprite(pompe, i*Ul-abs(World->HG.x)+9, j*Uh-abs(World->HG.y));
					break;
				case BARRE_JAUNE:
					gfx_Sprite(barreJaune, (i+0.25)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case LOSANGE:
					gfx_Sprite(losange, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case FLECHE:
					gfx_Sprite(fleche, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case BULLE:
					gfx_Sprite(bulle, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case PORTE_FERMEE:
					gfx_Sprite(portailFerme, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					break;
				case PORTE_OUVERTE:
					if(World->map[i][j+1]==PORTE_OUVERTE && World->map[i+1][j]==PORTE_OUVERTE){
						gfx_Sprite(portailOuvert, i*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					}else if(World->map[i][j+1]==PORTE_OUVERTE && World->map[i-1][j]==PORTE_OUVERTE && (i-1)*Ul-abs(World->HG.x)<0){ // idem
						gfx_Sprite(portailFerme, (i-1)*Ul-abs(World->HG.x), j*Uh-abs(World->HG.y));
					}
					break;
				case PETIT_CERCEAU:
					cerceau = true;
					if(World->map[i][j+1]==PETIT_CERCEAU){
						gfx_Sprite(DroitJaunePetit,    i*Ul-abs(World->HG.x)+15, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==PETIT_CERCEAU){
						gfx_Sprite(DroitJaunePetitHor, i*Ul-abs(World->HG.x),    j*Uh-abs(World->HG.y)+5);
					}
					break;
				case PETIT_GRIS:
					cerceau = true;
					if(World->map[i][j+1]==PETIT_GRIS){
						gfx_Sprite(DroitGrisPetit,    i*Ul-abs(World->HG.x)+15, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==PETIT_GRIS){
						gfx_Sprite(DroitGrisPetitHor, i*Ul-abs(World->HG.x),    j*Uh-abs(World->HG.y)+5);
					}
					break;
				case GRAND_CERCEAU:
					cerceau = true;
					if(World->map[i][j+1]==GRAND_CERCEAU){
						gfx_Sprite(DroitJauneGrand, i*Ul-abs(World->HG.x)+15, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==GRAND_CERCEAU){
						gfx_Sprite(DroitJauneGrand, i*Ul-abs(World->HG.x),    j*Uh-abs(World->HG.y)+5);
					}
					break;
				case GRAND_GRIS:
					cerceau = true;
					if(World->map[i][j+1]==GRAND_GRIS){
						gfx_Sprite(DroitGrisGrand, i*Ul-abs(World->HG.x)+15, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==GRAND_GRIS){
						gfx_Sprite(DroitGrisGrand, i*Ul-abs(World->HG.x),    j*Uh-abs(World->HG.y)+5);
					}
					break;
			}
		}
	}

	if(World->boule.degat){  // Affichage balle
		gfx_Sprite(pop, World->boule.Coo.x-World->boule.rayon, World->boule.Coo.y-World->boule.rayon);
	}else{
		if(World->boule.rayon==Ul/2.0){
			gfx_RotateSprite(balle, rotate_balle, (int)World->boule.theta);
			gfx_TransparentSprite_NoClip(rotate_balle, World->boule.Coo.x-World->boule.rayon, World->boule.Coo.y-World->boule.rayon);
		}else{
			gfx_RotateSprite(grosse_balle, rotate_grosse_balle, (int)World->boule.theta);
			gfx_TransparentSprite_NoClip(rotate_grosse_balle, World->boule.Coo.x-World->boule.rayon, World->boule.Coo.y-World->boule.rayon);
		}
	}

	if(cerceau){
		for(i=X_ORIGINE; i<(X_ORIGINE+gfx_lcdWidth/Ul)+2; i++){ 
			for(j=Y_ORIGINE; j<(Y_ORIGINE+gfx_lcdHeight/Uh)+1; j++){
				if(World->map[i][j]==PETIT_CERCEAU){
					if(World->map[i][j+1]==PETIT_CERCEAU){
						gfx_TransparentSprite(GaucheJaunePetit,  i*Ul-abs(World->HG.x)+5, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==PETIT_CERCEAU){
						gfx_TransparentSprite(GaucheJaunePetitHor,  i*Ul-abs(World->HG.x),   j*Uh-abs(World->HG.y)+15);
					}
				}else if(World->map[i][j]==PETIT_GRIS){
					if(World->map[i][j+1]==PETIT_GRIS){
						gfx_TransparentSprite(GaucheGrisPetit,  i*Ul-abs(World->HG.x)+5, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==PETIT_GRIS){
						gfx_TransparentSprite(GaucheGrisPetitHor,  i*Ul-abs(World->HG.x),   j*Uh-abs(World->HG.y)+15);
					}
				}else if(World->map[i][j]==GRAND_CERCEAU){
					if(World->map[i][j+1]==GRAND_CERCEAU){
						gfx_TransparentSprite(GaucheJauneGrand,  i*Ul-abs(World->HG.x)+5, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==GRAND_CERCEAU){
						gfx_TransparentSprite(GaucheJauneGrandHor,  i*Ul-abs(World->HG.x),   j*Uh-abs(World->HG.y)+15);
					}
				}else if(World->map[i][j]==GRAND_GRIS){
					if(World->map[i][j+1]==GRAND_GRIS){
						gfx_TransparentSprite(GaucheGrisGrand,  i*Ul-abs(World->HG.x)+5, j*Uh-abs(World->HG.y));
					}else if(World->map[i+1][j]==GRAND_GRIS){
						gfx_TransparentSprite(GaucheGrisGrandHor,  i*Ul-abs(World->HG.x),   j*Uh-abs(World->HG.y)+15);
					}
				}
			}
		}
	}
	for(d=0; d<World->nbEnnemis; d++){
		gfx_Sprite(ennemi, World->ennemis[d].cadre.bg.x, World->ennemis[d].cadre.bg.y);
	}
}

void main(void){
	uint8_t		 retour_menu		= 1;	// Retour selection du menu principal.
	uint8_t 	 retour_pause;				// Retour du menu de pause.
	uint8_t		 i 					= 0; 	// Delta de World.nbVie.
	uint8_t 	 j;							// Itération affichage choix du menu principal.
	uint8_t		 k;							// Boucle ennemis
	const char	 options[11][12]	= {{"Niveau 1"}, {"Niveau 2"}, {"(Niveau 3)"}, {"(Niveau 4)"}, {"(Niveau 5)"}, {"(Niveau 6)"}, {"(Niveau 7)"}, {"(Niveau 8)"}, {"(Niveau 9)"}, {"(Niveau 10)"}, {"(Niveau 11)"}};
	bool		 stop				= false;
	struct World World;

	gfx_Begin(gfx_8bpp);
	gfx_SetPalette(sprite_pal, sizeof_sprite_pal, 0);
	gfx_palette[0] = gfx_RGBTo1555(0,   218, 255);
	gfx_RotateSpriteCC(GaucheJaunePetit, GaucheJaunePetitHor);
	gfx_RotateSpriteCC(DroitJaunePetit,  DroitJaunePetitHor);
	gfx_RotateSpriteCC(GaucheJauneGrand, GaucheJauneGrandHor);
	gfx_RotateSpriteCC(DroitJauneGrand,  DroitJauneGrandHor);
	gfx_RotateSpriteCC(GaucheGrisPetit,  GaucheGrisPetitHor);
	gfx_RotateSpriteCC(DroitGrisPetit,   DroitGrisPetitHor);
	gfx_RotateSpriteCC(GaucheGrisGrand,  GaucheGrisGrandHor);
	gfx_RotateSpriteCC(DroitGrisGrand,   DroitGrisGrandHor);

	timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_0INT | TIMER1_DOWN;


dbg_Debugger();


	// MENU PRINCIPAL
	DEBUT:
	initialisation(&World);
	placageObjet(World.map, 11, 1, 0, 7, BRIQUE);
	do{ // Attente du relachement d'"enter".
		kb_Scan();
	}while(kb_Data[6] & kb_Enter);
	while(!(kb_Data[6] & kb_Enter)){
		timer_1_ReloadValue = timer_1_Counter = ONE_SECOND;
		draw(&World);
		for(j=1; j<12; j++){
			if(j==retour_menu){
				gfx_SetTextFGColor(2);
				gfx_PrintStringXY(options[j-1],  150, 50+j*10);
			}else{
				gfx_SetTextFGColor(1);
				gfx_PrintStringXY(options[j-1],  150, 50+j*10);
			}
		}
		update_balle(&World, ONE_SECOND-timer_1_Counter);
		gfx_SwapDraw();
		kb_Scan();
		if(kb_Data[7] & kb_Up || kb_Data[7] & kb_Down){
			if(kb_Data[7] & kb_Up && retour_menu>1){
				retour_menu--;
			}else if(kb_Data[7] & kb_Down && retour_menu<2){
				retour_menu++;
			}
		}
	}
	placageObjet(World.map, 11, 1, 0, 7, VIDE);

	NIVEAU:
	switch(retour_menu){
		case 1:
			initNiveau1(&World);
			break;
		case 2:
			initNiveau2(&World);
			break;
	}
	i=World.nbVie;
	
	while(!stop){
		kb_Scan();
		if(kb_Data[1] & kb_2nd){
			retour_pause=pause();
			gfx_SetPalette(sprite_pal, sizeof_sprite_pal, 0);
			gfx_palette[0] = gfx_RGBTo1555(0,   218, 255);
			switch(retour_pause){
				case 1:
					goto NIVEAU;
				case 2:
					goto DEBUT;
				case 3:
					goto FIN;
			}
		}
		timer_1_ReloadValue = timer_1_Counter = ONE_SECOND;

		stop = kb_Data[6] & kb_Clear;
		if(World.nbVie==-1){
			stop=true; //Sortie après mort
			delay(500);
		}else{
			draw(&World);
			affiche_score(&World);
			gfx_SwapDraw();
			if(i==World.nbVie+1){  // Temporisation après perte de vie et mort
				World.barreBonus.tempsRestant	= 0;
				delay(500);
				timer_1_Counter+=ONE_SECOND/2.0;
				i--;
			}
			deplacement(&World.boule);
			if(update_balle(&World, ONE_SECOND-timer_1_Counter)){
				goto FIN;
			}
			centrageFenetre(&World);
			
			
		}
	}
	
	FIN:
	gfx_End();
	for(i=0; i<(short int)World.BD.x/Ul; i++){
		free(World.map[i]);
	}
	prgm_CleanUp();
}
