#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Book {
    int id;
    char title[100];
    char author[100];
    char category[50];
};

struct User {
    char username[50];
    char password[50];
};

struct Issue {
    int bookId;
    int studentId;
    char username[50];
    int isReturned;
    time_t issueDate;
    int fine;
    int finePaid;
};

struct Student {
    int studentId;
    char name[50];
    char contact[15];
};

void addBook() {
    struct Book b, temp;
    FILE *fp;
    int found = 0;

    printf("\nEnter Book ID: ");

    if (scanf("%d", &b.id) != 1 || b.id <= 0) {
        printf("\nInvalid Book ID! Enter a positive number.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    fp = fopen("books.dat", "rb");

    if (fp != NULL) {
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (temp.id == b.id) {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (found == 1) {
        printf("\nBook ID already exists!\n");
        return;
    }

    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);

    if (b.title[0] == '\n') {
        printf("\nBook Title cannot be empty!\n");
        return;
    }

    printf("Enter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);

    if (b.author[0] == '\n') {
        printf("\nAuthor Name cannot be empty!\n");
        return;
    }

    printf("Enter Book Category: ");
    fgets(b.category, sizeof(b.category), stdin);

    if (b.category[0] == '\n') {
        printf("\nBook Category cannot be empty!\n");
        return;
    }

    fp = fopen("books.dat", "ab");

    if (fp == NULL) {
        printf("File opening error!\n");
        return;
    }

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("\nBook added successfully!\n");
}

void displayBooks() {

    struct Book b;
    FILE *fp;

    fp = fopen("books.dat", "rb");

    if (fp == NULL) {
        printf("\nNo books found!\n");
        return;
    }

    printf("\n--- BOOK RECORDS ---\n");

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("\nBook ID: %d", b.id);
        printf("\nTitle: %s", b.title);
        printf("\nAuthor: %s", b.author);
        printf("\nCategory: %s", b.category);
    }

    fclose(fp);
}

void searchBook() {
    struct Book b;
    FILE *fp;
    int searchID, found = 0;

    printf("\nEnter Book ID to search: ");

    if (scanf("%d", &searchID) != 1 || searchID <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        return;
    }

    getchar();

    fp = fopen("books.dat", "rb");

    if (fp == NULL) {
        printf("\nNo books found!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == searchID) {
            printf("\nBook Found!\n");
            printf("Book ID: %d\n", b.id);
            printf("Title: %s", b.title);
            printf("Author: %s\n", b.author);
            printf("Category: %s", b.category);

            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("\nBook not found!\n");
    }

    fclose(fp);
}

void searchBookByCategory() {
    struct Book b;
    FILE *fp;
    char category[50];
    int found = 0;

    fp = fopen("books.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Books Found!\n");
        return;
    }

    printf("\n===== SEARCH BY CATEGORY =====\n");
    printf("Enter Category: ");
    scanf(" %49[^\n]", category);

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        b.category[strcspn(b.category, "\n")] = '\0';

        if (strcmp(b.category, category) == 0) {
            printf("\nBook ID: %d", b.id);
            printf("\nTitle: %s", b.title);
            printf("\nAuthor: %s", b.author);
            printf("\nCategory: %s\n", b.category);
            printf("-------------------------\n");

            found = 1;
        }
    }

    if (found == 0) {
        printf("\nNo Books Found in This Category!\n");
    }

    fclose(fp);
}

void updateBook() {
    struct Book b;
    FILE *fp;
    int id, found = 0;

    fp = fopen("books.dat", "rb+");

    if (fp == NULL) {
        printf("\nNo books found!\n");
        return;
    }

    printf("\nEnter Book ID to update: ");

    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        fclose(fp);
        return;
    }
    getchar();

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {

            printf("\nEnter New Book Title: ");
            fgets(b.title, sizeof(b.title), stdin);

            if (b.title[0] == '\n') {
                printf("\nTitle cannot be empty!\n");
                fclose(fp);
                return;
            }

            printf("Enter New Author Name: ");
            fgets(b.author, sizeof(b.author), stdin);

            if (b.author[0] == '\n') {
                printf("\nAuthor cannot be empty!\n");
                fclose(fp);
                return;
            }

            printf("Enter New Category: ");
            fgets(b.category, sizeof(b.category), stdin);

            if (b.category[0] == '\n') {
                printf("\nCategory cannot be empty!\n");
                fclose(fp);
                return;
            }

            fseek(fp, -(long)sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);

            found = 1;
            printf("\nBook updated successfully!\n");
            break;
        }
    }

    if (found == 0) {
        printf("\nBook not found!\n");
    }

    fclose(fp);
}

