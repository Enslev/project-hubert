
struct xs {
	int head;
	int size;
	int data[12];
};


void addNewData(struct xs arr, int data){
	int index = (arr.head % arr.size == 1) ? 0 : arr.head++;
	arr.data[index] = data;
}

int getPastEntry(struct xs arr, int entry){
	// if index > arr.size throw error

	int index = (arr.head - entry) % arr.size;
	return arr.data[index];
}

