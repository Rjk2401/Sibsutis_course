#include <stdio.h>

#include "header.h"


int main(int argc, char **argv)
{
    pthread_t clients_threads[NUMBER_OF_CLIENTS]; //массив идентификаторов потокв клиентов     
    struct pthread_data data[NUMBER_OF_CLIENTS]; //данные для потоков
    pthread_t loader_thread; //идентификатор потока погрузчика
    
    /*инициализация мьютекса*/
    pthread_mutex_init(&mutex, NULL);
    
    /*чтобы при каждом запуске разные значения инициализировались*/
    srand(time(NULL));
    
    /*инициализация магазина*/
    initialize_shops(shops);
    
    /*вывод начальной конфигурации магазина*/
    print_shops(shops);
    
    /*вывод начальных потребностей покупателей*/
    printf("Beginning requirement:\n");
    for(int i = 0; i < NUMBER_OF_CLIENTS; i++)
    {
        data[i].client_requirement = MIN_REQUIREMENT+rand()%RANGE_REQUIREMENT;
        printf("|%d client_requirement = %d|\n",i,data[i].client_requirement);
    }
    
    /*создание потоков клиентов и погрузчика*/
    create_threads(clients_threads,&loader_thread,data);

    /*присоединение всех потоков клиентов*/
    join_all_clients(clients_threads);   
    
    /*отправка sigint погрузчику*/
    pthread_cancel(loader_thread);
    
    /*присоединение потока погрузчика*/
    join_loader_thread(loader_thread);
    
    /*конец*/
    printf("ALL client_requirement=0\nExit\n");

    return 0;
}
