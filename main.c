#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define NB_LIGNES_JOUEURS 5
#define NB_COLONNES_JOUEURS 16

#define NB_LIGNES_HEROS 5
#define NB_COLONNES_HEROS 7

#define NB_LIGNES_UNITES 12
#define NB_COLONNES_UNITES 8

#define NB_LIGNES_HEROSJOUEURS 25
#define NB_COLONNES_HEROSJOUEURS 11

#define NB_LIGNES_UNITESJOUEURS 60
#define NB_COLONNES_UNITESJOUEURS 11

#define NB_LIGNES_CARTE 10
#define NB_COLONNES_CARTE 10

#define NB_EMPLACEMENT 6
#define NB_ESPACES_CENTRE 46
#define WIDTH_C_CASE_JOUEUR 40
#define WIDTH_C_VIDE 14
#define WIDTH_NB_C_C_J 5
#define WIDTH_MENU 39

#define TAILLE_BLOC 40


#define NB_LIGNES_UNITESBATAILLE 12
#define NB_COLONNES_UNITESBATAILLE 13

#define NB_LIGNES_PLATEAUBATAILLE 12
#define NB_COLONNES_PLATEAUBATAILLE 12

#define JOUEUR_HUMAIN 0
#define LARGEUR_ECRAN_EN_CARACTERES 151
#define NB_EMPLACEMENTS_UNITES 6



typedef enum {NUM_JOUEUR_J, LIGNE_J, COLONNE_J, OR, BOIS, PIERRE, FER,
SOUFFRE,SULFURE, GEMME_ROUGE, GEMME_BLEU, GEMME_JAUNE, NB_UNITES_TUEES,
NB_BATAILLE_GAGNES, NB_UNITES_PERDUES, NB_BATAILLE_PERDUES} E_JOUEURS;

typedef enum {NUM_HERO_H, BONUS_ATTAQUE_H, BONUS_DEFENSE_H,
CHANCE_CRITIQUE_H, MORAL_H, POINTS_ATTAQUE_SPECIALE_H, PRIX_H} E_HEROS;

typedef enum {NUM_UNITE_U, TYPE_U, SANTE_U, ATTAQUE_U, DEFENSE_U, DEGATS_U,
MOUVEMENTS_U, PRIX_U} E_UNITES;

typedef enum {NUM_JOUEUR_HJ, NUM_HERO_HJ, HERO_PRINCIPAL, POINTS_EXPERIENCE,
NIVEAU_HJ, POINTS_COMPETENCE_HJ, BONUS_ATTAQUE_HJ, BONUS_DEFENSE_HJ,
CHANCE_CRITIQUE_HJ, MORAL_HJ, POINTS_ATTAQUE_SPECIALE_HJ} E_HEROSJOUEURS;

typedef enum {NUM_JOUEUR_UJ, NUM_UNITE_UJ, TYPE_UJ, SANTE_UJ, ATTAQUE_UJ,
DEFENSE_UJ, DEGATS_UJ, MOUVEMENTS_UJ, NOMBRE_UJ, NIVEAU_UJ, UNITE_ACTIVE} E_UNITESJOUEURS;

typedef enum { NUM_JOUEUR_UB, NUM_UNITE_UB, TYPE_UB, SANTE_RESTANTE_UB,
ATTAQUE_UB, DEFENSE_UB, DEGATS_UB, MOUVEMENTS_UB, NOMBRE_UNITES_UB, LIGNE_UB,
COLONNE_UB, FINTOUR_UB, SANTE_UNITAIRE_UB} E_UNITESBATAILLE;

void chargerFichierVersTableau(
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes],
  char *nomFichier){
      FILE *filePointer = fopen(nomFichier,"r");
        for(int i=0; i<nb_lignes; i++){
          for(int j=0; j<nb_colonnes; j++){
            fscanf(filePointer, "%d", &tab[i][j]);
          }
        }
      fclose(filePointer);
}//PASS

void sauvegarderTableauDansFichier(
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes],
  char *nomFichier){
    FILE *filePointer = fopen(nomFichier, "w");
      for(int i=0; i<nb_lignes; i++){
        for(int j=0; j<nb_colonnes; j++){
          (j + 1 != nb_colonnes) ? fprintf(filePointer, "%d ",tab[i][j]) : fprintf(filePointer, "%d",tab[i][j]);
        }
        (i + 1 != nb_lignes) ? fprintf(filePointer, "\n") : 0;
      }
      fclose(filePointer);
}//PASS

