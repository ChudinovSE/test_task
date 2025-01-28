/*!
\file
\brief Заголовочный файл с константами для тестирования обработки данных

Данный файл содержит в себе определения констант для корректного тестирования 
утилиты обработки данных. Константы заданны в соответствии с требованиями п.4 
тестового задания
*/
#include "./data_processing/stat_data/stat_data.h"
#define PATH_FILE_A "./data_file_a.data"
#define PATH_FILE_B "./data_file_b.data"
#define PATH_FILE_RES "./data_file_res.data"

const StatData case_1_in_a[2] ={
    {
        .id = 90889, 
        .count = 13, 
        .cost = 3.567, 
        .primary = 0, 
        .mode=3 
    },
    {
        .id = 90089, 
        .count = 1, 
        .cost = 88.90, 
        .primary = 1, 
        .mode=0 }
};
const StatData case_1_in_b[2] ={
    {
        .id = 90089, 
        .count = 13, 
        .cost = 0.011, 
        .primary = 0, 
        .mode=2 
    },
    {
        .id = 90189, 
        .count = 1000, 
        .cost = 1.00003, 
        .primary = 1, 
        .mode=2
    }
};
const StatData case_1_out[3] ={
    {
        .id = 90189, 
        .count = 1000, 
        .cost = 1.00003, 
        .primary = 1, 
        .mode = 2 
    },
    {
        .id = 90889, 
        .count = 13, 
        .cost = 3.567, 
        .primary = 0, 
        .mode = 3 
    },
    {
        .id = 90089, 
        .count = 14, 
        .cost = 88.911, 
        .primary = 0, 
        .mode = 2 
    }
};