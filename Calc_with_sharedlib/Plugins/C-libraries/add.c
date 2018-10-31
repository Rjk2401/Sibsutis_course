#include "operations.h"

char name_function[]="add";

void add(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = first_number.real + second_number.real;
    result->img = first_number.img + second_number.img;
}