void chargerJeuComplet(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    chargerFichierVersTableau(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"joueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"heros_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"unites_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"herosJoueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"carte_sauvegarde.txt");
}//PASS


void initialiserNouveauJeu(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    chargerFichierVersTableau(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"joueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"heros_original.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"unites_original.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"herosJoueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"unitesJoueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"carte_original.txt");
}//PASS

void sauvegarderJeuComplet(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    sauvegarderTableauDansFichier(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"joueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"heros_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"unites_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"herosJoueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"carte_sauvegarde.txt");
}//PASS

int chercherIndiceAvecUneCondition(
  int colonneConcernee,
  int valeur,
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes] ){
    int i=0;
    while(i<(nb_lignes-1) && tab[i][colonneConcernee] != valeur){
      i++;
    }
    if(tab[i][colonneConcernee] == valeur){
      return i;
    }
    return -1;
}//PASS

int chercherIndiceAvecDeuxConditions(
  int colonneConcernee1,
  int valeur1,
  int colonneConcernee2,
  int valeur2,
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes] ){
    int i=0;

    while(i<(nb_lignes-1) && (tab[i][colonneConcernee1] != valeur1 || tab[i][colonneConcernee2] != valeur2)){
      i++;
    }
    if(tab[i][colonneConcernee1] == valeur1 && tab[i][colonneConcernee2] == valeur2){
      return i;
    }
    return -1;
}//PASS

void success(SDL_Surface* ecran,char* text){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO);
    menu = IMG_Load("success.png");
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    text_font = TTF_OpenFont("Teko-Medium.ttf", 35);
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 19);

    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          continuer = 0;
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char* phrase = "Clicker sur entrer pour continuer..";
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 230;
          positionText.x = (400 - strlen(text)*11)/2;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
          positionText.y = 325;
          positionText.x = 20;
          texte = TTF_RenderText_Blended(font, phrase, blanc);
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}

int alert(SDL_Surface* ecran,char* text){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;
  int indiceOption = 0;

    SDL_Init(SDL_INIT_VIDEO);
    menu = IMG_Load("alert.png");
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    text_font = TTF_OpenFont("Teko-Medium.ttf", 35);
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);

    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Color rouge = {255,0,0};
    SDL_Color vert = {34,139,24};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          if(indiceOption == 0){
            return 1;
          }else if(indiceOption == 1){
            return 0;
          }
        }else if(event.key.keysym.sym == SDLK_LEFT){
          if(indiceOption > 0){
            indiceOption--;
          }
        }else if(event.key.keysym.sym == SDLK_RIGHT){
          if(indiceOption < 1){
            indiceOption++;
          }
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char * phrase[2];
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 210;
          positionText.x = (400 - strlen(text)*11)/2;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);

          phrase[0] = "OUI";
          phrase[1] = "NON";
        int x_depart = 90;
        SDL_Color couleur_a_utiliser;
        for (int i = 0; i < 2; i++){
          if(i == indiceOption){
            if(i == 0){
              couleur_a_utiliser = vert;
            }else{
              couleur_a_utiliser = rouge;
            }
            positionText.y = 300;
          }else{
            couleur_a_utiliser = blanc;
            positionText.y = 305;
          }
          texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
          positionText.x = x_depart + i * 150;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
        }
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}


