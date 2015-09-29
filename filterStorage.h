#ifndef FILTERSTORAGE_H_
#define FILTERSTORAGE_H_

struct xs {
	int next;
	int size;
	int data[100];
};

void addNextData(struct xs *s, int dataPoint);
int getPastEntry(struct xs *s, int entry);

#endif /* FILTERSTORAGE_H_ */
