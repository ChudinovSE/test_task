/*!
\file
\brief Фаил с реализацией функций для обработки тестовых данных

Данный файл содержит в себе реализации функций, 
используемых в демонстрационной программе для обработки тестовых данных
*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "stat_data.h"

/*!
Функция проверят наличие в массиве структур @StatData элемента с id, 
равным id в переданном экземпляре @StatData. При наличии такого, меняет значение 
полей элемента, при отсутствии добавляет очередной элемент в итоговый массив


\param[in] n_res Указатель на целое число, равное количеству структур в итоговом
массиве 
\param[in] data_array Указатель на массив структур @StatData
\param[in] item Указатель на экземпляр @StatData
\return Код ошибки 0 - успешное выполнение
*/
int _merj_or_insert(int *n_res, StatData *data_array, StatData *item) {
    int found = 0;
        for (int j = 0; j < *n_res; j++) {
            if (data_array[j].id == item->id) {
                data_array[j].count += item->count;
                data_array[j].cost += item->cost;
                data_array[j].primary = data_array[j].primary && item->primary;
                data_array[j].mode = (data_array[j].mode > item->mode) ? data_array[j].mode : item->mode;
                found = 1;
                break;
            }
        }
        if (!found) {
            data_array[*n_res] = *item;
            (*n_res)++;
        }
    return 0;
}

int _cmp_stat_data(const void *_a, const void *_b)
{
    StatData *a = (StatData *) _a;
    StatData *b = (StatData *) _b;
    return a->cost > b->cost;
}

int _print_stat_data_debug(StatData *data)
{
printf("StatData{\n");
printf("id: %ld,\n", data->id);
printf("count: %d,\n", data->count);
printf("cost: %.3e,\n", data->cost);
printf("primary: %c \n", data->primary & 0x1 ? 'y' : 'n');
printf("mode: ");
printf("%c", (data->mode & 0x1 == 0x1) ? '1' : '0');
printf("%c", (data->mode & 0x2 == 0x2) ? '1' : '0');
printf("%c", (data->mode & 0x4 == 0x4) ? '1' : '0');
printf("\n}\n");
}

int print_stat_data_table(const StatData *data, int *n)
{
    int rows = (*n > 10) ? 10 : *n;
    printf("First %d rows of result dump\n", rows);
    for (int i = 0; i < rows; i++)
    {
        printf("id: %5ld    ", data[i].id);
        printf("count: %5d    ", data[i].count);
        printf("cost: %.3e    ", data[i].cost);
        printf("primary: %c     ", data[i].primary & 0x1 ? 'y' : 'n');
        printf("mode: ");
        printf("%c", (data[i].mode & 0x4) ? '1' : '0');
        printf("%c", (data[i].mode & 0x2) ? '1' : '0');
        printf("%c", (data[i].mode & 0x1) ? '1' : '0');
        printf("\n");
    }
    return 0;
}


int store_dump(char *path, const StatData *data_array, int *n)
{   
    int size = *n * sizeof(StatData);
    FILE *dump_file_p = fopen(path, "wb");
    if (!dump_file_p)
    {
        return 1;
    }
    char *c;
    c = (char *)n;
    for (int i = 0; i < sizeof(*n); i++)
    {
        putc(*c++, dump_file_p);
    }
   
    c = (char *)data_array;
    for (int i = 0; i < size; i++)
    {
        putc(*c, dump_file_p);
        c++;
    }

    fclose(dump_file_p);

    return 0;

}

StatData* load_dump(char *path, int *n)
{
    char *c;    
    int m = sizeof(int);
   
    FILE * dump_file_p = fopen(path, "r");
    if (!dump_file_p)
    {
        return NULL;
    }

    int *ptr_count = malloc(m);
    if (!ptr_count)
    {
        return NULL;
    }

    c = (char *)ptr_count;
    for (int i = 0; i < sizeof(int); i++)
    {
        *c= getc(dump_file_p);
        c++;
    }
    *n = *ptr_count;
    free(ptr_count);    

    StatData * res = malloc(*n * sizeof(StatData));
    if (!res)
    {
        return NULL;
    }
    c = (char *)res;

    for (int i = 0; i < (*n * sizeof(StatData)); i++)
    {
        *c= getc(dump_file_p);
        c++;
    }

    fclose(dump_file_p);
    return res;
}

int sort_dump_std(StatData *data_array, int *n)
{
    qsort(data_array, *n, sizeof(StatData), _cmp_stat_data);
    return 0;
}

int sort_dump(StatData *data_array, int *n)
{
    if (*n < 2) {
        return 0;
    };
    StatData tmp = data_array[0];
    for (int i = *n - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (data_array[j].cost > data_array[j + 1].cost)
            {
                tmp = data_array[j];
                data_array[j] = data_array[j + 1];
                data_array[j + 1] = tmp;
            }
        }
    }
    return 0;
}

StatData* join_dump(StatData *data_array_a, int *n_a, StatData *data_array_b, int *n_b, int *n_res)
{
    // Выделение памяти для итогового массива, длина не может превышать сумму длин исходных массивов
    StatData *data_array_res = malloc((*n_a + *n_b) * sizeof(StatData));
    if (!data_array_res)
    {
        return NULL;
    }

    *n_res = 0;
    // добавление элементов из первого массива в итоговой, с обработкой элементов, с одинаковыми id
    for (int i = 0; i < *n_a; i++) {
        _merj_or_insert(n_res, data_array_res, data_array_a + i);
    }
    // добавление элементов из второго массива в итоговой, с обработкой элементов, с одинаковыми id
    for (int i = 0; i < *n_b; i++) {
        _merj_or_insert(n_res, data_array_res, data_array_b + i);
    }

    // корректировка количества выделенной памяти, на случай если длина итогового массива меньше 
    // суммы длин исходных массивов, освободить не используемую память
    data_array_res = realloc(data_array_res, *n_res * sizeof(StatData));
    return data_array_res;
}

int generation_rnd_data_array(StatData *data_array, int *n)
{
    for (int i = 0; i < *n; i++)
    {
        StatData rnd_data_item = {
            .id = rand(), 
            .count = rand(), 
            .cost = (float) (rand()) / 3, 
            .primary = rand()%2, 
            .mode=rand()%8 
        };
        data_array[i] = rnd_data_item;
    }
    return 0;
    
}

