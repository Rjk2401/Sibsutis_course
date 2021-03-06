#include <stdio.h>

#define N 6 //число столбцов и строк двумерного массива

/* Замена четных и нечетных столбцов местами */
void change_cols(short* massive)
{
    short help;
    for(short i = 0; i < N; i++)
    {
        for(short j = 0; j < N; j = j + 2)
        {
            help = *(massive + N * i + j);
            *(massive + i * N + j) = *(massive + i * N + (j + 1));
            *(massive + i * N + (j + 1)) = help;
        }
    }
}

//заполнение матрицы
void fill_mas(short* massive)
{
    short count = 1;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            *(massive + i * N + j) = count++;
        }
    }
}

//вывод матрицы
void show_mas(short* massive)
{
     for(short i = 0; i < N; i++)
    {
        for(short j = 0; j < N; j++)
        {
            printf("%2d ", *(massive + i * N + j));
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{  
    short a[N][N]; 
    
    fill_mas(&a[0][0]);

    printf("Начальная матрица:\n");    
    show_mas(&a[0][0]);

    //Замена столбцов, при условии что N четное
    if(N % 2 == 0)
    {
        change_cols(&a[0][0]);
        printf("Матрица после переставления:\n");
        show_mas(&a[0][0]);
    }
    else
    {
        printf("Число столбцов должно быть четное\n");
    } 
    
    return 0;
}
