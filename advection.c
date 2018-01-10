#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define c 1.0 // m/s 
#define N_PUNTOS 1000 
#define X_MIN -2 // m 
#define X_MAX 2 // m 
#define N_T 700
 

int main(){
	
		
	float dt = 1e-3; // s 

	//printf("%E", dt);

	/* ARRAY DE POSICION CON N_PUNTOS */
	
	float *x = malloc(N_PUNTOS*sizeof(float));
	float dx = (X_MAX - X_MIN)/1000.0; 
	//printf("%f", dx);
 	x[0] = X_MIN;
	x[N_PUNTOS-1] = X_MAX;
     
   	for( int i = 1; i < N_PUNTOS-1; i++ ) {
      		x[i] = x[i-1] + dx ; /* set element at location i to i + 100 */
   	}
   	// IMPRIME EL ARRAY 
   	/*for( int i = 0; i < N_PUNTOS+1; i++ ) {
      		printf("x[%d] = %f\n", i, x[i] );
   	}*/

	/* ARRAY DE SLN N_T X N_PUNTOS */
	
    	float (*y)[N_PUNTOS] = malloc(sizeof(float[N_T][N_PUNTOS]));

	float alpha = c*dt/dx; 

	/* para probar uso N_PUNTOS = 6 Y N_T = 2 */
	
	/* CONDICIONES INICIALES PARA t = 0 */
	for( int j = 0; j < N_T; j++){ 
		for( int i = 0; i < N_PUNTOS; i++){
			if( j == 0 && fabs(x[i]) <= 0.5 ){
				y[j][i] = -(x[i]*x[i]) + 0.25;
			}else {     
				y[j][i] = 0;
			}
		}
	}

	/* CONDICIONES DE FRONTERA EN LOS BORDES */
	for( int j = 0; j < N_T; j++){ 
		for( int i = 0; i < N_PUNTOS; i++){
			if( i == 0 || i == N_PUNTOS-1 ){		
				y[j+1][i] == y[j][i]; 
			}
		//printf("y[%d][%d]=%f", j, i, y[j][i]);
        	//printf("\n");
		}
	}
	
	/* PASO DEL TIEMPO CON DIFERENCIAS FINITAS */
	for( int j = 0; j < N_T-1; j++){ 
		for( int i = 1; i < N_PUNTOS-1; i++){
			y[j+1][i] = y[j][i] - alpha * ( y[j][i] - y[j][i-1]);  
			//printf("y[%d][%d]=%f", j, i, y[j][i]);
        		//printf("\n");
		}
	//printf("y[%d]=%.6f", j, y[j]);
        //printf("\n");
	}

	/* GENERA ARCHIVOS */	
	
	FILE *x_x = fopen("x_x.dat", "w");
	
	for(int i = 0; i < N_PUNTOS; i++)
	{
            	fprintf(x_x, "%f ",  x[i]);
        	fprintf(x_x, "\n");
    	}
	fclose(x_x);
		
	FILE *y_0x = fopen("y_0x.dat", "w");
	
	for(int i = 0; i < N_PUNTOS; i++)
	{
            	fprintf(y_0x, "%f ",  y[0][i]);
        	fprintf(y_0x, "\n");
    	}
	fclose(y_0x);

	FILE *y_350x = fopen("y_350x.dat", "w");
	
	for(int i = 0; i < N_PUNTOS; i++)
	{
            	fprintf(y_350x, "%f ",  y[350][i]);
        	fprintf(y_350x, "\n");
    	}
	fclose(y_350x);

	FILE *y_600x = fopen("y_600x.dat", "w");
	
	for(int i = 0; i < N_PUNTOS; i++)
	{
            	fprintf(y_600x, "%f ",  y[600][i]);
        	fprintf(y_600x, "\n");
    	}
	fclose(y_600x);

	return 0;

}

