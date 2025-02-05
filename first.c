// 403105809
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <math.h>
#define min_number_of_rooms 5
#define max_number_of_room 10
#define min_size_of_h_w 8

typedef struct
{
    int x;
    int y;
} pair;

typedef struct
{
    pair point;
    int health;
    char type;
    int room;
    bool awake;
} monster;

typedef struct
{
    int score;
    int max_items;
    char name[50];
    char email[70];
    char password[50];
    char security_word[200];
    char difficulty[20];
    char hero_color[10];
    int gold;
    int exprience;
} Gamer;

typedef struct first
{
    pair pos;
    time_t time;
} food;

typedef struct
{
    char name[80];
    int difficulty; // 1:easy  2: medium  3:hard
    int color;
    int score;
    int gold;
    pair position;
    int level;
    int health;

    int num_foods;

    int num_of_eaten_food;
    time_t start_time;
    int exprience;
    int hunger_level;

    int num_weopen;
    int num_mace;
    int num_dagger;
    int num_magic_wand;
    int num_normal_arrow;
    int num_sword;

    // type of food
} player;

typedef struct
{
    int size_y;
    int y;

    int size_x;
    int x;

    bool visited;

    int doors_num;
    pair doors[4];

    int number_of_gold;

    int num_food;

    int num_monster;

} room;

void menu(Gamer *g);
void draw_border();
int sign_in(Gamer *g);
int validation_password(char *password);
int validation_email(char *eamil);
int login(Gamer *g);
void login_as_guest(Gamer *g);
char *random_pass();
void game_menu(Gamer *g, const char *filename);
void sort_scores(const char *filename, Gamer *g);
void show_scores(player *scores, int i, Gamer *g, int);
void new_game(Gamer *g);

int dfs_visit(char **map, char **mark, int x, int y, pair **parent, pair finish, int, int);
void initialize(char ***mark, pair ***parent);
void draw_path(pair start, pair current, pair **parent, char **map);
void setting(Gamer *g);
void difficulty(Gamer *g);
void hero_setting(Gamer *g);
void resume_the_level(Gamer *g, player user, int rows, int cols, char map[rows][cols],
                      int level, int new1_floor_or_old2);
void continue_game(Gamer *g, const char *filename);

int main()
{
    Gamer g;
    setlocale(LC_ALL, "");
    initscr();
    clear();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); //

    start_color();
    halfdelay(1);
    can_change_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);

    menu(&g);
    clear();
    draw_border();
    const char *filename = "scores.txt";
    game_menu(&g, filename);

    char game_all_scores[100][1000];

    refresh();
    getch();
    endwin();
    return 0;
}

void menu(Gamer *g)
{
    char *menu[] = {"sign in", "login", "login as guest"};
    int choice = 0;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(menu[i])) / 2, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 3 - 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 3 - 1) ? 0 : choice + 1;
        else if (ch == 10)
        {

            if (choice == 0)
            {
                if (sign_in(g))
                    break;
            }
            else if (choice == 1)
            {
                if (login(g))
                    break;
            }
            else if (choice == 2)
            {
                login_as_guest(g);
                break;
            }
        }
    }
    refresh();
    return;
}

void draw_border()
{
    start_color();
    for (int x = 0; x < COLS; x += 2)
    {
        mvprintw(0, x, "-");
        mvprintw(LINES - 1, x, "-");
    }
    for (int x = 1; x < COLS; x += 2)
    {
        mvprintw(0, x, "*");
        mvprintw(LINES - 1, x, "*");
    }
    for (int y = 0; y < LINES; y += 2)
    {
        mvprintw(y, 0, "^");
        mvprintw(y, COLS - 1, "^");
    }
    for (int y = 1; y < LINES; y += 2)
    {
        mvprintw(y, 0, "*");
        mvprintw(y, COLS - 1, "*");
    }
}

int sign_in(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);

    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if (access(new_user, F_OK) == 0)
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! USERNAMME IS ALREADY TAKEN !!")) / 2, "!! USERNAMME IS ALREADY TAKEN !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("welcome !") - 30) / 2 + 4, "welcom %s !", g->name);
    mvprintw((LINES - 2) / 2 + 3, (COLS - strlen("Enter your email :")) / 2, "Enter your email : ");
    getnstr(g->email, 70);
    char email[100];
    strcpy(email, g->email);

    if (!validation_email(email))
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! INVALID EMAL ADDRESS !!")) / 2, "!! INVALID EMAIL ADDRESS !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your password if you need random password write ::rand :")) / 2, "Enter your password if you need random password write ::rand : ");
    getnstr(g->password, 50);
    if (strcmp(g->password, "::rand") == 0)
    {
        strcpy(g->password, random_pass());
    }

    char password[100];
    strcpy(password, g->password);

    if (!validation_password(password))
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! INVALID PASSWORD !!")) / 2, "!! INVALID PASSWORD !!");
        attroff(COLOR_PAIR(1));
        refresh();
        return 0;
    }
    clear();
    draw_border();
    mvprintw(((LINES - 2) / 2), (COLS - 2 - strlen("please write your highschool name you can use it instead of password ")) / 2, "please write your highschool name you can use it instead of password ");
    getnstr(g->security_word, 100);

    mkdir(new_user, 0777); // new folder with name of user

    char user_info[200];
    sprintf(user_info, "./%s/%s.info.txt", g->name, g->name);
    FILE *info = fopen(user_info, "w");
    fprintf(info, "name : %s\n", g->name);
    fprintf(info, "email : %s\n", g->email);
    fprintf(info, "password : %s\n", g->password);
    fprintf(info, "security_word : %s\n", g->security_word);
    fclose(info);
    refresh();
    return 1;
}

int validation_email(char *email)
{
    int length = strlen(email);
    int sign = 0;
    int sign_num;
    int signdot = 0;
    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@' && i > 0)
        {
            sign = 1;
            sign_num = i;
        }
        if (email[i] == '.' && i > sign_num + 1 && sign_num > 0 && i < length - 1)
        {
            signdot = 1;
        }
    }
    if (sign && signdot)
    {
        return 1;
    }

    return 0;
}

int validation_password(char *password)
{
    int length = strlen(password);
    if (length < 7)
    {
        return 0;
    }
    int digit = 0;
    int upercase = 0;
    int small = 0;
    for (int i = 0; i < length; i++)
    {
        char c = password[i];

        if (c >= '1' && c <= '9')
        {
            digit = 1;
        }
        if (c >= 'a' && c <= 'z')
        {
            small = 1;
        }
        if (c >= 'A' && c <= 'Z')
        {
            upercase = 1;
        }
    }
    if (digit && upercase && small)
    {
        return 1;
    }
    return 0;
}

