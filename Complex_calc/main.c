#include <stdbool.h>
#include <stdio.h>

#include "operations.h"

int main(int argc, char** argv)
{
    int action = 0;
    bool flag = false; //для выхода из цикла while(1) через case(action=5)
    complex_number first_number;
    complex_number second_number;
    complex_number result;

    while(1)
    {
        printf("Select operation:\n");
        printf("1-add; 2-sub; 3-mul; 4-div; 5-quit.\n");
        while(scanf("%d", &action) == 0) //на случай, если введена не цифра
        {
            printf("Enter number! try again\n");
            while(getchar() != '\n')
                ;
        }

        if(action >= 1 && action <= 4) //нужен ли ввод ввод чисел
        {
            printf("Enter number1 real and img part separated by space\n");
            scanf("%f %f", &first_number.real, &first_number.img);
            printf("Enter number2 real and img part separated by space\n");
            scanf("%f %f", &second_number.real, &second_number.img);
        }

        switch(action)
        {
        case 1:
            add(first_number, second_number, &result);
            break;
        case 2:
            sub(first_number, second_number, &result);
            break;
        case 3:
            mul(first_number, second_number, &result);
            break;
        case 4:
            div(first_number, second_number, &result);
            break;
        case 5:
            flag = true;
            printf("Exit\n");
            break;
        default:
            printf("Wrong action\n");
            break;
        }

        if(flag == true) //выход из бесконечного цикла
        {
            break;
        }

        if(action >= 1 && action <= 4) // если выбрано арифмитическое действие выводим результат
        {
            print_complex_number(result);
        }
    }
    return 0;
}
