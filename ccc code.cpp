#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[50];
char currentRole[20];

// ======================= PASSWORD INPUT (HIDDEN) =======================
void inputPassword(char *pwd) {
    char ch;
    int i = 0;
    while ((ch = getch()) != 13) { // 13 = Enter
        if (ch == 8 && i > 0) { // Backspace
            printf("\b \b");
            i--;
        } else if (ch != 8) {
            pwd[i++] = ch;
            printf("*");
        }
    }
    pwd[i] = '\0';
}

// ======================= LOGIN SYSTEM =======================
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("===== LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    inputPassword(password);
    printf("\n");

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentUser, fileUser);
            strcpy(currentRole, fileRole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ======================= STUDENT FUNCTIONS =======================
void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    printf("Enter Roll: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (!fp) {
        printf("No student records found.\n");
        return;
    }

    printf("\n===== STUDENT LIST =====\n");
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
}

void searchStudent() {
    int roll;
    int found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    printf("Enter Roll to Search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            printf("FOUND ? Roll: %d | Name: %s | Marks: %.2f\n",
                   s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found.\n");

    fclose(fp);
}

void updateStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Roll to Update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            found = 1;
            printf("New Name: ");
            scanf("%s", s.name);
            printf("New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Updated!\n");
    else printf("Student Not Found.\n");
}

void deleteStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Roll to Delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Deleted!\n");
    else printf("Student Not Found.\n");
}

// ======================= MENUS =======================
void adminMenu() {
    int ch;
    while (1) {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n2. Display\n3. Search\n4. Update\n5. Delete\n6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid Option!\n");
        }
    }
}

void staffMenu() {
    int ch;
    while (1) {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n2. Search Student\n3. Update Student\n4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: return;
            default: printf("Invalid Option!\n");
        }
    }
}

void userMenu() {
    int ch;
    while (1) {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n2. Search Student\n3. Logout\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid Option!\n");
        }
    }
}

void guestMenu() {
    printf("\n===== GUEST MENU =====\n");
    printf("You have read-only access.\n");
    displayStudents();
}

// ======================= MAIN =======================
int main() {
    if (loginSystem()) {
        printf("Login Success! Welcome %s (%s)\n", currentUser, currentRole);

        if (strcmp(currentRole, "ADMIN") == 0) adminMenu();
        else if (strcmp(currentRole, "STAFF") == 0) staffMenu();
        else if (strcmp(currentRole, "USER") == 0) userMenu();
        else guestMenu();
    }
    else {
        printf("Access Denied.\n");
    }

    return 0;
}
