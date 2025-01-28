/*!
\file
\brief Фаил запуска тестирования утилиты в соответствии с п.4 тестового задания

Данный файл содержит в себе реализацию алгоритма тестирования утилиты обработки данных,
обработки ошибок и вывода результатов тестирования, а так же временных метрик работы утилиты
*/
#include <stdio.h>

#include "./test.h"
#include "./data_processing/data_processing.h"

int main()
{
    TimeMetrics metrics;
    int case_1_in_a_len = sizeof(case_1_in_a)/sizeof(StatData);
    int case_1_in_b_len = sizeof(case_1_in_b)/sizeof(StatData);
    if (store_dump(PATH_FILE_A, case_1_in_a, &case_1_in_a_len)){
        perror("Error save file A");
        return 1;
    };
    if (store_dump(PATH_FILE_B, case_1_in_b, &case_1_in_b_len)){
        perror("Error save file B");
        return 1;
    };
    
    int c = processing_data(PATH_FILE_A, PATH_FILE_B, PATH_FILE_RES, &metrics);
    switch (c)
    {
    case 0:
        printf("Testing was successful!\n");
        print_time_metrics(&metrics);
        break;
    case 1:
        perror("Error load data from file!\n");
        break;
    case 2:
        perror("Error join arrays!\n");
        break;
    case 3:
        perror("Error save result file!\n");
        break;
    default:
        perror("Unknown error!\n");
        break;
    }
    printf("Done!\n");
}