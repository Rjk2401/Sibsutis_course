typedef struct complex_number
{
    float real;
    float img;
} complex_number;

#define MENU_SIZE 50 
#define BUFFER_SIZE 2

/*Вывод комплексного числа*/
void print_complex_number(complex_number);
/*Формирование меню в зависимости от подключенных плагинов*/
void create_menu(char [], char *[], int);

