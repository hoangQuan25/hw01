#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_TRIES 3

typedef struct User {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int status;
    struct User* next;
} User;

User* head = NULL; 
User* logged_in_user = NULL; 

void load_users() {
    FILE *file = fopen("account.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    User* temp;
    while (!feof(file)) {
        temp = (User*) malloc(sizeof(User));
        fscanf(file, "%s %s %d", temp->username, temp->password, &temp->status);
        temp->next = head;
        head = temp;
    }
    fclose(file);
}

void save_users() {
    FILE *file = fopen("account.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    User* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->username, temp->password, temp->status);
        temp = temp->next;
    }
    fclose(file);
}

User* find_user(const char* username) {
    User* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void register_user() {
    char username[MAX_USERNAME_LEN], password[MAX_PASSWORD_LEN];
    printf("Username: ");
    scanf("%s", username);

    if (find_user(username) != NULL) {
        printf("Account existed\n");
        return;
    }

    printf("Password: ");
    scanf("%s", password);

    User* new_user = (User*) malloc(sizeof(User));
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->status = 1; 
    new_user->next = head;
    head = new_user;

    FILE *file = fopen("account.txt", "a"); 
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s %s %d\n", new_user->username, new_user->password, new_user->status);
    fclose(file);

    printf("Successful registration\n");
}


void sign_in() {
    char username[MAX_USERNAME_LEN], password[MAX_PASSWORD_LEN];
    printf("Username: ");
    scanf("%s", username);
    
    User* user = find_user(username);
    if (user == NULL) {
        printf("Cannot find account\n");
        return;
    }
    if (user->status == 0) {
        printf("Account is blocked\n");
        return;
    }

    int tries = 0;
    while (tries < MAX_TRIES) {
        printf("Password: ");
        scanf("%s", password);
        if (strcmp(user->password, password) == 0) {
            logged_in_user = user;
            printf("Hello %s\n", user->username);
            return;
        }
        printf("Password is incorrect\n");
        tries++;
    }
    user->status = 0;
    save_users();
    printf("Password is incorrect. Account is blocked\n");
}

void search_user() {
    if (logged_in_user == NULL) {
        printf("Account is not signed in\n");
        return;
    }
    char username[MAX_USERNAME_LEN];
    printf("Username: ");
    scanf("%s", username);
    
    User* user = find_user(username);
    if (user == NULL) {
        printf("Cannot find account\n");
    } else {
        printf("Account is %s\n", user->status == 1 ? "active" : "blocked");
    }
}

void sign_out() {
    if (logged_in_user == NULL) {
        printf("Account is not signed in\n");
        return;
    }
    printf("Goodbye %s\n", logged_in_user->username);
    logged_in_user = NULL;
}

void display_menu() {
    printf("USER MANAGEMENT PROGRAM\n");
    printf("-----------------------\n");
    printf("1. Register\n");
    printf("2. Sign in\n");
    printf("3. Search\n");
    printf("4. Sign out\n");
    printf("Your choice (1-4, other to quit): ");
}

int main() {
    int choice;
    load_users();

    do {
        display_menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                sign_in();
                break;
            case 3:
                search_user();
                break;
            case 4:
                sign_out();
                break;
            default:
                printf("Exiting program...\n");
        }
    } while (choice >= 1 && choice <= 4);

    return 0;
}
