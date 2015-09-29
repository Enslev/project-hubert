#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sensor.h"
#include "qrs.h"
#include "filters.h"
#include "filterStorage.h"

int main(int argc, char *argv[]){

	int counter = 0;

	// Setup filter storage
	struct xs x = {.next = 0, .size = 100};
	struct xs x_low = {.next = 0, .size = 100};
	struct xs x_high = {.next = 0, .size = 100};
	struct xs x_der = {.next = 0, .size = 100};
	struct xs x_sqr = {.next = 0, .size = 100};
	struct xs x_mwi = {.next = 0, .size = 100};

	// Zero initialise filter storage arrays
	for (int i = 0; i < x.size; i++){
		x.data[i] = 0;
		x_low.data[i] = 0;
		x_high.data[i] = 0;
		x_der.data[i] = 0;
		x_sqr.data[i] = 0;
		x_mwi.data[i] = 0;
	}

	// Define real-time data stream
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename, "r");
	int fileSize = 10000;

	while (counter < fileSize){
		// apply filters
		addNextData( &x, getNextData(file) );
		addNextData( &x_low, low_pass(&x, &x_low) );
		addNextData( &x_high, high_pass(&x_low, &x_high) );
		addNextData( &x_der, derivative(&x_high) );
		addNextData( &x_sqr, square(&x_der) );
		addNextData( &x_mwi, moving_window_integration(&x_sqr) );

		QRS(getPastEntry(&x_mwi, 0));

		counter++;
	}

	return 0;
}
