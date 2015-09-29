struct xs {
	int next;
	int size;
	int data[100];
};

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
	entry = abs(entry);

	if (entry >= s->size){
		printf("ERROR! entry too large in: getPastEntry");
		exit(1);
	}

	int index = (s->next - 1) - entry;
	index = (index < 0) ? s->size - abs(index) : index ;

	return s->data[index];
}