void executerAction(//here
  SDL_Surface* ecran,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){
  int choix = 0, ind_J_humain = 0, unite = 0, dizaine = 0, centaine = 0, num_lig_J = 0, num_col_J = 0, ind_H_Principal = 0;

    ind_J_humain = chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
    num_lig_J = joueurs[ind_J_humain][LIGNE_J];
    num_col_J = joueurs[ind_J_humain][COLONNE_J];
    dizaine = (carte[num_lig_J][num_col_J]/10)%10;
    centaine = (carte[num_lig_J][num_col_J]/100);

    // if((dizaine)==0){
    //   printf("\n");
    //   for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //   printf("**** Case Vide! ****\n");
    // }else if((dizaine)==1){
    //   do{
    //     printf("\n");
    //     for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //     printf("Confirmez vous l\'entree en bataille avec l\'ennemie %d ?(oui=1, non=0) : ",centaine);
    //     scanf("%d",&choix);
    //   }while(choix < 0 || choix > 1);
    //
    //   if(choix == 1){
    //     deroulerBataille(JOUEUR_HUMAIN,centaine,joueurs,herosJoueurs,unitesJoueurs);
    //   }
    // }else if((dizaine)==2){
    //   if(centaine != 0){
    //     do{
    //       printf("\n");
    //       for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //       printf("Confirmez vous l\'ajout de %d pieces d\'or a votre fortune personelle ?(oui=1, non=0) : ",centaine);
    //       scanf("%d",&choix);
    //     }while(choix < 0 || choix > 1);
    //
    //     if(choix == 1){
    //       printf("\n");
    //       for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //       printf("Un tresor de montant %d pieces a ete decouvert et a ete ajoute a votre fortune ! ",centaine);
    //       joueurs[ind_J_humain][OR] += centaine;
    //       carte[num_lig_J][num_col_J] %= 100;
    //     }
    //   }else{
    //     printf("\n");
    //     for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //     printf("Coffre de tresor deja ouvert !");
    //   }
    // }else if((dizaine)==3){
    //   if(centaine != 0){
    //     do{
    //       printf("\n");
    //       for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //       printf("Confirmez vous l\'ajout de %d Points d\'experience a votre fortune personelle ?(oui=1, non=0) : ",centaine);
    //       scanf("%d",&choix);
    //     }while(choix < 0 || choix > 1);
    //
    //     if(choix == 1){
    //       ind_H_Principal = chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,HERO_PRINCIPAL,1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
    //       if(ind_H_Principal != -1){
    //         printf("\n");
    //         for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //         printf("Un Coffre de montant %d Points a ete decouvert et a ete ajoute a votre fortune ! ",centaine);
    //         herosJoueurs[ind_H_Principal][POINTS_EXPERIENCE] += centaine;
    //         carte[num_lig_J][num_col_J] %= 100;
    //       }else{
    //         for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //         printf("Vous n\'avez pas un hero principal");
    //       }
    //     }
    //   }else{
    //     printf("\n");
    //     for(int s=0; s<NB_ESPACES_CENTRE; s++) printf(" ");
    //     printf("Caisse d\'experience deja ouvert !");
    //   }
    // }
}//PASS


void degagerNuages(
  int lig,
  int col,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE] ){
    int lig_min = lig-2, lig_max = lig+2, col_min = col-2, col_max = col+2, contenu_case = 0;

    for(int i=lig_min; i<=lig_max; i++){
      for(int j=col_min; j<=col_max; j++){
        contenu_case = carte[i][j] % 10;
        if(contenu_case == 1){
          if((i >= 0) && (i <= NB_LIGNES_CARTE-1) && (j >= 0) && (j <= NB_COLONNES_CARTE-1)){
               carte[i][j] /= 10;
               carte[i][j] *= 10;
          }
        }
      }
    }
}//PASS

void avancer(
  char action,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS] ){
    int ind_J_humain = 0, ans_lig_J = 0, ans_col_J = 0, existe_J = 0, nouv_lig_J = 0, nouv_col_J = 0, etat_change = 0;

    ind_J_humain = chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
    ans_lig_J = joueurs[ind_J_humain][LIGNE_J];
    ans_col_J = joueurs[ind_J_humain][COLONNE_J];
    existe_J = carte[ans_lig_J][ans_col_J] % 10 ;

    if((existe_J == 2)){
      if(action == 'h' && ans_lig_J > 0){
        nouv_lig_J = --joueurs[ind_J_humain][LIGNE_J];
        nouv_col_J = ans_col_J;
        etat_change = 1;
      }
      else if(action == 'b' && ans_lig_J < NB_LIGNES_CARTE-1){
        nouv_lig_J = ++joueurs[ind_J_humain][LIGNE_J];
        nouv_col_J = ans_col_J;
        etat_change = 1;
      }
      else if(action == 'g' && ans_col_J > 0){
        nouv_col_J = --joueurs[ind_J_humain][COLONNE_J];
        nouv_lig_J = ans_lig_J;
        etat_change = 1;
      }
      else if(action == 'd' && ans_col_J < NB_COLONNES_CARTE-1){
        nouv_col_J = ++joueurs[ind_J_humain][COLONNE_J];
        nouv_lig_J = ans_lig_J;
        etat_change = 1;
      }
      if(etat_change){
        degagerNuages(nouv_lig_J,nouv_col_J,carte);
        carte[ans_lig_J][ans_col_J] /= 10;
        carte[ans_lig_J][ans_col_J] *= 10;
        carte[nouv_lig_J][nouv_col_J] /= 10;
        carte[nouv_lig_J][nouv_col_J] *= 10;
        carte[nouv_lig_J][nouv_col_J] += 2;
      }
    }
}//PASS


