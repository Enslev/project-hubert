#include <stdio.h>

int main(int argc, char *argv[]) {

	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename, "r");
	int value;
	int max = 0;

	while (value != EOF) {

		fscanf(file, "%i", &value);
		max = value > max ? value : max;

	}

	printf("Max value: %d", max);

	return 0;
}