int login(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);

    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if (access(new_user, F_OK) != 0)
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! username not found !!")) / 2, "!! username not found !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your password  if you forgot it write ::forgot:")) / 2, "Enter your password  if you forgot it write ::forgot: ");
    getnstr(g->password, 50);

    char password[100];
    strcpy(password, g->password);
    char user_info[200];
    sprintf(user_info, "./%s/%s.info.txt", g->name, g->name);

    FILE *file = fopen(user_info, "r");

    if (strcmp(g->password, "::forgot") == 0)
    {
        clear();
        draw_border();
        mvprintw((LINES - 2) / 2, (COLS - strlen("write your highschool name : ")) / 2, "write your highschool name : ");
        char school[100], school_name[100];
        getnstr(school_name, 100);
        char temp[100];
        while (fgets(temp, 100, file))
        {
            if (strstr(temp, "security_word : ") != NULL)
            {
                sscanf(temp, "security_word : %s", school);
                break;
            }
        }
        fclose(file);
        if (strcmp(school, school_name) == 0)
        {
            return 1;
        }
        else
        {
            attron(COLOR_PAIR(1));
            mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! it's wrong !!")) / 2, "!! it's wrong !!");
            attroff(COLOR_PAIR(1));
            return 0;
        }
    }

    else
    {
        char temp[200];
        char user_password[100];
        while (fgets(temp, 200, file))
        {
            if (strstr(temp, "password : ") != NULL)
            {
                sscanf(temp, "password : %s", user_password);
                break;
            }
        }
        fclose(file);

        if (strcmp(password, user_password) == 0)
        {
            return 1;
        }
        else
        {
            attron(COLOR_PAIR(1));
            mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! password is incorrect !!")) / 2, "!! password is incorrect !!");
            attroff(COLOR_PAIR(1));
            refresh();
            getch();
            return 0;
        }
    }
}

void login_as_guest(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("press a key to start")) / 2, "press a key to start");

    getch();
    strcpy(g->difficulty, "MEDIUM"); // default medium for guests
    strcpy(g->hero_color, "GREEN");
}

char *random_pass()
{
    static char pass[12];
    srand(time(NULL)); // yeksan nabashand
    char number[2];
    char uper[5];
    char small[5];
    for (int i = 0; i < 2; i++)
    {
        int n = rand();
        number[i] = (n % 10) + '0'; // random digit}
    }
    for (int i = 0; i < 5; i++)
    {
        int up = rand();
        uper[i] = (up % 26) + 'A';
    }
    for (int i = 0; i < 4; i++)
    {
        int sma = rand();
        small[i] = (sma % 26) + 'a';
    }
    pass[0] = number[1];
    pass[1] = uper[0];
    pass[2] = uper[1];
    pass[3] = small[0];
    pass[4] = number[0];
    pass[5] = uper[2];
    pass[6] = uper[3];
    pass[7] = small[1];
    pass[8] = small[2];
    pass[9] = small[3];
    pass[10] = uper[4];
    pass[11] = '\0';
    return pass;
}

void game_menu(Gamer *g, const char *filename)
{
    char *choices[] = {
        "New Game",
        "Continue Previous Game",
        "Score Table",
        "Setting"};
    int choice = 0;
    int num_choices = 4;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < num_choices; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw((LINES - num_choices) / 2 + i, (COLS - strlen(choices[i])) / 2, "%s", choices[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? num_choices - 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == num_choices - 1) ? 0 : choice + 1;
        else if (ch == 10)
        {
            if (choice == 0)
            {
                new_game(g);
                break;
            }
            else if (choice == 1)
            {
                continue_game(g, filename);
                break;
            }
            else if (choice == 2)
            {
                sort_scores(filename, g);
            }
            else if (choice == 3)
            {
                setting(g);
            }
        }
    }

    refresh();
    return;
}

void setting(Gamer *g)
{
    char *string[] = {"Difficulty", "Hero setting", "Back"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                difficulty(g);
            }
            else if (choice == 1)
            {
                hero_setting(g);
            }
            else if (choice == 2)
            {
                return;
            }
        }
    }
}

void difficulty(Gamer *g)
{
    char *string[] = {"EASY", "MEDIUM", "HARD"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                strcpy(g->difficulty, "EASY");
            }
            else if (choice == 1)
            {
                strcpy(g->difficulty, "MEDIUM");
            }
            else if (choice == 2)
            {
                strcpy(g->difficulty, "HARD");
            }
            break;
        }
    }
}

void hero_setting(Gamer *g)
{

    char *hero_color[] = {"RED", "BLUE", "GREEN"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(hero_color[i])) / 2, "%s", hero_color[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                strcpy(g->hero_color, "RED");
            }
            else if (choice == 1)
            {
                strcpy(g->hero_color, "BLUE");
            }
            else if (choice == 2)
            {
                strcpy(g->hero_color, "GREEN");
            }
            break;
        }
    }
}

/* void scores_save(g)
{
FILE * file= fopen (scores, "a");
if(file==NULL){
    file = fopen(scores, "w");
}
fprintf(file, "%s : %d\n",g->name, g->score);
fclose(file);
} */

void sort_scores(const char *filename, Gamer *g)
{
    player scores[100];
    int i = 0;

    FILE *file = fopen(filename, "r");

    while (fscanf(file, "%[^:]:%d:%d:%d\n", scores[i].name, &scores[i].score, &scores[i].gold, &scores[i].exprience) != EOF)
    {
        i++;
    }
    fclose(file);

    for (int j = 0; j < i - 1; j++)
    {
        for (int h = j + 1; h < i; h++)
        {
            if (scores[h].score > scores[j].score)
            {
                player temp = scores[j];
                scores[j] = scores[h];
                scores[h] = temp;
            }
        }
    }

    int current_person;

    for (int k = 0; k < i; k++)
    {
        if (strcmp(g->name, scores[k].name) == 0)
        {
            current_person = k;
        }
    }

    show_scores(scores, i, g, current_person);
}

