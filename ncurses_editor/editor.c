#include <curses.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MAX_NAME_SIZE 20

void edit_mode(WINDOW* edit_wnd);
void save_file(char* filename, WINDOW* edit_wnd);
void open_file(char* filename, WINDOW* edit_wnd);

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char** argv)
{
    WINDOW* menu;     //меню действий
    WINDOW* edit_wnd; //окно редактирования

    initscr();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1);
    cbreak();
    curs_set(1);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLUE); //цвета для меню
    refresh();
    menu = newwin(1, COLS - 1, 0, 0); //окно меню
    wbkgd(menu, COLOR_PAIR(1));       //окраска меню

    wprintw(menu, "  F2-Open file    |    F3-Save file    |    F10-Exit without saving  ");
    wrefresh(menu);

    edit_wnd = newwin(LINES - 2, COLS - 2, 1, 0); //окно редактировния

    edit_mode(edit_wnd);
}

void edit_mode(WINDOW* edit_wnd)
{
    int buf, row = 1, col = 0;
    char filename[MAX_NAME_SIZE] = " ";
    while(1)
    {
        wrefresh(edit_wnd);
        move(row, col);
        buf = getch();
        if(buf >= 32 && buf < 127) // если введены символы,а не служебные клавиши
        {
            col++;
        }
        else //служебные клавиши
        {
            switch(buf)
            {
            case KEY_DOWN:
                if(row < LINES - 1)
                    row++;
                break;
            case KEY_UP:
                if(row > 1)
                    row--;
                break;
            case KEY_LEFT:
                if(col > 0)
                    col--;
                break;
            case KEY_RIGHT:
                if(col < COLS - 1)
                    col++;
                break;
            case KEY_BACKSPACE:
                wmove(edit_wnd, row - 1, col - 1);
                if(col > 0)
                {
                    wdelch(edit_wnd);
                    col--;
                }
                break;
            case 10: // Enter
                col = 0;
                if(row < LINES - 1)
                    row++;
                break;
            case KEY_F(10): // exit
                endwin();
                exit(EXIT_SUCCESS);
            case KEY_F(3): // save

                if(strcmp(filename, " ") != 0) //если имя не пустое
                {
                    save_file(filename, edit_wnd);
                    endwin();
                    exit(EXIT_SUCCESS);
                }
                else
                    break;
            case KEY_F(2): // open_file
                open_file(filename, edit_wnd);
                wrefresh(edit_wnd);
                break;
            default:
                break;
            }
        }
    }
}

void open_file(char* filename, WINDOW* edit_wnd)
{
    WINDOW* subwnd;
    int file_fd;
    int numRead;
    char buf;
    //создаем окно для ввода имени файла
    subwnd = derwin(edit_wnd, 6, 27, LINES / 2 - 5, COLS / 2 - 10);
    wbkgd(subwnd, COLOR_PAIR(1));
    wprintw(subwnd, "Enter path to file or ESC:\n");
    wgetnstr(subwnd, filename, MAX_NAME_SIZE);
    filename[MAX_NAME_SIZE] = 0;
    wrefresh(subwnd);

    file_fd = open(filename, O_RDONLY);

    if(file_fd <= 0) //если файл не открылся
    {
        wprintw(subwnd, "File %s not open!\n", filename);
        wprintw(subwnd, "Press any button to close");
        wgetch(subwnd);
        wclear(subwnd);
        wbkgd(subwnd, COLOR_PAIR(0));
        delwin(subwnd);
	filename=" ";
    }
    else //если файл открылся
    {

        move(1, 0);
        do
        {
            // чтение по одному символу из файла
            numRead = read(file_fd, &buf, 1);
            printw("%c", buf);
        } while(numRead > 0);

        close(file_fd);
        wclear(subwnd);
        wbkgd(subwnd, COLOR_PAIR(0));
        delwin(subwnd);
        wrefresh(edit_wnd);
    }
}

void save_file(char* filename, WINDOW* edit_wnd)
{
    int file_fd;
    int row = 1, col = 0;
    char ch;
    int num_cols_in_row = COLS;

    file_fd = open(filename, O_WRONLY);

    if(file_fd <= 0)
    {
        move(5, 10);
        wprintw(edit_wnd, "File %s not found !\n", filename);
    }
    else
    {
        for(row = 1; row < LINES - 1; row++)
        {
            num_cols_in_row = COLS - 1;
            /*определяем длину строки (до пробела)=> пробелы в конце строки удаляются*/
            while(mvinch(row, num_cols_in_row) == ' ' && num_cols_in_row >= 0)
                num_cols_in_row--;

            for(col = 0; col <= num_cols_in_row; col++)
            {
                ch = mvinch(row, col);
                write(file_fd, &ch, 1); //запись текущего символа в строке
            }
            ch = '\n';
            write(file_fd, &ch, 1); //перевод на новую строку
        }

        close(file_fd);
    }
}
