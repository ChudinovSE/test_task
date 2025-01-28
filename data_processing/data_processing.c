/*!
\file
\brief Фаил с реализацией функций для обработки тестовых данных 

Данный файл содержит в себе реализации функций, 
используемых в демонстрационной программе для обработки данных
реализация утилиты п.3 тестового задания
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./stat_data/stat_data.h"
#include "./metrics/metrics.h"

int generation_rnd_data_files(char *path_file_a, int *n_a, char *path_file_b, int *n_b)
{
    StatData * data_array_a = malloc(*n_a * sizeof(StatData));
    if (!data_array_a){
        return 1;
    };
    StatData * data_array_b = malloc(*n_b * sizeof(StatData));
    if (!data_array_a){
        return 1;
    };
    generation_rnd_data_array(data_array_a, n_a);
    generation_rnd_data_array(data_array_b, n_b);
    
    if (!store_dump(path_file_a, data_array_a, n_a)){
        return 2;
    };
    if (!store_dump(path_file_b, data_array_b, n_b)){
        return 2;
    };
    free(data_array_a);
    free(data_array_b);
    return 0;

}

int processing_data(char *path_file_a, char *path_file_b, char *path_file_res, TimeMetrics *metrics)
{
    int n_a = 0;
    struct timespec tick, tock;
    clock_gettime(CLOCK_REALTIME, &tick);
    StatData *data_array_a = load_dump(path_file_a, &n_a);
    if (!data_array_a){
        return 1;
    };
    clock_gettime(CLOCK_REALTIME, &tock);
    metrics->load_dump_a_ns = tock.tv_nsec - tick.tv_nsec;

    int n_b = 0;
    clock_gettime(CLOCK_REALTIME, &tick);
    StatData *data_array_b = load_dump(path_file_b, &n_b);
    if (!data_array_b){
        return 1;
    };
    clock_gettime(CLOCK_REALTIME, &tock);
    metrics->load_dump_b_ns = tock.tv_nsec - tick.tv_nsec;

    print_stat_data_table(data_array_a, &n_a);
    print_stat_data_table(data_array_b, &n_b);

    int n_res = 0;
    clock_gettime(CLOCK_REALTIME, &tick);
    StatData *data_array_res = join_dump(data_array_a, &n_a, data_array_b, &n_b, &n_res);
    if (data_array_res == NULL){
        return 2;
    };
    clock_gettime(CLOCK_REALTIME, &tock);
    metrics->join_dumps_ns = tock.tv_nsec - tick.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &tick);
    sort_dump(data_array_res, &n_res);
    clock_gettime(CLOCK_REALTIME, &tock);
    metrics->sort_dump_ns = tock.tv_nsec - tick.tv_nsec;

    print_stat_data_table(data_array_res, &n_res);

    clock_gettime(CLOCK_REALTIME, &tick);
    if (store_dump(path_file_res, data_array_res, &n_res)){
        return 3;
    };
    clock_gettime(CLOCK_REALTIME, &tock);
    metrics->save_res_dump_ns = tock.tv_nsec - tick.tv_nsec;
    return 0;

}
