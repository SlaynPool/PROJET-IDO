#ifndef DETERMINANT_H
#define DETERMINANT_H
double detMat(double** tab, int lg, int col);
double** createMat(int i, int j);
void delMat(double** tab, int i, int j);


double calculMoteur(int motor, float pitch, float yaw,float roll, float throttle, int k);

#endif
