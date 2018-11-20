#include <stdio.h>

#include "header.h"

void sig_winch(int signo);

int main(int argc, char** argv)
{
    WINDOW* menu;     //меню действий
    WINDOW* edit_wnd; //окно редактирования

    initscr();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1); //преобразование ESC-последовательностей в константы
    cbreak();
    curs_set(1); //отображение курсора
    start_color(); //включение поддержки цветов
    init_pair(1, COLOR_BLACK, COLOR_BLUE); //цвета для меню и всплывающих окон
    refresh();
    menu = newwin(1, COLS - 1, 0, 0); //окно меню
    wbkgd(menu, COLOR_PAIR(1));       //окраска меню

    wprintw(menu, "  F2-Open file    |    F3-Save file and exit    |    F10-Exit without saving  ");
    wrefresh(menu);
    
    edit_wnd = newwin(LINES - 2, COLS - 2, 1, 0); //окно редактировния

    delwin(menu);//освобождаем ресурсы (меню на экране останется)

    edit_mode(edit_wnd);
    return 0;
}


void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}
