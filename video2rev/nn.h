#ifndef NN_H
#define NN_H

#include<stdio.h>
#include<stddef.h>
#include<math.h>

#ifndef NN_MALLOC
#include<stdlib.h>
#define NN_MALLOC malloc
#endif

#ifndef NN_ASSERT
#include<assert.h>
#define NN_ASSERT assert
#endif

typedef struct{
    size_t rows;
    size_t cols;
    size_t stride;
    float *es;
} Mat;

#define MAT_AT(m, i, j) (m).es[(i)*(m).stride + (j)]

float rand_float(void);
float sigmoidf(float x);

Mat mat_alloc(size_t rows, size_t cols);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low, float high);
Mat mat_dot(Mat dst, Mat a, Mat b);
Mat mat_row(Mat m, size_t row);
void mat_copy(Mat dst, Mat src);
Mat mat_sum(Mat dst, Mat a);
void mat_sig(Mat m);
void mat_print(Mat m, const char *name);
#define MAT_PRINT(m) mat_print(m, #m)

#endif

#ifdef NN_IMPLEMENTATION


float rand_float(void){
    return (float) rand() / (float) RAND_MAX;
}

float sigmoidf(float x){
    return 1.f / (1.f + expf(-x));
}

Mat mat_alloc(size_t rows, size_t cols){
    Mat m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.es = NN_MALLOC(sizeof(*m.es) * rows * cols);
    NN_ASSERT(m.es != NULL);
    return m;
}

void mat_fill(Mat m, float x){
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            MAT_AT(m, i, j) = x;
        }
    }
}

void mat_rand(Mat m, float low, float high){
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            MAT_AT(m, i, j) = rand_float()*(high - low) + low;
        }
    }
}

Mat mat_dot(Mat dst, Mat a, Mat b){    
    NN_ASSERT(a.cols == b.rows);
    NN_ASSERT(dst.cols == b.cols);
    NN_ASSERT(dst.rows == a.rows);
    size_t n = a.cols;
    
    for(size_t i = 0; i < dst.rows; i++){
        for(size_t j = 0; j < dst.cols; j++){
            MAT_AT(dst, i, j) = 0;
            for(size_t k = 0; k < n; k++){
                MAT_AT(dst, i, j) += MAT_AT(a, i, k)*MAT_AT(b, k, j);
            }
        }
    }
}


Mat mat_row(Mat m, size_t row){
    return (Mat){
        .rows = 1,
        .cols = m.cols,
        .stride = m.stride,
        .es = &MAT_AT(m, row, 0),
    };
}

void mat_copy(Mat dst, Mat src){
    NN_ASSERT(dst.rows == src.rows);
    NN_ASSERT(dst.cols == src.cols);
    for(size_t i = 0; i< dst.rows; i++){
        for(size_t j = 0; j< dst.cols; j++){
            MAT_AT(dst, i, j) = MAT_AT(src, i, j);
        }
    }
}


Mat mat_sum(Mat dst, Mat a){
    NN_ASSERT(dst.rows = a.rows);
    NN_ASSERT(dst.cols = a.cols);
    for(size_t i = 0; i < dst.rows ; i++){
        for(size_t j = 0; j < dst.cols; j++){
            MAT_AT(dst, i, j) += MAT_AT(a, i, j);
        }
    }
}

void mat_sig(Mat m){
    for(size_t i = 0; i< m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            MAT_AT(m, i, j) = sigmoidf(MAT_AT(m, i, j));
        }
    }
}

void mat_print(Mat m, const char *name){
    printf("%s = [\n", name);
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            printf("    %f ", MAT_AT(m, i, j));
        }
        printf("\n");
    }
    printf("]\n");
}



#endif