void show_scores(player *scores, int count, Gamer *g, int current_person)
{
    clear();
    int minute, second;
    int c = '\0';
    draw_border();

    if (count == 0)
    {
        mvprintw((LINES - 2) / 2, (COLS - 2) / 2, "No score to display");
    }
    else
    {
        int h = 0;
        while (c != 'q')
        {
            clear();
            mvprintw(7, 25 + 12, "%s", "Name");
            mvprintw(7, 40 + 12, "%lc", L'⭐');
            mvprintw(7, 57 + 12, "%lc", L'💰');
            mvprintw(7, 70 + 12, "%lc", L'🕝');

            for (int i = h; i < h + 6; i++)
            {
                if (i == 0)
                {
                    if (i == current_person)
                    {
                        attron(COLOR_PAIR(4));
                        mvprintw(12, 10 + 12, "%lc", L'🥇');
                        mvprintw(12, 25 + 12, "%s", scores[0].name);
                        mvprintw(12, 40 + 12, "%d", scores[0].score);
                        mvprintw(12, 57 + 12, "%d", scores[0].gold);
                        int minute = (scores[0].exprience) / 60;
                        int second = scores[0].exprience % 60;
                        mvprintw(12, 70 + 12, "%d:%d", minute, second);
                        attroff(COLOR_PAIR(4));
                    }
                    else
                    {
                        mvprintw(12, 10 + 12, "%lc", L'🥇');
                        mvprintw(12, 25 + 12, "%s", scores[0].name);
                        mvprintw(12, 40 + 12, "%d", scores[0].score);
                        mvprintw(12, 57 + 12, "%d", scores[0].gold);
                        minute = (scores[0].exprience) / 60;
                        second = scores[0].exprience % 60;
                        mvprintw(12, 70 + 12, "%d:%d", minute, second);
                    }
                }

                else if (i == 1)
                {
                    if (i == current_person)
                    {
                        attron(COLOR_PAIR(4));
                        mvprintw(17, 10 + 12, "%lc", L'🥈');
                        mvprintw(17, 25 + 12, "%s", scores[1].name);
                        mvprintw(17, 40 + 12, "%d", scores[1].score);
                        mvprintw(17, 57 + 12, "%d", scores[1].gold);
                        minute = (scores[1].exprience % 3600) / 60;
                        second = scores[1].exprience % 60;
                        mvprintw(17, 70 + 12, "%d:%d", minute, second);
                        attroff(COLOR_PAIR(4));
                    }
                    else
                    {
                        mvprintw(17, 10 + 12, "%lc", L'🥈');
                        mvprintw(17, 25 + 12, "%s", scores[1].name);
                        mvprintw(17, 40 + 12, "%d", scores[1].score);
                        mvprintw(17, 57 + 12, "%d", scores[1].gold);
                        minute = (scores[1].exprience % 3600) / 60;
                        second = scores[1].exprience % 60;
                        mvprintw(17, 70 + 12, "%d:%d", minute, second);
                    }
                }

                else if (i == 2)
                {
                    if (i == current_person)
                    {
                        attron(COLOR_PAIR(4));
                        mvprintw(22, 10 + 12, "%lc", L'🥉');
                        mvprintw(22, 25 + 12, "%s", scores[2].name);
                        mvprintw(22, 40 + 12, "%d", scores[2].score);
                        mvprintw(22, 57 + 12, "%d", scores[2].gold);
                        minute = (scores[2].exprience % 3600) / 60;
                        second = scores[2].exprience % 60;
                        mvprintw(22, 70 + 12, "%d:%d", minute, second);
                        attroff(COLOR_PAIR(4));
                    }
                    else
                    {
                        mvprintw(22, 10 + 12, "%lc", L'🥉');
                        mvprintw(22, 25 + 12, "%s", scores[2].name);
                        mvprintw(22, 40 + 12, "%d", scores[2].score);
                        mvprintw(22, 57 + 12, "%d", scores[2].gold);
                        minute = (scores[2].exprience % 3600) / 60;
                        second = scores[2].exprience % 60;
                        mvprintw(22, 70 + 12, "%d:%d", minute, second);
                    }
                }

                else
                {
                    if (i == current_person)
                    {
                        attron(COLOR_PAIR(4));
                        mvprintw(5 * (i - h) + 12, 10 + 12, "%d", i + 1);
                        mvprintw(5 * (i - h) + 12, 25 + 12, "%s", scores[i].name);
                        mvprintw(5 * (i - h) + 12, 40 + 12, "%d", scores[i].score);
                        mvprintw(5 * (i - h) + 12, 57 + 12, "%d", scores[i].gold);
                        minute = (scores[i].exprience % 3600) / 60;
                        second = scores[i].exprience % 60;
                        mvprintw(5 * (i - h) + 12, 70 + 12, "%d:%d", minute, second);
                        attroff(COLOR_PAIR(4));
                    }
                    else
                    {
                        mvprintw(5 * (i - h) + 12, 10 + 12, "%d", i + 1);
                        mvprintw(5 * (i - h) + 12, 25 + 12, "%s", scores[i].name);
                        mvprintw(5 * (i - h) + 12, 40 + 12, "%d", scores[i].score);
                        mvprintw(5 * (i - h) + 12, 57 + 12, "%d", scores[i].gold);
                        minute = (scores[i].exprience % 3600) / 60;
                        second = scores[i].exprience % 60;
                        mvprintw(5 * (i - h) + 12, 70 + 12, "%d:%d", minute, second);
                    }
                }
            }
            c = getch();
            refresh();
            if (c == 10)
            {
                h += 6;
            }
        }
    }
    refresh();
    getch();
}

bool player_initial_pos = 0;
int num_rooms = 0;
int initial_x, initial_y;

void initilizing_map(int rows, int cols, char map[rows][cols])
{

    for (int y = 2; y <= rows; y++)
    {
        for (int x = 2; x <= cols; x++)
        {
            map[y][x] = ' ';
        }
    }
    return;
}

int check_collision(int cols, char map[][cols], int y, int x, int width, int height)
{
    for (int y1 = y; y1 <= y + width; y1++)
    {
        for (int x1 = x; x1 <= x + height; x1++)
        {
            if (map[y1][x1] != ' ' ||
                map[y1 + 3][x1] != ' ' || map[y1][x1 + 3] != ' ' ||
                map[y1 - 3][x1] != ' ' || map[y1][x1 - 3] != ' ')
            {
                return 1;
            }
        }
    }
    return 0;
}

int check_possible_hallway(int temp_x, int temp_y, int x, int y, int rows,
                           int cols, char map[rows][cols])
{
    int count = 0;
    int y_in_way, x_in_way;
    for (y_in_way = temp_y; y_in_way != y;)
    {
        if (map[y_in_way][temp_x] == '#')
        {
            return 1;
        }
        else if (map[y_in_way][temp_x] == '|' || map[y_in_way][temp_x] == '-')
        {
            count++;
            if (count > 2)
                return 1;
        }

        if (temp_y < y)
        {
            y_in_way++;
        }
        else if (temp_y > y)
        {
            y_in_way--;
        }
    }
    for (x_in_way = temp_x; x_in_way != x;)
    {
        if (map[y_in_way][x_in_way] == '#')
        {
            return 1;
        }
        else if (map[y_in_way][x_in_way] == '|' || map[y_in_way][x_in_way] == '-')
        {
            count++;
            if (count > 2)
                return 1;
        }
        if (temp_x < x)
        {
            x_in_way++;
        }
        else if (temp_x > x)
        {
            x_in_way--;
        }
    }
    if (count > 2)
        return 1;
    return 0;
}

typedef struct
{
    char c;
    bool visited;

} cell;

int check_collision_hallway_and_room(int y, int x, int rows, int cols, char map[rows][cols])
{
    if (map[y - 1][x] == '.' || map[y + 1][x] == '.' || map[y][x + 1] == '.' || map[y][x - 1] == '.')
        return 1;

    return 0;
}

int check_position_of_object(int y, int x, int rows, int cols, char map[rows][cols], room map_rooms[6])
{
    for (int i = 0; i < 6; i++)
    {
        for (int yl = map_rooms[i].y; yl <= map_rooms[i].y + map_rooms[i].size_y; yl++)
        {
            for (int xl = map_rooms[i].x; xl <= map_rooms[i].x + map_rooms[i].size_x; xl++)
            {
                if (y == yl && x == xl)
                {
                    return i;
                }
            }
        }
    }

    // if it isn't in any room
    return 11;
}

int num_of_treasure_room = 0;
void generate_treasure_room(room map_rooms[6], int current_room)
{
    if (current_room == 3)
    {
        num_of_treasure_room = 4;
    }
    else
        num_of_treasure_room = 3;
}

int room_number_of_password_door;
monster monsts[12];

food foods[5];
int il = 1;
int zarib_henger_and_health;
int zarib_time_hunger;

time_t time_speed;
time_t time_health;
int speed;
int health;

