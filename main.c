#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "jpeg_manage.h"
#include "c_obscure.h"

int mem2inta(JSAMPLE *jpeg_memory, int *int_array, int len);
int ina2mem(JSAMPLE *jpeg_memory, int *int_array, int len);

double get_distance_m(void *a, void *b) {
    int *fa = a, *fb = b;
    return sqrt((fa[0] - fb[0]) * (fa[0] - fb[0]) + (fa[1] - fb[1]) * (fa[1] - fb[1]) + (fa[2] - fb[2]) * (fa[2] - fb[2]));
}

void get_center_m(void *data, double *data_belong, double sum, int k, int m, int class_index, void *data_center, int data_length) {
    double sum_center[3];
    int * data_i = (int *)data;
    for(int loop_i = 0; loop_i < data_length; ++loop_i) {
        sum_center[0] += pow(data_belong[loop_i * k + class_index], m) * data_i[loop_i * 3 + 0];
        sum_center[1] += pow(data_belong[loop_i * k + class_index], m) * data_i[loop_i * 3 + 1];
        sum_center[2] += pow(data_belong[loop_i * k + class_index], m) * data_i[loop_i * 3 + 2];
    }
    sum_center[0] /= sum;
    sum_center[1] /= sum;
    sum_center[2] /= sum;
    data_i = (int *)data_center;
    data_i[class_index * 3 + 0] = (int) sum_center[0];
    data_i[class_index * 3 + 1] = (int) sum_center[1];
    data_i[class_index * 3 + 2] = (int) sum_center[2];
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("usage: ./main inname outname\n");
        return 1;
    }
    JSAMPLE *jpeg_memory;
    int width, height, length;
    int *pix_array;
    jpeg_manage_file_to_memory(argv[1], &jpeg_memory, &width, &height);
    length = width * height;
    pix_array = (int *)malloc(sizeof(int) * length * 3);
    mem2inta(jpeg_memory, pix_array, length * 3);

    c_obscure_init(pix_array
    , length
    , sizeof(int) * 3
    , 0.1
    , 2
    , 100
    , get_distance_m
    , 4
    , get_center_m);
    c_obscure_run();
    c_obscure_destory();

    ina2mem(jpeg_memory, pix_array, length * 3);
    jpeg_manage_memory_to_file(argv[2], jpeg_memory, width, height);

    free(jpeg_memory);
    return 0;
}

int mem2inta(JSAMPLE *jpeg_memory, int *int_array, int len) {
    for(int loop_i = 0; loop_i < len; loop_i++) {
        *(int_array + loop_i) = *(jpeg_memory + loop_i);
    }
    return 0;
}

int ina2mem(JSAMPLE *jpeg_memory, int *int_array, int len) {
    for(int loop_i = 0; loop_i < len; loop_i++) {
        *(jpeg_memory + loop_i) = *(int_array + loop_i);
    }
    return 0;
}