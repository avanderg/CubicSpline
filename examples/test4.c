#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../cube_spline.h"


int main(int argc, char *argv[]) {

    /* This code interpolates an arduino photoresistor voltage */

    /* Set my known x values and their corresponding y values
       You can check these just by trying e^0, e^.1, and e^.2 on a 
       calculator.
    */

    float x[3] = {0, 2.5, 5}; /* Voltage measured */
    float y[3] = {0, 1000, 2000}; /* Lux out */

    float x2[6] = {0.1, 0.6, 2.5, 4, 4.3, 5.0};
    float y2[6] = {0.1, 1, 10, 43, 100, 1000};
    /* We have 5 points */
    int num_points = 3;
    int num_points2 = 6;
    int i; /* loop index, as usual */
    /* Just some dumby values to test our interpolation, in the real world
       this part would probably be in loop and you'd get the value from a 
       sensor then interpolate it.
    */
    float vals[13] = {0.1, 0.6, 2.5, 4.3, 5, 0.2, 0.5, 2.7, 3, 4.2, 4.7,
        4.8, 4.9}; 
    /* result of the interpolation */
    float result;

    /* Struct S for interpolation info */
    S output;
    S *output_ptr = NULL;

    output.x = x;
    output.y = y;
    
    S output2;
    S* output2_ptr = NULL;

    output2.x = x2;
    output2.y = y2;

    /* out for checking return values */
    int out;

    output_ptr = nat_cubic_spline(num_points, &output);
    output2_ptr = nat_cubic_spline(num_points2, &output2);

    for (i=0; i<num_points-1; i++) {
        /* print polynomial coeffs to compare with a known cubic spline
           using these initial points
        */
        printf("i: %d\n", i);
        printf("a: %.6f, b: %.2f, c: %.2f, d:%.2f\n\n",
                output.a[i], output.b[i], 
                output.c[i], output.d[i]);

    }
    
    /* Now use our spline on each val we made up */
    for (i=0; i<13; i++) {

        /* Make sure to test the return value before we use the result */
        if ((out=evaluate(output_ptr, vals[i], &result)) < 0) {
            /* Should fail on none of the inputs */
           printf("I failed: %d\n", out);
        }
        /* print the input x value and the interpolated y */
        printf("input: %.2f, result: %.7f\n", vals[i], result); 
    }

    for (i=0; i<num_points2-1; i++) {
        /* print polynomial coeffs to compare with a known cubic spline
           using these initial points
        */
        printf("i: %d\n", i);
        printf("a: %.6f, b: %.2f, c: %.2f, d:%.2f\n\n",
                output2.a[i], output2.b[i], 
                output2.c[i], output2.d[i]);

    }
    
    /* Now use our spline on each val we made up */
    for (i=0; i<13; i++) {

        /* Make sure to test the return value before we use the result */
        if ((out=evaluate(output2_ptr, vals[i], &result)) < 0) {
            /* Should fail on none of the inputs */
           printf("I failed: %d\n", out);
        }
        /* print the input x value and the interpolated y */
        printf("input: %.2f, result: %.7f\n", vals[i], result); 
    }
}
