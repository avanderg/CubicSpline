#include <stdlib.h>
#include "spline.h"


S *nat_cubic_spline(float *x, float *y, int num_points) {
    float x_delta[num_points-1]; /* x differences */
    float y_delta[num_points-1]; /* y diffrences */
    float A[num_points][num_points]; /* matrix for solving for c */
    float h[num_points]; /* vector for solving for c */
    /* Output struct */ 
    S *output_fun; 
    /* Output parameters */ 
    float *a;
    float *b;
    float *c;
    float *d;

    /* Goal: create interpolating functions of the form:
       S_j(x) = a_j(x) + b_j(x - x_j) + c_j(x - x_j)^2 + d_j(x - x_j)^3
       where 0 < j < num_points
    */

    /* First, want to solve: Ac = h where A is a matrix and c and h are 
       vectors 
    */

    int i; /* loop index */

    /* If there aren't enough points to interpolate, bail */
    if (num_points < 3) {
        return NULL;
    }

    /* Allocate space on the heap for all output parameters */
    output_fun = safe_malloc(sizeof(S));
    a = safe_malloc(num_points*sizeof(float));
    b = safe_malloc(num_points*sizeof(float));
    c = safe_malloc(num_points*sizeof(float));
    d = safe_malloc(num_points*sizeof(float));

    /* Assign parameters to our S struct */
    output_fun->a = a;
    output_fun->b = b;
    output_fun->c = c;
    output_fun->d = d;
    output_fun->x = x;
    output_fun->y = y;
    output_fun->num_points = num_points;

    /* Build x diff and y diff */
    for (i=1; i<num_points; i++) {
        x_delta[i-1] = x[i] - x[i-1];
        y_delta[i-1] = y[i] - y[i-1];
    }

    /* Build "a" vector (just y) */
    for (i=0; i<num_points; i++) {
        a[i] = y[i];
    }


    /* Build A matrix */
    build_A_matrix(x_delta, num_points, A);


    /* Build h vector */
    build_h_vector(h, x_delta, a, num_points);

    /* Solve matrix equation for c vector (Ac = h) */
    solve_matrix(c, h, num_points, A);

    /* Build b vector */
    build_b_vector(b, x_delta, y_delta, c, num_points);


    /* Build d vector */
    build_d_vector(d, x_delta, c, num_points);
    
    /* Return S struct containing all the coeffs and init vals */
    return output_fun;
}

void build_A_matrix(float *x_delta, int num_points, 
        float A[][num_points]) {
    
    int i;

    /* Set top and bottom corners */
    A[0][0] = 1;
    A[num_points-1][num_points-1] = 1;

    /* Fill in the matrix by natural cubic spline algorithm */
    for (i=1; i<num_points-1; i++) {
        A[i][i-1] = x_delta[i-1];
        A[i][i] = 2*(x_delta[i-1]+x_delta[i]);
        A[i][i+1] = x_delta[i];
    }

}

float *build_h_vector(float *h, float *x_delta, float *a, int num_points) {
    int i;

    /* Set top and bottom */
    h[0] = 0.;
    h[num_points-1] = 0.;

    /* Fill in the vector by natural cubic spline algorithm */
    for (i=1; i<num_points-1; i++) {
        h[i] = 3.*((a[i+1]-a[i])/x_delta[i] - (a[i]-a[i-1])/x_delta[i-1]);
    }


    return h;
}

float *solve_matrix(float *x, float *h, int num_points, 
        float A[][num_points]) {
    /* Solves tridiagonal matrix equation Ax = h 
       for tridiagonal matrix A using Thomas' algorithm.
       This requires the matrix to be diagonally dominant or symmetric 
       positive definite. This should always be the case for our spline.
    */ 

    float a[num_points]; /* values to the left of diagonal of A */
    float b[num_points]; /* values on the diagonal of A */
    float c[num_points]; /* values to the right of diagonal of A */
    int i; /* loop index */
    float w[num_points]; /* used as a temp variable */

    /* Set end points */
    b[0] = 1.;
    c[0] = 0.;

    a[num_points-1] = 0;
    b[num_points-1] = 1;

    /* Build a, b, c */
    for (i=1; i<num_points-1; i++) {
        a[i] = A[i][i-1];
        b[i] = A[i][i];
        c[i] = A[i][i+1];
    }

   /* Apply Thomas' algorithm */
   for (i=1; i<num_points; i++) {
       w[i] = a[i] / b[i-1];
       b[i] = b[i] - w[i]*c[i-1];
       h[i] = h[i] - w[i]*h[i-1];

   } 

   /* Back substitute x */
   x[num_points-1] = h[num_points-1] / b[num_points-1];

   for (i=num_points-2; i>=0; i--) {
       x[i] = (h[i] - c[i]*x[i+1]) / b[i];
   }

   return x;
}

