#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

#define Max_History 100

typedef struct User {
    int id; char name[50]; float balance; int historyCount;
    char transactionHistory[Max_History][200];
    struct User* next; // pointer for next user
} User;

User* head = NULL;
int userCount = 0;

// Function to validate and enter a valid integer number (for ID)
int enterValidNumber(const char* Text) {
    int num;
    while (true) {
        printf("%s", Text);
        if (scanf("%d", &num) == 1 && num >= 0) {
            while (getchar() != '\n');
            break;
        } else {
            printf("Invalid input, please enter a valid positive number.\n");
            while (getchar() != '\n');
        }
    }
    return num;
}

// Function to validate and enter a valid float number (for amount)
float enterValidFloatNumber(const char* Text) {
    float num;
    while (true) {
        printf("%s", Text);
        if (scanf("%f", &num) == 1 && num > 0) {
            break;
        } else {
            printf("Invalid input, please enter a valid positive amount.\n");
            while (getchar() != '\n');
        }
    }
    return num;
}

// function to add a transaction to the user's history
void addTransaction(User* user, const char* transaction) {
    if (user -> historyCount < Max_History) {
        strcpy(user -> transactionHistory[user -> historyCount], transaction);
        user -> historyCount++;
    } else {
        printf("Transaction history full for user %s\n", user->name);
    }
}

