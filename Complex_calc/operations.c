#include <stdio.h>

#include "operations.h"

void print_complex_number(complex_number number)
{
    if(number.img != (int)number.img || number.real != (int)number.real) // если хотя бы одна часть не целая
    {
	if (number.img < 0) printf("result=%0.2f%0.2f*i\n", number.real, number.img);
	else printf("result=%0.2f+%0.2f*i\n", number.real, number.img);
    }
    else // числа целые
    {
	if (number.img < 0) printf("result=%d%d*i\n", (int)number.real, (int)number.img);
	else printf("result=%d+%d*i\n", (int)number.real, (int)number.img);
    }
}

void add(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = first_number.real + second_number.real;
    result->img = first_number.img + second_number.img;
}

void sub(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = first_number.real - second_number.real;
    result->img = first_number.img - second_number.img;
}

void mul(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = first_number.real * second_number.real - first_number.img * second_number.img;
    result->img = first_number.real * second_number.img + second_number.real * first_number.img;
}

void div(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = (first_number.real * second_number.real + first_number.img * second_number.img) /
        (second_number.real * second_number.real + second_number.img * second_number.img);
    result->img = (second_number.real * first_number.img - first_number.real * second_number.img) /
        (second_number.real * second_number.real + second_number.img * second_number.img);
}
