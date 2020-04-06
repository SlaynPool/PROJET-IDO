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
    int col1[4];
    col1[0]=-1;
    col1[1]=1;
    col1[2]=-1;
    col1[3]=1;
    int col2[4];
    col2[0]=-1;
    col2[1]=-1;
    col2[2]=1;
    col2[3]=1;

    int col3[4];
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
     matrice[motor][0]=roll;
     matrice[motor][1]=pitch;
     matrice[motor][2]=yaw;
     matrice[motor][3]=throttle;


     matrice[ligne1][0]=col1[0]*k;
     matrice[ligne1][1]=col1[1]*k; 
     matrice[ligne1][2]=col1[2]*k;
     matrice[ligne1][3]=col1[3]*k;
     
     matrice[ligne2][0]=col2[0]*k;
     matrice[ligne2][1]=col2[1]*k; 
     matrice[ligne2][2]=col2[2]*k;
     matrice[ligne2][3]=col2[3]*k;
     
     matrice[ligne3][0]=col3[0]*k;
     matrice[ligne3][1]=col3[1]*k; 
     matrice[ligne3][2]=col3[2]*k;
     matrice[ligne3][3]=col3[3]*k;

   
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
double calculMot(int motor, float pitch, float yaw,float roll, float throttle, int k){
    double** ma_matrice;
    ma_matrice=createMat(4,4);
    remplisage(ma_matrice,motor,4,4,pitch,yaw,roll,throttle,k);
    double res;
    res=detMat(ma_matrice,4,4);
    //printf("detMat= %lf\n",res);

    res= res/(16*k);
   
    delMat(ma_matrice,4,4);
    return res;


 }

/*
int main(){
    double result;
    result=calculMot(0,4,4,4,4,5);
    printf("res final =%lf\n",result);
    return 0;
    
}*/
