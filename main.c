//
//  Baker's Cove - Employee Manager
//  Course: Programming 2 - CMP1025
//  Group Members: Javia Clarke 2209358, Dejahnae Faircloough 2406846, Ougaba Gray 2405837, Daeshaun McIntyre 2403178
//  Tutor: Christopher Panther
//  Login: Username - bcove_admin | Password - bcove123
//
//  This program oversees the employee records side of the system.
//  It handles admin sign in, adding, listing, finding, updating and deleting employees.
//  All of whihc is saved to employee.csv via the sequential file approach.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ID          10
#define MAX_NAME        50
#define MAX_ROLE        20
#define MAX_STATUS      15
#define MAX_PASSWORD    50
#define MAX_USERNAME    30
#define MAX_LINE        200

//
#define EMPLOYEE_FILE   "employee.csv"
#define ADMIN_FILE      "bcove_admin.txt"

//  Employee roles at Baker's Cove
#define NUM_ROLES 5
const char *VALID_ROLES[NUM_ROLES] = {
    "Baker", "Server", "Cashier", "Manager", "Cleaner"
};

// Valid employee statuses
#define NUM_STATUSES 3
const char *VALID_STATUSES[NUM_STATUSES] = {
    "Active", "On Leave", "Terminated"
};

// Employee Structure
typedef struct {
    char id[MAX_ID];
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    char role[MAX_ROLE];
    char status[MAX_STATUS];
} Employee;

// Authenticates the admin using credentials from bcove_admin.txt
void adminSignIn() {
    char storedUsername[MAX_USERNAME];
    char storedPassword[MAX_PASSWORD];
    char enteredUsername[MAX_USERNAME];
    char enteredPassword[MAX_PASSWORD];
    int attempts = 3;

    // 1. Open the admin file. If it fails then we print error and exit.
    FILE *file = fopen(ADMIN_FILE, "r");
    if (file == NULL) {
        printf("ERROR! Unable to open admin file.\n");
        exit(1);
    }

    // 2. Read the stored credentials from the file
    fscanf(file, "%s %s", storedUsername, storedPassword);
    fclose(file);

    if (strlen(storedPassword) < 6) {
        printf("ERROR! Admin password does not meet minimum length requirement.\n");
        exit(1);
    }

    // 3. Loop for limited attempts
    while (attempts > 0) {
        printf("Username: ");
        scanf("%s", enteredUsername);

        printf("Password: ");
        scanf("%s", enteredPassword);

        if (strcmp(enteredUsername, storedUsername) == 0 &&
            strcmp(enteredPassword, storedPassword) == 0) {
                printf("Welcome, %s!\n", storedUsername);
                return;
            } else {
                attempts--;
                if (attempts > 0)
                    printf("Incorrect credentials. %d attempt(s) remaining.\n", attempts);
                }
    }
    printf("Too many failed attempts. Exiting.\n");
    exit(1);
}

int isValidRole(char *role) {
    for (int i = 0; i < NUM_ROLES; i++) {
        if (strcmp(role, VALID_ROLES[i]) == 0)
            return 1;  // found it
    }
    return 0;  // not found
}

int isValidStatus(char *status) {
    for (int i = 0; i < NUM_STATUSES; i++) {
        if (strcmp(status, VALID_STATUSES[i]) == 0)
            return 1;
    }
    return 0;
}

// Adds new employee record to employee.csv with a generated ID
void addEmployee() {
    printf("\n==== Add New Employee ====\n");
    Employee emp;

    // 1. Ask for employee ID
    srand(time(NULL));
    int randNum = rand() % 9000 + 1000;
    sprintf(emp.id, "EMP%d", randNum);
    printf("Generated Employee ID: %s\n", emp.id);

    // 2. Check if ID already exists in the file
    FILE *check = fopen(EMPLOYEE_FILE, "r");
    if (check != NULL) {
        char line[MAX_LINE];
        char existingID[MAX_ID];
        while (fgets(line, sizeof(line), check)) {
            sscanf(line, "%[^,]", existingID);
            if (strcmp(existingID, emp.id) == 0) {
                printf("Employee ID already exists.\n");
                fclose(check);
                return;
            }
        }
        fclose(check);
    }

    // 3. Ask for first and last name
    printf("Enter First Name: ");
    scanf("%s", emp.firstName);
    printf("Enter Last Name: ");
    scanf("%s", emp.lastName);

    // 4. Ask for role and validate it
    printf("Enter Role (Baker/Server/Cashier/Manager/Cleaner): ");
    scanf("%s", emp.role);
    if (!isValidRole(emp.role)) {
        printf("Invalid role. Employee not added.\n");
        return;
    }

    // 5. Ask for status and validate
    printf("Enter Status (Active/On Leave/Terminated): ");
    scanf(" %[^\n]", emp.status);
    if (!isValidStatus(emp.status)) {
        printf("Invalid status.\n");
        return;
    }

    // 6. Open the file in append mode and write the new employee
    FILE *file = fopen(EMPLOYEE_FILE, "a");
    if (file == NULL) {
        printf("ERROR! Could not open employee file.\n");
        return;
    }
    fprintf(file, "%s,%s,%s,%s,%s\n", emp.id, emp.firstName, emp.lastName, emp.role, emp.status);
    fclose(file);
    printf("Employee added successfully.\n");
}