void deroulerJeu(
  SDL_Surface* ecran,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){
    SDL_Surface *joueur = NULL, *terrain_invisible = NULL, *terrain_visible = NULL,
      *coffre_or = NULL, *coffre_or_overt = NULL, *caisse_xp = NULL, *ennemi1 = NULL,
      *ennemi2 = NULL, *ennemi3 = NULL, *ennemi4 = NULL, *caisse_xp_overt = NULL;
    SDL_Rect position, positionJoueur;
    SDL_Event event;
    int continuer = 1, alerte = 1;

    joueur = IMG_Load("joueur.png");
    terrain_invisible = IMG_Load("terrain_invisible.png");
    terrain_visible = IMG_Load("terrain_visible.png");
    coffre_or = IMG_Load("coffre_or.png");
    coffre_or_overt = IMG_Load("coffre_or_overt.png");
    caisse_xp = IMG_Load("caisse_xp.png");
    caisse_xp_overt = IMG_Load("xp.png");
    ennemi1 = IMG_Load("ennemi1.png");
    ennemi2 = IMG_Load("ennemi2.png");
    ennemi3 = IMG_Load("ennemi3.png");
    ennemi4 = IMG_Load("ennemi4.png");

    SDL_EnableKeyRepeat(100, 100);
    while(continuer){
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_UP){
            avancer('h',carte,joueurs);
          }else if(event.key.keysym.sym == SDLK_DOWN){
            avancer('b',carte,joueurs);
          }else if(event.key.keysym.sym == SDLK_RIGHT){
            avancer('d',carte,joueurs);
            joueur = IMG_Load("joueur.png");
          }else if(event.key.keysym.sym == SDLK_LEFT){
            avancer('g',carte,joueurs);
            joueur = IMG_Load("joueur_g.png");
          }
          alerte = 0;
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 89, 158, 42));
        for (int i = 0 ; i < NB_LIGNES_CARTE ; i++){
            for (int j = 0 ; j < NB_COLONNES_CARTE ; j++){
                position.y = i * TAILLE_BLOC;
                position.x = j * TAILLE_BLOC;

                if(carte[i][j]%10 == 0){
                  SDL_BlitSurface(terrain_visible, NULL, ecran, &position);
                  if((carte[i][j]/10)%10 == 1){
                    if(carte[i][j]/100 == 1){
                      SDL_BlitSurface(ennemi1, NULL, ecran, &position);
                    }else if(carte[i][j]/100 == 2){
                      SDL_BlitSurface(ennemi2, NULL, ecran, &position);
                    }else if(carte[i][j]/100 == 3){
                      SDL_BlitSurface(ennemi3, NULL, ecran, &position);
                    }else if(carte[i][j]/100 == 4){
                      SDL_BlitSurface(ennemi4, NULL, ecran, &position);
                    }
                  }else if((carte[i][j]/10)%10 == 2){
                    SDL_BlitSurface(coffre_or, NULL, ecran, &position);
                  }else if((carte[i][j]/10)%10 == 3){
                    SDL_BlitSurface(caisse_xp, NULL, ecran, &position);
                  }
                }else if(carte[i][j]%10 == 1){
                  SDL_BlitSurface(terrain_invisible, NULL, ecran, &position);
                }else if(carte[i][j]%10 == 2){
                  SDL_BlitSurface(terrain_visible, NULL, ecran, &position);
                  if(carte[i][j]/100 == 1){
                    SDL_BlitSurface(ennemi1, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez attacker l\'ennemi 1 ?")){
                        success(ecran,"la bataille va commencer !");
                      }
                    }
                  }else if(carte[i][j]/100 == 2){
                    SDL_BlitSurface(ennemi2, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez attacker l\'ennemi 2 ?")){
                        success(ecran,"la bataille va commencer !");
                      }
                    }
                  }else if(carte[i][j]/100 == 3){
                    SDL_BlitSurface(ennemi3, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez attacker l\'ennemi 3 ?")){
                        success(ecran,"la bataille va commencer !");
                      }
                    }
                  }else if(carte[i][j]/100 == 4){
                    SDL_BlitSurface(ennemi4, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez attacker l\'ennemi 4 ?")){
                        success(ecran,"la bataille va commencer !");
                      }
                    }
                  }else if((carte[i][j]/10)%10 == 2){
                    SDL_BlitSurface(coffre_or_overt, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez ouvrir cette coffre ?")){
                        success(ecran,"Sucess !");
                      }
                    }
                  }else if((carte[i][j]/10)%10 == 3){
                    SDL_BlitSurface(caisse_xp_overt, NULL, ecran, &position);
                    if(alerte == 0){
                      alerte = 1;
                      if(alert(ecran,"Vouz voulez ouvrir cette caisse ?")){
                        success(ecran,"Sucess !");
                      }
                    }
                  }
                  SDL_BlitSurface(joueur, NULL, ecran, &position);
                }
            }
        }
        SDL_Flip(ecran);
    }
    SDL_EnableKeyRepeat(0, 0);
    SDL_FreeSurface(joueur);
    SDL_FreeSurface(caisse_xp_overt);
    SDL_FreeSurface(coffre_or_overt);
    SDL_FreeSurface(terrain_visible);
    SDL_FreeSurface(terrain_invisible);
    SDL_FreeSurface(coffre_or);
    SDL_FreeSurface(caisse_xp);
    SDL_FreeSurface(ennemi1);
    SDL_FreeSurface(ennemi2);
    SDL_FreeSurface(ennemi3);
    SDL_FreeSurface(ennemi4);
}


