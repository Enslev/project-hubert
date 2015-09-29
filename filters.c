int low_pass(struct xs *x, struct xs *y){
	int y1 = getPastEntry(y, 0); // (y, 1-1)
	int y2 = getPastEntry(y, 1); // (y, 2-1)
	int xn = getPastEntry(x, 0);
	int x6 = getPastEntry(x, 6);
	int x12 = getPastEntry(x, 12);

	return 2*y1 - y2 + (xn - 2*x6 + x12)/32;
}

int high_pass(struct xs *x, struct xs *y){
	int y1 = getPastEntry(y, 0); // (y, 1-1)
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
