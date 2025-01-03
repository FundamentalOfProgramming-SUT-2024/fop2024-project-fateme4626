#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


typedef struct {
    int score;
    int max_items;
    char name[50];
    char email[70];
    char password[50];
    char security_word[200];
    char difficulty[20];
    char hero_color[10];
} Gamer;

void menu(Gamer *g);
void draw_border();
int sign_in(Gamer *g);
int validation_password(char *password);
int validation_email(char *eamil);
int login(Gamer *g);
void login_as_guest(Gamer*g);
char * random_pass();
void game_menu(Gamer *g);



int main() {
    Gamer g;
    initscr();
    clear();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    can_change_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    menu(&g);
    clear();
    draw_border();
    game_menu(&g);
    const char *filename="scores.txt";
    char game_all_scores[100][1000];
    endwin();
    return 0;
}

void menu(Gamer *g) {
    char *menu[] = {"sign in", "login", "login as guest"};
    int choice = 0;
    
    while (1) {
        clear();
        draw_border();
        for (int i = 0; i < 3; ++i) {
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
        else if (ch == 10){
    
    if (choice == 0) {
       if( sign_in(g))
       break;
    } else if (choice == 1) {
     if(login(g))
     break;
    }
    else if (choice == 2) {
     login_as_guest(g);
     break;
    }
    }
    }
    refresh();
    return ;
}

void draw_border() {
    start_color();
    for (int x = 0; x < COLS; x += 2) {
        mvprintw(0, x, "-");
        mvprintw(LINES - 1, x, "-");
    }
    for (int x = 1; x < COLS; x += 2) {
        mvprintw(0, x, "*");
        mvprintw(LINES - 1, x, "*");
    }
    for (int y = 0; y < LINES; y += 2) {
        mvprintw(y, 0, "^");
        mvprintw(y, COLS - 1, "^");
    }
    for (int y = 1; y < LINES; y += 2) {
        mvprintw(y, 0, "*");
        mvprintw(y, COLS - 1, "*");
    }
}

int sign_in(Gamer *g) {
    clear();
    draw_border();
     mvprintw((LINES - 2) / 2 , (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);
    
    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if(access(new_user, F_OK)==0){
    attron(COLOR_PAIR(1));
    mvprintw((LINES-2)/2 + 4, (COLS-2-strlen("!! USERNAMME IS ALREADY TAKEN !!"))/2, "!! USERNAMME IS ALREADY TAKEN !!");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
    return 0;
    }
   

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("welcome !")-30) / 2 + 4, "welcom %s !",g->name);
    mvprintw((LINES - 2) / 2 + 3, (COLS - strlen("Enter your email :")) / 2, "Enter your email : ");
    getnstr(g->email,70);
    char email[100];
    strcpy(email, g->email);

    if(!validation_email(email)){
    attron(COLOR_PAIR(1));
    mvprintw((LINES-2)/2 + 4, (COLS-2-strlen("!! INVALID EMAL ADDRESS !!"))/2, "!! INVALID EMAIL ADDRESS !!");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
    return 0;
    }
    
     clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your password if you need random password write ::rand :")) / 2, "Enter your password if you need random password write ::rand : ");
    getnstr(g->password, 50);
    if (strcmp(g->password, "::rand") == 0) {
       strcpy(g->password, random_pass());
    }

    char password[100];
    strcpy(password, g->password);

    if(!validation_password(password))
    {
    attron(COLOR_PAIR(1));
    mvprintw((LINES-2)/2 + 4, (COLS-2-strlen("!! INVALID PASSWORD !!"))/2, "!! INVALID PASSWORD !!");
    attroff(COLOR_PAIR(1));
    refresh();
    return 0;
    }
    clear ();
    draw_border();
    mvprintw( ((LINES-2)/2), (COLS-2-strlen("please write your highschool name you can use it instead of password "))/2, "please write your highschool name you can use it instead of password ");
    getnstr(g->security_word, 100);
   


    mkdir (new_user , 0777);//new folder with name of user

    char user_info[100];
    sprintf(user_info, "./%s/%s.info.txt", g->name , g->name);
    FILE* info= fopen(user_info, "w");
    fprintf(info, "name : %s\n",g->name);
    fprintf(info, "email : %s\n",g->email);
    fprintf(info, "password : %s\n", g->password);
    fprintf(info, "security_word : %s\n", g->security_word );
    fclose(info);
    refresh();
   return 1;
}