void lancerMenuMagasin(
  SDL_Surface* ecran,
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){

    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;
    TTF_Font *title_font= NULL;
    SDL_Surface *texte = NULL;
    int continuer = 1;
    int indiceOption = 0;

      SDL_Init(SDL_INIT_VIDEO);



      menu = IMG_Load("background.jpg");
      positionMenu.x = 0;
      positionMenu.y = 0;

      TTF_Init();
      title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
      font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);


      SDL_Color blanc = {255,255,255};
      SDL_Color bleu = {130, 236, 251};
      SDL_Rect positionText;

      while (continuer){
        SDL_WaitEvent( & event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
            if(indiceOption == 0){
                continuer = 0;
            }else if(indiceOption == 1){

            }
          }else if(event.key.keysym.sym == SDLK_UP){
            if(indiceOption > 0){
              indiceOption--;
            }
          }else if(event.key.keysym.sym == SDLK_DOWN){
            if(indiceOption < 1){
                indiceOption++;
            }
          }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

        if(font != 0){
          char * phrase[2];
            texte = TTF_RenderText_Blended(title_font, "Magasin", bleu);
            positionText.y = 100;
            positionText.x = 125;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "Acheter des Heros";
            phrase[1] = "Acheter des Unites";
          int y_depart = 210;
          SDL_Color couleur_a_utiliser;
          for (int i = 0; i < 2; i++){
            if(i == indiceOption){
              couleur_a_utiliser = bleu;
              positionText.x = 80;
            }else{
              couleur_a_utiliser = blanc;
              positionText.x = 75;
            }
            texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            positionText.y = y_depart + i * 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
          }
        }
        SDL_Flip(ecran);
      }
      SDL_FreeSurface(texte);
      TTF_CloseFont(font);
      SDL_FreeSurface(menu);
}

void lancerMenuCampement(
  SDL_Surface* ecran,
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;
    TTF_Font *title_font= NULL;
    SDL_Surface *texte = NULL;
    int continuer = 1;
    int indiceOption = 0;

      SDL_Init(SDL_INIT_VIDEO);


      menu = IMG_Load("background.jpg");
      positionMenu.x = 0;
      positionMenu.y = 0;

      TTF_Init();
      title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
      font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);


      SDL_Color blanc = {255,255,255};
      SDL_Color bleu = {130, 236, 251};
      SDL_Rect positionText;

      while (continuer){
        SDL_WaitEvent( & event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
            if(indiceOption == 0){

            }else if(indiceOption == 1){

            }
          }else if(event.key.keysym.sym == SDLK_UP){
            if(indiceOption > 0){
              indiceOption--;
            }
          }else if(event.key.keysym.sym == SDLK_DOWN){
            if(indiceOption < 1){
                indiceOption++;
            }
          }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

        if(font != 0){
          char * phrase[2];
            texte = TTF_RenderText_Blended(title_font, "Campement", bleu);
            positionText.y = 100;
            positionText.x = 90;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "Configuration Heros";
            phrase[1] = "Configuration Unites";
          int y_depart = 210;
          SDL_Color couleur_a_utiliser;
          for(int i = 0; i < 2; i++){
            if(i == indiceOption){
              couleur_a_utiliser = bleu;
              positionText.x = 70;
            }else{
              couleur_a_utiliser = blanc;
              positionText.x = 65;
            }
            texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            positionText.y = y_depart + i * 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
          }
        }
        SDL_Flip(ecran);
      }
      SDL_FreeSurface(texte);
      TTF_CloseFont(font);
      SDL_FreeSurface(menu);
}


