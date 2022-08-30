#include "digitalFilter.h"


digitalFilter::digitalFilter(int filterOrder_userdef, bool isIIR)
{
	filterOrder = filterOrder_userdef;
        IIR = isIIR;
	
	b = new double [filterOrder + 1];
	a = new double [filterOrder + 1];

	x = new double [filterOrder + 1];
	u = new double [filterOrder + 1];

	// Initialize the arrays with zeros
	for(int i = 0; i < (filterOrder + 1); i++)
	{
		b[i] = 0.0;
		a[i] = 0.0;
		x[i] = 0.0;
		u[i] = 0.0;
	}
}	

digitalFilter::digitalFilter(int filterOrder_userdef, bool isIIR, double *b_userdef, double *a_userdef)
{
	filterOrder = filterOrder_userdef;
        IIR = isIIR;
	
	b = new double [filterOrder + 1];
	a = new double [filterOrder + 1];

	x = new double [filterOrder + 1];
	u = new double [filterOrder + 1];

	// Initialize the arrays
	for(int i = 0; i < (filterOrder + 1); i++)
	{
		b[i] = b_userdef[i];
		a[i] = a_userdef[i];
		x[i] = 0.0;
		u[i] = 0.0;
	}
}


double digitalFilter::getNextFilteredValue(double u_current)
{
	/* Shift x2 and u2 vectors, losing the last elements and putting new u2 value in zeroth spot. */
	for (int i = filterOrder ; i > 0 ; i--) {
		x[i] = x[i-1];
		u[i] = u[i-1];
	}
	u[0] = u_current; 

	/* Simulate system. */
	double output = b[0] * u[0];
	  
        // if we have an IIR filter            
        if(IIR)
        {
            for (int i = 1 ; i < (filterOrder+1) ; i++) {
                    output += b[i] * u[i] - a[i] * x[i];
            }
        }

       // if we have an FIR filter
       else
       {
            for (int i = 1 ; i < (filterOrder+1) ; i++) {
                    output += b[i] * u[i];
            }
       }

	/* Put the result in shared memory and in the x2 vector. */
	x[0] = output;

	return output;
}

digitalFilter::~digitalFilter(void)
{
	delete[] x;
	delete[] u;
	delete[] a;
	delete[] b;
}
