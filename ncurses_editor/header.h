#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <curses.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_NAME_SIZE 30
#define MIN_SYMBOL 32
#define MAX_SYMBOL 127
#define ARGV_INPUT 1
#define NO_ARGV_INPUT 0

/*Режим редактирования*/
void edit_mode(WINDOW* edit_wnd, char*);
/*Сохранение файла*/
void save_file(char*, WINDOW*);
/*открытие файла*/
void open_file(char*, WINDOW*,int);
/*выход из редактора*/
void exit_editor(WINDOW*);