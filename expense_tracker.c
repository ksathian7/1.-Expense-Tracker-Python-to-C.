#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_EXPENSES 500
#define STR_LEN 50

// ----------------- Structures (Defining Data Structures) -----------------

typedef struct {
    char username[STR_LEN];
    char password[STR_LEN];
    float salary;
} User;

typedef struct {
    char username[STR_LEN];
    char category[STR_LEN];
    float amount;
} Expense;

// ----------------- Helper Functions for File Operations -----------------

// Registers a new user and appends to user.txt
void signUp() {
    printf("\n--- Sign Up ---\n");
    char username[STR_LEN], password[STR_LEN];
    float salary;

    printf("Enter new username: ");
    scanf("%s", username);
    printf("Enter new password: ");
    scanf("%s", password);
    printf("Enter your monthly salary: ");
    if (scanf("%f", &salary) != 1) {
        printf("Invalid salary. Please enter a number.\n\n");
        // Clear input buffer
        while (getchar() != '\n');
        return;
    }

    // Check for duplicate username
    FILE *file = fopen("user.txt", "r");
    if (file != NULL) {
        char u[STR_LEN], p[STR_LEN];
        float s;
        while (fscanf(file, "%[^,],%[^,],%f\n", u, p, &s) != EOF) {
            if (strcmp(u, username) == 0) {
                printf("Username already exists. Try a different one.\n\n");
                fclose(file);
                return;
            }
        }
        fclose(file);
    }

    // Append new user data
    file = fopen("user.txt", "a");
    if (file == NULL) {
        printf("[Error] Could not open file.\n");
        return;
    }
    fprintf(file, "%s,%s,%.2f\n", username, password, salary);
    fclose(file);
    printf("[Success] Account created successfully!\n\n");
}

// Logs in a user, returns salary via pointer, returns 1 on success, 0 on failure
int login(char *loggedInUser, float *userSalary) {
    printf("\n--- Login ---\n");
    char username[STR_LEN], password[STR_LEN];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen("user.txt", "r");
    if (file == NULL) {
        printf("No users found. Please sign up first.\n\n");
        return 0;
    }

    char u[STR_LEN], p[STR_LEN];
    float s;
    while (fscanf(file, "%[^,],%[^,],%f\n", u, p, &s) != EOF) {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            printf("[Success] Login successful!\n\n");
            strcpy(loggedInUser, username);
            *userSalary = s;
            fclose(file);
            return 1;
        }
    }

    printf("[Error] Invalid credentials. Try again.\n\n");
    fclose(file);
    return 0;
}

// Appends an expense to money.txt (using txt instead of csv for simple C parsing)
void addExpense(const char *username) {
    char category[STR_LEN];
    float amount;

    printf("Enter category (Food, Travel, etc.): ");
    scanf("%s", category);
    printf("Enter amount: ");
    if (scanf("%f", &amount) != 1) {
        printf("Invalid amount. Please enter a number.\n\n");
        while (getchar() != '\n');
        return;
    }

    FILE *file = fopen("money.txt", "a");
    if (file == NULL) {
        printf("[Error] Could not open expense file.\n");
        return;
    }
    fprintf(file, "%s,%s,%.2f\n", username, category, amount);
    fclose(file);
    printf("[Success] Expense added!\n\n");
}

// Calculates and prints expenses for the logged-in user
float viewExpenses(const char *username) {
    float total = 0.0;
    int found = 0;
    printf("\n--- Your Expenses ---\n");

    FILE *file = fopen("money.txt", "r");
    if (file == NULL) {
        printf("No expenses recorded yet.\n\n");
        return 0.0;
    }

    char u[STR_LEN], c[STR_LEN];
    float amt;
    while (fscanf(file, "%[^,],%[^,],%f\n", u, c, &amt) != EOF) {
        if (strcmp(u, username) == 0) {
            printf("%s - %.2f\n", c, amt);
            total += amt;
            found = 1;
        }
    }
    fclose(file);

    if (!found) {
        printf("No expenses recorded.\n");
    } else {
        printf("Total Expense: %.2f\n\n", total);
    }
    return total;
}

// Clears only the logged-in user's data by writing other records to a temporary file
void clearExpenses(const char *username) {
    FILE *file = fopen("money.txt", "r");
    if (file == NULL) return;

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        fclose(file);
        return;
    }

    char u[STR_LEN], c[STR_LEN];
    float amt;
    while (fscanf(file, "%[^,],%[^,],%f\n", u, c, &amt) != EOF) {
        if (strcmp(u, username) != 0) {
            fprintf(tempFile, "%s,%s,%.2f\n", u, c, amt);
        }
    }
    fclose(file);
    fclose(tempFile);

    remove("money.txt");
    rename("temp.txt", "money.txt");
    printf("[Cleared] Your expenses have been cleared!\n\n");
}

// ----------------- Main Menu Logic loops -----------------

void mainMenu(const char *username, float salary) {
    int choice;
    while (1) {
        printf("Logged in as: %s\n", username);
        printf("Monthly Salary: %.2f\n", salary);
        printf("1. Add Expense\n2. View Expenses\n3. Clear Expenses\n4. View Savings\n5. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid selection.\n\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            addExpense(username);
        } else if (choice == 2) {
            viewExpenses(username);
        } else if (choice == 3) {
            clearExpenses(username);
        } else if (choice == 4) {
            float total = viewExpenses(username); // Reuses the calculation logic
            printf("\n[Savings] Estimated Savings: %.2f\n\n", salary - total);
        } else if (choice == 5) {
            printf("[Locked] Logging out...\n\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n\n");
        }
    }
}

int main() {
    int option;
    char loggedInUser[STR_LEN];
    float userSalary = 0.0;

    while (1) {
        printf("{Start} Welcome to Expense Calculator\n");
        printf("1. Login\n2. Sign Up\n3. Exit\n");
        printf("Enter option: ");
        if (scanf("%d", &option) != 1) {
            printf("Invalid alternative. Try again.\n\n");
            while (getchar() != '\n');
            continue;
        }

        if (option == 1) {
            if (login(loggedInUser, &userSalary)) {
                mainMenu(loggedInUser, userSalary);
            }
        } else if (option == 2) {
            signUp();
        } else if (option == 3) {
            printf("[Exit] Exiting... See you again!\n");
            break;
        } else {
            printf("Invalid option. Try again.\n\n");
        }
    }
    return 0;
}