void display_map(player *user, int cols, int rows, char map[rows][cols], room map_rooms[6], int n)
{
    clear();
    refresh();
    for (int i = 0; i < 6; i++)
    {

        for (int x = map_rooms[i].y; x <= map_rooms[i].y + map_rooms[i].size_y; x++)
        {
            for (int y = map_rooms[i].x; y <= map_rooms[i].x + map_rooms[i].size_x; y++)
            {
                if (i == num_of_treasure_room && i != 0 &&
                    (map[x][y] == '|' || map[x][y] == '-' || map[x][y] == '.' || map[x][y] == '+'))
                {
                    attron(COLOR_PAIR(5) | A_BOLD);
                    mvaddch(x, y, map[x][y]);
                    attroff(COLOR_PAIR(5));
                }

                else if (map[x][y] == '.' || map[x][y] == '-' || map[x][y] == '|' || map[x][y] == '+')
                {
                    attron(COLOR_PAIR(4) | A_BOLD);
                    mvaddch(x, y, map[x][y]);
                    attroff(COLOR_PAIR(4));
                }
                else
                {
                    if (map_rooms[i].visited == 1)
                    {
                        switch (map[x][y])
                        {
                        case '$':
                        {
                            attron(COLOR_PAIR(5) | A_BOLD);
                            mvaddch(x, y, map[x][y]);
                            attroff(COLOR_PAIR(5));
                            break;
                        }
                        case 'f':
                        {
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", '*');
                            attroff(COLOR_PAIR(3));
                            break;
                        }
                        case 'M':
                        case 'd':
                        case 'W':
                        case 'A':
                        case 'S':
                            attron(COLOR_PAIR(1) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(1));
                            break;

                        case 'D':
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(3));
                            break;
                        case 'F':
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(3));
                            break;
                        case 'G':
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(3));
                            break;
                        case 's':
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(3));
                            break;
                        case 'U':
                            attron(COLOR_PAIR(3) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(3));
                            break;

                        case 'h':

                            attron(COLOR_PAIR(1) | A_BOLD);
                            mvprintw(x, y, "%c", map[x][y]);
                            attroff(COLOR_PAIR(1));
                            break;
                        }
                    }
                    else
                    {
                        attron(COLOR_PAIR(4));
                        mvaddch(x, y, '.');
                        attroff(COLOR_PAIR(4));
                    }
                }
            }
        }
    }

    for (int y = 2; y <= rows; y++)
    {
        for (int x = 2; x <= cols; x++)
        {
            if (map[y][x] == '#')
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, map[y][x]);
                attroff(COLOR_PAIR(2));
            }
            if (map[y][x] == '<')
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, '<');
                attroff(COLOR_PAIR(2));
            }
        }
    }
    // mvaddch(user->position.y, user->position.x, 'H');
    time_t current_time;
    time(&current_time);
    user->exprience = (int)difftime(current_time, user->start_time);
    refresh();
    attron(COLOR_PAIR(3));
    mvprintw(LINES - 1, 2, "level: %d", user->level);
    mvprintw(LINES - 1, 15, "Gold: %d %lc", user->gold, L'💰');
    mvprintw(LINES - 1, 30, "score : %d %lc", user->score, L'⭐');
    // har 30 second hunger level+=1

    user->hunger_level = user->exprience / zarib_time_hunger;
    user->health -= user->hunger_level / (il * 3);

    if (user->hunger_level / (il * 3) != 0)
    {
        il++;
    }

    mvprintw(LINES - 1, 47, "Health :");
    for (int i = 1; i <= user->health; i++)
    {
        mvprintw(LINES - 1, i + 47 + strlen("Health :"), "%lc", L'🟩');
    }
    attroff(COLOR_PAIR(3) | A_BOLD);

    return;
}

int number_of_gold;
void gold_placed(room map_rooms[6], int rows, int cols, char map[rows][cols])
{
    // except room 4 , 3 all rooms have 4 golds
    for (int i = 0; i < 3; i++)
    {
        for (int k = 0; k <= 2; k++)
        {
            int x, y;
            do
            {
                x = rand() % map_rooms[i].size_x + map_rooms[i].x;
                y = rand() % map_rooms[i].size_y + map_rooms[i].y;
            } while (map[y][x] != '.');
            map_rooms[i].number_of_gold++;
            map[y][x] = '$';
        }
    }
}

void health_placed(room map_rooms[6], int rows, int cols, char map[rows][cols])
{
    srand(time(NULL));
    int a2 = rand() % 6;

    int y, x;

    do
    {
        x = rand() % map_rooms[a2].size_x + map_rooms[a2].x;
        y = rand() % map_rooms[a2].size_y + map_rooms[a2].y;
    } while (map[y][x] != '.');
    map[y][x] = 'h';
    time_health = time(NULL);
}

void food_placed(room map_rooms[6], int rows, int cols, char map[rows][cols])
{
    // except room 4 , 3 all rooms have 4 golds
    for (int k = 1; k < 3; k++)
    {
        int i;
        do
        {
            i = rand() % 6;
        } while (map_rooms[i].num_food != 0);

        for (int k = 0; k <= 2; k++)
        {
            int x, y;
            do
            {
                x = rand() % map_rooms[i].size_x + map_rooms[i].x;
                y = rand() % map_rooms[i].size_y + map_rooms[i].y;
            } while (map[y][x] != '.');
            map_rooms[i].num_food++;
            map[y][x] = 'f';
        }
    }
}

void monster_placed(room map_rooms[6], int rows, int cols, char map[rows][cols])
{

    int i, k = 0;

    i = rand() % 6;

    int x, y;
    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_monster++;
    map[y][x] = 'D'; // deamon
    monsts[k].health = 5;
    monsts[k].point.x = x;
    monsts[k].point.y = y;
    int room = check_position_of_object(y, x, rows, cols, map, map_rooms);
    monsts[k].room = room;
    monsts[k].awake = 0;
    monsts[k++].type = 'D';

    for (int y = 0; y < 2; y++)
    {
        i = rand() % 6;

        do
        {
            x = rand() % map_rooms[i].size_x + map_rooms[i].x;
            y = rand() % map_rooms[i].size_y + map_rooms[i].y;
        } while (map[y][x] != '.');
        map_rooms[i].num_monster++;
        map[y][x] = 'F'; // fire breathing monster
        i = rand() % 6;
        monsts[k].health = 10;
        monsts[k].point.x = x;
        monsts[k].point.y = y;
        int room = check_position_of_object(y, x, rows, cols, map, map_rooms);
        monsts[k].room = room;
        monsts[k].awake = 0;

        monsts[k++].type = 'F';

        do
        {
            x = rand() % map_rooms[i].size_x + map_rooms[i].x;
            y = rand() % map_rooms[i].size_y + map_rooms[i].y;
        } while (map[y][x] != '.');
        map_rooms[i].num_monster++;
        map[y][x] = 'U'; // undeed
        monsts[k].health = 30;
        monsts[k].point.x = x;
        monsts[k].point.y = y;
        room = check_position_of_object(y, x, rows, cols, map, map_rooms);
        monsts[k].room = room;
        monsts[k].awake = 0;

        monsts[k++].type = 'U';
        return;
    }

    i = rand() % 6;

    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_monster++;
    map[y][x] = 'G'; // giant
    monsts[k].health = 15;
    monsts[k].point.x = x;
    monsts[k].point.y = y;
    room = check_position_of_object(y, x, rows, cols, map, map_rooms);
    monsts[k].room = room;
    monsts[k].awake = 0;

    monsts[k++].type = 'G';

    for (int y = 0; y < 2; y++)
    {
        i = rand() % 6;

        do
        {
            x = rand() % map_rooms[i].size_x + map_rooms[i].x;
            y = rand() % map_rooms[i].size_y + map_rooms[i].y;
        } while (map[y][x] != '.');
        map_rooms[i].num_monster++;
        map[y][x] = 's'; //  snake
        monsts[k].health = 20;
        monsts[k].point.x = x;
        monsts[k].point.y = y;
        int room = check_position_of_object(y, x, rows, cols, map, map_rooms);
        monsts[k].room = room;
        monsts[k].awake = 0;

        monsts[k++].type = 's';
    }
}

