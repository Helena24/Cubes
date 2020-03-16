//
//  main.cpp
//  Marching
//
//  Created by helena adiduyulmus on 21/01/2020.
//  Copyright © 2020 helena adiduyulmus. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

/* déclaration d'un tableau de 13 lignes et 15 colonnes, qui correspondent aux 15 configurations de bases connues pour
l'algorithme des marching cubes */

int configbase[15][13]= {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 1, 11, 1, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 2, 11, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 10, 3, 8, 10, 8, 9, 10, -1, -1, -1, -1},
    {9, 10, 11, 8, 9, 11, -1, -1, -1, -1, -1, -1, -1},
    {6, 7, 11, 2, 3, 10, 3, 8, 10, 8, 9, 10, -1},
    {3, 2, 11, 5, 6, 10, 1, 9, 0, 4, 7, 8, -1},
    {11, 7, 4, 11, 4, 2, 2, 9, 4, 2, 9, 1, -1},
    {2, 3, 7, 2, 7, 9, 7, 9, 4, 2, 10, 9, -1},
    {3, 2, 11, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1},
    {4, 5, 9, 3, 1, 11, 1, 10, 11, -1, -1, -1, -1},
    {4, 5, 9, 11, 7, 6, 2, 10, 1, -1, -1, -1, -1},
    {3, 2, 7, 2, 7, 6, 1, 0, 4, 4, 5, 1, -1},
    {2, 11, 8, 8, 4, 5, 2, 8, 5, 2, 1, 5, -1}
};

int rotationsommet[24][8]={
    {0, 1, 2, 3, 4, 5, 6, 7}, // position initiale des sommets
    {1, 2, 3, 0, 5, 6, 7, 4}, // rotation selon l'axe central, 3 possibilités
    {2, 3, 0, 1, 6, 7, 4, 5},
    {3, 0, 1, 2, 7, 4, 5, 6},
    {4, 5, 1, 0, 7, 6, 2, 3}, // rotation selon l'axe horizontal, 3 possibilités
    {7, 6, 5, 4, 3, 2, 1, 0},
    {3, 2, 6, 7, 0, 1, 5, 4},
    {1, 5, 6, 2, 0, 4, 7, 3}, // rotation selon l'axe ventral, 3 possibilités
    {5, 4, 7, 6, 1, 0, 3, 2},
    {4, 0, 3, 7, 5, 1, 2, 6},
    {7, 3, 2, 6, 4, 0, 1, 5}, // rotation selon la diagonale 4;2
    {5, 6, 2, 1, 4, 7, 3, 0},
    {7, 4, 0, 3, 6, 5, 1, 2}, // rotation selon la diagonale 5;3
    {2, 6, 7, 3, 1, 5, 4, 0},
    {0, 3, 7, 4, 1, 2, 6, 5}, // rotation selon la diagonale 6;0
    {0, 4, 5, 1, 3, 7, 6, 2},
    {5, 1, 0, 4, 6, 2, 3, 7}, // rotation selon la diagonale 7;1
    {2, 1, 5, 6, 3, 0, 4, 7},
    {6, 7, 3, 2, 5, 4, 0, 1}, //rotation selon MA 4,2
    {1, 0, 4, 5, 2, 3, 7, 6}, //rotation selon MA 6,0
    {3, 7, 4, 0, 2, 6, 5, 1}, //rotation selon MA 5,3
    {6, 2, 1, 5, 7, 3, 0, 4}, //rotation selon MA 7,1
    {4, 7, 6, 5, 0, 3, 2, 1}, //rotation selon MA 8,10
    {6, 5, 4, 7, 2, 1, 0, 3} // rotation selon MA 9, 11
};

