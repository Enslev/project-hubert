#ifndef FILTERS_H_
#define FILTERS_H_
#include "filterStorage.h"

int low_pass(struct xs *x, struct xs *y);
int high_pass(struct xs *x, struct xs *y);
int derivative(struct xs *x);
int square(struct xs *x);
int moving_window_integration(struct xs *x);

#endif /* FILTERS_H_ */
