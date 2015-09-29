int averageRR(int rrArr[], int size);
int searchBack(int peaks[500][2], int head, int t2);
int findPeak(int newValue);

void QRS(int value) {

	static int SPKF = 5000;			// An estimate of the value of an R-peak.
	static int NPKF = 700;			// An estimate of the value of a noise peak.
	static int THRESHOLD1 = 3500;	// All peaks higher than this value is classified as an R-peak.
	static int THRESHOLD2 = 1750;	// Used during the searchback procedure (Generally 0.5 * THRESHOLD1).
	static int RR_AVERAGE1 = 165;	// The average of the 8 most recent RR-intervals regardless of their value.
	static int RR_AVERAGE2 = 165;	// The average of the 8 most recent RR-intervals, for which the corresponding R-peak is higher than THRESHOLD1.
	static int RR_LOW = 150;		// Defined as 92% of RR_AVERAGE2.
	static int RR_HIGH = 190;		// Defined as 116% of RR_AVERAGE2.
	static int RR_MISS = 270;		// Defined as 166% of RR_AVERAGE2.

	static int peaks[500][2] = {
			{5000, 0}
	};
	static int rPeaks[500][2];
	static int recentRR[8] = { 0 };
	static int recentRR_OK[8] = { 0 };
	static int peakHead = 0;
	static int rHead = 2;
	static int recentHead = 0;
	static int recentOkHead = 0;

	static int n = 0;
	static double sampleRate = 250.0;
	static double totalGroupDelay = 0.1875; // 25 ms + 80 ms + 10 ms + 72.5 ms = 187.5 ms

	n++;


	// Calculate RR
	int rIndexNow = (rHead % 500 == 0) ? 499 : (rHead % 500)-1 ;
	int RR = n - rPeaks[rIndexNow][1];

	if (RR == RR_MISS * 5){
		printf("WARNING: 5 successive R-peaks missed \n");
	}

	int peak = findPeak(value);
	if (!peak) {
		return;
	}

	int peakIndex = peakHead++ % 500;
	peaks[peakIndex][0] = peak;
	peaks[peakIndex][1] = n;

	if (peak <= THRESHOLD1) {
		NPKF = 0.125 * peak + 0.875 * NPKF;
		THRESHOLD1 = NPKF + 0.25 * (SPKF - NPKF);
		THRESHOLD2 = 0.5 * THRESHOLD1;
		return;
	}

	if (RR > RR_LOW &&
		RR < RR_HIGH) {

		if (peak < 2000){
			printf("WARNING: R-peak below 2000 \n");
		};

		int pulse = (int) (sampleRate * 60.0) / ((double) RR );
		double time = (n/sampleRate) - totalGroupDelay;

		printf("time: %.2f s, value: %d, pulse: %d ", time, peak, pulse);
		printf("\n");

		int rIndex = rHead++ % 500;
		rPeaks[rIndex][0] = peak;
		rPeaks[rIndex][1] = n;

		//Store RR
		int rrIndex =  recentHead++ % 8;
		recentRR[rrIndex] = RR;

		//Store RR_OK
		int rrOkIndex =  recentOkHead++ % 8;
		recentRR_OK[rrOkIndex] = RR;

		SPKF = 0.125 * peak + 0.875 * SPKF;
		RR_AVERAGE1 = averageRR(recentRR, 8);
		RR_AVERAGE2 = averageRR(recentRR_OK, 8);
		RR_LOW = RR_AVERAGE2 * 0.92;
		RR_HIGH = RR_AVERAGE2 * 1.16;
		RR_MISS = RR_AVERAGE2 * 1.66;
		THRESHOLD1 = NPKF + 0.25 * (SPKF - NPKF);
		THRESHOLD2 = 0.5 * THRESHOLD1;

		return;
	}

	if (RR <= RR_MISS) {
		return;
	}

	// searchback
	double peak2[2];
	int peak2Index = searchBack(peaks, peakHead, THRESHOLD2);
	peak2[0] = peaks[peak2Index][0];
	peak2[1] = peaks[peak2Index][1];

	int rIndex = rHead++ % 500;
	rPeaks[rIndex][0] = peak2[0];
	rPeaks[rIndex][1] = peak2[1];

	int rrIndex =  recentHead++ % 8;
	recentRR[rrIndex] = RR;

	SPKF = 0.25 * peak2[0] + 0.75 * SPKF;
	RR_AVERAGE1 = averageRR(recentRR, 8);
	RR_LOW = 0.92 * RR_AVERAGE1;
	RR_HIGH = 1.16 * RR_AVERAGE1;
	RR_MISS = 1.66 * RR_AVERAGE1;
	THRESHOLD1 = NPKF + 0.25 * (SPKF - NPKF);
	THRESHOLD2 = 0.5 * THRESHOLD1;

	return;

}

int averageRR(int rrArr[], int size) {
	int avg = 0;
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (rrArr[i] > 0) {
			count++;
		}
		avg += rrArr[i];
	}
	return avg / count;
}

int searchBack(int peaks[500][2], int head, int t2) {

	for (int i = 0; i < 500; i++) {
		head--;
		if (head < 0) { head += 500; };
		int index = head % 500;

		if (peaks[index][0] > t2) {
			return index;
		}
	}

	return 0;

}

int findPeak(int newVal) {
	static int oldVal = 0;
	static int ascending = 1;

	if (ascending && oldVal > newVal) {
		int peak = oldVal;
		oldVal = newVal;
		ascending = 0;

		return peak;
	}

	if (!ascending && oldVal < newVal) {
		ascending = 1;
	}

	oldVal = newVal;

	return 0;
}
