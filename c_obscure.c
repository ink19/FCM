#include "c_obscure.h"

void *data = NULL, *data_center;
double *data_belong;
double *data_distence;
int data_length = 0, loop_limit = 0, m = 0, data_size = 0, k = 0;
double accuracy = 0;
get_distance_t get_distance;
get_center_t get_center;

extern void c_obscure_init(void *_data, int _data_length, int _data_size, double _accuracy, int _m, int _loop_limit, get_distance_t _get_distance, int _k, get_center_t _get_center) {
    data = _data;
    data_length = _data_length;
    data_size = _data_size;
    accuracy = _accuracy;
    m = _m;
    loop_limit = _loop_limit;
    get_distance = _get_distance;
    k = _k;
    data_belong = (double *) malloc(sizeof(double) * k * data_length);
    data_distence = (double *) malloc(sizeof(double) * k * data_length);
    data_center = malloc(data_size * k);
    get_center = _get_center;
}
extern void c_obscure_run() {
    int loop_times = 0, loop_i, loop_j, loop_k, max_belong_index;
    double last_J = 0, J = 0, sum = 0, max_belong;
    srand((unsigned int) time(NULL));

    for(loop_i = 0; loop_i < k; loop_i++) {
        memcpy(data_center + loop_i * data_size, data + (rand() % data_length) * data_size, data_size);
    }

    while(loop_times < loop_limit) {
        loop_times ++;
        for(loop_i = 0; loop_i < data_length; ++loop_i) {
            for(loop_j = 0; loop_j < k; ++loop_j)
                data_distence[loop_i *k + loop_j] = (*get_distance)(data + loop_i * data_size, data_center + loop_j * data_size);
        }

        for(loop_i = 0; loop_i < data_length; ++loop_i) {
            for(loop_j = 0; loop_j < k; ++loop_j) {
                sum = 0;
                for(loop_k = 0; loop_k < k; ++ loop_k) {
                    max_belong = data_distence[loop_i * k + loop_k];
                    sum += pow(data_distence[loop_i * k + loop_j] / (max_belong?max_belong:0.0000001), 2.0 / (m - 1));
                }
                data_belong[loop_i * k + loop_j] = 1/(sum?sum:0.0000001);
            }

        }

        last_J = J;
        J = 0;
        for(loop_i = 0; loop_i < data_length; ++loop_i) {
            for(loop_j = 0; loop_j < k; ++loop_j) {
                J += pow(data_belong[loop_i * k + loop_j], m) * pow(data_belong[loop_i * k + loop_j], 2);
            }
        }

        if(pow(last_J - J, 2) < accuracy) break;

        for(loop_j = 0; loop_j < k; ++loop_j) {
            sum = 0;
            for(loop_i = 0; loop_i < data_length; ++ loop_i) {
                sum += pow((data_belong[loop_i * k + loop_j]), m);
            }

            (*get_center)(data, data_belong, sum, k, m, loop_j, data_center, data_length);
        }
    }

    for(loop_i = 0; loop_i < data_length; ++loop_i) {
        max_belong = 0;
        max_belong_index = 0;
        for(loop_j = 0; loop_j < k; ++loop_j) {
            if(data_belong[loop_i * k + loop_j] > max_belong) {
                max_belong = data_belong[loop_i * k + loop_j];
                max_belong_index = loop_j;
            }
        }
        memcpy(data + loop_i * data_size, data_center + max_belong_index * data_size, data_size);
    }
}
extern void c_obscure_destory() {
    free(data_belong);
    free(data_distence);
    free(data_center);
}