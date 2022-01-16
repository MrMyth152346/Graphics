#include<iostream>
#include<unordered_map>
#include<stdlib.h>
#include<string>
#if defined(WIN32) or defined(_WIN32) or defined(__WIN32__) or defined(__NT__)
#include<Windows.h>
#endif
#if defined(__linux__) or defined(linux)
#include<unistd.h>
#include<sys/ioctl.h>
#endif

#define BLACK  MYTH::FUNCTIONS::Color::get(1)
#define PURPLE MYTH::FUNCTIONS::Color::get(2)
#define GRAY   MYTH::FUNCTIONS::Color::get(3)
#define BLUE   MYTH::FUNCTIONS::Color::get(4)
#define GREEN  MYTH::FUNCTIONS::Color::get(5)
#define CYAN   MYTH::FUNCTIONS::Color::get(6)
#define RED    MYTH::FUNCTIONS::Color::get(7)
#define PINK   MYTH::FUNCTIONS::Color::get(8)
#define YELLOW MYTH::FUNCTIONS::Color::get(9)
#define WHITE  MYTH::FUNCTIONS::Color::get(10)

namespace MYTH
{
    struct window
    {
        int _columns_;
        int _rows_;
    };

    struct TEXTBOX
    {
        int x = 0;
        int y = 0;
        int border_x = 0;
        int border_y = 0;
        int text_color;
        int border_color;
        bool border_auto = true;
        bool text_aligned = true;
        bool border_visible = true;
        std::string text;
        std::string border_urc = "+";
        std::string border_drc = "+";
        std::string border_ulc = "+";
        std::string border_dlc = "+";
        std::string border_s = "-";
        std::string border_u = "|";
    };

    namespace FUNCTIONS 
    {
        int init()
        {
            #if defined(WIN32) or defined(_WIN32) or defined(__WIN32__) or defined(__NT__)
                #define WINDOWS
                return 1;
            #elif defined(__linux__) or defined(linux)
                #define LINUX
                return 2;
            #else
                std::cout << "Unsupported system." << std::endl;
                exit(0);
            #endif
        }

        namespace Mouse
        {
            void set_Pos(int x, int y)
            {
                #ifdef WINDOWS
                    COORD Coord;

                    Coord.X = x;
                    Coord.Y = y;

                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);
                #elif defined(LINUX)
                    printf("\033[%d;%dH", y+1, x+1);
                #endif
            }

            void set_Visible(bool condition)
            {
                #ifdef WINDOWS
                    CONSOLE_CURSOR_INFO info;
                    info.dwSize = 100;
                    info.bVisible = condition;
                    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
                #endif
            }

        }
        
        namespace Color
        {
            int get(int color)
            {
                #ifdef WINDOWS
                    switch (color)
                    {
                        case 1:
                            return 0;
                        case 2:
                            return 5;
                        case 3:
                            return 8; 
                        case 4:
                            return 9;
                        case 5:
                            return 10; 
                        case 6:
                            return 11; 
                        case 7:
                            return 12;
                        case 8:
                            return 13;
                        case 9:
                            return 14;
                        case 10:
                            return 15;  
                        default:
                            return 15;
                    }
                #elif defined(LINUX)
                    switch (color)
                    {
                        case 1:
                            return 30;
                        case 2:
                            return 35;
                        case 3:
                            return 90;
                        case 4:
                            return 34;
                        case 5:
                            return 32;
                        case 6:
                            return 36;
                        case 7:
                            return 31;
                        case 8:
                            return 32;
                        case 9:
                            return 95;
                        case 10:
                            return 97;
                        default:
                            return 97;
                    }
                #endif
            }