// Displays all employee records stored in employee.csv
void listEmployees() {
    printf("\n==== Employee List ====\n");
    FILE *file = fopen(EMPLOYEE_FILE, "r");
    if (file == NULL) {
        printf("ERROR! No employee records found.\n");
        return;
    }

    Employee emp;
    char line[MAX_LINE];
    int count = 0;

    printf("\n%-10s %-15s %-15s %-12s %-12s\n",
           "ID", "First Name", "Last Name", "Role", "Status");
    printf("---------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               emp.id, emp.firstName, emp.lastName, emp.role, emp.status);
        printf("\n%-10s %-15s %-15s %-12s %-12s\n",
               emp.id, emp.firstName, emp.lastName, emp.role, emp.status);
        count++;
    }

    if (count == 0)
        printf("No employees found.\n");

    fclose(file);
}

// Finds an employee from employee.csv based on their id and shows their record
void findEmployee () {
    printf("\n==== Find Employee ====\n");
    char searchID[MAX_ID];
    printf("Enter Employee ID to search: ");
    scanf("%s", searchID);

    FILE *file = fopen(EMPLOYEE_FILE, "r");
    if (file == NULL) {
        printf("No employee records found.\n");
        return;
    }

    Employee emp;
    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               emp.id, emp.firstName, emp.lastName, emp.role, emp.status);
        if (strcmp(emp.id, searchID) == 0) {
            printf("\n--- Employee Found ---\n");
            printf("ID: %s\n", emp.id);
            printf("Name: %s %s\n", emp.firstName, emp.lastName);
            printf("Role: %s\n", emp.role);
            printf("Status: %s\n", emp.status);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee not found.\n");

    fclose(file);
}

// Finds and updates employee information in employee.csv
void updateEmployee () {
    printf("\n==== Update Employee ====\n");
    char updateID[MAX_ID];
    printf("Enter Employee ID to update: ");
    scanf("%s", updateID);

    // 1. Read all employees into an array
    Employee employees[100];
    int count = 0;

    FILE *file = fopen(EMPLOYEE_FILE, "r");
    if (file == NULL) {
        printf("No employee records found.\n");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               employees[count].id,
               employees[count].firstName,
               employees[count].lastName,
               employees[count]. role,
               employees[count].status);
        count++;
    }
    fclose(file);

    // 2. Find the matching employee
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].id, updateID) ==0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Employee not found.\n");
        return;
    }

    // 3. Ask what to update
    int choice;
    printf("What would you like to update?\n");
    printf("1. Role\n");
    printf("2. Status\n");
    printf("3. Both\n");
    scanf("%d", &choice);

    if (choice == 1 || choice == 3) {
        printf("Enter new Role (Baker/Server/Cashier/Manager/Cleaner): ");
        scanf("%s", employees[found].role);
        if (!isValidRole(employees[found].role)) {
            printf("Invalid role. Update cancelled.\n");
            return;
        }
    }

    if (choice == 2 || choice == 3) {
        printf("Enter new Status (Active/On Leave/Terminated): ");
        scanf(" %[^\n]", employees[found].status);
        if (!isValidStatus(employees[found].status)) {
            printf("Invalid status. Update cancelled.\n");
            return;
        }
    }

    // 4. Rewrite the file with updated records
    file = fopen(EMPLOYEE_FILE, "w");
    if (file == NULL) {
        printf("ERROR! Could not save updates.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
                employees[i].id,
                employees[i].firstName,
                employees[i].lastName,
                employees[i].role,
                employees[i].status);
    }

    fclose(file);
    printf("Employee updated successfully.\n");
}

// Finds and deletes employee record from employee.csv
void deleteEmployee () {
    printf("\n==== Delete Employee ====\n");
    char deleteID[MAX_ID];
    printf("Enter Employee ID to delete: ");
    scanf("%s", deleteID);

    // 1. Read all employees into an array
    Employee employees[100];
    int count = 0;

    FILE *file = fopen(EMPLOYEE_FILE, "r");
    if (file == NULL) {
        printf("No employee records found.\n");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
               employees[count].id,
               employees[count].firstName,
               employees[count].lastName,
               employees[count].role,
               employees[count].status);
        count++;
    }
    fclose(file);

    // 2. Find the matching employee
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].id, deleteID) ==0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Employee not found.\n");
        return;
    }

    // 3. Rewrite the file without the deleted employee
    file = fopen(EMPLOYEE_FILE, "w");
    if (file == NULL) {
        printf("ERROR! Could not save changes.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i == found) continue; //This is what skip the deleted employee
        fprintf(file, "%s,%s,%s,%s,%s\n",
                employees[i].id,
                employees[i].firstName,
                employees[i].lastName,
                employees[i].role,
                employees[i].status);
    }

    fclose(file);
    printf("Employee deleted successfully.\n");
}

int main () {
    int choice;

    adminSignIn();

    do {
        printf("\n==== Baker's Cove - Admin Console  ====\n");
        printf("1. Add Employee\n");
        printf("2. List Employees\n");
        printf("3. Find Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addEmployee(); break;
            case 2: listEmployees(); break;
            case 3: findEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: printf("Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice !=6);

    return 0;
}
