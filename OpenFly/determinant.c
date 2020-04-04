#include <stdio.h>
#include <stdlib.h>
#include "determinant.h"
void affiche(double** matrice, int i, int j){
    for(int a=0;a<i;a++){
        for(int b=0;b<j;b++){
            printf("%lf ",matrice[a][b]);
        }
    printf("\n");
    }
}

int remplisage(double** matrice,int motor,int i, int j, float pitch, float yaw,float roll, float throttle, int k){
    int col1[3];
    col1[0]=-1;
    col1[1]=1;
    col1[2]=-1;
    col1[3]=1;
    int col2[3];
    col2[0]=-1;
    col2[1]=-1;
    col2[2]=1;
    col2[3]=1;

    int col3[3];
    col3[0]=1;
    col3[1]=-1;
    col3[2]=-1;
    col3[3]=1;

    int ligne1, ligne2, ligne3;
    if(motor==0){
        ligne1=1;
        ligne2=2;
        ligne3=3;
    }
    if(motor==1){
        ligne1=0;
        ligne2=2;
        ligne3=3;
    }
    if(motor==2){
        ligne1=0;
        ligne2=1;
        ligne3=3;
    }
    if(motor==3){
        ligne1=0;
        ligne2=1;
        ligne3=2;
    }

 
    for(int a=0; a<i;a++){
        for(int b=0; b<j;b++){
            if(a==motor){
                if (b==0){
                    matrice[a][b]=roll;
                }
                if (b==1){
                    matrice[a][b]=pitch;
                }
                if(b==2){
                    matrice[a][b]=yaw;
                }
                if (b==3){
                    matrice[a][b]=throttle;
                }
            }
            if(a==ligne1){
                if (b==0){
                    matrice[a][b]=col1[0]*k;
                }
                if (b==1){
                    matrice[a][b]=col1[1]*k;
                }
                if(b==2){
                    matrice[a][b]=col1[2]*k;
                }
                if (b==3){
                    matrice[a][b]=col1[3]*k;
                }
            }
            if(a==ligne2){
                if (b==0){
                    matrice[a][b]=col2[0]*k;
                }
                if (b==1){
                    matrice[a][b]=col2[1]*k;
                }
                if(b==2){
                    matrice[a][b]=col2[2]*k;
                }
                if (b==3){
                    matrice[a][b]=col2[3]*k;
                }
            }
            if(a==ligne3){
                if (b==0){
                    matrice[a][b]=col3[0]*k;
                }
                if (b==1){
                    matrice[a][b]=col3[1]*k;
                }
                if(b==2){
                    matrice[a][b]=col3[2]*k;
                }
                if (b==3){
                    matrice[a][b]=col3[3]*k;
                    
                }
            }
        }


    }
    
    return 0;
}


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
    int t,p,s;
    int i=0,y=0;
    int m=0;
    double** tab2=createMat(lg-1,col-1);
    if (lg==col){
        if (lg==2){ // SI ON A UN TABLEAU DE 2 CASES SUR 2, ON FAIT CA
	    //printf("Blabla tab 2x2\n");
	        det=tab[0][0]*tab[1][1]-tab[0][1]*tab[1][0];
	        delMat(tab2,lg-1,col-1);
            return det;
        }
        else { // SINON ON FAIT CA
		    for (t=0;t<lg;t++){
			    for (s=0;s<lg;s++){
				    if (s!=t){
				        for (p=1;p<lg;p++){
					        tab2[i][y]=tab[s][p];
				            y++;
				        }
				        y=0;
				        i++;
				    }
			    }
		    i=0;
		    m=t+1;
            m=m%2;
		    if (m==0){
			    m=m-1;
		    }
		    det=detMat(tab2,lg-1,col-1)*tab[t][0]*m+det;

		  }
	    }
    }
    delMat(tab2,lg-1,col-1);
    return det;
}
int main(){
    double** ma_matrice;
    ma_matrice=createMat(4,4);
    remplisage(ma_matrice,0,4,4,4,4,4,4,5);
    int res;
    res=detMat(ma_matrice,4,4);
    affiche(ma_matrice,4,4);
    printf("%d",res);


 }
