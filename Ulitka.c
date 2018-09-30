#include <stdio.h>

#define N 6 //число столбцов и строк двумерного массива

/* заполнение массива по улитке*/
void fill_mas_by_ulitka(short* massive)
{
    short Size_square = N; //размер текущего квадрата(оболочки)
    short I = 0, J = 0; // для запоминания границ этих оболочек
    short i = 0, j = 0; //индексы массива
    short count = 1; //счетчик заполняющих чисел
    
    while(count <= N * N) //пока счетчик меньше квадрата размерности массива
    {
        for(j = J; j < Size_square; j++) //заполнение слева-направо
        {
            *(massive + N * i + j) = count++;
        }
        j--; /*это и все дальнейшие уменьшения/увеличения j(i) после for цикла (j--,i--,j++,i++)
 необходимы, т.к j(i) изменяются только после проверки условия */

        if(count <= N * N) // для вышерасположенного цикла это условие не ставиться, т.к. на том цикле закончится заполнение
        {
            for(i = I + 1; i < Size_square; i++) //заполнение сверху-вниз
            {
                *(massive + N * i + j) = count++;
            }
            i--;

            for(j = Size_square - 2; j >= J; j--)//заполнение справа-налево
            {
                *(massive + N * i + j) = count++;
            }
            j++;

            for(i = Size_square - 2; i >= I + 1; i--)//заполнение снизу-вверх
            {
                *(massive + N * i + j) = count++;
            }
        }
        i++;
        
        Size_square = Size_square - 1; //уменьшаем размер квадрата(оболочки)
        J++; //Изменяем положение граничных точек
        I++;
        
    }
}

//вывод двумерного массива
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

    fill_mas_by_ulitka(&a[0][0]);

    show_mas(&a[0][0]);

    return 0;
}