void deleteBook() {
    struct Book b;
    FILE *fp, *temp;
    int id, found = 0;

    printf("\nEnter Book ID to delete: ");

    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        return;
    }

    getchar();

    fp = fopen("books.dat", "rb");

    if (fp == NULL) {
        printf("\nNo books found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL) {
        printf("\nFile creation error!\n");
        fclose(fp);
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found == 1) {
        if (remove("books.dat") == 0 &&
            rename("temp.dat", "books.dat") == 0) {
            printf("\nBook deleted successfully!\n");
        } else {
            printf("\nError updating book file!\n");
        }
    } else {
        remove("temp.dat");
        printf("\nBook not found!\n");
    }
}

void registerUser() {
    struct User u, temp;
    FILE *fp;
    int found = 0;

    printf("\n===== USER REGISTRATION =====\n");

    printf("Enter Username: ");

    if (scanf("%49s", u.username) != 1) {
        printf("\nInvalid Username!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Password: ");

    if (scanf("%49s", u.password) != 1) {
        printf("\nInvalid Password!\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen("users.dat", "rb");

    if (fp != NULL) {
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (strcmp(temp.username, u.username) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (found == 1) {
        printf("\nUsername Already Exists!\n");
        return;
    }

    fp = fopen("users.dat", "ab");

    if (fp == NULL) {
        printf("\nFile opening error!\n");
        return;
    }

    if (fwrite(&u, sizeof(u), 1, fp) == 1) {
        printf("\nRegistration Successful!\n");
    } else {
        printf("\nRegistration Failed!\n");
    }

    fclose(fp);
}

int loginUser() {
    struct User u;
    char username[50], password[50];
    FILE *fp;
    int found = 0;

    printf("\n===== USER LOGIN =====\n");

    printf("Enter Username: ");

    if (scanf("%49s", username) != 1) {
        printf("\nInvalid Username!\n");
        while (getchar() != '\n');
        return 0;
    }

    printf("Enter Password: ");

    if (scanf("%49s", password) != 1) {
        printf("\nInvalid Password!\n");
        while (getchar() != '\n');
        return 0;
    }

    fp = fopen("users.dat", "rb");

    if (fp == NULL) {
        printf("\nNo registered users found!\n");
        return 0;
    }

    while (fread(&u, sizeof(u), 1, fp) == 1) {
        if (strcmp(u.username, username) == 0 &&
            strcmp(u.password, password) == 0) {

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found == 1) {
        printf("\nLogin Successful! Welcome %s!\n", username);
    } else {
        printf("\nInvalid Username or Password!\n");
    }

    return found;
}

int adminLogin() {
    char username[50], password[50];

    printf("\n===== ADMIN LOGIN =====\n");

    printf("Enter Admin Username: ");
    if (scanf("%49s", username) != 1) {
        printf("\nInvalid Username Input!\n");
        while (getchar() != '\n');
        return 0;
    }

    printf("Enter Admin Password: ");
    if (scanf("%49s", password) != 1) {
        printf("\nInvalid Password Input!\n");
        while (getchar() != '\n');
        return 0;
    }

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0) {

        printf("\nAdmin Login Successful!\n");
        return 1;

    } else {
        printf("\nInvalid Admin Username or Password!\n");
        return 0;
    }
}

void adminDashboard() {
    struct Book b;
    struct User u;
    struct Issue i;

    FILE *fp;

    int totalBooks = 0;
    int totalUsers = 0;
    int issuedBooks = 0;
    int returnedBooks = 0;
    int availableBooks = 0;

    fp = fopen("books.dat", "rb");

    if (fp != NULL) {
        while (fread(&b, sizeof(b), 1, fp) == 1) {
            totalBooks++;
        }
        fclose(fp);
    }

    fp = fopen("users.dat", "rb");

    if (fp != NULL) {
        while (fread(&u, sizeof(u), 1, fp) == 1) {
            totalUsers++;
        }
        fclose(fp);
    }

    fp = fopen("issues.dat", "rb");

    if (fp != NULL) {
        while (fread(&i, sizeof(i), 1, fp) == 1) {
            if (i.isReturned == 0) {
                issuedBooks++;
            } else {
                returnedBooks++;
            }
        }
        fclose(fp);
    }

    availableBooks = totalBooks - issuedBooks;

    printf("\n===== ADMIN DASHBOARD =====\n");
    printf("Total Books: %d\n", totalBooks);
    printf("Total Registered Users: %d\n", totalUsers);
    printf("Total Issued Books: %d\n", issuedBooks);
    printf("Total Returned Books: %d\n", returnedBooks);
    printf("Available Books: %d\n", availableBooks);
    printf("============================\n");
}

void returnBook() {
    struct Issue i;
    FILE *fp, *temp;

    int bookId, found = 0;
    char username[50];

    time_t currentDate;
    double daysPassed;
    int fine = 0;

    printf("\n===== RETURN BOOK =====\n");

    printf("Enter Book ID: ");

    if (scanf("%d", &bookId) != 1 || bookId <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Username: ");

    if (scanf("%49s", username) != 1) {
        printf("\nInvalid Username!\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen("issues.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Issued Books Found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL) {
        printf("\nFile opening error!\n");
        fclose(fp);
        return;
    }

    while (fread(&i, sizeof(i), 1, fp) == 1) {

        if (i.bookId == bookId &&
            strcmp(i.username, username) == 0 &&
            i.isReturned == 0) {

            currentDate = time(NULL);

            daysPassed = difftime(currentDate, i.issueDate) / 86400;

            fine = 0;

            if (daysPassed > 30) {
                fine = ((int)daysPassed - 30) * 5;
            }

            i.fine = fine;
            i.finePaid = 0;
            i.isReturned = 1;

            found = 1;
        }

        fwrite(&i, sizeof(i), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found == 1) {

        if (remove("issues.dat") == 0 &&
            rename("temp.dat", "issues.dat") == 0) {

            printf("\nBook Returned Successfully!\n");
            printf("Total Fine: %d\n", fine);

        } else {
            printf("\nError updating issue file!\n");
        }

    } else {
        remove("temp.dat");
        printf("\nIssued Book Record Not Found!\n");
    }
}

void issueBook() {
    struct Book b;
    struct Issue i;
    struct Student s;

    FILE *fp, *ip, *sp;

    int found = 0;
    int studentFound = 0;
    int alreadyIssued = 0;

    printf("\n===== ISSUE BOOK =====\n");

    printf("Enter Book ID: ");

    if (scanf("%d", &i.bookId) != 1 || i.bookId <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Student ID: ");

    if (scanf("%d", &i.studentId) != 1 || i.studentId <= 0) {
        printf("\nInvalid Student ID!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Username: ");

    if (scanf("%49s", i.username) != 1) {
        printf("\nInvalid Username!\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen("books.dat", "rb");

    if (fp == NULL) {
        printf("\nNo books available!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == i.bookId) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found == 0) {
        printf("\nBook Not Found!\n");
        return;
    }

    sp = fopen("students.dat", "rb");

    if (sp == NULL) {
        printf("\nNo students registered!\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, sp) == 1) {
        if (s.studentId == i.studentId) {
            studentFound = 1;
            break;
        }
    }

    fclose(sp);

    if (studentFound == 0) {
        printf("\nStudent ID Not Found!\n");
        return;
    }

    ip = fopen("issues.dat", "rb");

    if (ip != NULL) {
        struct Issue temp;

        while (fread(&temp, sizeof(temp), 1, ip) == 1) {
            if (temp.bookId == i.bookId &&
                temp.isReturned == 0) {
                alreadyIssued = 1;
                break;
            }
        }

        fclose(ip);
    }

    if (alreadyIssued == 1) {
        printf("\nBook is Already Issued!\n");
        return;
    }

    i.isReturned = 0;
    i.issueDate = time(NULL);
    i.fine = 0;
    i.finePaid = 0;

    ip = fopen("issues.dat", "ab");

    if (ip == NULL) {
        printf("\nFile opening error!\n");
        return;
    }

    fwrite(&i, sizeof(i), 1, ip);
    fclose(ip);

    printf("\nBook Issued Successfully!\n");
}

void displayIssuedBooks() {
    struct Issue i;
    FILE *fp;
    int issuedCount = 0;
    int returnedCount = 0;

    fp = fopen("issues.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Issued Books Found!\n");
        return;
    }

    printf("\n===== ISSUED BOOKS RECORD =====\n");

    while (fread(&i, sizeof(i), 1, fp) == 1) {
        printf("\nBook ID: %d", i.bookId);
        printf("\nStudent ID: %d", i.studentId);
        printf("\nUsername: %s", i.username);
        printf("\nFine Amount: Rs. %d", i.fine);
        printf("\nFine Payment: %s", i.finePaid == 1? "Paid": "Unpaid");

        if (i.isReturned == 0) {
            printf("\nStatus: Issued\n");
            issuedCount++;
        } else {
            printf("\nStatus: Returned\n");
            returnedCount++;
        }

        printf("-----------------------------\n");
    }

    printf("\n===== BOOK SUMMARY =====\n");
    printf("Total Issued Books: %d\n", issuedCount);
    printf("Total Returned Books: %d\n", returnedCount);

    fclose(fp);
}

void bookAvailability() {
    struct Book b;
    struct Issue i;

    FILE *fp, *ip;

    int totalBooks = 0;
    int issuedBooks = 0;
    int availableBooks = 0;

    fp = fopen("books.dat", "rb");

    if (fp != NULL) {
        while (fread(&b, sizeof(b), 1, fp) == 1) {
            totalBooks++;
        }
        fclose(fp);
    }

    ip = fopen("issues.dat", "rb");

    if (ip != NULL) {
        while (fread(&i, sizeof(i), 1, ip) == 1) {
            if (i.isReturned == 0) {
                issuedBooks++;
            }
        }
        fclose(ip);
    }

    availableBooks = totalBooks - issuedBooks;

    if(availableBooks < 0){
       availableBooks = 0;
    }

    printf("\n===== BOOK AVAILABILITY =====\n");
    printf("Total Books: %d\n", totalBooks);
    printf("Issued Books: %d\n", issuedBooks);
    printf("Available Books: %d\n", availableBooks);
}

void addStudent() {
    struct Student s, temp;
    FILE *fp;
    int found = 0;
    int i;

    printf("\n===== ADD STUDENT =====\n");

    printf("Enter Student ID: ");

    if (scanf("%d", &s.studentId) != 1 || s.studentId <= 0) {
        printf("\nInvalid Student ID!\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen("students.dat", "rb");

    if (fp != NULL) {
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (temp.studentId == s.studentId) {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (found == 1) {
        printf("\nStudent ID Already Exists!\n");
        return;
    }

    printf("Enter Student Name: ");

    if (scanf(" %49[^\n]", s.name) != 1) {
        printf("\nInvalid Student Name!\n");
        return;
    }

    printf("Enter Contact Number: ");

    if (scanf("%14s", s.contact) != 1) {
        printf("\nInvalid Contact Number!\n");
        return;
    }

    if (strlen(s.contact) != 10) {
        printf("\nContact Number Must Be 10 Digits!\n");
        return;
    }

    for (i = 0; i < 10; i++) {
        if (s.contact[i] < '0' || s.contact[i] > '9') {
            printf("\nContact Number Must Contain Only Digits!\n");
            return;
        }
    }

    fp = fopen("students.dat", "ab");

    if (fp == NULL) {
        printf("\nFile opening error!\n");
        return;
    }

    if (fwrite(&s, sizeof(s), 1, fp) == 1) {
        printf("\nStudent Added Successfully!\n");
    } else {
        printf("\nStudent Addition Failed!\n");
    }

    fclose(fp);
}

void displayStudents() {
    struct Student s;
    FILE *fp;
    int count = 0;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Student Records Found!\n");
        return;
    }

    printf("\n===== STUDENT RECORDS =====\n");

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("\nStudent ID: %d", s.studentId);
        printf("\nName: %s", s.name);
        printf("\nContact: %s", s.contact);
        printf("\n-------------------------\n");

        count++;
    }

    printf("\nTotal Students: %d\n", count);

    fclose(fp);
}

void issueHistory() {
    struct Issue i;
    FILE *fp;
    int count = 0;

    fp = fopen("issues.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Issue History Found!\n");
        return;
    }

    printf("\n===== ISSUE HISTORY =====\n");

    while (fread(&i, sizeof(i), 1, fp) == 1) {

        printf("\nBook ID: %d", i.bookId);
        printf("\nStudent ID: %d", i.studentId);
        printf("\nUsername: %s", i.username);

        printf("\nIssue Date: %s", ctime(&i.issueDate));

        if (i.isReturned == 0) {
            printf("Status: Issued\n");
        } else {
            printf("Status: Returned\n");
        }

        printf("Fine Amount: Rs. %d\n", i.fine);
        if (i.finePaid == 1) {
           printf("Fine Payment: Paid\n");
        } else {
           printf("Fine Payment: Unpaid\n");
        }
        printf("-----------------------------\n");

        count++;
    }

    printf("\nTotal Issue Records: %d\n", count);

    fclose(fp);
}

void payFine() {
    struct Issue i;
    FILE *fp, *tp;

    int bookId, studentId;
    int found = 0;
    int writeError = 0;

    printf("\n===== PAY FINE =====\n");

    printf("Enter Book ID: ");

    if (scanf("%d", &bookId) != 1 || bookId <= 0) {
        printf("\nInvalid Book ID!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Student ID: ");

    if (scanf("%d", &studentId) != 1 || studentId <= 0) {
        printf("\nInvalid Student ID!\n");
        while (getchar() != '\n');
        return;
    }

    fp = fopen("issues.dat", "rb");

    if (fp == NULL) {
        printf("\nNo Issue Records Found!\n");
        return;
    }

    tp = fopen("temp.dat", "wb");

    if (tp == NULL) {
        printf("\nFile Opening Error!\n");
        fclose(fp);
        return;
    }

    while (fread(&i, sizeof(i), 1, fp) == 1) {

        if (i.bookId == bookId &&
            i.studentId == studentId &&
            i.isReturned == 1 &&
            i.fine > 0 &&
            i.finePaid == 0) {

            i.finePaid = 1;
            found = 1;
        }

        if (fwrite(&i, sizeof(i), 1, tp) != 1) {
            writeError = 1;
            break;
        }
    }

    fclose(fp);
    fclose(tp);

    if (writeError == 1) {
        remove("temp.dat");
        printf("\nError writing issue records!\n");
        return;
    }

    if (found == 1) {

        if (remove("issues.dat") == 0 &&
            rename("temp.dat", "issues.dat") == 0) {

            printf("\nFine Paid Successfully!\n");

        } else {
            printf("\nError updating issue file!\n");
        }

    } else {
        remove("temp.dat");
        printf("\nUnpaid Fine Record Not Found!\n");
    }
}

int main() {
    int choice;
    int loggedIn = 0;
    int adminLoggedIn = 0;

    do {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. User Registration\n");
        printf("2. User Login\n");
        printf("3. Admin Login\n");
        printf("4. Add Book\n");
        printf("5. Display Books\n");
        printf("6. Search Book\n");
        printf("7. Search Book by Category\n");
        printf("8. Update Book\n");
        printf("9. Delete Book\n");
        printf("10. Logout\n");
        printf("11. Issue Book\n");
        printf("12. Return Book\n");
        printf("13. Display Issued Books\n");
        printf("14. Book Availability\n");
        printf("15. Add Student\n");
        printf("16. Display Students\n");
        printf("17. Issue History\n");
        printf("18. Pay Fine\n");
        printf("19. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");

          while (getchar() != '\n');

          choice = 0;
          continue;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                adminLoggedIn = 0;
                loggedIn = loginUser();
                break;

            case 3:
                loggedIn = 0;
                adminLoggedIn = adminLogin();
                if (adminLoggedIn == 1){
                   adminDashboard();
                }
                break;

            case 4:
                if (adminLoggedIn == 1){
                   addBook();
                } else {
                   printf("\nOnly Admin can add Books!\n");
                }
                break;

            case 5:
                if (loggedIn == 1 || adminLoggedIn == 1){
                   displayBooks();
                } else {
                   printf("\nPlease Login First!\n");
                }
                break;

            case 6:
                if (loggedIn == 1 || adminLoggedIn == 1){
                   searchBook();
                } else {
                   printf("\nPlease Login First!\n");
                }
                break;

            case 7:
                if (loggedIn == 1 || adminLoggedIn == 1) {
                   searchBookByCategory();
                } else {
                   printf("\nPlease Login First!\n");
                }
                break;

            case 8:
                if (adminLoggedIn == 1){
                   updateBook();
                } else {
                   printf("\nOnly Admin can update Books!\n");
                }
                break;

            case 9:
                if (adminLoggedIn == 1){
                   deleteBook();
                } else {
                   printf("\nOnly Admin can delete Books!\n");
                }
                break;

            case 10:
                if (loggedIn == 1 || adminLoggedIn == 1){
                    loggedIn = 0;
                    adminLoggedIn = 0;
                    printf("\nLogout Successful!\n");
                } else{
                    printf("\nYou are not logged in!\n");
                }
                break;

            case 11:
                if (loggedIn == 1 || adminLoggedIn == 1) {
                    issueBook();
                } else {
                    printf("\nPlease Login First!\n");
                }
                break;

            case 12:
                if (loggedIn == 1 || adminLoggedIn == 1) {
                    returnBook();
                } else {
                    printf("\nPlease Login First!\n");
                }
                break;

            case 13:
                if (loggedIn == 1 || adminLoggedIn == 1) {
                    displayIssuedBooks();
                } else {
                    printf("\nPlease Login First!\n");
                }
                break;

            case 14:
                if (loggedIn == 1 || adminLoggedIn == 1) {
                    bookAvailability();
                } else {
                    printf("\nPlease Login First!\n");
                }
                break;

            case 15:
                if (adminLoggedIn == 1) {
                    addStudent();
                } else {
                    printf("\nAdmin Login Required!\n");
                }
                break;

            case 16:
                if (adminLoggedIn == 1) {
                    displayStudents();
                } else {
                    printf("\nAdmin Login Required!\n");
                }
                break;

            case 17:
                if (adminLoggedIn == 1) {
                    issueHistory();
                } else {
                    printf("\nAdmin Login Required!\n");
                }
                break;

            case 18:
                if (adminLoggedIn == 1) {
                    payFine();
                } else {
                    printf("\nAdmin Login Required!\n");
                }
                break;

            case 19:
                printf("\nThank you for using our Library!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 19);

    return 0;
}