void placed_weopen(room map_rooms[6], int rows, int cols, char map[rows][cols])
{
    // except room 4 , 3 all rooms have 4 golds

    int i;

    i = rand() % 6;

    int x, y;
    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_food++;
    map[y][x] = 'M'; // mace

    i = rand() % 6;

    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_food++;
    map[y][x] = 'd'; // dagger

    i = rand() % 6;

    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_food++;
    map[y][x] = 'W'; // magic wand

    i = rand() % 6;

    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_food++;
    map[y][x] = 'A'; // normal arrow

    i = rand() % 6;

    do
    {
        x = rand() % map_rooms[i].size_x + map_rooms[i].x;
        y = rand() % map_rooms[i].size_y + map_rooms[i].y;
    } while (map[y][x] != '.');
    map_rooms[i].num_food++;
    map[y][x] = 'S'; // sword
    return;
}

#define min_size_of_height_and_weight 8
#define max_size_of_height_and_weight 10

int draw_map_for_other_floor(int rows, player *user,
                             int cols, char map[rows][cols], room map_rooms[6],
                             int floor)
{

    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int current_center_y;
    int current_center_x;
    int previous_center_y;
    int previous_center_x;
    int room_number = 6;
    int collision_of_hallway = 0;

    collision_of_hallway = 0;
    num_rooms = 0;

    // draw_first_room_of_floor
    // center of_room is initial_x and initial_y
    do
    {
        size_y = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;
        size_x = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;
        first_x = user->position.x - size_x / 2;
        first_y = user->position.y - size_y / 2;
    } while (first_x < 2 || first_y < 2);
    map_rooms[num_rooms].size_x = size_x;
    map_rooms[num_rooms].size_y = size_y;
    map_rooms[num_rooms].x = first_x;
    map_rooms[num_rooms].y = first_y;
    for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
    {
        for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
        {
            map[y][x] = '.';
        }
    }
    for (int yy = first_y; yy <= first_y + size_y; yy++)
    {
        map[yy][first_x] = '|';
        map[yy][first_x + size_x] = '|';
    }
    for (int xx = first_x; xx <= first_x + size_x; xx++)
    {
        map[first_y][xx] = '-';
        map[first_y + size_y][xx] = '-';
    }

    num_rooms++;
    current_center_y = first_y + size_y / 2;
    current_center_x = first_x + size_x / 2;

    while (num_rooms < room_number)
    {
        bool collision;
        int room_loop_counter = 0;
        int temp_x = first_x, temp_y = first_y, temp_size_y = size_y, temp_size_x = size_x;
        do
        {
            collision = 0;
            do
            {
                first_y = rand() % rows - 2;
                first_x = rand() % cols - 2;

                size_y = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;
                size_x = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;

            } while (first_y < 4 || first_x + size_x > cols - 4 || first_y + size_y > rows - 4 || first_x < 4);

            room_loop_counter++;
            if (room_loop_counter > 100)
            {
                return 0;
            }

        } while (check_collision(cols, map, first_y, first_x, size_y, size_x) ||

                 check_possible_hallway(temp_x + temp_size_x / 2, temp_y + temp_size_y / 2,
                                        first_x + size_x / 2, first_y + size_y / 2,
                                        rows, cols, map));

        map_rooms[num_rooms].size_x = size_x;
        map_rooms[num_rooms].size_y = size_y;
        map_rooms[num_rooms].x = first_x;
        map_rooms[num_rooms].y = first_y;

        for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
        {
            for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
            {
                map[y][x] = '.';
            }
        }
        for (int yy = first_y; yy <= first_y + size_y; yy++)
        {
            map[yy][first_x] = '|';
            map[yy][first_x + size_x] = '|';
        }
        for (int xx = first_x; xx <= first_x + size_x; xx++)
        {
            map[first_y][xx] = '-';
            map[first_y + size_y][xx] = '-';
        }
        num_rooms++;

        previous_center_x = current_center_x;
        previous_center_y = current_center_y;
        current_center_y = first_y + size_y / 2;
        current_center_x = first_x + size_x / 2;
        if (num_rooms > 1)
        {
            int y_in_way, x_in_way;
            for (y_in_way = previous_center_y; y_in_way != current_center_y;)
            {
                if (map[y_in_way][previous_center_x] == ' ')
                {
                    map[y_in_way][previous_center_x] = '#';
                }
                else if (map[y_in_way][previous_center_x] == '-' || map[y_in_way][previous_center_x] == '|')
                    map[y_in_way][previous_center_x] = '+';
                if (previous_center_y < current_center_y)
                {
                    y_in_way++;
                }
                else if (previous_center_y > current_center_y)
                {
                    y_in_way--;
                }
            }
            for (x_in_way = previous_center_x; x_in_way != current_center_x;)
            {

                if (map[y_in_way][x_in_way] == ' ')
                    map[y_in_way][x_in_way] = '#';
                else if (map[y_in_way][x_in_way] == '-' || map[y_in_way][x_in_way] == '|')
                {
                    map[y_in_way][x_in_way] = '+';
                    int i = check_position_of_object(y_in_way, x_in_way, rows, cols, map,
                                                     map_rooms);
                    map_rooms[i].doors[map_rooms[i].doors_num].x = x_in_way;
                    map_rooms[i].doors[map_rooms[i].doors_num++].y = y_in_way;
                }
                if (previous_center_x < current_center_x)
                {
                    x_in_way++;
                }
                else if (previous_center_x > current_center_x)
                {
                    x_in_way--;
                }
            }
        }
    }

    map[user->position.y][user->position.x] = '<';

    display_map(user, cols, rows, map, map_rooms, 0);

    do
    {
        initial_y = rand() % rows;
        user->position.y = initial_y;
        initial_x = rand() % cols;
        user->position.x = initial_x;
    } while (map[initial_y][initial_x] != '.');

    if (user->level == 4)
    {
        int current_room = check_position_of_object(user->position.y, user->position.x,
                                                    rows, cols, map, map_rooms);
        generate_treasure_room(map_rooms, current_room);
    }
    return 1;
}

