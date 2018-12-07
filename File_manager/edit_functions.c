#define _GNU_SOURCE
#include <stdio.h>

#include "header.h"

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

void read_entries(char* dir_name, int* number_files, struct dirent*** namelist)
{
    *number_files = scandir(dir_name, namelist, 0, alphasort);
    /*обработка при получении отрицательного *number_files в файле my_manager.c */
}

void free_entries(struct dirent** list, int n)
{
    for(int i = 0; i < n; i++)
    {
        free(list[i]);
        list[i] = NULL;
    }
    free(list);
    list = NULL;
}

void print_entries(WINDOW* wnd, WINDOW* wnd_property, struct dirent** list, int n)
{
    struct stat file_info;
    wclear(wnd);
    wclear(wnd_property);
    for(int i = 0; i < n; i++)

    {
        stat(list[i]->d_name, &file_info);
        wprintw(wnd, "%s\n", list[i]->d_name);
        wprintw(wnd_property, "%d\n", file_info.st_size);
    }
    wrefresh(wnd);
    wrefresh(wnd_property);
}

void up(WINDOW* wnd_design, int* row, int col, int* file_number)
{
    wprintw(wnd_design, " ");
    (*row)--;
    wmove(wnd_design, *row, col);
    wprintw(wnd_design, "*");
    (*file_number)--;
}

void down(WINDOW* wnd_design, int* row, int col, int* file_number)
{
    wprintw(wnd_design, " ");
    (*row)++;
    wmove(wnd_design, *row, col);
    wprintw(wnd_design, "*");
    (*file_number)++;
}

void change_mode(WINDOW* design_left, WINDOW* design_right, int* mode, char* current_dir_left, char* current_dir_right)
{
    if(*mode == 0)
    {
        chdir(current_dir_right); //меняем текущую директорию
        wbkgd(design_left, COLOR_PAIR(1) | A_BOLD);
        wbkgd(design_right, COLOR_PAIR(2) | A_BOLD);
        *mode = 1;
    }
    else
    {
        chdir(current_dir_left); //меняем текущую директорию
        wbkgd(design_right, COLOR_PAIR(1) | A_BOLD);
        wbkgd(design_left, COLOR_PAIR(2) | A_BOLD);
        *mode = 0;
    }
    wrefresh(design_right);
    wrefresh(design_left);
}

void painting_wnd(WINDOW* left_wnd,
    WINDOW* left_wnd_property,
    WINDOW* right_wnd,
    WINDOW* right_wnd_property,
    WINDOW* left_design,
    WINDOW* right_design,
    WINDOW* help_wnd,
    WINDOW* current_left,
    WINDOW* current_right)
{
    /*оформление окон*/
    wbkgd(left_wnd, COLOR_PAIR(1) | A_BOLD);
    wbkgd(right_wnd, COLOR_PAIR(1) | A_BOLD);
    wbkgd(left_wnd_property, COLOR_PAIR(1) | A_BOLD);
    wbkgd(right_wnd_property, COLOR_PAIR(1) | A_BOLD);
    wbkgd(left_design, COLOR_PAIR(1) | A_BOLD);
    wbkgd(right_design, COLOR_PAIR(1) | A_BOLD);
    wbkgd(help_wnd, COLOR_PAIR(1) | A_BOLD);
    wbkgd(current_left, COLOR_PAIR(2) | A_BOLD); //в начале левое окно рабочее
    wbkgd(current_right, COLOR_PAIR(1) | A_BOLD);

    box(left_design, '|', '-');
    box(right_design, '|', '-');

    wmove(left_design, 1, COLS / 8 - 2);
    wprintw(left_design, "Name");
    wmove(left_design, 1, 3 * COLS / 8 + 1);
    wprintw(left_design, "Size");
    wmove(right_design, 1, COLS / 8 - 2);
    wprintw(right_design, "Name");
    wmove(right_design, 1, 3 * COLS / 8 + 1);
    wprintw(right_design, "Size");
    wmove(help_wnd, 0, 1);
    wprintw(help_wnd, "Enter-change directory | TAB - Switch window | F5- copy file | F10-Exit");
}

void initial_position(WINDOW* wnd, int* row, int* col)
{
    *row = 2;
    *col = 1;
    wprintw(wnd, " ");
    wmove(wnd, *row, *col);
    wprintw(wnd, "*");
    wrefresh(wnd);
}