void createAccount() {
    User* newUser = (User*) malloc(sizeof (User));
    if(newUser == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newUser -> historyCount = 0; // Initialize transaction history count to 0
    newUser -> next = NULL;
    newUser->balance = 0.0f;

    printf("Create User Account\n********************\n");
    newUser -> id = userCount + 1;

    printf("Enter name: ");
    scanf(" %[^\n]", newUser->name);

    float amount = enterValidFloatNumber("Enter amount to deposit: ");
    newUser->balance += amount; // Update the balance

    // Output the initial deposit as a transaction
    char transaction[200];
    snprintf(transaction, sizeof(transaction), "Initial deposit: %.2f", newUser -> balance);
    addTransaction(newUser, transaction);
    printf("Transaction added: %s\n", transaction);

    // Insert the new user at the end of the linked list
    if (head == NULL) {
        head = newUser;
    } else {
        User* temp = head;
        while (temp -> next != NULL) { temp = temp -> next; }
        temp -> next = newUser;
    }
    userCount++;
    printf("Account created successfully! \n");
}

// Function to find a user by ID
User* findUserById(int id) {
    User* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void updateAccount() {
    printf("Update Account\n********************\n");
    bool found = false;

    do {
        int id = enterValidNumber("Enter account ID to update: ");
        User* user = findUserById(id);
        if (user != NULL) {
            found = true;

            printf("Enter new name (leave blank to keep current): ");
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            if (newName[0] != '\n') {
                newName[strcspn(newName, "\n")] = 0;
                strcpy(user->name, newName);
            }

            printf("Enter new balance (negative number to keep current): ");
            float newBalance;
            scanf("%f", &newBalance);
            if (newBalance >= 0) {
                user->balance = newBalance;
            }

            printf("Account updated successfully!\n");
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }
    } while (!found);
}

// Function to check account balance
void checkBalance() {
    printf("Check Account Balance\n********************\n");
    bool found = false;

    do {
        int id = enterValidNumber("Enter account ID to check balance:");
        User* user = findUserById(id);

        if (user != NULL) {
            found = true;
            printf("Account Balance for %s: %.2f\n", user->name, user->balance);
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }
    } while (!found);
}

void depositMoney() {
    printf("Deposit Money\n********************\n");
    bool found = false;
    do {
        int id = enterValidNumber("Enter account ID to deposit money: ");
        User* user = findUserById(id);

        if (user != NULL) { // Check if the user exists
            found = true;
            float amount = enterValidFloatNumber("Enter amount to deposit: ");
            if (amount > 0) {
                user->balance += amount; // Update the balance
                printf("Deposit successful! New balance: %.2f\n", user->balance);

                // Record the transaction
                char transaction[200];
                snprintf(transaction, sizeof(transaction), "Deposited %.2f", amount);
                addTransaction(user, transaction);
            } else {
                printf("Invalid deposit amount! Must be greater than zero.\n");
            }
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }

    } while (!found); // Repeat until a valid user is found
}

void withdrawMoney() {
    printf("Withdraw Money\n********************\n");
    bool found = false;

    do {
        int id = enterValidNumber("Enter account ID to withdraw money:");
        User* user = findUserById(id);
        if (user != NULL) {
            found = true;
            float amount = enterValidFloatNumber("Enter amount to withdraw:");

            if (amount > user->balance) {
                printf("Insufficient balance!\n");
            } else {
                user->balance -= amount;
                printf("Withdrawal successful! New balance: %.2f\n", user->balance);

                // Add transaction to history
                char transaction[200];
                snprintf(transaction, sizeof(transaction), "Withdrew %.2f", amount);
                addTransaction(user, transaction);
            }
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }
    } while (!found);
}

void transferMoney() {
    printf("Transfer Money\n********************\n");

    // Get sender's account
    int fromId = enterValidNumber("Enter your account ID: ");
    User* fromUser = findUserById(fromId);
    if (fromUser == NULL) {
        printf("Your account not found!\n");
        return;
    }

    // Get recipient's account
    int toId = enterValidNumber("Enter recipient's account ID: ");
    User* toUser = findUserById(toId);
    if (toUser == NULL) {
        printf("Recipient's account not found!\n");
        return;
    }

    // Enter the transfer amount
    float amount = enterValidFloatNumber("Enter amount to transfer: ");
    if (amount > fromUser->balance) {
        printf("Insufficient balance! You have %.2f available.\n", fromUser->balance);
        return;
    } else {
        fromUser->balance -= amount;
        toUser->balance += amount;
        printf("Transfer successful! Your new balance is %.2f\n", fromUser->balance);

        // Record transactions for both accounts
        char transactionFrom[200], transactionTo[200];
        snprintf(transactionFrom, sizeof(transactionFrom), "Transferred %.2f to %s (ID: %d)", amount, toUser->name, toUser->id);
        addTransaction(fromUser, transactionFrom);
        snprintf(transactionTo, sizeof(transactionTo), "Received %.2f from %s (ID: %d)", amount, fromUser->name, fromUser->id);
        addTransaction(toUser, transactionTo);
    }
}

void deleteAccount() {
    printf("Delete Account\n********************\n");
    bool found = false;

    do {
        int id = enterValidNumber("Enter account ID to delete: ");
        User* user = findUserById(id);

        if (user != NULL) {
            found = true;
            //if the user to delete is the head of the list
            if (head == user) {
                head = user->next;
            } else {
                User* temp = head;
                while (temp->next != user) {
                    temp = temp->next;
                }
                temp->next = user->next; // Unlink the user from the list
            };

            free(user);
            printf("Account deleted successfully!\n");
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }
    } while (!found);
}

void viewTransactionHistory() {
    printf("View Transaction History\n********************\n");
    bool found = false;

    do {
        int id = enterValidNumber("Enter account ID to view transaction history:");
        User* user = findUserById(id);
        if (user != NULL) {
            found = true;
            if (user->historyCount == 0) {
                printf("No transaction history for user %s.\n", user->name);
            } else {
                printf("Transaction History for %s (ID: %d):\n", user->name, user->id);
                for (int i = 0; i < user->historyCount; i++) {
                    printf("%d. %s\n", i + 1, user->transactionHistory[i]);
                }
            }
        } else {
            printf("Your provided ID is not found. Please try again.\n");
        }
    } while (!found);
}

void viewUsers() {
    printf("View All Users\n********************\n");

     if (head == NULL) {
         printf("No users found!\n");
         return;
     } else {
         User* temp = head;
         printf("ID    Name           Balance\n");
         printf("-----------------------------\n");
         while (temp != NULL) {
             printf("%d     %-15s%.2f\n", temp->id, temp->name, temp->balance);
             temp = temp->next;
         }
     }
}

int main() {
    int option;
    do {
        option = enterValidNumber("\n 1. Create_Account\n 2. Update_Account\n 3. Check_Balance(ByOne)\n 4. Deposit_Money\n 5. Withdraw_Money\n 6. Transfer_Money\n 7. Delete_Account\n 8. View_Transaction_History\n 9. View_Users(All) \n 0. Exit \n Enter your option: ");

        switch (option) {
            case 1: createAccount(); break;
            case 2: updateAccount(); break;
            case 3: checkBalance(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: transferMoney(); break;
            case 7: deleteAccount(); break;
            case 8: viewTransactionHistory(); break;
            case 9: viewUsers(); break;
            case 0: printf("\n Existing Program. \n"); break;
            default: printf("Invalid Option!\n"); break;
        }
    } while (option != 0);

    return 0;
}