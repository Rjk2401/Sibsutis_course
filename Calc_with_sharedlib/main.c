#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "operations.h"

int main(int argc, char** argv)
{
    int action = 0;
    bool flag = false;  //для выхода из цикла while(1) через case(action=5)
    char menu[50] = ""; //меню выбора действий калькулятора
    complex_number first_number;
    complex_number second_number;
    complex_number result;
    
    /*Указатели на адреса подключаемых библиотек*/
    void* add_library = NULL; 
    void* sub_library = NULL;
    void* mul_library = NULL;
    void* div_library = NULL;
    void* print_library = NULL;
    /*Указатели на функции*/
    void (*addptr)(complex_number, complex_number, complex_number*) = NULL;
    void (*subptr)(complex_number, complex_number, complex_number*) = NULL;
    void (*mulptr)(complex_number, complex_number, complex_number*) = NULL;
    void (*divptr)(complex_number, complex_number, complex_number*) = NULL;
    void (*printptr)(complex_number) = NULL;
    
    add_library = dlopen("libadd.so", RTLD_LAZY);
    sub_library = dlopen("libsub.so", RTLD_LAZY);
    mul_library = dlopen("libmul.so", RTLD_LAZY);
    div_library = dlopen("libdiv.so", RTLD_LAZY);
    print_library = dlopen("libprint.so", RTLD_LAZY);
      
    addptr = dlsym(add_library, "add");
    subptr = dlsym(sub_library, "sub");
    mulptr = dlsym(mul_library, "mul");
    divptr = dlsym(div_library, "div");
    printptr = dlsym(print_library, "print_complex_number");

    /*Формирование меню в зависимости от подключенных библиотек*/
    if(add_library)
        strcat(menu, "1-add;");
    if(sub_library)
        strcat(menu, "2-sub;");
    if(mul_library)
        strcat(menu, "3-mul;");
    if(div_library)
        strcat(menu, "4-div;");
    strcat(menu, "5-quit.\n");

    while(1)
    {
        printf("Select operation:\n");
        printf("%s", menu);
        while(scanf("%d", &action) == 0) //на случай, если введена не цифра
        {
            printf("Enter number! try again\n");
            while(getchar() != '\n');
        }        
        
        if(action >= 1 && action <= 4) //нужен ли ввод ввод чисел
        {    
            if((action == 1 && addptr != 0) || (action == 2 && subptr != 0) || (action == 3 && mulptr != 0) ||
                (action == 4 && divptr != 0)) //Ввод чисел (при условии что функция подгружена)
            {
                printf("Enter number1 real and img part separated by space\n");
                scanf("%f %f", &first_number.real, &first_number.img);
                printf("Enter number2 real and img part separated by space\n");
                scanf("%f %f", &second_number.real, &second_number.img);
            }
        }

        switch(action)
        {
        case 1:
            if(addptr)
            {
                addptr(first_number, second_number, &result);
                printptr(result);
            }
            else
                printf("unavailable function\n");
            break;
        case 2:
            if(subptr)
            {
                subptr(first_number, second_number, &result);
                printptr(result);
            }
            else
                printf("unavailable function\n");
            break;
        case 3:
            if(mulptr)
            {
                mulptr(first_number, second_number, &result);
                printptr(result);
            }
            else
                printf("unavailable function\n");
            break;
        case 4:
            if(divptr)
            {
                divptr(first_number, second_number, &result);
                printptr(result);
            }
            else
                printf("unavailable function\n");
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
            if (add_library) dlclose(add_library); //удаление библиотек из памяти 
            if (sub_library) dlclose(sub_library);
            if (mul_library) dlclose(mul_library);
            if (div_library) dlclose(div_library);
            if (print_library) dlclose(print_library);
            break;
        }
    }
    return 0;
}
