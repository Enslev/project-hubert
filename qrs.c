void QRS(int value) {
	static double SPKF = 0;			// An estimate of the value of an R-peak.
	static double NPKF = 0;			// An estimate of the value of a noise peak.
	static double THRESHOLD1 = 0;	// All peaks higher than this value is classified as an R-peak.
	static double THRESHOLD2 = 0;	// Used during the searchback procedure (Generally 0.5 * THRESHOLD1).
	static double RR_AVERAGE1 = 0;	// The average of the 8 most recent RR-intervals regardless of their value.
	static double RR_AVERAGE2 = 0;	// The average of the 8 most recent RR-intervals, for which the corresponding R-peak is higher than THRESHOLD1.
	static double RR_LOW = 0;		// Defined as 92% of RR_AVERAGE2.
	static double RR_HIGH = 0;		// Defined as 116% of RR_AVERAGE2.
	static double RR_MISS = 0;		// Defined as 166% of RR_AVERAGE2.

	static int timer = 0;

	static double peaks[500];
	static double rPeaks[500];
	static double recentRR[8];
	static double recentRR_OK[8];
	static int peakHead = 0;
	static int rHead = 0;
	static int recentHead = 0;
	static int recentOkHead = 0;

	timer++;


	int peak = findPeak(value);
	if (!peak) {
		return;
	}

	int peakIndex = peakHead++ % 500;
	peaks[peakIndex] = peak;

	if (peak <= THRESHOLD1) {
		NPKF = 0.125 * peak + 0.875 * NPKF;
		THRESHOLD1 = NPKF + 0.25 * (SPKF - NPKF);
		THRESHOLD2 = 0.5 * THRESHOLD1;
		return;
	}

	// Calculate RR
	int RR = timer;
	timer = 0;

	if (RR > RR_LOW &&
		RR < RR_HIGH) {

		//Store  peak in RPeaks
		int rIndex = rHead++ % 500;
		rPeaks[rIndex] = peak;

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

	// TODO: what if no peak2 is found??
	double peak2 = searchBack(peaks, peakHead, THRESHOLD2);

	int rIndex = rHead++ % 500;
	rPeaks[rIndex] = peak2;

	int rrIndex =  recentHead++ % 8;
	recentRR[rrIndex] = RR;

	SPKF = 0.25 * peak2 + 0.75 * SPKF;
	RR_AVERAGE1 = averageRR(recentRR, 8);
	RR_LOW = 0.92 * RR_AVERAGE1;
	RR_HIGH = 1.16 * RR_AVERAGE1;
	RR_MISS = 1.66 * RR_AVERAGE1;
	THRESHOLD1 = NPKF + 0.25 * (SPKF - NPKF);
	THRESHOLD2 = 0.5 * THRESHOLD1;

	return;

}

double averageRR(int rrArr[], int size) {
	double avg = 0;
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (rrArr[i] > 0) {
			count++;
		}
		avg += rrArr[i];
	}
	return avg / (double) count;
}

double searchBack(double peaks[], int head, double t2) {

	for (int i = 0; i < 500; i++) {
		head--;
		if (head < 0) { head += 500; };
		int index = head % 500;

		if (peaks[index] > t2) {
			return (double) peaks[index];
		}
	}

	return (double) 0.0;

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