int main(int argc, char * argv[]){
  SDL_Surface *ecran = NULL, *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;
  int screen = 0;
  int nb_choix = 0;

    int joueurs       [NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS];
    int heros         [NB_LIGNES_HEROS][NB_COLONNES_HEROS];
    int unites        [NB_LIGNES_UNITES][NB_COLONNES_UNITES];
    int herosJoueurs  [NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS];
    int unitesJoueurs [NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS];
    int carte         [NB_LIGNES_CARTE][NB_COLONNES_CARTE];
    int indiceOption = 0;

      SDL_Init(SDL_INIT_VIDEO);
      SDL_WM_SetIcon(IMG_Load("heros_icon.png"), NULL);
      ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
      SDL_WM_SetCaption("Heros Magic War", NULL);

      menu = IMG_Load("background.jpg");
      positionMenu.x = 0;
      positionMenu.y = 0;

      TTF_Init();
      title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
      font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);


      SDL_Color blanc = {255,255,255};
      SDL_Color bleu = {130, 236, 251};
      SDL_Rect positionText;

      SDL_EnableKeyRepeat(100, 100);
      while(continuer){
        SDL_WaitEvent( & event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
            if(indiceOption == 0 && screen == 0){
              initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              screen = 1;
            }else if(indiceOption == 1 && screen == 0){
              chargerJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              screen = 1;
              indiceOption = 0;
            }else if(indiceOption == 0 && screen == 1){
              lancerMenuMagasin(ecran,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 1 && screen == 1){
              lancerMenuCampement(ecran,joueurs,herosJoueurs,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 2 && screen == 1){
              deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 3 && screen == 1){
              initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 4 && screen == 1){
              if(alert(ecran,"Vouz Voulez Quitter ?")){
                continuer = 0;
              }
            }
          }else if(event.key.keysym.sym == SDLK_UP){
            if(indiceOption > 0){
              indiceOption--;
            }
          }else if(event.key.keysym.sym == SDLK_DOWN){
            if(screen == 0 && indiceOption < 1){
                indiceOption++;
            }else if(screen ==1 && indiceOption < 4){
                indiceOption++;
            }
          }
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

        if(font != 0){
          char * phrase[5];
          if(screen == 0){
            texte = TTF_RenderText_Blended(title_font, "HEROS MAGIC WAR", bleu);
            positionText.y = 100;
            positionText.x = 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "NOUVELLE PARTIE";
            phrase[1] = "CHARGER PARTIE";
            nb_choix = 2;
          }else if(screen == 1){
            texte = TTF_RenderText_Blended(title_font, "Menu Principal", bleu);
            positionText.x = 60;
            positionText.y = 90;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "Magasin";
            phrase[1] = "Campement";
            phrase[2] = "Reprendre le jeu en cours";
            phrase[3] = "Nouvelle partie";
            phrase[4] = "Quitter";
            font = TTF_OpenFont("PressStart2P-Regular.ttf", 12);
            nb_choix = 5;
          }

          int y_depart = 210;
          SDL_Color couleur_a_utiliser;
          if(screen == 1){
            y_depart = 155;
          }
          for(int i = 0; i < nb_choix; i++){
            if(i == indiceOption){
              couleur_a_utiliser = bleu;
              positionText.x = 100;
            }else{
              couleur_a_utiliser = blanc;
              positionText.x = 95;
            }
            texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            positionText.y = y_depart + i * 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
          }
        }
        SDL_Flip(ecran);
      }
      SDL_EnableKeyRepeat(0, 0);
      SDL_FreeSurface(texte);
      TTF_CloseFont(font);
      SDL_FreeSurface(menu);
      TTF_Quit();
      SDL_Quit();

      return EXIT_SUCCESS;
}

// if(alerte == 0){
//   alerte = 1;
//   if(alert(ecran,"Vouz voulez ouvrir cette caisse ?")){
//     return;
//   }
// }
