/* This is a pure C example, only difference in arduino usage would be to
   change the printf statements to Serial.print() and replace main with setup
   then implement some sort of loop. 
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../cube_spline.h"


int main(int argc, char *argv[]) {

    /* This code interpolates e^x near 0 */

    /* Set my known x values and their corresponding y values
       You can check these just by trying e^0, e^.1, and e^.2 on a 
       calculator.
    */

    float x[3] = {0, 0.1, 0.2};
    float y[3] = {1, 1.10517, 1.2214};
    /* We have 3 points */
    int num_points = 3;
    int i; /* loop index, as usual */
    /* Just some dumby values to test our interpolation, in the real world
       this part would probably be in loop and you'd get the value from a 
       sensor then interpolate it.
    */
    float vals[5] = {0, 0.2, 0.1, 0.15, 0.05}; 
    /* result of the interpolation */
    float result;

    /* Struct S for interpolation info */

    S output;
    S *output_ptr = NULL;
    /* out for checking return values */
    int out;

    output.x = x;
    output.y = y;

    output_ptr = nat_cubic_spline(num_points, &output);

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
    for (i=0; i<5; i++) {

        /* Make sure to test the return value before we use the result */
        if ((out=evaluate(output_ptr, vals[i], &result)) < 0) {
            /* Should fail on none of the inputs */
           printf("I failed: %d\n", out);
        }
        /* print the input x value and the interpolated y */
        printf("input: %.2f, result: %.7f\n", vals[i], result); 
    }


   
}

#ifdef ARDUINO_EX
/* On an Arduino the code would look something like this: */

    /* This code interpolates e^x near 0 */

    /* Make parameters global so we don't lose them when
       we exit the setup or loop function. Make S global so we can get it
       whenever we want.
    */

    S output;
    S *output_ptr = NULL;
    float x[3] = {0, 0.1, 0.2};
    float y[3] = {1, 1.10517, 1.2214};
    int num_points = 3;

    void setup() {
        /* Do Arduino setup and what not
               .
               .
               .
               .
        */
        /* Make the spline */
        output_ptr = nat_cubic_spline(num_points, &output);
    }

    void loop() {
        float val;
        float result;
        int out; /* for errors */
        

        val = read_from_some_sensor_or_something();

        if ((out=evaluate(output, val, &result)) < 0) {
           Serial.print("I failed: ");
           Serial.println(out);
        }
        /* result now has interpolated value, can write it to sd card
           or whatever, I just print it
        */
        Serial.print("input:");
        Serial.print(val);
        Serial.print(", result: ");
        Serial.println(result);
    }

#endif
