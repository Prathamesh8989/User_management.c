#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<windows.h>

#define MAX_USERS 10 
#define CREDENTIAL_LENGTH 30 

typedef struct{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();
void fix_fgets_input(char*);
void input_credentials(char*username , char*password);

int main(){
    int option;
    int user_index;
    while(1){
    printf("\n Welcome to User Management");
    printf("\n1. Register");
    printf("\n2. Login");
    printf("\n3. Exit");
    printf("\nSelect and option: ");
    scanf("%d",&option);
    getchar(); // Consume extra enter 
    switch (option)
    {
    case 1:
        register_user();
        break;
    case 2:
        user_index = login_user();
        if(user_index >=0){
            printf("Login Successful! Welcome, %s!",users[user_index].username);
        }else{
            printf("\nLogin failed! Incorrect username or password .\n");
        }
        break;
    case 3:
        printf("\nExiting program.\n");
        return 0 ; 
        break;
    default:
        printf("\nInvalid option. Please try again.\n");
        break;
    }
    }

    return 0 ; 
}

void register_user(){
    if (user_count == MAX_USERS){
        printf("\n Maximum %d users are supported! No more registrations Allowed!!!!!!\n",MAX_USERS);
        return;
    }

    int new_index = user_count;
    printf("\nRegister a new user ");
    input_credentials(users[new_index].username,users[new_index].password);
    user_count++;
    printf("Registration Successfull!!!");
}

int login_user(){
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
    
    input_credentials(username, password);

    for(int i = 0; i < user_count; i++) {
        // Remove the semicolon at the end of the condition
        if(strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return i; // Only return the index if the username and password match
        }
    }
    return -1; // Return -1 if no match is found
}

void fix_fgets_input(char* string){

    int index = strcspn(string,"\n");
    string[index] = '\0';


}

#ifdef _WIN32
    #include <conio.h>  // For Windows-specific input handling
#else
    #include <termios.h>
    #include <unistd.h>
#endif

void input_credentials(char* username, char* password) {
    printf("\nEnter username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_input(username);

    printf("Enter password (masking enabled): ");
    fflush(stdout);

    int i = 0;
    char ch;

#ifdef _WIN32
    // Windows implementation using _getch()
    while ((ch = _getch()) != '\r') { // '\r' is Enter on Windows
        if (ch == '\b') {  // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Move cursor back, clear character, move back again
            }
        } else if (i < CREDENTIAL_LENGTH - 1) {
            password[i++] = ch;
            printf("*"); // Mask character
        }
    }
#else
    // POSIX implementation for Linux/macOS
    struct termios old_props, new_props;

    tcgetattr(STDIN_FILENO, &old_props); // Save terminal settings
    new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);

    while ((ch = getchar()) != '\n' && ch != EOF) { // Stop at newline
        if (ch == '\b' || ch == 127) { // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Move cursor back, clear character, move back
            }
        } else if (i < CREDENTIAL_LENGTH - 1) {
            password[i++] = ch;
            printf("*"); // Mask character
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_props); // Restore terminal settings
#endif

    password[i] = '\0'; // Null-terminate the password
}

