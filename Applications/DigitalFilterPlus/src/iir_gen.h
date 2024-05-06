// iir_gen header

#include "dfplus.h"

void elliptic_filter_roots(filter_t* FilterSpecification);
void non_elliptic_filter_roots(filter_t* FilterSpecification);
void high_pass_transform(SLComplexRect_s* x, long n);
void bandpass_bandstop_transform(SLComplexRect_s* x, long n, long ftype, double w0);
