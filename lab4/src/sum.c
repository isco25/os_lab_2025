#include "sum.h"
#include <pthread.h>

int Sum(const struct SumArgs *args) {
    int sum = 0;
    if (args == NULL || args->array == NULL) {
        return 0;
    }
    
    for (int i = args->begin; i < args->end; i++) {
        sum += args->array[i];
    }
    return sum;
}

void *ThreadSum(void *args) {
    if (args == NULL) {
        return (void *)(size_t)0;
    }
    
    struct SumArgs *sum_args = (struct SumArgs *)args;
    return (void *)(size_t)Sum(sum_args);
}