int validation_email(char *email)
{
int length = strlen(email);
int sign=0;
int sign_num;
int signdot=0;
for(int i=0; i< strlen(email); i++){
    if ( email[i] == '@' && i>0){
     sign=1;
     sign_num=i;
    }
    if( email[i]=='.' && i>sign_num+1 && sign_num>0 && i<length-1){
        signdot=1;
    }
}
if(sign && signdot){
    return 1;
}

return 0;
}

int validation_password(char * password){
    int length = strlen (password);
    if(length<7){
        return 0;
    }
        int digit=0;
        int upercase=0;
        int small=0;
    for(int i=0 ;i <length;i++){
            char c=password[i];

    if (c>='1' && c<= '9')
    {
        digit=1;
    }
    if (c>='a' && c<= 'z')
    {
        small=1;
    }
    if (c>='A' && c<= 'Z')
    {
        upercase=1;
    }
    }
    if(digit && upercase && small){
        return 1;
    }
    return 0;
}

int login(Gamer *g) {
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);
    
    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if (access(new_user, F_OK) != 0) {
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
    char user_info[100];
    sprintf(user_info, "./%s/%s.info.txt", g->name, g->name);

    FILE *file = fopen(user_info, "r");

    if(strcmp(g->password, "::forgot")==0){
        clear();
        draw_border();
    mvprintw((LINES - 2) / 2 , (COLS - strlen("write your highschool name : ")) / 2, "write your highschool name : ");
    char school[100],school_name[100];
    getnstr(school_name, 100);
    char temp[100];
    while (fgets(temp, 100, file)) {
        if (strstr(temp, "security_word : ") != NULL) {
            sscanf(temp, "security_word : %s", school);
            break;
        }
    } fclose (file);
    if (strcmp(school, school_name) == 0) {
        return 1;
    } else {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! it's wrong !!")) / 2, "!! it's wrong !!");
        attroff(COLOR_PAIR(1));
        return 0;
    }

}
   

else{
    char temp[200];
    char user_password[100];
    while (fgets(temp, 200, file)) {
        if (strstr(temp, "password : ") != NULL) {
            sscanf(temp, "password : %s", user_password);
            break;
        }
    }
    fclose(file);
    

    
    if (strcmp(password, user_password) == 0) {
        return 1;
    } else {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! password is incorrect !!")) / 2, "!! password is incorrect !!");
        attroff(COLOR_PAIR(1)); refresh();
    getch();
        return 0;
    }
   
}
}
void login_as_guest(Gamer*g){
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("press a key to start")) / 2, "press a key to start");
    getch();
}
char* random_pass(){
static char pass[12];
srand(time(NULL)); //yeksan nabashand
char number[2];
char uper[5];
char small[5];
for(int i=0;i<2;i++){
int n = rand();
 number[i]= (n%10)+'0'; //random digit}
}
for(int i=0;i<5;i++){
int up= rand();
uper[i]= (up%26)+'A'; 
}
for(int i=0;i<4;i++){
int sma= rand();
small[i]= (sma%26)+'a'; 
}
pass[0]=number[1];
pass[1]=uper[0];
pass[2]=uper[1];
pass[3]=small[0];
pass[4]=number[0];
pass[5]=uper[2];
pass[6]=uper[3];
pass[7]=small[1];
pass[8]=small[2];
pass[9]=small[3];
pass[10]=uper[4];
pass[11]='\0';
return pass;
}