void init_again(WINDOW* left_wnd,
    WINDOW* left_wnd_property,
    WINDOW* right_wnd,
    WINDOW* right_wnd_property,
    WINDOW* left_design,
    WINDOW* right_design,
    WINDOW* help_wnd,
    WINDOW* current_left,
    WINDOW* current_right)
{

    keypad(stdscr, 1);
    noecho();
    refresh();
    cbreak();
    curs_set(0); //отображение курсора отключено (* управление);
    wclear(help_wnd);

    painting_wnd(left_wnd, left_wnd_property, right_wnd, right_wnd_property, left_design, right_design, help_wnd,
        current_left, current_right);
    wrefresh(left_design);
    wrefresh(right_design);
    wrefresh(left_wnd);
    wrefresh(right_wnd);
    wrefresh(left_wnd_property);
    wrefresh(right_wnd_property);
    wrefresh(help_wnd);
    wrefresh(current_left);
    wrefresh(current_right);
}

void* thread_copy(void* arg)
{
    FILE *fp1, *fp2;
    char mas[SIZE_BUF_FOR_COPY];
    struct thread_data* my_data = (struct thread_data*)arg;
    size_t read;

    if((fp1 = fopen(my_data->first_file, "r")) == NULL)
    {
        wmove(my_data->copy_wnd, 4, 0);
        wprintw(my_data->copy_wnd, "wrong path!");
        wrefresh(my_data->copy_wnd);
        pthread_exit(0);
    }

    if((fp2 = fopen(my_data->second_file, "w+")) == NULL)
    {
        wmove(my_data->copy_wnd, 4, 0);
        wprintw(my_data->copy_wnd, "wrong path!");
        wrefresh(my_data->copy_wnd);
        fclose(fp1);
        pthread_exit(0);
    }

    while((read = fread(mas, 1, SIZE_BUF_FOR_COPY, fp1)) != 0)
    {
        fwrite(mas, 1, read, fp2);
    }
    fclose(fp1);
    fclose(fp2);

    pthread_exit(0);
}

void* thread_status_copy(void* arg)
{
    struct thread_data* my_data = (struct thread_data*)arg;
    struct stat file1_info;
    struct stat file2_info;

    if(stat(my_data->first_file, &file1_info) != 0)
    {
        pthread_exit(0);
    }

    file2_info.st_size = 0;

    while(file2_info.st_size < file1_info.st_size)
    {
        if(stat(my_data->second_file, &file2_info) == 0)
        {
            wmove(my_data->copy_wnd, 3, 0);
            wprintw(my_data->copy_wnd, "status of copy:%%%.1f",
                (double)((double)file2_info.st_size / file1_info.st_size) * 100);
            wrefresh(my_data->copy_wnd);
        }
    }

    pthread_exit(0);
}

void fill_data_threads(struct thread_data* data, WINDOW* copy_wnd, char* filemane, char* curent_dir)
{
    char path[MAX_PATH_SIZE];
    curs_set(1);
    echo();
    wbkgd(copy_wnd, COLOR_PAIR(3) | A_BOLD);
    wprintw(copy_wnd, "Enter path to copy file:\n");
    wgetnstr(copy_wnd, path, MAX_PATH_SIZE);
    path[MAX_PATH_SIZE] = 0;

    /*формирование данных для потоков*/
    data->copy_wnd = copy_wnd;
    getcwd(curent_dir, MAX_PATH_SIZE);
    strcat(curent_dir, "/");
    strcat(curent_dir, filemane);
    strcpy(data->first_file, curent_dir);

    strcpy(data->second_file, path);
}

int check_path_exist(char* path)
{
    struct stat file_info;
    if(stat(path, &file_info) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void type_msg_wnd(WINDOW* copy_wnd,int row, int col, char *message)
{
   wmove(copy_wnd, row, col);
   wprintw(copy_wnd, message);
   wrefresh(copy_wnd); 
}

void create_threads(pthread_t* tid_copy_thread, pthread_t* tid_status_thread, struct thread_data* data)
{
    int result;
    result = pthread_create(tid_copy_thread, NULL, thread_copy, data);
    if(result != 0)
    {
        perror("Creating the first TCP thread");
        exit(EXIT_FAILURE);
    }

    result = pthread_create(tid_status_thread, NULL, thread_status_copy, data);
    if(result != 0)
    {
        perror("Creating the second TCP thread");
        exit(EXIT_FAILURE);
    }

}

void join_all_threads(pthread_t tid_copy_thread, pthread_t tid_status_thread)
{
    int result;
    result = pthread_join(tid_copy_thread, NULL);
    if(result != 0)
    {
        perror("Joining the first thread");
        exit(EXIT_FAILURE);
    }

    result = pthread_join(tid_status_thread, NULL);
    if(result != 0)
    {
        perror("Joining the first thread");
        exit(EXIT_FAILURE);
    }
}