int draw_map_for_first_floor(int rows, player *user, int cols, char map[rows][cols], room map_rooms[6],
                             int floor)
{

    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int current_center_y;
    int current_center_x;
    int previous_center_y;
    int previous_center_x;
    int room_number = 6;
    int collision_of_hallway = 0;

    collision_of_hallway = 0;
    num_rooms = 0;
    while (num_rooms < room_number)
    {
        bool collision;
        int room_loop_counter = 0;
        int temp_x = first_x, temp_y = first_y, temp_size_y = size_y, temp_size_x = size_x;
        do
        {
            collision = 0;
            do
            {
                first_y = rand() % rows - 2;
                first_x = rand() % cols - 2;

                size_y = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;
                size_x = (rand() % (max_size_of_height_and_weight - min_size_of_height_and_weight)) + min_size_of_height_and_weight;

            } while (first_y < 5 || first_x + size_x > cols - 5 || first_y + size_y > rows - 5 || first_x < 5);

            room_loop_counter++;
            if (room_loop_counter > 100)
            {

                return 0;
            }
        } while (check_collision(cols, map, first_y, first_x, size_y, size_x) ||

                 check_possible_hallway(temp_x + temp_size_x / 2, temp_y + temp_size_y / 2,
                                        first_x + size_x / 2, first_y + size_y / 2,
                                        rows, cols, map));

        map_rooms[num_rooms].size_x = size_x;
        map_rooms[num_rooms].size_y = size_y;
        map_rooms[num_rooms].x = first_x;
        map_rooms[num_rooms].y = first_y;

        for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
        {
            for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
            {
                map[y][x] = '.';
            }
        }
        for (int yy = first_y; yy <= first_y + size_y; yy++)
        {
            map[yy][first_x] = '|';
            map[yy][first_x + size_x] = '|';
        }
        for (int xx = first_x; xx <= first_x + size_x; xx++)
        {
            map[first_y][xx] = '-';
            map[first_y + size_y][xx] = '-';
        }
        num_rooms++;

        if (num_rooms > 0)
        {
            previous_center_x = current_center_x;
            previous_center_y = current_center_y;
        }
        // room center
        current_center_y = first_y + size_y / 2;
        current_center_x = first_x + size_x / 2;
        if (num_rooms > 1)
        {
            int y_in_way, x_in_way;
            for (y_in_way = previous_center_y; y_in_way != current_center_y;)
            {
                if (map[y_in_way][previous_center_x] == ' ')
                {
                    map[y_in_way][previous_center_x] = '#';
                }
                else if (map[y_in_way][previous_center_x] == '-' || map[y_in_way][previous_center_x] == '|')
                    map[y_in_way][previous_center_x] = '+';
                if (previous_center_y < current_center_y)
                {
                    y_in_way++;
                }
                else if (previous_center_y > current_center_y)
                {
                    y_in_way--;
                }
            }
            for (int x_in_way = previous_center_x; x_in_way != current_center_x;)
            {

                if (map[y_in_way][x_in_way] == ' ')
                    map[y_in_way][x_in_way] = '#';
                else if (map[y_in_way][x_in_way] == '-' || map[y_in_way][x_in_way] == '|')
                {
                    map[y_in_way][x_in_way] = '+';
                    int i = check_position_of_object(y_in_way, x_in_way, rows, cols, map,
                                                     map_rooms);
                    map_rooms[i].doors[map_rooms[i].doors_num].x = x_in_way;
                    map_rooms[i].doors[map_rooms[i].doors_num++].y = y_in_way;
                }
                if (previous_center_x < current_center_x)
                {
                    x_in_way++;
                }
                else if (previous_center_x > current_center_x)
                {
                    x_in_way--;
                }
            }
        }
    }

    display_map(user, cols, rows, map, map_rooms, 0);

    int yy, xx;
    do
    {
        yy = rand() % rows - 3;
        xx = rand() % cols - 3;

    } while (map[yy][xx] != '.');
    map[yy][xx] = '<';

    do
    {
        initial_y = rand() % rows;
        user->position.y = initial_y;
        initial_x = rand() % cols;
        user->position.x = initial_x;
    } while (map[initial_y][initial_x] != '.');

    return 1;
}

    int  u = 1;

void show_foods(player user)
{
    refresh();
    WINDOW *food_menu = newwin(LINES - 20, COLS / 2, (LINES) / 6, (COLS) / 4);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    wattron(food_menu, COLOR_PAIR(6));
    box(food_menu, 0, 0);
    wattroff(food_menu, COLOR_PAIR(6));
    wattron(food_menu, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(food_menu, 2, 40, "%s", "List of Foods");
    mvwprintw(food_menu, 4, 1, "Hunger level:");

    for (int i = 0; i < user.hunger_level; i++)
    {
        mvwprintw(food_menu, 4, 15 + i, "%lc", L'🟥');
    }

    char *menu[] = {"eat food", "Back"};
    int choice = 0;

    while (1)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (i == choice)
                wattron(food_menu, A_REVERSE);
            if (i == 0)
                mvwprintw(food_menu, 16 + 2 * i, 32, "%s : %d", menu[i], user.num_foods);
            else
                mvwprintw(food_menu, 16 + 2 * i, 32, "%s", menu[i]);

            if (i == choice)
                wattroff(food_menu, A_REVERSE);
        }

        wrefresh(food_menu);
        wattroff(food_menu, COLOR_PAIR(4));

        int ch = getch();
        switch (ch)
        {
        case KEY_UP:
            choice = (choice == 0) ? 1 : choice - 1;
            break;
        case KEY_DOWN:
            choice = (choice == 1) ? 0 : choice + 1;
            break;
        case 10: // Enter key
            if (choice == 0 && user.num_foods > 0)
            {
                user.num_foods--;
                user.hunger_level--;
                if (foods[user.num_foods].time > 12) // sami shodan
                {
                    user.health--;
                    if (user.health == 0)
                        return;
                }
                else{
                
                if (user.health < 20)
                {
                    user.num_of_eaten_food++;
                    user.health += user.num_of_eaten_food / (u * 3);
                    if (user.num_of_eaten_food / (u * 3) != 0)
                    {
                        u++;
                    }
                }
                }
            }

            else if (choice == 1)
            {
                delwin(food_menu);
                refresh();
                return;
            }
            break;
        }
    }

    wrefresh(food_menu);
    return;
}

int default_weopen = 0;

void show_weapons(player user)
{
    refresh();
    WINDOW *weopen_menu = newwin(LINES - 20, COLS / 2, (LINES) / 6, (COLS) / 4);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    wattron(weopen_menu, COLOR_PAIR(6));
    box(weopen_menu, 0, 0);
    wattroff(weopen_menu, COLOR_PAIR(6));
    wattron(weopen_menu, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(weopen_menu, 2, 32, "%s", "List of Weopens");
    wattroff(weopen_menu, COLOR_PAIR(4));

    char *menu[] = {"Mace", "Dagger", "Magic Wand", "Normal Arrow", "Sword", "Back"};
    int choice = 0;

    while (1)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (i == choice)
                wattron(weopen_menu, A_REVERSE);
            if (i == 0)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s : %d  ", menu[i], user.num_mace);
            else if (i == 1)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s : %d  %lc  ", menu[i], user.num_dagger, L'🗡');
            else if (i == 2)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s : %d  %lc  ", menu[i], user.num_magic_wand, L'🪄');
            else if (i == 3)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s : %d  %lc  ", menu[i], user.num_normal_arrow, L'🏹');
            else if (i == 4)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s : %d  %lc  ", menu[i], user.num_sword, L'⚔');
            if (i == 5)
                mvwprintw(weopen_menu, 8 + 2 * i, 32, "%s", menu[i]);

            if (i == choice)
                wattroff(weopen_menu, A_REVERSE);
        }

        wrefresh(weopen_menu);

        int ch = getch();
        switch (ch)
        {
        case KEY_UP:
            choice = (choice == 0) ? 2 : choice - 1;
            break;
        case KEY_DOWN:
            choice = (choice == 5) ? 0 : choice + 1;
            break;
        case 10: // Enter key
            if (choice == 0 && user.num_foods > 0)
            {
            }
            else if (choice == 1)
            {
                // عملکرد برای غذای ویژه
            }
            else if (choice == 5)
            {
                delwin(weopen_menu);
                refresh();
                return;
            }
            break;
        }
    }

    wrefresh(weopen_menu);
    return;
}

