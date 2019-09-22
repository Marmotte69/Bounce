// convpng v7.2
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __sprite__
#define __sprite__
#include <stdint.h>

#define sprite_transparent_color_index 0

#define brique_width 30
#define brique_height 30
#define brique_size 902
extern uint8_t brique_data[902];
#define brique ((gfx_sprite_t*)brique_data)
#define briqueBleu_width 30
#define briqueBleu_height 30
#define briqueBleu_size 902
extern uint8_t briqueBleu_data[902];
#define briqueBleu ((gfx_sprite_t*)briqueBleu_data)
#define bulle_width 30
#define bulle_height 30
#define bulle_size 902
extern uint8_t bulle_data[902];
#define bulle ((gfx_sprite_t*)bulle_data)
#define fleche_width 30
#define fleche_height 30
#define fleche_size 902
extern uint8_t fleche_data[902];
#define fleche ((gfx_sprite_t*)fleche_data)
#define losange_width 30
#define losange_height 30
#define losange_size 902
extern uint8_t losange_data[902];
#define losange ((gfx_sprite_t*)losange_data)
#define pompe_width 12
#define pompe_height 30
#define pompe_size 362
extern uint8_t pompe_data[362];
#define pompe ((gfx_sprite_t*)pompe_data)
#define ennemi_width 30
#define ennemi_height 30
#define ennemi_size 902
extern uint8_t ennemi_data[902];
#define ennemi ((gfx_sprite_t*)ennemi_data)
#define degonfleur_width 30
#define degonfleur_height 30
#define degonfleur_size 902
extern uint8_t degonfleur_data[902];
#define degonfleur ((gfx_sprite_t*)degonfleur_data)
#define GaucheJaunePetit_width 10
#define GaucheJaunePetit_height 60
#define GaucheJaunePetit_size 602
extern uint8_t GaucheJaunePetit_data[602];
#define GaucheJaunePetit ((gfx_sprite_t*)GaucheJaunePetit_data)
#define GaucheJauneGrand_width 10
#define GaucheJauneGrand_height 60
#define GaucheJauneGrand_size 602
extern uint8_t GaucheJauneGrand_data[602];
#define GaucheJauneGrand ((gfx_sprite_t*)GaucheJauneGrand_data)
#define DroitJauneGrand_width 10
#define DroitJauneGrand_height 60
#define DroitJauneGrand_size 602
extern uint8_t DroitJauneGrand_data[602];
#define DroitJauneGrand ((gfx_sprite_t*)DroitJauneGrand_data)
#define DroitJaunePetit_width 10
#define DroitJaunePetit_height 60
#define DroitJaunePetit_size 602
extern uint8_t DroitJaunePetit_data[602];
#define DroitJaunePetit ((gfx_sprite_t*)DroitJaunePetit_data)
#define GaucheGrisPetit_width 10
#define GaucheGrisPetit_height 60
#define GaucheGrisPetit_size 602
extern uint8_t GaucheGrisPetit_data[602];
#define GaucheGrisPetit ((gfx_sprite_t*)GaucheGrisPetit_data)
#define GaucheGrisGrand_width 10
#define GaucheGrisGrand_height 60
#define GaucheGrisGrand_size 602
extern uint8_t GaucheGrisGrand_data[602];
#define GaucheGrisGrand ((gfx_sprite_t*)GaucheGrisGrand_data)
#define DroitGrisGrand_width 10
#define DroitGrisGrand_height 60
#define DroitGrisGrand_size 602
extern uint8_t DroitGrisGrand_data[602];
#define DroitGrisGrand ((gfx_sprite_t*)DroitGrisGrand_data)
#define DroitGrisPetit_width 10
#define DroitGrisPetit_height 60
#define DroitGrisPetit_size 602
extern uint8_t DroitGrisPetit_data[602];
#define DroitGrisPetit ((gfx_sprite_t*)DroitGrisPetit_data)
#define Petitcerceau_width 5
#define Petitcerceau_height 16
#define Petitcerceau_size 82
extern uint8_t Petitcerceau_data[82];
#define Petitcerceau ((gfx_sprite_t*)Petitcerceau_data)
#define balle_width 30
#define balle_height 30
#define balle_size 902
extern uint8_t balle_data[902];
#define balle ((gfx_sprite_t*)balle_data)
#define bonus3_width 30
#define bonus3_height 30
#define bonus3_size 902
extern uint8_t bonus3_data[902];
#define bonus3 ((gfx_sprite_t*)bonus3_data)
#define bonus2_width 30
#define bonus2_height 30
#define bonus2_size 902
extern uint8_t bonus2_data[902];
#define bonus2 ((gfx_sprite_t*)bonus2_data)
#define bonus1_width 30
#define bonus1_height 30
#define bonus1_size 902
extern uint8_t bonus1_data[902];
#define bonus1 ((gfx_sprite_t*)bonus1_data)
#define barreJaune_width 15
#define barreJaune_height 30
#define barreJaune_size 452
extern uint8_t barreJaune_data[452];
#define barreJaune ((gfx_sprite_t*)barreJaune_data)
#define pop_width 30
#define pop_height 30
#define pop_size 902
extern uint8_t pop_data[902];
#define pop ((gfx_sprite_t*)pop_data)
#define portailFerme_width 60
#define portailFerme_height 60
#define portailFerme_size 3602
extern uint8_t portailFerme_data[3602];
#define portailFerme ((gfx_sprite_t*)portailFerme_data)
#define portailOuvert_width 60
#define portailOuvert_height 60
#define portailOuvert_size 3602
extern uint8_t portailOuvert_data[3602];
#define portailOuvert ((gfx_sprite_t*)portailOuvert_data)
#define sizeof_sprite_pal 512
extern uint16_t sprite_pal[256];

#endif
