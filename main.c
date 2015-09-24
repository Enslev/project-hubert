#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sensor.h"
#include "qrs.h"

struct xs {
	int next;
	int size;
	int data[100];
};

void addNextData(struct xs *s, int dataPoint);
int getPastEntry(struct xs *s, int entry);
int low_pass(struct xs *x, struct xs *y);
int high_pass(struct xs *x, struct xs *y);
int derivative(struct xs *x);
int square(struct xs *x);
int moving_window_integration(struct xs *x);

int main(int argc, char *argv[]){

	int counter = 0;
	int n = 1;

	struct xs x = {.next = 0, .size = 100};
	struct xs x_low = {.next = 0, .size = 100};
	struct xs x_high = {.next = 0, .size = 100};
	struct xs x_der = {.next = 0, .size = 100};
	struct xs x_sqr = {.next = 0, .size = 100};
	struct xs x_mwi = {.next = 0, .size = 100};

	// TODO: better method for zero initializing the data arrays
	for (int i = 0; i < x.size; i++){
		x.data[i] = 0;
		x_low.data[i] = 0;
		x_high.data[i] = 0;
		x_der.data[i] = 0;
		x_sqr.data[i] = 0;
		x_mwi.data[i] = 0;
	}


	static const char filename[] = "ECG900K.txt";
	FILE *file = fopen(filename, "r");

	while ( /*x.data[getPastEntry(&x, 0)] != EOF && */ counter < 1000){
		addNextData( &x, getNextData(file) );

		/*
		if ( x.data[getPastEntry(&x, 0)] == EOF ){
			printf("EOF!!!!");
			break;
		}
		*/

		addNextData( &x_low, low_pass(&x, &x_low) );
		addNextData( &x_high, high_pass(&x_low, &x_high) );
		addNextData( &x_der, derivative(&x_high) );
		addNextData( &x_sqr, square(&x_der) );
		addNextData( &x_mwi, moving_window_integration(&x_sqr) );

		//printf("%d\n", getPastEntry(&x_mwi, 0));

		// send x_mwi to QRS
		QRS(getPastEntry(&x_mwi, 0));


		//printf("x_mwi: %d %d \n", n, getPastEntry(&x_mwi, 0));
		//printf("n: %d - %d \n", n, x.data[x.next]);
		//printf("n: %d \n", n);

		//printf("\n");
		n++;
		counter++;

	}

	return 0;
}


void addNextData(struct xs *s, int dataPoint){
	s->data[s->next] = dataPoint;

	if (s->next + 1 == s->size){
		s->next = 0;
	}
	else {
		s->next = s->next + 1;
	}
}

int getPastEntry(struct xs *s, int entry){
	if ( abs(entry) >= s->size){
		printf("ERROR! entry too large in: getPastEntry");
		exit(1);
	}

	if (entry < 0){
		printf("ERROR! past entry cannot be a negative number in: getPastEntry");
		exit(1);
	}

	int bla = (s->next - 1) - entry;
	int index;
	if (bla < 0){
		//index = s->size + (bla % s->size);
		index = s->size - abs(bla);
	}
	else {
		index = bla;
	}

	return s->data[index];
}

int low_pass(struct xs *x, struct xs *y){
	int y1 = getPastEntry(y, 1-1);
	int y2 = getPastEntry(y, 2-1);
	int xn = getPastEntry(x, 0);
	int x6 = getPastEntry(x, 6);
	int x12 = getPastEntry(x, 12);

	return 2*y1 - y2 + (xn - 2*x6 + x12)/32;
}


int high_pass(struct xs *x, struct xs *y){
	int y1 = getPastEntry(y, 1-1);
	int xn = getPastEntry(x, 0);
	int x16 = getPastEntry(x, 16);
	int x17 = getPastEntry(x, 17);
	int x32 = getPastEntry(x, 32);

	return y1 - xn/32 + x16 - x17 + x32/32;
}

int derivative(struct xs *x){
	int xn = getPastEntry(x, 0);
	int x1 = getPastEntry(x, 1);
	int x3 = getPastEntry(x, 3);
	int x4 = getPastEntry(x, 4);

	return (2*xn + x1 - x3 - 2*x4)/8;
}


int square(struct xs *x){
	return getPastEntry(x, 0) * getPastEntry(x, 0);
}


int moving_window_integration(struct xs *x){
	int N = 30;
	int sum = 0;

	for (int i = 0; i < N; i++){
		sum += getPastEntry(x, i);
	}
	return sum/N;
}