float *build_b_vector(float *b, float *x_delta, float *y_delta, float *c, 
                                        int num_points) {
    int i; /* loop index */

    /* Build b by natural cubic spline */
    for (i=0; i<num_points-1; i++) {
        b[i] = y_delta[i]/x_delta[i] - x_delta[i]/3*(2*c[i] + c[i+1]);
    }

    return b;
}

float *build_d_vector(float *d, float *x_delta, float *c, int num_points) {

    int i; /* loop index */

    /* Build d by natural cubic spline */
    for (i=0; i<num_points-1; i++) {
        d[i] = (c[i+1] - c[i]) / (3*x_delta[i]);
    }
    
    return d;
}

int evaluate(S *function, float val, float *result) {
    /* Use the interpolation to evaluate a val, answer stored in
       result
    */

    int i; /* loop index */

    /* if the val is less than the smallest value, outside of range,
       bail.
    */
    if (val < function->x[0]) {
        /* -1 represents too small of a val ... not sure how errno is used
           in arduino so I'm using return values to set error types.
        */
        return -1;
    }
    
    /* If the val is greater than the largest value, outside of range,
       bail.
    */
    else if (val > function->x[function->num_points-1]) {
        /* Set error val */
        return -2;
    }
    
    for (i=0; i<function->num_points-1; i++) {
        /* If val equals an element in my x array, just return the 
           corresponding y val
        */ 
        if (almost_equals(val, function->x[i])) {
            *result = function->y[i];
            /* return val of 0 means no errors */
            return 0; 
        }
        /* Note: 
           Some clock cycles are definitely wasted here, could probably
           check the  first element before loop starts and drop checking the 
           ith element and only check the i+1st element. This runs plenty 
           fast for my uses though.
        */

        /* Check the next element also, since I use the range next */
        else if (almost_equals(val, function->x[i+1])) {
            *result = function->y[i+1];
            return 0;
        }
        else if (val > function->x[i] && val < function->x[i+1]) {
            /* If the val falls between 2 initial x values, find the value the
               interpolation gives.
            */
            *result = spline_func(function, val, i);
            return 0;
        }
    } 

    /* Something has gone horribly wrong */
    return -3;
}

float spline_func(S *function, float val, int i) {
    /* This function selects the appropriate spline function and evaluates
       that function at val.
    */

    /* Each p is a different term in the polynomial (number after p is the 
       power)
    */
    float p0;
    float p1;
    float p2;
    float p3;

    /* It's useful to copy and paste the functional form of our spline from 
       above:

       S_i(x) = a_i(x) + b_i(x - x_i) + c_i(x - x_i)^2 + d_i(x - x_i)^3
       where 0 < i < num_points.

       We take the ith element from the a, b, c, d, and x arrays for the 
       {letter}_i terms. The x term is val in this function. S_i(x) is the 
       returned value from the interpolation.
    */

    /* This could waste some space and clock cycles, depending on how smart the
       compiler is, could make it a one liner ... but ick.
    */
    p0 = function->a[i];
    p1 = function->b[i]*(val - function->x[i]);
    p2 = function->c[i]*(val - function->x[i])*(val - function->x[i]);
    p3 = function->d[i]*(val - 
            function->x[i])*(val - function->x[i])*(val - function->x[i]);
    return p0 + p1 + p2 + p3;
}

int almost_equals(float a, float b) {
    /* Comparing floats for exact equality is shady, they won't ever be
       exactly the same, so use this function to test if they're close enough.
    */

    float c = a - b; /* c is the difference */

    /* Make sure c is positive */
    if (c < 0) {
        c = -1*c;
    }
    
    /* If the difference is sufficiently small, return 1 (true) */
    if (c < 0.00000001) {
        return 1;
    }
    
    /* Otherwise, return 0 (false) */
    else {
        return 0;
    }
}

void *safe_malloc(size_t size) {
    /* Error checks malloc so I don't have to write so many if statements */

    void *ptr;
    if (!(ptr = malloc(size))) {
        /* If malloc failed, exit ... really wish I could report the error */
        exit(1);
    }
    return ptr;
}
