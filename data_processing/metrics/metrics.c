/*!
\file
\brief Фаил с реализацией функций для обработки метрик

Данный файл содержит в себе реализации функций, 
используемых в демонстрационной программе для обработки метрик
*/
#include <stdio.h>

#include"./metrics.h"

TimeUnits _convert_ns(long ns) {
    TimeUnits result;
    result.ms = ns / 1000000;
    ns %= 1000000; 
    result.us = ns / 1000;
    result.ns = ns % 1000; 

    return result;
}

int print_time_metrics(TimeMetrics *metrics)
{   
    // Преобразование числа наносекунд в метриках в человекочитаемый формат
    TimeUnits load_dump_a_units = _convert_ns(metrics->load_dump_a_ns);
    TimeUnits load_dump_b_units = _convert_ns(metrics->load_dump_b_ns);
    TimeUnits join_dumps_units = _convert_ns(metrics->join_dumps_ns);
    TimeUnits sort_dump_units = _convert_ns(metrics->sort_dump_ns);
    TimeUnits save_res_dump_units = _convert_ns(metrics->save_res_dump_ns);
    // Вывод в виде форматированной таблицы, для удобства
    printf("Time metrics of tests: \n");
    printf("load dump a: %ld ms, %ld us %ld ns\n", load_dump_a_units.ms, load_dump_a_units.us, load_dump_a_units.ns) ;
    printf("load dump b: %ld ms, %ld us %ld ns\n", load_dump_b_units.ms, load_dump_b_units.us, load_dump_b_units.ns) ;
    printf("join dumps:  %ld ms, %ld us %ld ns\n", join_dumps_units.ms, join_dumps_units.us, join_dumps_units.ns) ;
    printf("sort dump:   %ld ms, %ld us %ld ns\n", sort_dump_units.ms, sort_dump_units.us, sort_dump_units.ns) ;
    printf("save dump:   %ld ms, %ld us %ld ns\n", save_res_dump_units.ms, save_res_dump_units.us, save_res_dump_units.ns) ;
};