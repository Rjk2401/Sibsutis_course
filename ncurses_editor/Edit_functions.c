#include "header.h"

void edit_mode(WINDOW* edit_wnd,char *filename)
{
    int buf;
    int row = 1, col = 0; //нулевая строка занята меню
    
    while(1)
    {
        wrefresh(edit_wnd);
        move(row, col);
        buf = getch();
        if(buf >= MIN_SYMBOL && buf < MAX_SYMBOL) // если введены символы,а не служебные клавиши
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
                if(row > 1) //чтобы на меню не залезал
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
                    delch();
                    col--;
                }
                break;
            case 10: // Enter
                col = 0;
                if(row < LINES - 1)
                    row++;
                break;
            case KEY_F(10): // exit
                exit_editor(edit_wnd);
                break;
            case KEY_F(3): // save end exit
                save_file(filename, edit_wnd);
                delwin(edit_wnd);
                endwin();
                exit(EXIT_SUCCESS);
            case KEY_F(2): // open_file
                wclear(edit_wnd);
                wrefresh(edit_wnd);
                open_file(filename, edit_wnd,NO_ARGV_INPUT);
                wrefresh(edit_wnd);
                row = 1;
                col = 0;
                break;
            default:
                break;
            }
        }
    }
}

void open_file(char* filename, WINDOW* edit_wnd,int argv_or_not)
{
    WINDOW* subwnd;
    int file_fd;
    int numRead;
    char buf;
    //создаем окно для ввода имени файла (более менее в центре)
    subwnd = derwin(edit_wnd, 6, 30, LINES / 2 - 5, COLS / 2 - 10);
    wbkgd(subwnd, COLOR_PAIR(1)); //закрашиваем окно и шрифт 
    if (argv_or_not==0) //если открытие не через переданный параметр argv[1]
    {       
    wprintw(subwnd, "Enter path to file:\n");
    wgetnstr(subwnd, filename, MAX_NAME_SIZE);
    filename[MAX_NAME_SIZE] = 0;
    wrefresh(subwnd);
    }

    file_fd = open(filename, O_RDONLY);

    if(file_fd <= 0) //если файл не открылся
    {
        wprintw(subwnd, "File %s not open!\n", filename);
        wprintw(subwnd, "Press any button to close");
        wgetch(subwnd);
        wclear(subwnd);
        wbkgd(subwnd, COLOR_PAIR(0));
        delwin(subwnd);
    }
    else //если файл открылся
    {
        move(1, 0);
        do
        {
            // чтение по одному символу из файла
            numRead = read(file_fd, &buf, 1);
            addch(buf);
        } while(numRead > 0);

        close(file_fd);
        wclear(subwnd);
        wbkgd(subwnd, COLOR_PAIR(0));
        delwin(subwnd);
        wrefresh(edit_wnd);
    }
}

void exit_editor(WINDOW* edit_wnd)
{
    WINDOW* subwnd;
    char answer[4]; //да или нет
    wrefresh(edit_wnd);

    //создаем окно для подтверждения выхода
    subwnd = derwin(edit_wnd, 6, 27, LINES / 2 - 5, COLS / 2 - 10);
    wbkgd(subwnd, COLOR_PAIR(1));
    wprintw(subwnd, "Exit from editor?(yes/no):\n");
    //пока не ввели yes или no запрашиваем их ввод
    do
    {
        wgetnstr(subwnd, answer, 4);
    } while((strcmp(answer, "yes") != 0) && (strcmp(answer, "no") != 0));

    //если ввели "да"- выходим
    if(strcmp(answer, "yes") == 0)
    {
        delwin(subwnd);
        delwin(edit_wnd);
        endwin();
        exit(EXIT_SUCCESS);
    }
    else //если "нет" очищаем и удаляем под-окно
    {
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
    int num_cols_in_row; //число столбцов в строки
    WINDOW* subwnd;

    if(strcmp(filename, " ") == 0) //если имя пустое
    {
        //создаем окно для ввода нового имени файла
        subwnd = derwin(edit_wnd, 6, 30, LINES / 2 - 5, COLS / 2 - 10);
        wbkgd(subwnd, COLOR_PAIR(1)); //закрашиваем окно и шрифт
        wprintw(subwnd, "Enter name new file:\n");
        wgetnstr(subwnd, filename, MAX_NAME_SIZE);
        filename[MAX_NAME_SIZE] = 0;
        wclear(subwnd);
        wbkgd(subwnd, COLOR_PAIR(0));
        delwin(subwnd);
        wrefresh(edit_wnd);
    }

    file_fd = open(filename, O_WRONLY | O_CREAT, 0666);

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
            /*определяем длину текущей строки (до пробела)=> пробелы в конце строки удаляются*/
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