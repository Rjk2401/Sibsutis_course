#include <stdio.h>
#include <string.h>

#include "interface.h"

void print_complex_number(complex_number number)
{
    if(number.img != (int)number.img || number.real != (int)number.real) // если хотя бы одна часть не целая
    {
        if(number.img < 0)
            printf("result=%0.2f%0.2f*i\n", number.real, number.img);
        else
            printf("result=%0.2f+%0.2f*i\n", number.real, number.img);
    }
    else // числа целые
    {
        if(number.img < 0)
            printf("result=%d%d*i\n", (int)number.real, (int)number.img);
        else
            printf("result=%d+%d*i\n", (int)number.real, (int)number.img);
    }
}

void create_menu(char *menu, char *mas_function_name[], int count_plugins)
{
    char buf[BUFFER_SIZE];  //для конвертации из чисел в строку через sprintf 
    
    for(int i = 0; i < count_plugins; i++)
    {
        sprintf(buf, "%d", i + 1);
        strcat(menu, buf);
        strcat(menu, "-");
        strcat(menu, mas_function_name[i]);
        strcat(menu, "; ");
    }
    sprintf(buf, "%d", count_plugins + 1);
    strcat(menu, buf);
    strcat(menu, "-quit.\n");
}