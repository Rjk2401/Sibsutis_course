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
    wprintw(help_wnd, "Enter-change directory | TAB - Switch window | F10-Exit");
}

void initial_position(WINDOW* wnd, int *row,int *col)
{
    *row = 2;
    *col = 1;
    wprintw(wnd, " ");
    wmove(wnd, *row, *col);
    wprintw(wnd, "*");
    wrefresh(wnd);
}
