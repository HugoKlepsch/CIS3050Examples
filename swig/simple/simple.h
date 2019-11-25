/*
 * Header file to declare the GSL wrapper function names
 *
 * $Id: gslMatrix.h 365 2007-03-09 18:50:45Z andrew $
 */
#ifndef	__SIMPLE_WRAPPER__
#define	__SIMPLE_WRAPPER__

typedef struct simple_data {
	float value_;
} simple_data;

/*
 * Provide prototypes for the functions we are declaring
 */
simple_data *simpleAlloc(float value);
void simpleFree(simple_data *s);
float simpleGetValue(simple_data *s);
void simpleSetValue(simple_data *s, float newValue);

#endif
