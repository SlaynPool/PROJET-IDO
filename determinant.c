#include <stdio.h>
#include <stdlib.h>

double detMat(double**,int,int);

void afficheMat(double**,int,int);
void delMat(double**,int,int);


double** createMat(int i, int j){
    double** tab;
    tab=malloc(i*sizeof(double*));
    for (int k=0;k<i;k++){
        tab[k]=malloc(j*sizeof(double));
    }
return tab;
}

void delMat(double** tab, int i, int j){
for (int k=0; k<i;k++){
    free(tab[k]);
}
free(tab);
}




double detMat(double** tab, int lg, int col){
double det=0;
int taboule,pantalon,serviette;
int tshirt=0,pizza=0;
int mouton=0;
double** tab2=createMat(lg-1,col-1);
    if (lg==col){
        if (lg==2){ // SI ON A UN TABLEAU DE 2 CASES SUR 2, ON FAIT CA
	  //printf("Blabla tab 2x2\n");
	  det=tab[0][0]*tab[1][1]-tab[0][1]*tab[1][0];
	  delMat(tab2,lg-1,col-1);
          return det;
        }
        else { // SINON ON FAIT CA
		for (taboule=0;taboule<lg;taboule++){
			for (serviette=0;serviette<lg;serviette++){
				if (serviette!=taboule){


				for (pantalon=1;pantalon<lg;pantalon++){

					tab2[tshirt][pizza]=tab[serviette][pantalon];
				pizza++;
				}
				pizza=0;
				tshirt++;
				}
			}
		tshirt=0;

		mouton=taboule+1;
		mouton=mouton%2;
		if (mouton==0){
			mouton=mouton-1;
		}
		det=detMat(tab2,lg-1,col-1)*tab[taboule][0]*mouton+det;

		}
	}
        }
    delMat(tab2,lg-1,col-1);

return det;
}

