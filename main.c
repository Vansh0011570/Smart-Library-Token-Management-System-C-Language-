C Programming, Data Structures, File Handling, Record Management, Modular Functions, CLI Application, Token System, Fine Calculation, Educational Systems
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 10
#define MAX_STUDENTS 10
#define MAX_TOKENS 3

typedef struct {
    int book_id;
    char title[50];
    int available;
} Book;

typedef struct {
    int book_id;
    char title[50];
    char issue_date[20];
} Token;

typedef struct {
    int id;
    char name[50];
    int token_count;
    Token tokens[MAX_TOKENS];
} Student;

Book books[MAX_BOOKS] = {
    {101, "C Programming", 1},
    {102, "Data Structures", 1},
    {103, "Operating Systems", 1},
    {104, "Computer Networks", 1}
};

Student students[MAX_STUDENTS] = {
    {1, "Amit Sharma", 0},
    {2, "Riya Mehta", 0}
};

char* get_today_date() {
    static char buffer[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return buffer;
}

void list_books() {
    printf("\nAvailable Books:\n");
    for (int i = 0; i < 4; i++) {
        printf("Book ID: %d | Title: %s | Status: %s\n", books[i].book_id, books[i].title, books[i].available ? "Available" : "Issued");
    }
}

void list_students() {
    printf("\nRegistered Students:\n");
    for (int i = 0; i < 2; i++) {
        printf("ID: %d | Name: %s\n", students[i].id, students[i].name);
    }
}

void issue_book(int student_id, int book_id) {
    Student* s = NULL;
    Book* b = NULL;

    for (int i = 0; i < 2; i++)
        if (students[i].id == student_id) s = &students[i];

    for (int i = 0; i < 4; i++)
        if (books[i].book_id == book_id) b = &books[i];

    if (!s || !b) {
        printf("Invalid Student or Book ID.\n");
        return;
    }

    if (!b->available) {
        printf("Book not available.\n");
        return;
    }

    if (s->token_count >= MAX_TOKENS) {
        printf("Token limit reached. Return a book before issuing another.\n");
        return;
    }

    Token t;
    t.book_id = book_id;
    strcpy(t.title, b->title);
    strcpy(t.issue_date, get_today_date());

    s->tokens[s->token_count++] = t;
    b->available = 0;

    printf("Book '%s' issued to %s on %s.\n", b->title, s->name, t.issue_date);
}

void return_book(int student_id, int book_id) {
    Student* s = NULL;
    Book* b = NULL;

    for (int i = 0; i < 2; i++)
        if (students[i].id == student_id) s = &students[i];

    for (int i = 0; i < 4; i++)
        if (books[i].book_id == book_id) b = &books[i];

    if (!s || !b) {
        printf("Invalid Student or Book ID.\n");
        return;
    }

    for (int i = 0; i < s->token_count; i++) {
        if (s->tokens[i].book_id == book_id) {
            time_t now = time(NULL);
            struct tm issue_tm = {0};
            sscanf(s->tokens[i].issue_date, "%d-%d-%d", &issue_tm.tm_year, &issue_tm.tm_mon, &issue_tm.tm_mday);
            issue_tm.tm_year -= 1900;
            issue_tm.tm_mon -= 1;
            time_t issue_time = mktime(&issue_tm);
            double days = difftime(now, issue_time) / (60 * 60 * 24);
            double fine = 0;
            if (days > 14) fine = (days - 14) * 2;

            for (int j = i; j < s->token_count - 1; j++)
                s->tokens[j] = s->tokens[j + 1];
            s->token_count--;
            b->available = 1;

            printf("Book returned. Days kept: %.0f | Fine: ₹%.2f\n", days, fine);
            return;
        }
    }

    printf("Book was not issued to this student.\n");
}

void view_student_tokens(int student_id) {
    Student* s = NULL;
    for (int i = 0; i < 2; i++)
        if (students[i].id == student_id) s = &students[i];

    if (!s) {
        printf("Student not found.\n");
        return;
    }

    printf("\nBooks issued to %s:\n", s->name);
    if (s->token_count == 0) {
        printf("No books currently issued.\n");
        return;
    }

    for (int i = 0; i < s->token_count; i++) {
        printf("Book: %s | Issued on: %s\n", s->tokens[i].title, s->tokens[i].issue_date);
    }
}

int main() {
    int choice, sid, bid;
    while (1) {
        printf("\n===== Smart Library Menu =====\n");
        printf("1. List Available Books\n");
        printf("2. List Registered Students\n");
        printf("3. Issue a Book\n");
        printf("4. Return a Book\n");
        printf("5. View Student Issued Books\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: list_books(); break;
            case 2: list_students(); break;
            case 3:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                printf("Enter Book ID to issue: ");
                scanf("%d", &bid);
                issue_book(sid, bid);
                break;
            case 4:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                printf("Enter Book ID to return: ");
                scanf("%d", &bid);
                return_book(sid, bid);
                break;
            case 5:
                printf("Enter Student ID: ");
                scanf("%d", &sid);
                view_student_tokens(sid);
                break;
            case 0:
                printf("Thank you for using the Smart Library System.\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}

