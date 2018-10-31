#include "operations.h"

char name_function[]="div";

void div(complex_number first_number, complex_number second_number,complex_number *result)
{
    result->real = (first_number.real * second_number.real + first_number.img * second_number.img) /
        (second_number.real * second_number.real + second_number.img * second_number.img);
    result->img = (second_number.real * first_number.img - first_number.real * second_number.img) /
        (second_number.real * second_number.real + second_number.img * second_number.img);
}