            void set(int color)
            {
                #ifdef WINDOWS
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, color);
                #elif defined(LINUX) 
                    std::cout << "\033[" + std::to_string(color) + "m";
                #endif
            }
        }
        
        void clearConsole()
        {
            #ifdef WINDOWS
                system("cls");
            #elif defined(LINUX)
                system("clear");
            #endif
        }
        

        window get_WindowSize()
        {
            #ifdef WINDOWS
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                window Window;

                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

                Window._columns_ = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                Window._rows_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

                return Window;
            #elif defined(LINUX)
                struct winsize w;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

                window Window;

                Window._columns_ = w.ws_col;
                Window._rows_ = w.ws_row;

                return Window;
            #endif
        }
    }
    
    class GRAPHICS 
    {
    public:

            void print_Textbox(TEXTBOX textbox)
            {
                
                window window = FUNCTIONS::get_WindowSize();

                std::unordered_map<int, std::string> words;
                std::string word = "";
                std::string longest_word = "";
                int word_number = 0;
                int lines = 0;

                for (size_t i = 0; i <= textbox.text.size(); i++)
                {
                    if (textbox.text[i] == '\n')
                        lines++;

                    if (textbox.text[i] == '\n' && word != "" || i == textbox.text.size()) 
                    {   
                        if (longest_word == "")
                            longest_word = word;
                        if (longest_word != "" && word.size() > longest_word.size())
                            longest_word = word;
                        words[word_number] = word;
                        word = "";
                        word_number++;
                    }else 
                    {
                        word += textbox.text[i];
                    }
                }
                int text_x_start;
                int text_x_end;
                int text_y;
                int num = 1;

                if (textbox.border_auto == true)
                {
                    text_x_start = textbox.x - (longest_word.size() / 2);
                    text_x_end = textbox.x + (longest_word.size() / 2);
                    text_y = textbox.y;
                }
                else if (textbox.border_x != 1 && textbox.border_y != 1)
                {
                    text_x_start = textbox.x - (textbox.border_x / 2);
                    text_x_end = textbox.x + (textbox.border_x / 2);
                    text_y = textbox.y;
                }
                else
                {
                    text_x_start = 1;
                    text_x_end = 1;
                    text_y = 1;
                }
                    

                FUNCTIONS::Color::set(textbox.text_color);

                int next_line = 0;
                FUNCTIONS::Mouse::set_Pos(text_x_start, text_y);
                for (size_t i = 0; i < words.size(); i++)
                {
                    if (textbox.text_aligned == true && words[i].size() != longest_word.size()) {
                        for (int z = 0; z < ((longest_word.size() / 2) - (words[i].size() / 2)); z++)
                            std::cout << " ";
                        std::cout << words[i];
                    }
                    else if (textbox.border_auto == false) {
                        for (int z = 0; z < text_x_end - text_x_start; z++)
                            if (z <= textbox.text.size())
                                std::cout << words[i][z];
                    }
                    else {
                        std::cout << words[i];
                    }
                    if (i != words.size()) {
                        FUNCTIONS::Mouse::set_Pos(text_x_start, text_y + num);
                        num++;
                    }
                }

                if (textbox.border_visible == true) {
                    if (textbox.border_color != 0)
                        FUNCTIONS::Color::set(textbox.border_color);

                    for (int i = 0; i <= (lines + textbox.border_y); i++)
                    {
                        FUNCTIONS::Mouse::set_Pos(text_x_start - 1, text_y + lines - i);
                        std::cout << textbox.border_u;
                        FUNCTIONS::Mouse::set_Pos(text_x_end + 1, text_y + lines - i);
                        std::cout << textbox.border_u;
                    }

                    FUNCTIONS::Mouse::set_Pos(text_x_start - 1, text_y - textbox.border_y - 1);
                    for (int i = text_x_start - 1; i <= text_x_end + 1; i++)
                    {
                        if (i == text_x_start - 1)
                            std::cout << textbox.border_ulc;
                        else if (i == text_x_end + 1)
                            std::cout << textbox.border_urc;
                        else
                            std::cout << textbox.border_s;
                    }
                    FUNCTIONS::Mouse::set_Pos(text_x_start - 1, text_y + lines + 1);
                    for (int i = text_x_start - 1; i <= text_x_end + 1; i++)
                    {
                        if (i == text_x_start - 1)
                            std::cout << textbox.border_dlc;
                        else if (i == text_x_end + 1)
                            std::cout << textbox.border_drc;
                        else
                            std::cout << textbox.border_s;
                    }
                }
                
                FUNCTIONS::Color::set(WHITE);
            }
    };
}
