#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <math.h>

#include "header.h"

extern struct Doublet retourneDoublet(float x, float y){
	struct Doublet a;
	a.x=x;
	a.y=y;
	return a; 
}
extern void placageObjet(int l, int h, int x, int y, uint8_t objet, uint8_t** map){
	int H, L = x+l, j;

	for(; x<L; x++){		
		H = y+h;
		for(j=y; j<H; j++){	
			map[x][j]=objet;
		}
	}
}
void centrageFenetre(struct World *World){
	struct Doublet termes = {0, 0};
	int i;
	unsigned char c = 0;

	if((int)(*World).boule.Coo.x<(int)gfx_lcdWidth/2 && (*World).HG.x<0){ // Lorsque mouvement vers gauche
		termes = retourneDoublet(1, 0);
		while((int)(*World).boule.Coo.x+termes.x<(int)gfx_lcdWidth*0.4 && (*World).HG.x+termes.x<0){
			termes.x++;
		}
	}else if((int)(*World).boule.Coo.x>(int)gfx_lcdWidth/2 && (*World).BD.x>gfx_lcdWidth){ // Lorsque mouvement vers la droite
		termes = retourneDoublet(-1,0);
		while((int)(*World).boule.Coo.x+termes.x>(int)gfx_lcdWidth*0.6 && (*World).BD.x+termes.x>gfx_lcdWidth){
			termes.x--;
		}
	}
	if((int)(*World).boule.Coo.y>gfx_lcdHeight/2 && (*World).BD.y>gfx_lcdHeight){ // Lorsque mouvement vers le bas
		termes.y--;
		while((int)(*World).boule.Coo.y+termes.y>(int)gfx_lcdHeight*0.8 && (*World).BD.y+termes.y>gfx_lcdHeight){
			termes.y++;
		}
	}else if((int)(*World).boule.Coo.y<gfx_lcdHeight/2 && (*World).HG.y<0){ // Lorsque mouvement vers le haut
		termes.y++;
		while((int)(*World).boule.Coo.y+termes.y<(int)gfx_lcdHeight*0.2 && (*World).HG.y+termes.y<0){
			termes.y--;
		}
	}
	if(termes.x != 0 || termes.y != 0){
		(*World).HG.x+=termes.x;                   // Décalage point haut-gauche du niveau
		(*World).HG.y+=termes.y;
		(*World).BD.x+=termes.x;                   // Décalage point bas-droit du niveau
		(*World).BD.y+=termes.y;
		(*World).boule.Coo.x+=termes.x;            // Décalage de la min
		(*World).boule.Coo.y+=termes.y;
		(*World).boule.balleSave.bg.x+=termes.x;   // Décalage de la sauvegarde
		(*World).boule.balleSave.bg.y+=termes.y;
	}
}

void initialisation(struct World *World){
	int i, j, X=11, Y=8;

	// Pour menu principal
	(*World).boule.Coo.x				= 75;
	(*World).boule.Coo.y				= 15;
	(*World).BD							= retourneDoublet(11, 8);
	(*World).map 						= (uint8_t**) malloc(X * sizeof(uint8_t*));
		for(i=0; i<X; i++){
		(*World).map[i] = (uint8_t*) malloc(Y * sizeof (uint8_t));
	}
	(*World).HG							= retourneDoublet(0*Ul, 0*Uh);
	(*World).boule.rayon				= Ul/2.0;
	(*World).nbCerceau					= 1;
	

	// Paramétres généraux
	for(i=0; i<X; i++){
		for(j=0; j<Y; j++){
			(*World).map[i][j]			=VIDE;
		}
	}
	(*World).boule.possibiliteSaut		= false;
	(*World).boule.degat				= false;
	(*World).boule.vitesse				= retourneDoublet(0, 0);
	(*World).boule.force				= retourneDoublet(0, 0);
	(*World).boule.theta				= 0;
	(*World).boule.bonus				= 0;
	(*World).boule.collision			= false;
	(*World).score						= 0;
	(*World).nbVie						= 3;
	// Barre bonus
	(*World).barreBonus.taille.hauteur	= 15; // Passer en macros #define 0000
	(*World).barreBonus.taille.largeur	= gfx_lcdWidth;
	(*World).barreBonus.taille.bg		= retourneDoublet(0, 0);
	(*World).barreBonus.dureeMax		= 10; // Passer en macros #define 0000
	(*World).barreBonus.tempsRestant	= 0;
}

