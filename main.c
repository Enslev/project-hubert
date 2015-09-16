#include <stdio.h>
#include <math.h>
#include "sensor.h"
#include "filterdata.h"

int low_pass(int x[], int y[], int n);
int high_pass(int x[], int y[], int n);
int derivative(int x[], int y[], int n);
int square(int x[], int n);

int main(int argc, char *argv[]){

	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename, "r");

	int counter = 0;
	int n = 1;

	int x[1000];
	int x_low[1000];
	int x_high[1000];
	int x_der[1000];
	int x_sqr[1000];
	int x_mwi[1000];

	struct xs x_s;


	while (x[n] != EOF && counter < 25){

		//x[n] = getNextData(file);
		addNewData( x_s, getNextData(file) );

		x_low[n] = low_pass(x_s, x_low, n);
		x_high[n] = high_pass(x_low, x_high, n);
		x_der[n] = derivative(x_high, x_der, n);
		//x_sqr[n] = square(x_der, n);
		printf("%d \n", x_der[n]);

		n++; counter++;
	}


	return 0;
}

int low_pass(struct xs x, int y[], int n){
	int y1 = (n > 0) ? y[n-1] : 0;
	int y2 = (n > 1) ? y[n-2] : 0;
	int xn = getPastEntry(0);
	int x6 = getPastEntry(6);
	int x12 = getPastEntry(12);

	return 2*y1 - y2 + (xn - 2*x6 + x12)/32;
}

int low_passs(int x[], int y[], int n){
	int y1 = (n > 0) ? y[n-1] : 0;
	int y2 = (n > 1) ? y[n-2] : 0;
	int xn = x[n];
	int x6 = (n > 5) ? x[n-6] : 0;
	int x12 = (n > 11) ? x[n-12] : 0;

	return 2*y1 - y2 + (xn - 2*x6 + x12)/32;
}

int high_pass(int x[], int y[], int n){
	int y1 = (n > 0) ? y[n-1] : 0;
	int xn = x[n];
	int x16 = (n > 15) ? x[n-16] : 0;
	int x17 = (n > 16) ? x[n-17] : 0;
	int x32 = (n > 31) ? x[n-32] : 0;

	return y1 - xn/32 + x16 - x17 + x32/32;
}

int derivative(int x[], int y[], int n){
	int xn = x[n];
	int x1 = (n > 0) ? x[n-1] : 0;
	int x3 = (n > 2) ? x[n-3] : 0;
	int x4 = (n > 3) ? x[n-4] : 0;

	return (2*xn + x1 - x3 - 2*x4)/8;
}

int square(int x[], int n){
	int a = 4;
	double b = (double) a;
	return (int) (sqrt( b ));
}

/*
int moving_window_integration(int x[], int n){
	int N = 30;
	int res = 0;
	int xn[N] = { 0 };
	int i;

	for (i = 0; i < N; i++){
		xn[i] = x[nl];
	}

	for (i = 1; i <= N; i++){
		res += xn[]
	}


	return (1.0 / ((double) N)) * res;
}
*/
