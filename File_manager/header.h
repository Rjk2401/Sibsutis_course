#include <curses.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <sys/dir.h>

#define MAX_PATH_SIZE 256

/*обработчик сигнала*/
void sig_winch(int signo);

/*оформление окон*/
void painting_wnd(WINDOW* left_wnd,
    WINDOW* left_wnd_property,
    WINDOW* right_wnd,
    WINDOW* right_wnd_property,
    WINDOW* left_design,
    WINDOW* right_design,
    WINDOW* help_wnd,
    WINDOW* current_left,
    WINDOW* current_right);
    
/*установка курсоров в начальную позицию*/
void initial_position(WINDOW* wnd, int *row,int *col);

/*получение структуры dirent из указанной директории*/
void read_entries(char* dir_name, int* number_files, struct dirent*** namelist);

/*вывод в соответствующие окна имен и размеров файлов*/
void print_entries(WINDOW* wnd, WINDOW* wnd_property, struct dirent** list, int n);

/*освобождение памяти структур dirent*/
void free_entries(struct dirent** list, int n);

/*перемещение вверх*/
void up(WINDOW* wnd_design, int* row, int col, int* file_number);

/*перемещение вниз*/
void down(WINDOW* wnd_design, int* row, int col, int* file_number);

/*изменение активного окна*/
void change_mode(WINDOW* design_left, WINDOW* design_right, int* mode, char* current_dir_left, char* current_dir_right);
