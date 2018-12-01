#include <stdio.h>

#include "header.h"

int main(int argc, char** argv)
{
    struct stat file_info;
    struct dirent** namelist_left;             //для левой части
    struct dirent** namelist_right;            //для правой части
    int number_files_left;                     //число считанных файлов левой части
    int number_files_right;                    //число считанных файлов правой части
    int cmd;                                   //команда с клавиатуры
    char curent_dir_left[MAX_PATH_SIZE] = "";  //текущая левая директория
    char curent_dir_right[MAX_PATH_SIZE] = ""; //текущая правая директория
    char editor_path[MAX_PATH_SIZE] = "";      //директория редактора
    int row_left = 2;                          //текущее положение курсора левой части
    int col_left = 1;
    int row_right = 2; //текущее положение курсора правой части
    int col_right = 1;
    int file_number_left = 0;  //текущий (выделенный *) номер файла слева
    int file_number_right = 0; //текущий (выделенный *) номер файла справа
    int mode = 0;              //режим (0-левая часть;1-правая часть)
    pid_t pid;                 //идентификатор процесса

    WINDOW* left_wnd;           //левое окно с именами файлов
    WINDOW* left_wnd_property;  //левое окно c параметрами
    WINDOW* right_wnd;          //правое окно с именами
    WINDOW* right_wnd_property; //правое окно с параметрами
    WINDOW* left_design;        //оформление левой части
    WINDOW* right_design;       //оформление правой части
    WINDOW* help_wnd;           //окно описания функционала
    WINDOW* current_left;       //окно для текущей директории левой части
    WINDOW* current_right;      //окно для текущей директории правой части

    initscr();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1); //преобразование ESC-последовательностей в константы
    noecho();
    cbreak();
    curs_set(0);   //отображение курсора отключено (* управление)
    start_color(); //включение поддержки цветов
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);

    refresh();

    /*создание окон*/
    left_design = newwin(LINES - 2, COLS / 2, 0, 0);
    right_design = newwin(LINES - 2, COLS / 2, 0, COLS / 2);
    left_wnd = newwin(LINES - 5, 3 * COLS / 8 - 2, 2, 2);
    right_wnd = newwin(LINES - 5, 3 * COLS / 8 - 2, 2, COLS / 2 + 2);
    left_wnd_property = newwin(LINES - 5, COLS / 8 - 2, 2, 3 * COLS / 8 + 1);
    right_wnd_property = newwin(LINES - 5, COLS / 8 - 2, 2, 7 * COLS / 8 + 1);
    help_wnd = newwin(1, COLS, LINES - 1, 0);
    current_left = newwin(1, COLS / 2, LINES - 2, 0);
    current_right = newwin(1, COLS / 2, LINES - 2, COLS / 2);
    /*оформление окон*/
    painting_wnd(left_wnd, left_wnd_property, right_wnd, right_wnd_property, left_design, right_design, help_wnd,
        current_left, current_right);
    
    /*получаем путь к редактору*/
    getcwd(editor_path, MAX_PATH_SIZE);
    strcat(editor_path,"/Editor");
    /*получение и вывод текущей директории*/
    getcwd(curent_dir_left, MAX_PATH_SIZE);
    getcwd(curent_dir_right, MAX_PATH_SIZE);
    wprintw(current_left, "%s", curent_dir_left);
    wprintw(current_right, "%s", curent_dir_right);
    
    /*получение структуры struct dirent (изначально оддинаковые)*/
    number_files_left = scandir(".", &namelist_left, 0, alphasort);
    number_files_right = scandir(".", &namelist_right, 0, alphasort);
    if(number_files_left < 0)
        perror("scandir");
    else
    { /*вывод в окна информации о файлах в текущей директории (имя и размер)*/
        for(int i = 0; i < number_files_left; i++)

        {
            stat(namelist_left[i]->d_name, &file_info);
            wprintw(left_wnd, "%s\n", namelist_left[i]->d_name);
            wprintw(left_wnd_property, "%d\n", file_info.st_size);
            wprintw(right_wnd, "%s\n", namelist_right[i]->d_name);
            wprintw(right_wnd_property, "%d\n", file_info.st_size);
        }
    }

    /*отображение звездочек в качесте указателей*/
    wmove(left_design, row_left, col_left);
    wmove(right_design, row_right, col_right);
    wprintw(left_design, "*");
    wprintw(right_design, "*");
    /*обновление текущей (начальной) конфигурации*/
    wrefresh(left_design);
    wrefresh(right_design);
    wrefresh(left_wnd);
    wrefresh(right_wnd);
    wrefresh(left_wnd_property);
    wrefresh(right_wnd_property);
    wrefresh(help_wnd);
    wrefresh(current_left);
    wrefresh(current_right);

    /*Работаем пока не нажали F10*/
    while((cmd = getch()) != KEY_F(10))
    {
        wmove(left_design, row_left, col_left); //меняем положение курсоров
        wmove(right_design, row_right, col_right);

        switch(cmd)
        {
        case KEY_DOWN: //движение * (курсора) вниз
            if(mode == 0)
            {
                if(row_left <= number_files_left) //пока меньше числа файлов
                    down(left_design, &row_left, col_left, &file_number_left);
            }
            else
            {
                if(row_right <= number_files_right)
                    down(right_design, &row_right, col_right, &file_number_right);
            }
            break;
        case KEY_UP: //движение * (курсора) вверх
            if(mode == 0)
            {
                if(row_left > 2) //пока не залезли на границу другого окна
                    up(left_design, &row_left, col_left, &file_number_left);
            }
            else
            {
                if(row_right > 2)
                    up(right_design, &row_right, col_right, &file_number_right);
            }
            break;

        case 10:          // Enter - переход в другую директорию
            if(mode == 0) //если левая часть активна
            {             //сначала проверяем является ли файл директорией
                stat(namelist_left[file_number_left]->d_name, &file_info);
                if(S_ISDIR(file_info.st_mode)) // если это директория
                {
                    strcpy(curent_dir_left, namelist_left[file_number_left]->d_name); //запоминаем текущее имя файла
                    free_entries(namelist_left, number_files_left); //особождаем память предыдущей структуры dirent
                    /*выделяем память и получаем новую структуру dirent*/
                    read_entries(curent_dir_left, &number_files_left, &namelist_left);
                    if(number_files_left < 0)
                    { /*если прав доступа недостаточно, то получаем текущий ПОЛНЫЙ путь и формируем структуру dirent*/
                        getcwd(curent_dir_left, MAX_PATH_SIZE);
                        read_entries(curent_dir_left, &number_files_left, &namelist_left);
                    }
                    else //если прав достаточно,то переходим в выбранную директорию
                        chdir(curent_dir_left);

                    /*очистка окон*/
                    wclear(left_wnd);
                    wclear(left_wnd_property);
                    /*вывод имен и размеров в окна left_wnd, left_wnd_property*/
                    print_entries(left_wnd, left_wnd_property, namelist_left, number_files_left);
                    /*получаем новый путь и выводим в соответствующее окно*/
                    getcwd(curent_dir_left, MAX_PATH_SIZE);
                    wclear(current_left);
                    wprintw(current_left, "%s", curent_dir_left);
                    wrefresh(current_left);
                    /*cбрасываем номер текущего файла*/
                    file_number_left = 0;
                    /*установка указателя (*) в начало открытой директории*/
                     initial_position(left_design,&row_left, &col_left);                   
                }
                if(S_ISREG(file_info.st_mode)) // если это обычный файл
                {
                    pid=fork();
                    if (pid == 0)
                    {
                        if (execl(editor_path, "Editor", namelist_left[file_number_left]->d_name, NULL) < 0)
                        {               
                            exit(-1);
                        }  
                    }
                    else
                    {
                        wait(NULL);
                        //обновление окон после закрытие редактора                      
                        init_again(left_wnd, left_wnd_property, right_wnd, right_wnd_property, left_design, right_design, help_wnd,
        current_left, current_right);
                    }
                }
                
            }
            else //правая часть активна
            {   //дальнейшие действия аналогичны
                stat(namelist_right[file_number_right]->d_name, &file_info);
                if(S_ISDIR(file_info.st_mode))
                {
                    strcpy(curent_dir_right, namelist_right[file_number_right]->d_name);
                    free_entries(namelist_right, number_files_right);
                    read_entries(curent_dir_right, &number_files_right, &namelist_right);

                    if(number_files_right < 0)
                    {
                        getcwd(curent_dir_right, MAX_PATH_SIZE);
                        read_entries(curent_dir_right, &number_files_right, &namelist_right);
                    }
                    else
                        chdir(curent_dir_right);

                    wclear(right_wnd);
                    wclear(right_wnd_property);

                    print_entries(right_wnd, right_wnd_property, namelist_right, number_files_right);

                    getcwd(curent_dir_right, MAX_PATH_SIZE);
                    wclear(current_right);
                    wprintw(current_right, "%s", curent_dir_right);
                    wrefresh(current_right);
                    
                    file_number_right = 0;

                    initial_position(right_design, &row_right, &col_right);                   
                }
                if(S_ISREG(file_info.st_mode)) // если это обычный файл
                {
                    pid=fork();
                    if (pid == 0)
                    {
                        if (execl(editor_path, "Editor", namelist_right[file_number_right]->d_name, NULL) < 0)
                        {               
                            exit(-1);
                        }  
                    }
                    else
                    {
                        wait(NULL);
                        /*обновление окон после закрытие редактора*/                        
                        init_again(left_wnd, left_wnd_property, right_wnd, right_wnd_property, left_design, right_design, help_wnd,
        current_left, current_right);
                    }
                }
            }
            break;

        case '\t': // tab
        {
            change_mode(current_left, current_right, &mode, curent_dir_left, curent_dir_right);
            break;
        }
        }
        wrefresh(left_design);
        wrefresh(right_design);
    }//выход из while
    
    /*Нажата кнопка F10*/
    
    /*освобождаем память для структур и окон и выходим*/
    free_entries(namelist_left, number_files_left);
    free_entries(namelist_right, number_files_right);  
    
    delwin(left_wnd);
    delwin(right_wnd);
    delwin(left_design);
    delwin(right_design);
    delwin(left_wnd_property);
    delwin(right_wnd_property);
    delwin(current_left);
    delwin(current_right);
    delwin(help_wnd);

    endwin();
    
    return 0;
}