void game_menu(Gamer *g) {
    char *choices[] = {
        "New Game",
        "Continue Previous Game",
        "Score Table",
        "Setting"
    };
    int choice = 0;
    int num_choices = 4;

    while (1) {
        clear();
        draw_border();
        for (int i = 0; i < num_choices; ++i) {
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
            else if (ch == 10){
            if (choice == 0) {
               // new_game(g);
                break;
            } 
            else if (choice == 1) {
               // continue_game(g);
                break;
            }
            else if (choice == 2) {
               sort_scores(g);
            }
            else if (choice == 3) {
               setting(g);
            }
    }
    }
    
    refresh();
    return;
}

void new_game()
{

}

void continue_game()
{

}

void score_table()
{

}

void setting(Gamer *g) {
    char *string[] = {"Difficulty", "Hero setting","Back"};
    int choice = 0;
    int ch;

    while (1) {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                attron(A_REVERSE ); 
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice) {
                attroff(A_REVERSE); 
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP) {
            choice = (choice == 0) ? 2 : choice - 1;
        } else if (ch == KEY_DOWN) {
            choice = (choice == 2) ? 0 : choice + 1;
        } else if (ch == 10) { // کد ASCII برای Enter
            if (choice == 0) {
                difficulty(g);
            } else if (choice == 1) {
                hero_setting(g);
            }
            else if (choice == 2) {
                return ;
            }
        }
    }
}


void difficulty(Gamer *g) {
    char *string[] = {"EASY", "MEDIUM", "HARD"};
    int choice = 0;
    int ch;

    while (1) {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                attron(A_REVERSE); 
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice) {
                attroff(A_REVERSE ); 
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP) {
            choice = (choice == 0) ? 2 : choice - 1;
        } else if (ch == KEY_DOWN) {
            choice = (choice == 2) ? 0 : choice + 1;
        } else if (ch == 10) { // کد ASCII برای Enter
            if (choice == 0) {
                strcpy(g->difficulty , "EASY");
            } else if (choice == 1) {
               strcpy( g->difficulty ,"MEDIUM");
            } else if (choice == 2) {
                strcpy(g->difficulty , "HARD");
            }
            break;
        }
    }
}

void hero_setting(Gamer *g){
    
char *hero_color[]={"RED", "BLUE", "GREEN"};
int choice = 0;
    int ch;

    while (1) {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                attron(A_REVERSE); 
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(hero_color[i])) / 2, "%s", hero_color[i]);
            if (i == choice) {
                attroff(A_REVERSE ); 
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP) {
            choice = (choice == 0) ? 2 : choice - 1;
        } else if (ch == KEY_DOWN) {
            choice = (choice == 2) ? 0 : choice + 1;
        } else if (ch == 10) { // کد ASCII برای Enter
            if (choice == 0) {
                strcpy(g->hero_color , "RED");
            } else if (choice == 1) {
               strcpy( g->hero_color ,"BLUE");
            } else if (choice == 2) {
                strcpy(g->hero_color , "GREEN");
            }
            break;
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

  void sort_scores(const char *filename) {
    char game_all_scores[100][1000];
    int i = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fgets(game_all_scores[i], 1000, file) != NULL) {
        game_all_scores[i][strcspn(game_all_scores[i], "\n")] = '\0';
        i++;
    }
    fclose(file);

    for (int j = 0; j < i - 1; j++) { 
            
        for (int h = j + 1; h < i; h++) {
           char name1[1000];
            int score1;
            sscanf(game_all_scores[j], "%[^:] : %d", name1, &score1);

            char name2[1000];
            int score2;
            sscanf(game_all_scores[h], "%[^:] : %d", name2, &score2);

            if (score2 > score1) {
                char temp[1000];
                strcpy(temp, game_all_scores[j]);
                strcpy(game_all_scores[j], game_all_scores[h]);
                strcpy(game_all_scores[h], temp);
            }
        }
    }

    // چاپ نتایج
    printf("Sorted Scores:\n");
    for (int k = 0; k < i; k++) {
        printf("%s\n", game_all_scores[k]);
    }
}

    void score_table()
    {

    }
}