void monster_move(int num_room, player *user, int rows, int cols, char map[rows][cols], room map_rooms[6])
{
    for (int i = 0; i < 12; i++)
    {
        if (monsts[i].room == num_room && monsts[i].health != 0)
        {
            monsts[i].awake = 1;
        }
        else
            monsts[i].awake = 0;
    }
    int y_monst, x_monst;
    for (int y = 0; y < 12; y++)
    {
        y_monst = monsts[y].point.y;
        x_monst = monsts[y].point.x;
        if (monsts[y].awake == 1)
        {
            if (monsts[y].point.y < user->position.y)
                y_monst++;
            else if (monsts[y].point.y > user->position.y)
                y_monst--;

            if (monsts[y].point.x < user->position.x)
                x_monst++;
            else if (monsts[y].point.x > user->position.x)
                x_monst--;

            if (map[y_monst][x_monst] == '.')
            {
                map[monsts[y].point.y][monsts[y].point.x] = '.';
                monsts[y].point.y = y_monst;
                monsts[y].point.x = x_monst;
                map[monsts[y].point.y][monsts[y].point.x] = monsts[y].type;
                if ((x_monst == user->position.x && (abs(y_monst - user->position.y) == 1)) ||
                    (y_monst == user->position.y && (abs(x_monst - user->position.x) == 1)) ||
                    (y_monst == user->position.y && x_monst == user->position.x))
                {
                    mvprintw(2, 20, "the monster hit you");
                    getch();
                    mvprintw(2, 20, "                            ");
                    user->health--;
                    display_map(user, cols, rows, map, map_rooms, 1);
                    if (user->health == 0)
                        return;
                    else
                        display_map(user, cols, rows, map, map_rooms, 1);
                    return;
                }
            }
        }
    }
}

int move_char(int input, player *user, int cols, int rows, char map[rows][cols], room map_rooms[6], int n)
{

    display_map(user, cols, rows, map, map_rooms, n);
    refresh();
    /* if (player_initial_pos == 0)
     {
         do
         {
             initial_y = rand() % rows - 3;
             user->position.y = initial_y - 3;
             initial_x = rand() % cols;
             user->position.x = initial_x;
         } while (map[initial_y][initial_x] != '.');
         player_initial_pos = 1;
     } */

    if (input == KEY_UP)
        initial_y--;
    else if (input == KEY_DOWN)
        initial_y++;
    else if (input == KEY_RIGHT)
        initial_x++;
    else if (input == KEY_LEFT)
        initial_x--;
    else if (input == 'e')
    {
        // nodelay(stdscr, FALSE);
        show_foods(*user);
        return 1;
    }

    else if (input == 'i')
    {
        show_weapons(*user);
        display_map(user, cols, rows, map, map_rooms, n);
    }

    int current_room = check_position_of_object(initial_y, initial_x, rows, cols, map, map_rooms);
    monster_move(current_room, user, rows, cols, map, map_rooms);
    display_map(user, cols, rows, map, map_rooms, n);

    switch (map[initial_y][initial_x])
    {
    case '.':
        user->position.y = initial_y;
        user->position.x = initial_x;
        attron(COLOR_PAIR(user->color));
        mvaddch(user->position.y, user->position.x, '@');
        attroff(COLOR_PAIR(user->color));
        return 1;

    case '+':
        user->position.y = initial_y;
        user->position.x = initial_x;
        attron(COLOR_PAIR(user->color));
        mvaddch(user->position.y, user->position.x, '@');
        attroff(COLOR_PAIR(user->color));
        return 1;

    case '#':
        user->position.y = initial_y;
        user->position.x = initial_x;
        attron(COLOR_PAIR(user->color));
        mvaddch(user->position.y, user->position.x, '@');
        attroff(COLOR_PAIR(user->color));
        return 1;

    case '|':
    case '-':
    case ' ':
        initial_x = user->position.x;
        initial_y = user->position.y;
        attron(COLOR_PAIR(user->color));
        mvaddch(user->position.y, user->position.x, '@');
        attroff(COLOR_PAIR(user->color));
        return 1;

    case '<':
        initial_x = user->position.x;
        initial_y = user->position.y;
        return 2;

    case '$':
        int c = getch();
        if (c == 10)
        {
            user->gold++;
            user->score += 5; // gold= 5*score
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map_rooms[current_room].number_of_gold--;
            refresh();
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
        return 1;

    case 'f': // food
        c = getch();
        if (c == 10)
        {
            if (user->num_foods <= 4)
            {
                user->num_foods++;
                foods[user->num_foods - 1].time = time(NULL);
                foods[user->num_foods - 1].pos.x = initial_x;
                foods[user->num_foods - 1].pos.y = initial_y;
                user->position.y = initial_y;
                user->position.x = initial_x;
                attron(COLOR_PAIR(user->color));
                mvaddch(user->position.y, user->position.x, '@');
                attroff(COLOR_PAIR(user->color));
                map[initial_y][initial_x] = '.';
                display_map(user, cols, rows, map, map_rooms, n);
                attron(COLOR_PAIR(user->color));
                mvaddch(user->position.y, user->position.x, '@');
                attroff(COLOR_PAIR(user->color));
                map_rooms[current_room].num_food--;
                refresh();
            }
            else
            {
                user->position.y = initial_y;
                user->position.x = initial_x;
                attron(COLOR_PAIR(user->color));
                mvaddch(user->position.y, user->position.x, 'H');
                attroff(COLOR_PAIR(user->color));
            }
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }

        return 1;

    case 'M':
        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a weapon");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            refresh();
            user->num_mace++;
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
        return 1;
    case 'd':
        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a weapon");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            refresh();
            user->num_dagger++;
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
        return 1;

    case 'W':
        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a weapon");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            refresh();
            user->num_magic_wand++;
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
        return 1;

    case 'A':
        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a weapon");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            refresh();
            user->num_normal_arrow++;
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
        return 1;

    case 'S':
        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a weapon");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            refresh();
            user->num_sword++;
            break;
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
            break;
        }

    case 'D':
    case 'G':
    case 's':
    case 'U':
    case 'F':
        int num_monst;
        for (int y = 0; y < 12; y++)
        {
            if (monsts[y].point.x == initial_x && monsts[y].point.y == initial_y && monsts[y].health > 0)
            {
                num_monst = y;
                break;
            }
        }
        monsts[num_monst].health--;
        if (monsts[num_monst].health == 0)
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            user->score += 10;
            mvprintw(1, 20, "you killed the monster");
            getch();
            mvprintw(1, 20, "                        ");
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
        }
        else
        {
            mvprintw(1, 20, "you hit the monster");
            getch();
            mvprintw(1, 20, "                        ");
            initial_x = user->position.x;
            initial_y = user->position.y;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
        }

        return 1;

    case 'h':

        c = getch();
        if (c == 10)
        {
            mvprintw(1, 20, "you catched a spell");
            getch();
            mvprintw(1, 20, "                        ");
            user->position.y = initial_y;
            user->position.x = initial_x;
            if (user->health < 20)
                user->health++;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            map[initial_y][initial_x] = '.';
            display_map(user, cols, rows, map, map_rooms, n);
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            if (map[initial_y][initial_x] == '&')
                refresh();
        }
        else
        {
            user->position.y = initial_y;
            user->position.x = initial_x;
            attron(COLOR_PAIR(user->color));
            mvaddch(user->position.y, user->position.x, '@');
            attroff(COLOR_PAIR(user->color));
            move_char(c, user, cols, rows, map, map_rooms, n);
        }
    }
}

void save_player_info(Gamer *g, player user)
{
    char temp2[200];
    snprintf(temp2, sizeof(temp2), "./%s/%s.game.info.txt", g->name, g->name);
    FILE *file2 = fopen(temp2, "w");
    fprintf(file2, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %ld\n",
            user.name, user.difficulty, user.color, user.score, user.gold, user.position.y, user.position.x,
            user.level, user.health, user.num_foods, user.num_of_eaten_food, user.exprience, user.hunger_level,
            user.num_weopen, user.num_mace, user.num_dagger, user.num_magic_wand, user.num_normal_arrow, user.num_sword,
            user.start_time);

    fclose(file2);

    FILE* new= fopen("scores.txt", "a");
    fprintf(new, "%s:%d:%d:%d", user.name, user.score, user.gold, user.exprience);
    fclose(new);
}

