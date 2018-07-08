#ifndef __C_OBSCURE_H
#define __C_OBSCURE_H 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef double (*get_distance_t)(void *a, void *b);
typedef void (*get_center_t)(void *data, double *data_belong, double sum, int k,int m,int class_index, void *data_center, int data_length);

extern void c_obscure_init(void *data, int data_length, int data_size, double accuracy, int m, int loop_limit, get_distance_t _get_distance, int _k, get_center_t _get_center);
extern void c_obscure_run();
extern void c_obscure_destory();


#endif