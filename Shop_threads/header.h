#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define NUMBER_OF_SHOPS 5           //количество магазинов
#define MIN_PRODUCTS 400            //минимальное количество продуктов в магазине
#define MAX_PRODUCTS 500            //максимальное количество продуктов в магазине
#define RANGE_PRODUCTS 100          //диапазон количества покупок(для rand)

#define MIN_REQUIREMENT 4000        //минимальная потребность
#define MAX_REQUIREMENT 5000        //максимальная потребность
#define RANGE_REQUIREMENT 1000      //диапазон потребности (для rand)
#define SIZE_OF_BUY 250             //размер покупки
#define SIZE_OF_LOADER 200          //размер загрузки

#define TIME_TO_SLEEP 5             //время засыпания клиента
#define LOADER_SLEEP 3              //время засыпания погрузчика
#define NUMBER_OF_CLIENTS 3         //число клиентов (потоков)


int shops[NUMBER_OF_SHOPS];
pthread_mutex_t mutex;    

/*данные для потоков*/
struct pthread_data  
{
    int client_requirement;
    int client_number;
};


/*инициализация магазина случайным количеством продуктов*/
void initialize_shops(int *mas);

/*вывод на экран текущее количество продуктов во всех магазинах*/
void print_shops(int *mas);

/*потоковая функция покупателей*/
void* shopping(void* requirement);

/*потоковая функция погрузчика*/
void* loader(void* arg);

/*создание потоков клиентов и погрузчика*/
void create_threads(pthread_t clients_threads[],pthread_t *loader_thread,struct pthread_data *data);

/*ожидание завершения всех клиентов*/
void join_all_clients(pthread_t clients_threads[]);

/*ожидание завершения погрузчика*/
void join_loader_thread(pthread_t loader_thread);  
