typedef struct complex_number
{
    float real;
    float img;
} complex_number;

void print_complex_number(complex_number);
void add(complex_number, complex_number, complex_number*);
void sub(complex_number, complex_number, complex_number*);
void mul(complex_number, complex_number, complex_number*);
void div(complex_number, complex_number, complex_number*);
