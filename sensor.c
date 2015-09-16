#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"


int getNextData(FILE *file){
	// Implement me according to the Assignment 1 manual
	/*
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename, "r");
	int current;
	*/
	int data;
	fscanf(file, "%i", &data);

	return data;
}
