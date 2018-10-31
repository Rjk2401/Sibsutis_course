#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "interface.h"

int main(int argc, char** argv)
{
    complex_number first_number;
    complex_number second_number;
    complex_number result;
    int action = 0; //действие в меню
    char menu[MENU_SIZE] = ""; //меню выбора действий калькулятора     

    DIR* dir; 
    struct dirent* entry; // структура для readdir
    int count_plugins = 0; //счетчик плагинов
    int i = 0; //цикловой счетчик
    char** libs_name; //строковый массив имен плагинов
    
    dir = opendir("./Plugins");
    if(!dir)
    {
        perror("diropen");
        exit(1);
    };
    
    //подсчет количества плагинов
    while((entry = readdir(dir)) != NULL)
    {
        if(strstr(entry->d_name, ".so"))
            count_plugins++;
    };
    closedir(dir);

    //выделяем память для массива строк (имен плагинов)
    libs_name = (char**)malloc(sizeof(char*) * count_plugins);

    dir = opendir("./Plugins");
    //заносим имена плагинов в массив строк
    while((entry = readdir(dir)) != NULL)
    {
        if(strstr(entry->d_name, ".so"))
        {
            //выделяем память с учетом длины имени,'\0' и с "./Plugins/" т.е +11 байт,
            // calloc - т.к strcat требует nul terminated (malloc не инициализирует нулями)
            libs_name[i] = (char*)calloc((strlen(entry->d_name) + 11), sizeof(char));
            strcat(libs_name[i], "./Plugins/");
            strcat(libs_name[i], entry->d_name);
            i++;
        }
    };

    closedir(dir);

    /*Массив указателей на адреса подключаемых библиотек*/
    void** mas_lib;
    mas_lib = (void**)malloc(sizeof(void*) * count_plugins);
    /*Указатель на подключаемую функцию*/
    void (*func_ptr)(complex_number, complex_number, complex_number*);
    /*Массив имен функций*/
    char** mas_func_name;
    /*Выделяем память для массива имен функций*/
    mas_func_name = (char**)malloc(sizeof(char*) * count_plugins);

    for(i = 0; i < count_plugins; i++)
    {
        mas_lib[i] = dlopen(libs_name[i], RTLD_LAZY);
        mas_func_name[i] = (char*)dlsym(mas_lib[i], "name_function");
    }
    
   /*Формирование меню в зависимости от подключенных библиотек*/
    create_menu(menu, mas_func_name, count_plugins);

    while(1)
    {
        printf("Select operation:\n");
        printf("%s", menu);
        while(scanf("%d", &action) == 0) //на случай, если введена не цифра
        {
            printf("Enter number! try again\n");
            while(getchar() != '\n');
        }

        if(action >= 1 && action <= count_plugins)
        {
            printf("Enter number1 real and img part separated by space\n");
            scanf("%f %f", &first_number.real, &first_number.img);
            printf("Enter number2 real and img part separated by space\n");
            scanf("%f %f", &second_number.real, &second_number.img);
            func_ptr = dlsym(mas_lib[action - 1], mas_func_name[action - 1]);
            func_ptr(first_number, second_number, &result);
            print_complex_number(result);
        }

        if(action == count_plugins + 1)
        {
            printf("Exit\n");
            break;
        }
    }
    
    /*удаление библиотек из памяти*/
    for(i = 0; i < count_plugins; i++)
    {
        dlclose(mas_lib[i]);
    }
    /*освобождаем память строк и массивов*/
    for(i = 0; i < count_plugins; i++)
    {
        free(libs_name[i]); // освобождаем память для отдельной строки
    }
    free(libs_name);
    free(mas_lib);
    free(mas_func_name);

    return 0;
}
