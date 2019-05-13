#include <stdlib.h>
#ifndef NUMBER_POINTS 
#define NUMBER_POINTS 20
#endif

typedef struct S S;
struct S {
    float *a;
    float *b;
    float *c;
    float *d;
    float *x;
    float *y;
    int num_points;
};

#ifdef __cplusplus
extern "C" {
#endif
S *nat_cubic_spline(float *x, float *y, int num_points); 
#ifdef __cplusplus
}
#endif

void build_A_matrix(float *x_delta, int num_points, 
        float A[][NUMBER_POINTS]) ; 
float *solve_matrix(float *x, float *h, int num_points, 
        float A[][NUMBER_POINTS]); 
float *build_h_vector(float *h, float *x_delta, float *a, int num_points); 
float *build_b_vector(float *b, float *x_delta, float *y_delta, float *c, 
                        int num_points);
float *build_d_vector(float *d, float *x_delta, float *c, int num_points); 

#ifdef __cplusplus
extern "C" {
#endif
int evaluate(S *function, float val, float *result); 
#ifdef __cplusplus
}
#endif

float spline_func(S *function, float val, int i); 
int almost_equals(float a, float b);
void *safe_malloc(size_t size);