bool collisionCercleCercle(int x1, int y1, float d1, int x2, int y2, float d2){
	if(d1+d2>sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))){ // Si somme des diamètres supérieure à la distance entre les 2 centres
		return true;
	}
	return false;
}
bool collisionRectanglevsRectangle(struct Brique box1, struct Brique box2){
	if((box2.bg.x >= box1.bg.x + box1.largeur)   // trop à droite
	 || (box2.bg.x + box2.largeur <= box1.bg.x)  // trop à gauche
	 || (box2.bg.y >= box1.bg.y + box1.hauteur)  // trop en bas
	 || (box2.bg.y + box2.hauteur <= box1.bg.y)){// trop en haut
		return false;
   }else{
		return true;
	}
}
bool collisionPointCercle(int x, int y, struct Balle C){
   int d2 = (x-C.Coo.x)*(x-C.Coo.x) + (y-C.Coo.y)*(y-C.Coo.y);
   if(d2>C.rayon*C.rayon){
		return false;
	}else{
		return true;
	}
}
bool collisionPointRectangle(int curseur_x,int curseur_y, struct Brique box){
	if(curseur_x>=box.bg.x && curseur_x<box.bg.x+box.largeur && curseur_y>=box.bg.y && curseur_y<box.bg.y+box.hauteur){
		return true;
	}else{
		return false;
	}
}
bool projectionSurSegment(int Cx, int Cy, int Ax, int Ay, int Bx, int By){
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
struct Brique getBoxAutourCercle(struct Balle cercle){
	struct Brique carre;
	carre.bg.x = cercle.Coo.x-cercle.rayon;
	carre.bg.y = cercle.Coo.y-cercle.rayon;
	carre.hauteur = 2*cercle.rayon;
	carre.largeur = 2*cercle.rayon;
	return carre;
}
bool collisionCercleRectangle(struct Balle C1, struct Brique box1){
	 bool projvertical;
	 bool projhorizontal;
	
	struct Brique boxCercle = getBoxAutourCercle(C1);  // retourner la bounding box de l'image porteuse, ou calculer la bounding box.
	if(!(collisionRectanglevsRectangle(box1, boxCercle))){
		return false;   // premier test
	}
	if(collisionPointCercle(box1.bg.x, box1.bg.y, C1) || collisionPointCercle(box1.bg.x, box1.bg.y+box1.hauteur, C1) || collisionPointCercle(box1.bg.x+box1.largeur, box1.bg.y, C1) || collisionPointCercle(box1.bg.x+box1.largeur, box1.bg.y+box1.hauteur, C1)){
		return true;   // deuxieme test
	}
	if(collisionPointRectangle(C1.Coo.x, C1.Coo.y, box1)){
		return true;   // troisieme test
	}
	projvertical   = projectionSurSegment(C1.Coo.x, C1.Coo.y, box1.bg.x, box1.bg.y, box1.bg.x, box1.bg.y+box1.hauteur);
	projhorizontal = projectionSurSegment(C1.Coo.x, C1.Coo.y, box1.bg.x, box1.bg.y, box1.bg.x+box1.largeur, box1.bg.y);
	if(projvertical || projhorizontal){
		return true;	// cas E
	}
	return false;		// cas B
}
float distanceCentreCercleDroite(struct Doublet A, struct Doublet B, struct Balle C){
	struct Doublet u;	//# Vecteur u
	struct Doublet AC;	//# Vecteur AC
	float numerateur;	// norme du vecteur v
	float denominateur;	// norme de u

	u.x = B.x - A.x;
	u.y = B.y - A.y;
	AC.x = C.Coo.x - A.x;
	AC.y = C.Coo.y - A.y;
	numerateur = u.x*AC.y - u.y*AC.x;
	if(numerateur <0){
		numerateur = -numerateur;   // valeur absolue ; si c'est négatif, on prend l'opposé.
	}
	denominateur = sqrt(u.x*u.x + u.y*u.y);
	return numerateur / denominateur;;
}
bool collisionDroite(struct Doublet A, struct Doublet B, struct Balle C){
	float CI = distanceCentreCercleDroite(A, B, C);

	if(CI<C.rayon){
		return true;
	}else{
		return false;
	}
}
bool collisionSegment(struct Doublet A, struct Doublet B, struct Balle C){
	struct Doublet AB, AC, BC;	//# Vecteurs
	float pscal1;				// produit scalaire
	float pscal2;				// produit scalaire

	if(!(collisionDroite(A, B, C))){
		return false;  // si on ne touche pas la droite, on ne touchera jamais le segment
	}
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	AC.x = C.Coo.x - A.x;
	AC.y = C.Coo.y - A.y;
	BC.x = C.Coo.x - B.x;
	BC.y = C.Coo.y - B.y;
	pscal1 = AB.x*AC.x + AB.y*AC.y;
	pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;
	if(pscal1>=0 && pscal2>=0){
		return true;   // I entre A et B, ok.
	}
	// dernière possibilité, A ou B dans le cercle
	if(collisionPointCercle(A.x, A.y, C)){
		return true;
	}
	if(collisionPointCercle(B.x, B.y, C)){
		return true;
	}
	return false;
}
bool collisionLosange(struct Doublet A, struct Doublet B, struct Doublet C, struct Doublet D, struct Balle Boule){
		if(collisionSegment(A, B, Boule) ||
		   collisionSegment(B, C, Boule) ||
		   collisionSegment(C, D, Boule) ||
		   collisionSegment(D, A, Boule)){
			return true;
		}else{
			return false;
		}
}
bool libre(uint8_t** map, uint8_t i, uint8_t j){
	if(map[i][j]!=BRIQUE && map[i][j]!=BRIQUE_BLEU && map[i][j]!=PORTE_FERMEE && map[i][j]!=PORTE_OUVERTE){
		return true;
	}
	return false;
}

void deplacement(struct Balle *balle){
	int a = 0;

	if((*balle).bonus==3){
		a+=500;
	}

	if(kb_Data[7] & kb_Left){
		if((*balle).vitesse.x==0){
			(*balle).vitesse.x = -MAX_VITESSE_X/2.0;
		}else if((*balle).vitesse.x>-MAX_VITESSE_X/1.5){
			(*balle).vitesse.x--;
		}else{
			(*balle).vitesse.x = -MAX_VITESSE_X-a;
		}
		(*balle).collision = false;
	}else if(kb_Data[7] & kb_Right){
		if((*balle).vitesse.x==0){
			(*balle).vitesse.x = MAX_VITESSE_X/2.0;
		}else if((*balle).vitesse.x<MAX_VITESSE_X/1.5){
			(*balle).vitesse.x++;
		}else{
			(*balle).vitesse.x = MAX_VITESSE_X-a;
		}
		(*balle).collision = false;
	}else{
		(*balle).vitesse.x=0; // Mise à l'arrêt 
	}
	if(kb_Data[7] & kb_Up){
		if((*balle).bonus==1 || (*balle).possibiliteSaut){
			(*balle).vitesse.y	= -MAX_VITESSE_Y;
			(*balle).force.y	= 0;
			(*balle).collision	= false;
			(*balle).possibiliteSaut = false;
		}
	}
}
void collisionDroite0Deg  (struct Balle *balle, int y, float CoeffFri){
	if((*balle).vitesse.y>-50){
		(*balle).Coo.y+=y-((*balle).Coo.y-(*balle).rayon);
		(*balle).vitesse.y = 0;
	}else{
		(*balle).Coo.y+=2.0*(y-((*balle).Coo.y-(*balle).rayon));
		(*balle).vitesse.y = -(*balle).vitesse.y;
		(*balle).vitesse.x = CoeffFri*(*balle).vitesse.x;
		(*balle).vitesse.y = CoeffFri*(*balle).vitesse.y;
	}
	if(abs((*balle).vitesse.y)<MAX_VITESSE_Y && (*balle).bonus==2){
		if(kb_Data[7] & kb_Down){
			(*balle).force.y = MAX_VITESSE_Y;
			(*balle).vitesse.y = 0;
			(*balle).collision = false;
		}
	}
}
void collisionDroite90Deg (struct Balle *balle, int x, float CoeffFri){
	if(CoeffFri==0){
		(*balle).Coo.x	   = x-(*balle).rayon;
		(*balle).vitesse.x = 0;
	}else{
		(*balle).Coo.x	  -= 2.0*((*balle).Coo.x+(*balle).rayon-x);
		(*balle).vitesse.x = -(CoeffFri*(*balle).vitesse.x);
	}
}
void collisionDroite180Deg(struct Balle *balle, int y, float CoeffFri){
	if((*balle).vitesse.y<LIMITE_REBOND){
		(*balle).Coo.y	  -=((*balle).Coo.y+(*balle).rayon)-y;
		(*balle).vitesse.y = 0;
	}else{
		(*balle).Coo.y	  -= 2.0*((*balle).Coo.y+(*balle).rayon-y);
		(*balle).vitesse.y = -(*balle).vitesse.y;
		(*balle).vitesse.x = CoeffFri*(*balle).vitesse.x;
		(*balle).vitesse.y = CoeffFri*(*balle).vitesse.y;
	}
	if(abs((*balle).vitesse.y)<MAX_VITESSE_Y && (*balle).bonus!=2){
		(*balle).possibiliteSaut = true;
	}
}
void collisionDroite270Deg(struct Balle *balle, int x, float CoeffFri){
	if(CoeffFri==0){
		(*balle).Coo.x	   = x+(*balle).rayon;
		(*balle).vitesse.x = 0;
	}else{
		(*balle).Coo.x	  += 2.0*(x-((*balle).Coo.x-(*balle).rayon));
		(*balle).vitesse.x = CoeffFri*(*balle).vitesse.x;
	}
}
void collisionDroite135Deg(struct Balle *balle, struct Doublet a, struct Doublet b, float CoeffFri){
	float distanceRebond = (*balle).rayon-distanceCentreCercleDroite(a, b, *balle);
	float tamponVitesse = (*balle).vitesse.x;

	if((*balle).vitesse.x>-100){
		CoeffFri = 1;
	}
	(*balle).Coo.x+=2.0*distanceRebond;
	(*balle).Coo.y+=2.0*distanceRebond;
	(*balle).vitesse.x = abs((*balle).vitesse.y);
	(*balle).vitesse.y = abs(tamponVitesse);
	(*balle).vitesse.x = COEFFICIENT_FRICTION_BRIQUE*(*balle).vitesse.x;
	(*balle).vitesse.y = COEFFICIENT_FRICTION_BRIQUE*(*balle).vitesse.y;
	if(abs((*balle).vitesse.y)<500 && (*balle).bonus!=2){
		/*if(isKeyPressed(SDLK_UP)){
			(*balle).force.y = 89000;
			(*balle).vitesse.y = 0;
			(*balle).collision = false;
		}*/
	}
}
void collisionDroite45Deg (struct Balle *balle, struct Doublet a, struct Doublet b, float CoeffFri){
	float distanceRebond = (*balle).rayon-distanceCentreCercleDroite(a, b, *balle);
	float tamponVitesse  = (*balle).vitesse.x;

	if((*balle).vitesse.x<100){
		CoeffFri=1;
	}
	(*balle).Coo.x	  -= 2.0*distanceRebond;
	(*balle).Coo.y	  += 2.0*distanceRebond;
	(*balle).vitesse.x = abs((*balle).vitesse.y);
	(*balle).vitesse.y = abs(tamponVitesse);
	(*balle).vitesse.x = (*balle).vitesse.x*CoeffFri;
	(*balle).vitesse.y = (*balle).vitesse.y*CoeffFri;
	/*if(abs((*balle).vitesse.y)<500 && (*balle).bonus!=2){
		if(isKeyPressed(SDLK_UP)){
			(*balle).force.y = 89000;
			(*balle).vitesse.y = 0;
			(*balle).collision = false;
		}
	}*/
}
void collisionDroite225Deg(struct Balle *balle, struct Doublet a, struct Doublet b, float CoeffFri){
	float distanceRebond = (*balle).rayon-distanceCentreCercleDroite(a, b, *balle);
	float tamponVitesse = (*balle).vitesse.x;

	(*balle).Coo.x+=2.0*distanceRebond;
	(*balle).Coo.y-=2.0*distanceRebond;
	(*balle).vitesse.x = abs((*balle).vitesse.y);
	(*balle).vitesse.y = -abs(tamponVitesse);
	(*balle).vitesse.x = (*balle).vitesse.x*CoeffFri;
	(*balle).vitesse.y = (*balle).vitesse.y*CoeffFri;
}
void collisionDroite315Deg(struct Balle *balle, struct Doublet a, struct Doublet b, float CoeffFri){
	float distanceRebond = (*balle).rayon-distanceCentreCercleDroite(a, b, *balle);
	float tamponVitesse = (*balle).vitesse.x;

	(*balle).Coo.x+=2.0*distanceRebond;
	(*balle).Coo.y-=2.0*distanceRebond;
	(*balle).vitesse.x = -(*balle).vitesse.y;
	(*balle).vitesse.y = -tamponVitesse;
	(*balle).vitesse.x = (*balle).vitesse.x*CoeffFri;
	(*balle).vitesse.y = (*balle).vitesse.y*CoeffFri;
	
}

uint8_t collision(struct World* World){
	int i, j, a;
	uint8_t retour = NON;
	struct Doublet A, B, C, D, hd;
	//struct Balle   testBalle;
	struct Brique  testBrique = {{0, 0}, Uh, Ul};
	bool inhibeDessus;
	//testBalle.rayon			  = Ul;

	(*World).boule.collision = false;
	
	i = (abs((*World).HG.x)+(*World).boule.Coo.x)/Ul;
	j = (abs((*World).HG.y)+(*World).boule.Coo.y)/Uh;
	
	switch((*World).map[i][j]){     // Collision case actuelle
		case PETIT_CERCEAU:
			if(((*World).map[i][j+1]==PETIT_CERCEAU || (*World).map[i][j-1]==PETIT_CERCEAU) && (*World).boule.Coo.x>(i+0.3)*Ul-abs((*World).HG.x) && (*World).boule.Coo.x<(i+0.7)*Ul-abs((*World).HG.x)){ // MARGE 0000
				(*World).nbCerceau--;
				(*World).score+=500;
				(*World).map[i][j]=PETIT_GRIS;
				if((*World).map[i][j+1]==PETIT_CERCEAU){
					(*World).map[i][j+1]=PETIT_GRIS;
				}else{
					(*World).map[i][j-1]=PETIT_GRIS;
				}
			}else if(((*World).map[i+1][j]==PETIT_CERCEAU || (*World).map[i-1][j]==PETIT_CERCEAU) && (*World).boule.Coo.y>(j+0.3)*Ul-abs((*World).HG.y) && (*World).boule.Coo.y<(j+0.7)*Ul-abs((*World).HG.y)){ // MARGE 0000
				(*World).nbCerceau--;
				(*World).score+=500;
				(*World).map[i][j]=PETIT_GRIS;
				if((*World).map[i+1][j]==PETIT_CERCEAU){
					(*World).map[i+1][j]=PETIT_GRIS;
				}else{
					(*World).map[i-1][j]=PETIT_GRIS;
				}
			}
			// Pas de "break;".
		case PETIT_GRIS:
			if((*World).map[i][j-1]==PETIT_CERCEAU || (*World).map[i][j-1]==PETIT_GRIS){
				if((*World).boule.vitesse.x<0 && (*World).boule.vitesse.x>-MAX_VITESSE_X){
					while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x)+1, j*Uh-abs((*World).HG.y)+15, (*World).boule)){
						(*World).boule.Coo.x++;
					}
					retour = GAUCHE;
				}else if((*World).boule.vitesse.x>0 && (*World).boule.vitesse.x<MAX_VITESSE_X){
					while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x)-1, j*Uh-abs((*World).HG.y)+15, (*World).boule)){
						(*World).boule.Coo.x--;
					}
					retour = DROITE;
				}else{
					while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y)+16, (*World).boule)){ // Maintient sur bord intérieur
						(*World).boule.Coo.y--;
						//(*World).boule.vitesse.x*=COEFFICIENT_FRICTION_BRIQUE/3;
						(*World).boule.possibiliteSaut = true;
						(*World).boule.vitesse.y	   = 0;
					}
					if((*World).boule.Coo.x<(i+0.4)*Ul-abs((*World).HG.x)){ // Chute par côté adapté
						(*World).boule.Coo.x-=0.5;
					}else if((*World).boule.Coo.x>(i+0.6)*Ul-abs((*World).HG.x)){
						(*World).boule.Coo.x+=0.5;
					}
				}
				/*if(abs((*World).boule.vitesse.y)<MAX_VITESSE_Y && (*World).boule.bonus!=2){
					if(kb_Data[7] & kb_Up){
						(*World).boule.force.y	 = -MAX_VITESSE_Y; // A mettre dans deplacement() !! 00000
						(*World).boule.vitesse.y = 0;
						(*World).boule.collision = false;
					}
				}*/
			}else if((*World).map[i][j+1]==PETIT_CERCEAU || (*World).map[i][j+1]==PETIT_GRIS){
				while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y)+14, (*World).boule)){ // Maintient sur bord intérieur
					(*World).boule.Coo.y++;
					(*World).boule.vitesse.y=0;
					//(*World).boule.force.y=0; (Bonus 2 ??) 0000
				}
				if((*World).boule.Coo.x<(i+0.4)*Ul-abs((*World).HG.x)){ // Chute par côté adapté
					(*World).boule.Coo.x-=0.5;
				}else if((*World).boule.Coo.x>(i+0.6)*Ul-abs((*World).HG.x)){
					(*World).boule.Coo.x+=0.5;
				}
			}else if((*World).map[i-1][j]==PETIT_CERCEAU || (*World).map[i-1][j]==PETIT_GRIS){
				while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y)+15, (*World).boule)){
					(*World).boule.Coo.x--;
				}
			}
			else if((*World).map[i+1][j]==PETIT_CERCEAU || (*World).map[i+1][j]==PETIT_GRIS){
				while(collisionPointCercle((i+0.5)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y)+15, (*World).boule)){
					(*World).boule.Coo.x++;
				}
			}
			break;
	}
	if((*World).boule.vitesse.y<0){	// Collision cases du dessus
		for(a=i-1; a<i+2; a++){
			switch((*World).map[a][j-1]){
				case PORTE_OUVERTE:
					if(libre((*World).map, a, j)){
						A = retourneDoublet( a   *Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
						B = retourneDoublet((a+1)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre((*World).map, a, j)){
						A = retourneDoublet( a   *Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
						B = retourneDoublet((a+1)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							collisionDroite0Deg(&((*World).boule), j*Uh-abs((*World).HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BRIQUE:
					if(libre((*World).map, a, j)){
						testBrique.bg.x = a    *Ul-abs((*World).HG.x);// -(abs(x)) = +x <=> x<0   00000
						testBrique.bg.y = (j-1)*Uh-abs((*World).HG.y);
						if(collisionCercleRectangle((*World).boule, testBrique)){
							collisionDroite0Deg(&((*World).boule), j*Uh-abs((*World).HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur	= Ul*0.5;
					testBrique.bg.x 	= (a+0.25)*Ul-abs((*World).HG.x);// -(abs(x)) = +x <=> x<0   00000
					testBrique.bg.y		= (j-1)	  *Uh-abs((*World).HG.y);
					if(collisionCercleRectangle((*World).boule, testBrique)){
						return DEGAT;
					}
					break;
				case LOSANGE:
					A = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x),  j  *Uh-abs((*World).HG.y));
					B = retourneDoublet((a+1)  *Ul-abs((*World).HG.x), (j-0.5)*Uh-abs((*World).HG.y));
					C = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x), (j-1)  *Uh-abs((*World).HG.y));
					D = retourneDoublet( a     *Ul-abs((*World).HG.x), (j-0.5)*Uh-abs((*World).HG.y));					
					if(collisionLosange(A, B, C, D, (*World).boule)){
						(*World).score+=500;
						(*World).boule.balleSave.bg 	 = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x), (j-0.5)*Uh-abs((*World).HG.y));
						(*World).boule.balleSave.largeur = (*World).boule.rayon;
						(*World).map[a][j-1]       = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle((*World).boule.Coo.x, (*World).boule.Coo.y, (*World).boule.rayon, (a+0.5)*Ul-abs((*World).HG.x), (j-0.5)*Uh-abs((*World).HG.y), 0.5*Ul)){
						(*World).map[a][j-1]=VIDE;
						(*World).nbVie++;
						(*World).score+=1000;
					}
					break;
				case PETIT_GRIS: 
				case PETIT_CERCEAU:
					A = retourneDoublet((a+0.25)*Ul-abs((*World).HG.x),   j*Uh-abs((*World).HG.y)-7);
					B = retourneDoublet((a+0.25)*Ul-abs((*World).HG.x)+5, j*Uh-abs((*World).HG.y)-7);
					if(collisionSegment(A, B, (*World).boule) && ((*World).map[a][j-2]==PETIT_CERCEAU || (*World).map[a][j-2]==PETIT_GRIS)){
						collisionDroite0Deg(&(*World).boule, j*Uh-abs((*World).HG.y)-7, 1);
					}
					break;
			}
		}
	}else{ 							// Collision cases du dessous
		for(a=i-1; a<i+2; a++){
			switch((*World).map[a][j+1]){
				case PORTE_OUVERTE:
					if(libre((*World).map, a, j)){
						A = retourneDoublet( a   *Ul-abs((*World).HG.x), (j+1)*Uh-abs((*World).HG.y));
						B = retourneDoublet((a+1)*Ul-abs((*World).HG.x), (j+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre((*World).map, a, j)){
						A = retourneDoublet( a   *Ul-abs((*World).HG.x), (j+1)*Uh-abs((*World).HG.y));
						B = retourneDoublet((a+1)*Ul-abs((*World).HG.x), (j+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							collisionDroite180Deg(&((*World).boule), (j+1)*Uh-abs((*World).HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BRIQUE:
					if(libre((*World).map, a, j)){
						testBrique.bg.x = a    *Ul-abs((*World).HG.x);
						testBrique.bg.y = (j+1)*Uh-abs((*World).HG.y);
						if(collisionCercleRectangle((*World).boule, testBrique)){
							collisionDroite180Deg(&((*World).boule), (j+1)*Uh-abs((*World).HG.y), COEFFICIENT_FRICTION_BRIQUE);
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur = Ul*0.5;
					testBrique.bg.x    = (a+0.25)	*Ul-abs((*World).HG.x);
					testBrique.bg.y    = (j+1)		*Uh-abs((*World).HG.y);
					if(collisionCercleRectangle((*World).boule, testBrique)){
						return DEGAT;
					}
					break;
				case LOSANGE:
					A = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x), (j+2)  *Uh-abs((*World).HG.y));
					B = retourneDoublet((a+1)  *Ul-abs((*World).HG.x), (j+1.5)*Uh-abs((*World).HG.y));
					C = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x), (j+1)  *Uh-abs((*World).HG.y));
					D = retourneDoublet( a     *Ul-abs((*World).HG.x), (j+1.5)*Uh-abs((*World).HG.y));					
					if(collisionLosange(A, B, C, D, (*World).boule)){
						(*World).score+=500;
						(*World).boule.balleSave.bg 	 = retourneDoublet((a+0.5)*Ul-abs((*World).HG.x), (j+0.5)*Uh-abs((*World).HG.y));
						(*World).boule.balleSave.largeur = (*World).boule.rayon;
						(*World).map[a][j+1]			 = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle((*World).boule.Coo.x, (*World).boule.Coo.y, (*World).boule.rayon, (a+0.5)*Ul-abs((*World).HG.x), (j+0.5)*Uh-abs((*World).HG.y), 0.5*Ul)){
						(*World).map[a][j+1]= VIDE;
						(*World).nbVie++;
						(*World).score+=1000;
					}
					break;
				case PETIT_GRIS:
				case PETIT_CERCEAU:
					A = retourneDoublet((a+0.25)*Ul-abs((*World).HG.x), (j+1)*Uh-abs((*World).HG.y)+7);
					B = retourneDoublet((a+0.25)*Ul-abs((*World).HG.x)+5, (j+1)*Uh-abs((*World).HG.y)+7);
					if(collisionSegment(A, B, (*World).boule) && ((*World).map[a][j+2]==PETIT_CERCEAU || (*World).map[a][j+2]==PETIT_GRIS)){
						collisionDroite180Deg(&(*World).boule, (j+1)*Uh-abs((*World).HG.y)+7, 1);
					}
					break;
			}
		}
	}
	if((*World).boule.vitesse.x<0){	// Collision cases à gauche
		for(a=j-1; a<j+2; a++){
			switch((*World).map[i-1][a]){
				case PORTE_OUVERTE:
					if(libre((*World).map, i, a)){
						A = retourneDoublet(i*Ul-abs((*World).HG.x),  a   *Uh-abs((*World).HG.y));
						B = retourneDoublet(i*Ul-abs((*World).HG.x), (a+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre((*World).map, i, a)){
						A = retourneDoublet(i*Ul-abs((*World).HG.x),  a   *Uh-abs((*World).HG.y));
						B = retourneDoublet(i*Ul-abs((*World).HG.x), (a+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							collisionDroite270Deg(&((*World).boule), i*Ul-abs((*World).HG.x), 0);
							return GAUCHE;
						}
					}
					break;
				case BRIQUE:
					if(libre((*World).map, i, a)){
						testBrique.largeur = Ul;
						testBrique.bg.x    = (i-1)*Ul-abs((*World).HG.x);
						testBrique.bg.y    = a    *Uh-abs((*World).HG.y);
						if(collisionCercleRectangle((*World).boule, testBrique)){
							collisionDroite270Deg(&((*World).boule), i*Ul-abs((*World).HG.x), 0);
							return GAUCHE;
						}
					}
					break;
				case BARRE_JAUNE:
					if((*World).map[i][a]!=BRIQUE){
						testBrique.largeur = 0.5*Ul;
						testBrique.bg.x    = (i-0.75)*Ul-abs((*World).HG.x);
						testBrique.bg.y    = a       *Uh-abs((*World).HG.y);
						if(collisionCercleRectangle((*World).boule, testBrique)){
							return DEGAT;
						}
					}
					break;
				case LOSANGE:
					A = retourneDoublet((i-0.5)*Ul-abs((*World).HG.x), (a+1)  *Uh-abs((*World).HG.y));
					B = retourneDoublet( i     *Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));
					C = retourneDoublet((i-0.5)*Ul-abs((*World).HG.x),  a     *Uh-abs((*World).HG.y));
					D = retourneDoublet((i-1)  *Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));					
					if(collisionLosange(A, B, C, D, (*World).boule)){
						(*World).score+=500;
						(*World).boule.balleSave.bg 	   = retourneDoublet((i-0.5)*Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));
						(*World).boule.balleSave.largeur = (*World).boule.rayon;
						(*World).map[i-1][a]	   = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle((*World).boule.Coo.x, (*World).boule.Coo.y, (*World).boule.rayon, (i-0.5)*Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y), 0.5*Ul)){
						(*World).map[i-1][a]= VIDE;
						(*World).nbVie++;
						(*World).score+=1000;
					}
					break;
			}			
		}
	}else{ 							// Collision cases à droite
		for(a=j-1; a<j+2; a++){
			switch((*World).map[i+1][a]){
				case PORTE_OUVERTE:
					if(libre((*World).map, i, a)){
						A = retourneDoublet((i+1)*Ul-abs((*World).HG.x),  a   *Uh-abs((*World).HG.y));
						B = retourneDoublet((i+1)*Ul-abs((*World).HG.x), (a+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							return SORTIE;
						}
					}
					break;
				case PORTE_FERMEE:
					if(libre((*World).map, i, a)){
						A = retourneDoublet((i+1)*Ul-abs((*World).HG.x),  a   *Uh-abs((*World).HG.y));
						B = retourneDoublet((i+1)*Ul-abs((*World).HG.x), (a+1)*Uh-abs((*World).HG.y));
						if(collisionSegment(A, B, (*World).boule)){
							collisionDroite90Deg(&((*World).boule), (i+1)*Ul-abs((*World).HG.x), 0);
							return DROITE;
						}
					}
					break;
				case BRIQUE:
					if(libre((*World).map, i, a)){
						testBrique.largeur = Ul;
						testBrique.bg.x    = (i+1)*Ul-abs((*World).HG.x);
						testBrique.bg.y    = a    *Uh-abs((*World).HG.y);
						if(collisionCercleRectangle((*World).boule, testBrique)){
							collisionDroite90Deg(&((*World).boule), (i+1)*Ul-abs((*World).HG.x), 0);
							return DROITE;
						}
					}
					break;
				case BARRE_JAUNE:
					testBrique.largeur = Ul*0.5;
					testBrique.bg.x    = (i+1.25) *Ul-abs((*World).HG.x);
					testBrique.bg.y    = a		  *Uh-abs((*World).HG.y);
					if(collisionCercleRectangle((*World).boule, testBrique)){
						return DEGAT;
					}
					break;
				case LOSANGE:
					A = retourneDoublet((i+1.5)*Ul-abs((*World).HG.x), (a+1)  *Uh-abs((*World).HG.y));
					B = retourneDoublet((i+2)  *Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));
					C = retourneDoublet((i+1.5)*Ul-abs((*World).HG.x),  a     *Uh-abs((*World).HG.y));
					D = retourneDoublet((i+1)  *Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));					
					if(collisionLosange(A, B, C, D, (*World).boule)){
						(*World).score+=500;
						(*World).boule.balleSave.bg 	   = retourneDoublet((i+0.5)*Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y));
						(*World).boule.balleSave.largeur = (*World).boule.rayon;
						(*World).map[i+1][a]	   = FLECHE;
					}
					break;
				case BULLE:
					if(collisionCercleCercle((*World).boule.Coo.x, (*World).boule.Coo.y, (*World).boule.rayon, (i+1.5)*Ul-abs((*World).HG.x), (a+0.5)*Uh-abs((*World).HG.y), 0.5*Ul)){
						(*World).map[i+1][a]= VIDE;
						(*World).nbVie++;
						(*World).score+=1000;
					}
					break;
				/*case PETIT_CERCEAU:
					if(map[i+1][a-1]==PETIT_CERCEAU){
						A = retourneDoublet((i+1)*Ul-abs((*World).HG.x),   a*Uh-abs((*World).HG.y));
						B = retourneDoublet((i+1.5)*Ul-abs((*World).HG.x), a*Uh-abs((*World).HG.y)+15);
						while(collisionSegment(A, B, (*World).boule)){
							(*World).boule.Coo.y--;
							//collisionDroite45Deg(&(*World).boule, A, B, 1);
							//while(true);
						}
					}
					break;*/
			}
		}
	}
	/*for(i=0; i<MAXBRIQUEBLEU; i++){ // Collision avec briques bleues
		if(collisionCercleRectangle((*World).boule, (*World).briqueBleu[i]) && (*World).boule.collision==false){
			rebond(&((*World).boule), (*World).briqueBleu[i], COEFFICIENT_FRICTION_BRIQUE*2.0/*, true);
			(*World).boule.collision = true;
			return false;
		}
	}
	for(i=0; i<MAXBONUS1; i++){     // Collision avec bonus 1
		if(collisionCercleRectangle((*World).boule, (*World).bonus1[i].hitbox) && (*World).boule.collision==false){
			rebond(&((*World).boule), (*World).bonus1[i].hitbox, COEFFICIENT_FRICTION_BRIQUE);
			(*World).boule.bonus = 1;
			(*World).barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXBONUS2; i++){     // Collision avec bonus 2
		if(collisionCercleRectangle((*World).boule, (*World).bonus2[i]) && (*World).boule.collision==false){
			rebond(&(*World).boule, (*World).bonus2[i], COEFFICIENT_FRICTION_BRIQUE);
			(*World).boule.collision = true;
			(*World).boule.bonus = 2;
			(*World).barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXBONUS3; i++){     // Collision avec bonus 3
		if(collisionCercleRectangle((*World).boule, (*World).bonus3[i]) && (*World).boule.collision==false){
			rebond(&(*World).boule, (*World).bonus3[i], COEFFICIENT_FRICTION_BRIQUE);
			(*World).boule.bonus = 3;
			(*World).barreBonus.tempsRestant = 10;
			return false;
		}
	}
	for(i=0; i<MAXPOMPE; i++){      // Collision avec pompe
		if(collisionCercleRectangle((*World).boule, (*World).pompes[i]) && (*World).boule.collision==false){
			rebond(&(*World).boule, (*World).pompes[i], COEFFICIENT_FRICTION_BRIQUE);
			(*World).boule.rayon = Ul/2+Ul/6;
			(*World).boule.collision = true;
			i = MAXPOMPE;
		}
	}
	for(i=0; i<MAXDEGONFLEUR; i++){ // Collision avec degonfleur
		if(collisionCercleRectangle((*World).boule, (*World).degonfleurs[i].hitbox) && (*World).boule.collision==false){
			rebond(&(*World).boule, (*World).degonfleurs[i].hitbox, COEFFICIENT_FRICTION_BRIQUE);
			(*World).boule.rayon = Ul/2.0;
			(*World).boule.collision = true;
			return false;
		}
	}
	for(i=0; i<MAXBULLE; i++){      // Collision avec bulles
		if((*World).bulles[i].etat){
			if(collisionCercleCercle((*World).boule.Coo.x, (*World).boule.Coo.y, (*World).boule.rayon, (*World).bulles[i].cadre.bg.x+(*World).bulles[i].cadre.largeur/2.0, (*World).bulles[i].cadre.bg.y+(*World).bulles[i].cadre.hauteur/2.0, (*World).bulles[i].cadre.largeur*0.45)){
				(*World).bulles[i].etat = false;
				(*World).nbVie++;
				(*World).score+=1000;			
			}
		}
	}
	for(i=0; i<MAXSAUVEGARDE; i++){ // Collision avec losange de sauvegarde
		if((*World).sauvegardes[i].etat==false){
			A = retourneDoublet((*World).sauvegardes[i].cadre.bg.x, (*World).sauvegardes[i].cadre.bg.y+(*World).sauvegardes[i].cadre.hauteur/(float)2.0);
			B = retourneDoublet((*World).sauvegardes[i].cadre.bg.x+(*World).sauvegardes[i].cadre.largeur/(float)2.0, (*World).sauvegardes[i].cadre.bg.y+(*World).sauvegardes[i].cadre.hauteur);
			C = retourneDoublet((*World).sauvegardes[i].cadre.bg.x+(*World).sauvegardes[i].cadre.largeur, (*World).sauvegardes[i].cadre.bg.y+(*World).sauvegardes[i].cadre.hauteur/(float)2.0);
			D = retourneDoublet((*World).sauvegardes[i].cadre.bg.x+(*World).sauvegardes[i].cadre.largeur/(float)2.0, (*World).sauvegardes[i].cadre.bg.y);
			if(collisionLosange(A, B, C, D, (*World).boule)){
				(*World).score+=500;
				(*World).sauvegardes[i].etat = true;
				(*World).balleSave.bg = retourneDoublet((*World).sauvegardes[i].cadre.bg.x+(*World).sauvegardes[i].cadre.largeur/(float)2.0, (*World).sauvegardes[i].cadre.bg.y+(*World).sauvegardes[i].cadre.hauteur/(float)2.0);
				(*World).balleSave.largeur = (*World).boule.rayon;
			}
		}
	}
	for(i=0; i<MAXROND; i++){       // Collision avec le bas des ronds
		if(collisionCercleRectangle((*World).boule, (*World).cercle[i].bord[0])){
			rebond(&(*World).boule, (*World).cercle[i].bord[0], 0.05);
			(*World).boule.collision = true;
			return false;
		}
	}
	for(i=0; i<MAXROND; i++){       // Collision avec le haut des ronds
		if(collisionCercleRectangle((*World).boule, (*World).cercle[i].bord[1])){
			rebond(&(*World).boule, (*World).cercle[i].bord[1], 0.05);
			(*World).boule.collision = true;
			return false;
		}
	}
	for(i=0; i<MAXENNEMI; i++){     // Collision avec ennemis
		if(collisionCercleRectangle((*World).boule, (*World).ennemis[i].cadre) && (*World).boule.collision==false){
			return DEGAT;
		}
	}
	for(i=0; i<MAXTRIANGLE; i++){   // Collision avec briques triangulaires
		if((*World).briqueAngle[i].triangle[0].x==(*World).briqueAngle[i].triangle[2].x && (*World).briqueAngle[i].triangle[1].x<(*World).briqueAngle[i].triangle[0].x){       // Brique de la forme |\.
			if(CollisionSegment((*World).briqueAngle[i].bloc.bg, (*World).briqueAngle[i].triangle[2], (*World).boule)){     // Rebond contre dessous
				collisionDroite180Deg(&(*World).boule, (*World).briqueAngle[i].bloc.bg.y, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}
			/*else if(CollisionSegment((*World).briqueAngle[i].bloc.bg, (*World).briqueAngle[i].triangle[1], (*World).boule)){   // Rebond contre gauche (Inutile dans ce niveau)
				collisionDroite90Deg((*World).boule, (*World).briqueAngle[i].bloc.bg.x, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}
			else if(CollisionSegment((*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], (*World).boule)){ // Rebond en biais
				collisionDroite135Deg(&(*World).boule, (*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}
		}else if((*World).briqueAngle[i].triangle[0].x==(*World).briqueAngle[i].triangle[2].x && (*World).briqueAngle[i].triangle[1].x>(*World).briqueAngle[i].triangle[0].x){ // Brique de la forme /|
			/*Doublet bd = retourneDoublet((*World).briqueAngle[i].bloc.bg.x+(*World).briqueAngle[i].bloc.largeur, (*World).briqueAngle[i].bloc.bg.y);
			if(CollisionSegment((*World).briqueAngle[i].triangle[2], bd, (*World).boule)){                               // Rebond contre dessous (Inutile dans ce niveau)
				collisionDroite180Deg((*World).boule, bd.y, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else if(CollisionSegment(bd, (*World).briqueAngle[i].triangle[1], (*World).boule)){                         // Rebond contre droite (Inutile dans ce niveau)
				collisionDroite270Deg((*World).boule, bd.x, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else 
			if(CollisionSegment((*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], (*World).boule)){ // Rebond en biais
				collisionDroite45Deg(&(*World).boule, (*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}
		}else if((*World).briqueAngle[i].triangle[0].x==(*World).briqueAngle[i].triangle[1].x && (*World).briqueAngle[i].triangle[0].x>(*World).briqueAngle[i].triangle[2].x){ // Brique de la forme |/
			/*Doublet hg = retourneDoublet((*World).briqueAngle[i].bloc.bg.x, (*World).briqueAngle[i].bloc.bg.y+(*World).briqueAngle[i].bloc.hauteur);
			if(CollisionSegment((*World).briqueAngle[i].triangle[2], hg, (*World).boule)){                               // Rebond contre gauche (Inutile dans ce niveau)
				collisionDroite90Deg(&(*World).boule, hg.x, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else if(CollisionSegment((*World).briqueAngle[i].triangle[1], hg, (*World).boule)){                         // Rebond contre haut (Inutile dans ce niveau)
				collisionDroite0Deg(&(*World).boule, hg.y, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else 
			if(CollisionSegment((*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], (*World).boule)){ // Rebond en biais
				collisionDroite225Deg(&(*World).boule, (*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}
		}else if((*World).briqueAngle[i].triangle[0].x==(*World).briqueAngle[i].triangle[1].x && (*World).briqueAngle[i].triangle[0].x<(*World).briqueAngle[i].triangle[2].x){ // Brique de la forme \|
			hd = retourneDoublet((*World).briqueAngle[i].bloc.bg.x+(*World).briqueAngle[i].bloc.largeur, (*World).briqueAngle[i].bloc.bg.y+(*World).briqueAngle[i].bloc.hauteur);
			if(CollisionSegment((*World).briqueAngle[i].triangle[1], hd, (*World).boule)){                                     // Rebond contre haut (Inutile dans ce niveau)
				collisionDroite0Deg(&(*World).boule, hd.y, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else if(CollisionSegment(hd, (*World).briqueAngle[i].triangle[2], (*World).boule)){                               // Rebond contre droite (Inutile dans ce niveau)
				collisionDroite270Deg(&(*World).boule, hd.x, COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}else if(CollisionSegment((*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], (*World).boule)){ // Rebond en biais
				collisionDroite315Deg(&(*World).boule, (*World).briqueAngle[i].triangle[1], (*World).briqueAngle[i].triangle[2], COEFFICIENT_FRICTION_BRIQUE);
				(*World).boule.collision = true;
				return false;
			}		
		}
	}
	for(i=0; i<MAXEAU; i++){        // Collision avec eau
		if(collisionCercleRectangle((*World).boule, (*World).mer[i])){
			float a=pousseeArchimede((*World).boule,  (*World).mer[i].bg.y+(*World).mer[i].hauteur);
			if((*World).boule.vitesse.y<-100){
				(*World).boule.vitesse.y*=0.97;
			}else if((*World).boule.vitesse.y>50){
				(*World).boule.vitesse.y*=0.8;
			}
			(*World).boule.vitesse.x=(*World).boule.vitesse.x*0.97;
			if((*World).boule.rayon==Ul/2.0){
				a/=60.0;
			}else{
				a/=15.0;
			}
			(*World).boule.force.y-=a;
			i = MAXEAU;
		}
	}*/
	return retour;
}
/*void patternEnnemis(struct Ennemi *ennemi){
	float vitesse=0.3;

	if((*ennemi).axe){	// Déplacement vertical
		if((*ennemi).cadre.bg.y>(*ennemi).max){
			(*ennemi).direction=0;
			(*ennemi).cadre.bg.y-=vitesse;
		}else if((*ennemi).cadre.bg.y<(*ennemi).min){
			(*ennemi).direction=1;
			(*ennemi).cadre.bg.y+=vitesse;
		}else if((*ennemi).direction==1){
			(*ennemi).cadre.bg.y+=vitesse;
		}else{
			(*ennemi).cadre.bg.y-=vitesse;
		}
	}else{				// Déplacement horizontal
		if((*ennemi).cadre.bg.x+(*ennemi).cadre.largeur>(*ennemi).max){
			(*ennemi).direction=0;
			(*ennemi).cadre.bg.x-=vitesse;
		}else if((*ennemi).cadre.bg.x<(*ennemi).min){
			(*ennemi).direction=1;
			(*ennemi).cadre.bg.x+=vitesse;
		}else if((*ennemi).direction==1){
			(*ennemi).cadre.bg.x+=vitesse;
		}else{
			(*ennemi).cadre.bg.x-=vitesse;
		}
	}
}*/
bool update_balle(struct World* World, double deltaTemps){
	int8_t	b=1;
	uint8_t	typeCollision;
	deltaTemps/=3276.0;

	if((*World).boule.bonus==2){
		b=-1;
	}
	if(abs((*World).boule.force.y)!= MAX_VITESSE_Y){ // Application de la gravité si pas de départ de saut.
		(*World).boule.force.y		 = G.y * b;
	}
	if((*World).boule.degat){ // Remise de la balle au point de sauvegarde suite au "pop".
		(*World).boule.Coo			 = (*World).boule.balleSave.bg;
	}
	(*World).boule.degat				 = false;
	(*World).boule.possibiliteSaut	 = false;

	//(*World).boule.vitesse.x 	   = (float)deltaTemps*(*World).boule.force.x; // Newton 
	(*World).boule.vitesse.y			+= (float)deltaTemps*(*World).boule.force.y; //  "
	(*World).boule.Coo.x 				+= (float)deltaTemps*(*World).boule.vitesse.x;
	(*World).boule.Coo.y				+= (float)deltaTemps*(*World).boule.vitesse.y;
	typeCollision						 = collision(World);
	
	//(*World).boule.force.x		   = 0;
	(*World).boule.force.y			 	 = 0;

	if(typeCollision==SORTIE){
		return true;
	}else if(typeCollision==DEGAT){// Collision malus	
		(*World).boule.degat			 = true;
		(*World).boule.vitesse		 	 = retourneDoublet(0, 0);
		(*World).boule.force			 = retourneDoublet(0, 0);
		(*World).boule.bonus			 = 0;
		(*World).boule.rayon			 = (*World).boule.balleSave.largeur;
	}else{
		if(typeCollision==GAUCHE){
			(*World).boule.theta		-=10;
		}else if(typeCollision==DROITE){
			(*World).boule.theta		+=10;
		}else{
			(*World).boule.theta		+=asin((deltaTemps*(*World).boule.vitesse.x)/(*World).boule.rayon)*40.743*b; // *180/M_PI/360*256
		}
		while((*World).boule.theta>255){ // Utilitée pour éviter overflow ?? 0000
			(*World).boule.theta		-=255;
		}
	}
	

	return false;
}

#include "gfx/sprite.h"
gfx_UninitedSprite(rotate_balle, balle_width, balle_height);
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
	if((*World).nbVie==-1){
		gfx_PrintString("00");
	}else{
		gfx_PrintInt((*World).nbVie, 2);
	}

	// Affichage cerceaux
	for(i=0; i<(*World).nbCerceau; i++){
		gfx_TransparentSprite_NoClip(Petitcerceau, 48+i*7, 3);
	}
	
	// Affichage score
	gfx_SetTextXY(260, 7);
	gfx_PrintInt((*World).score, 7);
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
	const uint8_t X_ORIGINE=abs((*World).HG.x)/Ul, Y_ORIGINE=abs((*World).HG.y)/Uh;
	uint8_t  i, j, c, d;
	bool cerceau=false;
	
	gfx_FillScreen(0);

	for(i=X_ORIGINE; i<(X_ORIGINE+gfx_lcdWidth/Ul)+2; i++){          // Affichage briques
		for(j=Y_ORIGINE; j<(Y_ORIGINE+gfx_lcdHeight/Uh); j++){
			switch((*World).map[i][j]){
				case BRIQUE:
					gfx_Sprite(brique, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					break;
				case BARRE_JAUNE:
					gfx_Sprite(barreJaune, (i+0.25)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					break;
				case LOSANGE:
					gfx_Sprite(losange, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					break;
				case FLECHE:
					gfx_Sprite(fleche, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					break;
				case BULLE:
					gfx_Sprite(bulle, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					break;
				case PORTE_FERMEE:
					if((*World).map[i][j+1]==PORTE_FERMEE && (*World).map[i+1][j]==PORTE_FERMEE){
						gfx_Sprite(portailFerme, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					}else if((*World).map[i][j+1]==PORTE_FERMEE && (*World).map[i-1][j]==PORTE_FERMEE && (i-1)*Ul-abs((*World).HG.x)<0){ // Patch affichage si cases gauches de la porte hors fenêtre
						gfx_Sprite(portailFerme, (i-1)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					}
					break;
				case PORTE_OUVERTE:
					if((*World).map[i][j+1]==PORTE_OUVERTE && (*World).map[i+1][j]==PORTE_OUVERTE){
						gfx_Sprite(portailOuvert, i*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					}else if((*World).map[i][j+1]==PORTE_OUVERTE && (*World).map[i-1][j]==PORTE_OUVERTE && (i-1)*Ul-abs((*World).HG.x)<0){ // idem
						gfx_Sprite(portailFerme, (i-1)*Ul-abs((*World).HG.x), j*Uh-abs((*World).HG.y));
					}
					break;
				case PETIT_CERCEAU:
					cerceau = true;
					if((*World).map[i][j+1]==PETIT_CERCEAU){
						gfx_Sprite(DroitJaunePetit,  i    *Ul-abs((*World).HG.x)+15, j*Uh-abs((*World).HG.y));
					}else if((*World).map[i+1][j]==PETIT_CERCEAU){
						gfx_Sprite(DroitJaunePetitHor,  i*Ul-abs((*World).HG.x),   j*Uh-abs((*World).HG.y)+5);
					}
					break;
				case PETIT_GRIS:
					cerceau = true;
					if((*World).map[i][j+1]==PETIT_GRIS){
						gfx_Sprite(DroitGrisPetit,  i*Ul-abs((*World).HG.x)+15, j*Uh-abs((*World).HG.y));
					}else if((*World).map[i+1][j]==PETIT_GRIS){
						gfx_Sprite(DroitGrisPetitHor,  i*Ul-abs((*World).HG.x),   j   *Uh-abs((*World).HG.y)+5);
					}
					break;
			}
		}
	}

	if(!(*World).boule.degat){  // Affichage balle
		gfx_RotateSprite(balle, rotate_balle, (int)(*World).boule.theta);
		gfx_TransparentSprite_NoClip(rotate_balle, (*World).boule.Coo.x-(*World).boule.rayon, (*World).boule.Coo.y-(*World).boule.rayon);
	}else{
		gfx_Sprite(pop, (*World).boule.Coo.x-(*World).boule.rayon, (*World).boule.Coo.y-(*World).boule.rayon);
	}

	if(cerceau){
		for(i=X_ORIGINE; i<(X_ORIGINE+gfx_lcdWidth/Ul)+2; i++){ 
			for(j=Y_ORIGINE; j<(Y_ORIGINE+gfx_lcdHeight/Uh); j++){
				if((*World).map[i][j]==PETIT_CERCEAU){
					if((*World).map[i][j+1]==PETIT_CERCEAU){
						gfx_TransparentSprite(GaucheJaunePetit,  i*Ul-abs((*World).HG.x)+5, j*Uh-abs((*World).HG.y));
					}else if((*World).map[i+1][j]==PETIT_CERCEAU){
						gfx_TransparentSprite(GaucheJaunePetitHor,  i*Ul-abs((*World).HG.x),   j*Uh-abs((*World).HG.y)+15);
					}
				}else if((*World).map[i][j]==PETIT_GRIS){
					if((*World).map[i][j+1]==PETIT_GRIS){
						gfx_TransparentSprite(GaucheGrisPetit,  i*Ul-abs((*World).HG.x)+5, j*Uh-abs((*World).HG.y));
					}else if((*World).map[i+1][j]==PETIT_GRIS){
						gfx_TransparentSprite(GaucheGrisPetitHor,  i*Ul-abs((*World).HG.x),   j*Uh-abs((*World).HG.y)+15);
					}
				}
			}
		}
	}
}

void main(void){
	uint8_t		 retour_menu		= 1;	// Retour selection du menu principal.
	uint8_t 	 retour_pause;				// Retour du menu de pause.
	uint8_t		 i 					= 0; 	// Delta de World.nbVie.
	uint8_t 	 j;							// Itération affichage choix du menu principal.
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

	// MENU PRINCIPAL
	DEBUT:
	initialisation(&World);
	placageObjet(11, 1, 0, 7, BRIQUE, World.map);
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
	placageObjet(11, 1, 0, 7, VIDE, World.map);

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
				World.nbVie--;
				World.barreBonus.tempsRestant	= 0;
				delay(500);
				timer_1_Counter+=ONE_SECOND/2.0;
				i--;
			}
			deplacement(&World.boule);
			if(update_balle(&World, ONE_SECOND-timer_1_Counter)){
				goto FIN;
			}
			if(World.nbCerceau==0){
				World.map[(short unsigned int)World.CooPorte.x]  [(short unsigned int)World.CooPorte.y]   = PORTE_OUVERTE;
				World.map[(short unsigned int)World.CooPorte.x]  [(short unsigned int)World.CooPorte.y+1] = PORTE_OUVERTE;
				World.map[(short unsigned int)World.CooPorte.x+1][(short unsigned int)World.CooPorte.y]   = PORTE_OUVERTE;
				World.map[(short unsigned int)World.CooPorte.x+1][(short unsigned int)World.CooPorte.y+1] = PORTE_OUVERTE;
				World.nbCerceau--;
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