void control_play_in_a_floor(int rows, int cols, int floor,
                             room all_floor_rooms[6],
                             player *user, Gamer *g)

{
    num_of_treasure_room = 0;
    if (floor > 4)
    {
        return;
    }
    char map[rows][cols];
    room map_rooms[6];
    memset(map, 0, sizeof(map));

    for (int i = 0; i < 6; i++)
    {
        map_rooms[i].doors_num = 0;
        map_rooms[i].number_of_gold = 0;
        map_rooms[i].visited = 0;
    }
    int attemp = 0;

    if (floor == 0)
    {
        do
        {
            for (int i = 0; i < 6; i++)
            {
                map_rooms[i].size_x = 0;
                map_rooms[i].size_y = 0;
                map_rooms[i].x = 0;
                map_rooms[i].y = 0;
            }
            initilizing_map(rows - 1, cols - 1, map);
            attemp++;
            if (attemp > 1000)
            {
                printw("trying failed, try again");
                getch();
                return;
            }
        } while (!draw_map_for_first_floor(rows - 1, user, cols - 1, map, map_rooms, floor));
    }
    else
    {
        do
        {
            for (int i = 0; i < 6; i++)
            {
                map_rooms[i].size_x = 0;
                map_rooms[i].size_y = 0;
                map_rooms[i].x = 0;
                map_rooms[i].y = 0;
            }
            initilizing_map(rows - 1, cols - 1, map);
            attemp++;
            if (attemp > 1000)
            {
                printw("trying failed, try again");
                getch();
                return;
            }
        } while (!draw_map_for_other_floor(rows - 1, user, cols - 1, map, map_rooms, floor));
    }
    int input;

    rows--;
    cols--;
    gold_placed(map_rooms, rows, cols, map);
    food_placed(map_rooms, rows, cols, map);
    placed_weopen(map_rooms, rows, cols, map);
    monster_placed(map_rooms, rows, cols, map);
    health_placed(map_rooms, rows, cols, map);

    int k;
    // nodelay(stdscr, TRUE);
    do
    {
        int current_room = check_position_of_object(user->position.y, user->position.x,
                                                    rows, cols, map, map_rooms);
        if (current_room != 11)
            map_rooms[current_room].visited = 1;
        flushinp();
        if (user->health == 0)
        {
            //  nodelay(stdscr, FALSE);

            clear();
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw((LINES - 6) / 2, (COLS - 42) / 2,
                     "                         _           _   ");
            mvprintw((LINES - 6) / 2 + 1, (COLS - 42) / 2,
                     "  _   _  ___  _   _     | | ___  ___| |_ ");
            mvprintw((LINES - 6) / 2 + 2, (COLS - 42) / 2,
                     " | | | |/ _ \\| | | |    | |/ _ \\/ __| __|");
            mvprintw((LINES - 6) / 2 + 3, (COLS - 42) / 2,
                     " | |_| | (_) | |_| |    | | (_) \\__ \\ |_ ");
            mvprintw((LINES - 6) / 2 + 4, (COLS - 42) / 2,
                     "  \\__, |\\___/ \\__,_|    |_|\\___/|___/\\__|");
            mvprintw((LINES - 6) / 2 + 5, (COLS - 42) / 2,
                     "  |___/                                  ");
            attroff(COLOR_PAIR(1) | A_BOLD);

            refresh();
            getch();

            return;
        }
        display_map(user, cols, rows, map, map_rooms, 0);
        k = move_char(input, user, cols, rows, map, map_rooms, 0);

        if (k == 2 || k == 0)
            break;

        // usleep(100000);

    } while ((input = getch()) != 'q'); // q means quit

    if (input == 'q')
    {
        save_player_info(g, *user);
    }

    if (k == 2)
    {
        user->level++;
        control_play_in_a_floor(rows, cols, floor + 1,
                                all_floor_rooms, user, g);
    }
    return;
}

void new_game(Gamer *g)
{
    clear();
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);
    int level = 4;
    char all_floor[level][rows][cols];
    noecho();
    player user;

    memset(&user, 0, sizeof(player));
    strcpy(user.name, g->name);
    if (strcmp(g->difficulty, "EASY"))
    {
        user.difficulty = 1;
    }
    else if (strcmp(g->difficulty, "MEDIUM"))
        user.difficulty = 2;
    else
        user.difficulty = 3;
    // initilize player_info

    if (strcmp(g->hero_color, "RED") == 0)
    {
        user.color = 1;
    }
    else if (strcmp(g->hero_color, "BLUE") == 0)
        user.color = 4;
    else
        user.color = 3;

    user.gold = 0;
    user.score = 0;
    user.level = 1;
    user.health = 20;
    user.num_foods = 0;
    user.num_of_eaten_food = 0;
    user.hunger_level = 0;
    user.exprience = 0;
    user.num_mace = 0;
    user.num_dagger = 0;
    user.num_magic_wand = 0;
    user.num_normal_arrow = 0;
    user.num_sword = 0;
    user.num_weopen = 0;
    switch (user.difficulty)
    {
    case 1:
        zarib_henger_and_health = 2;
        zarib_time_hunger = 10; // second
        break;
    case 2:
        zarib_henger_and_health = 2;
        zarib_time_hunger = 5;
        break;
    case 3:
        zarib_henger_and_health = 2;
        zarib_time_hunger = 3;
        break;
    }
    //
    user.start_time = time(NULL);
    room all_floor_rooms[6];
    player_initial_pos = 0;
    control_play_in_a_floor(rows - 3, cols - 3, 0,
                            all_floor_rooms, &user, g);

    refresh();
}

void load_player_info(Gamer *g, player *user)
{
    char temp2[200];
    snprintf(temp2, sizeof(temp2), "./%s/%s.game.info.txt", g->name, g->name);
    FILE *file2 = fopen(temp2, "r");

    fscanf(file2, "%49s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %ld\n",
           user->name,
           &user->difficulty,
           &user->color,
           &user->score,
           &user->gold,
           &user->position.y,
           &user->position.x,
           &user->level,
           &user->health,
           &user->num_foods,
           &user->num_of_eaten_food,
           &user->exprience,
           &user->hunger_level,
           &user->num_weopen,
           &user->num_mace,
           &user->num_dagger,
           &user->num_magic_wand,
           &user->num_normal_arrow,
           &user->num_sword,
           &user->start_time);
    fclose(file2);
}

void continue_game(Gamer *g, const char *filename)
{
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    player user;
    memset(&user, 0, sizeof(player));
    load_player_info(g, &user);
    room all_floor_rooms[6];
    player_initial_pos = 0;
    switch (user.difficulty)
    {
    case 1:
        zarib_henger_and_health = 5;
        zarib_time_hunger = 40; // second
        break;
    case 2:
        zarib_henger_and_health = 4;
        zarib_time_hunger = 30;
        break;
    case 3:
        zarib_henger_and_health = 3;
        zarib_time_hunger = 20;
        break;
    }
    control_play_in_a_floor(rows - 3, cols - 3, user.level - 1,
                            all_floor_rooms, &user, g);

    refresh();
    getch();

    //  resume_the_level(g, user, rows - 1, cols - 1, map, user.level, 2);
}
