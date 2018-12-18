#include <stdio.h>

#include "header.h"

void initialize_shops(int *mas)
{
    for (int i=0; i<NUMBER_OF_SHOPS; i++)
    {
        mas[i]=MIN_PRODUCTS+rand()%RANGE_PRODUCTS;
    }

}

void print_shops(int *mas)
{
    printf("Shop: ");
    for (int i=0; i<NUMBER_OF_SHOPS; i++)
    {
        printf("%d ",mas[i]);
    }
    printf("\n");
}

void* shopping(void* requirement)
{
    struct pthread_data* data=(struct pthread_data*)requirement;

    int i=rand()%NUMBER_OF_SHOPS; //начальный магазин выбирается случайно

    while (data->client_requirement>0)
    {
        pthread_mutex_lock(&mutex);
        if (shops[i]>0)
        {
            /*если в магазине достаточно товаров для покупки(SIZE_OF_BUY)*/
            if (shops[i]>SIZE_OF_BUY)
            {
                data->client_requirement=data->client_requirement-SIZE_OF_BUY;
                shops[i]=shops[i]-SIZE_OF_BUY;
            }
            else /*если недостаточно товаров*/
            {
                data->client_requirement=data->client_requirement-shops[i];
                shops[i]=0;
            }
            if (data->client_requirement<0)
                data->client_requirement=0;
            printf("----------------\n");
            printf("[%d] client requirement: %d\n",data->client_number,data->client_requirement);
            print_shops(shops);
            printf("----------------\n");
            pthread_mutex_unlock(&mutex);

            sleep(TIME_TO_SLEEP);
        }
        else pthread_mutex_unlock(&mutex);

        /*зацикливаем походы в магазины*/
        if (i==NUMBER_OF_SHOPS-1) //если в последнем находились, что следующий - нулевой
            i=0;
        else
            i++; //если не в последнем магазине находились, то следующий +1
    }

    pthread_exit(0);
}


void* loader(void* arg)
{
    while (1)
    {

        pthread_mutex_lock(&mutex);
        printf("----------------\n");
        printf("Loader:\n");
        for (int i=0; i<NUMBER_OF_SHOPS; i++)
        {
            if (shops[i]<MAX_PRODUCTS)
            {
                shops[i]=shops[i]+SIZE_OF_LOADER;
                if (shops[i]>MAX_PRODUCTS)
                    shops[i]=MAX_PRODUCTS;
            }
        }
        print_shops(shops);
        printf("----------------\n");
        pthread_mutex_unlock(&mutex);
        sleep(LOADER_SLEEP);
    }

    pthread_exit(0);
}

void create_threads(pthread_t clients_threads[],pthread_t *loader_thread,struct pthread_data *data)
{
    int result;
    for(int i = 0; i < NUMBER_OF_CLIENTS; i++)
    {

        data[i].client_number=i;
        result = pthread_create(&clients_threads[i], NULL, shopping, &data[i]);
        if(result != 0)
        {
            perror("Creating the thread");
            exit(1);
        }
    }

    result = pthread_create(loader_thread, NULL, loader, NULL);
    if(result != 0)
    {
        perror("Creating the thread");
        exit(2);
    }

}


void join_all_clients(pthread_t clients_threads[])
{
    int result;
    for(int i = 0; i < NUMBER_OF_CLIENTS; i++)
    {
        result = pthread_join(clients_threads[i], NULL);
        if(result != 0)
        {
            perror("Joining the thread");
            exit(3);
        }
    }
}

void join_loader_thread(pthread_t loader_thread)
{
    int result;
    result = pthread_join(loader_thread, NULL);
    if(result != 0)
    {
        perror("Joining the thread");
        exit(4);
    }
}