int rotationarete[24][12]={
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // arêtes de départ
    {1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8}, // rotation selon l'axe central
    {2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9},
    {3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10},
    {4, 9, 0, 8, 6, 10, 2, 11, 7, 5, 1, 3}, // rotation selon l'axe horizontal
    {6, 5, 4, 7, 2, 1, 0, 3, 11, 10, 9, 8},
    {2, 10, 6, 11, 0, 9, 4, 8, 3, 1, 5, 7},
    {9, 5, 10, 1, 8, 7, 11, 3, 0, 4, 6, 2}, //rotation selon l'axe ventral
    {4, 7, 6, 5, 0, 3, 2, 1, 9, 8, 11, 10},
    {8, 3, 11, 7, 9, 1, 10, 5, 4, 0, 2, 6},
    {11, 2, 10, 6, 8, 0, 9, 4, 7, 3, 1, 5}, // rotation selon la diagonale 4,2
    {5, 10, 1, 9, 7, 11, 3, 8, 4, 6, 2, 0},
    {7, 8, 3, 11, 5, 0, 1, 10, 6, 4, 0, 2}, // rotation selon la diagonale 5,3
    {10, 6, 11, 2, 9, 4, 8, 0, 1, 5, 7, 3},
    {3, 11, 7, 8, 1, 10, 5, 9, 0, 2, 6, 4}, //rotation selon la diagonale 6,0
    {8, 4, 9, 0, 11, 6, 10, 2, 3, 7, 5, 1},
    {9, 0, 8, 4, 10, 2, 11, 6, 5, 1, 3, 7}, // rotation selon la diagonale 7,1
    {1, 9, 5, 10, 3, 8, 7, 11, 2, 0, 4, 6},
    {6, 11, 2, 10, 4, 8, 0, 9, 5, 7, 3, 1}, // rotation selon MA 4,2
    {0, 8, 4, 9, 2, 11, 6, 10, 1, 3, 7, 5}, // rotation selon MA 6,0
    {11, 7, 8, 3, 10, 5, 9, 1, 2, 6, 4, 0}, // rotation selon MA 5,3
    {10, 1, 9, 5, 11, 3, 8, 7, 6, 2, 0, 4}, //rotation selon MA 7,1
    {7, 6, 5, 4, 3, 2, 1, 0, 8, 11, 10, 9}, //rotation selon MA 8;10
    {5, 4, 7, 6, 1, 0, 3, 2, 10, 9, 8, 11} //rotation selon MA 9;11
};

/* faire un tableau avec les points allumés par exemple pour la configuration on a le sommet 3 allumé donc on va mettre 0 pour tous les sommets sauf la valeur 1 pour le sommet 3 */

int sommetallumes[15][8]={
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 1, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 1, 0, 1, 0, 1, 0, 0}
};

/*fonction index*/

int indexcalcul(int tab[8]){
    int index = 0;
    for (int k=0; k<8; k++){
        if (tab[k] == 1){
            index += pow(2,k);
        }
    }
    return index;
}

/*fonction initialisation lookuptable*/

void initlookuptable(int tab[256][13]){
    for (int i=0; i<256; i++){
        for (int j=0; j<13;j++){
            tab[i][j] = -1;
        }
    }
}

/*fonction affichage tableau*/

void affichetab(int tab[], int length){
    for (int i=0; i<length; i++){
        std::cout<<tab[i]<<" ";
    }
    std::cout<<std::endl;
}

/*fonction affichage tableau*/

void affichetab2D(int tab[24][12], int length, int nb){
    for (int i=0; i<length; i++){
        for (int j=0; j<nb; j++){
        std::cout<<tab[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

/*allocation dynamique*/

struct point3D{
    bool allume;
};

point3D*** allocTab(int sizex, int sizey, int sizez) {
    point3D*** tab = new point3D * *[sizex];
    for (int i = 0; i < sizex; i++) {
        tab[i] = new point3D * [sizey];
    }
    for (int i =0; i<sizex; i++){
        for (int j = 0; j < sizey; j++) {
            tab[i][j]=new point3D[sizez];
        }
    }

    return tab;
}

/*fonction main*/

int main(){
    int lookup[256][13];
    initlookuptable(lookup);
    for (int i=1; i<15; i++){
        for (int l=3; l<24; l++){
            int sommetrotationallume[8]={0,0,0,0,0,0,0,0};
            for (int m=0; m<8; m++){
                if (sommetallumes[i][m]==1){
                    for (int n=0; n<8; n++){
                        if (rotationsommet[l][n]==m){
                            sommetrotationallume[n]=1;
                        }
                    }
                }
            }
            int o=0;
            int areteallume;
            int areterotationallume[13]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
            o=0;
            while (o<13 && configbase[i][o]!= -1){
                if (configbase[i][o] != -1){
                    areteallume = configbase[i][o];
                    for (int p=0; p<12; p++){
                        if (rotationarete[l][p] == areteallume){
                            areterotationallume[o] = p;
                        }
                       
                    }
                    o++;
                }
            }
            int indice_ligne = indexcalcul(sommetrotationallume);
            //std::cout<<indice_ligne<<" ";
            for (int q=0; q<12; q++){
                lookup[indice_ligne][q]=areterotationallume[q];
                lookup[255-indice_ligne][q]=areterotationallume[q];
            }
        }
    }
    for (int i=0; i<256; i++){
        for (int j=0; j<13; j++){
            std::cout<<lookup[i][j];
        }
        std::cout<<std::endl;
    }
}
    
