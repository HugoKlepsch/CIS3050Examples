%module simple

%{
#include "simple.h"
%}

simple_data *simpleAlloc(float value);
void simpleFree(simple_data *s);
float simpleGetValue(simple_data *s);
void simpleSetValue(simple_data *s, float newValue);
