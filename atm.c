#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "accounts.dat"
#define LOG_FILE "log.txt"

/*
=========================================
👥 CONTRIBUTORS (FOR GITHUB SUBMISSION)
=========================================

Member 1 - Arshiya Pathan (Project Lead)
- Core structure, login system, file handling

Member 2
- Deposit & withdraw logic improvement

Member 3
- UI improvement & input validation

Member 4
- Transfer system & PIN change feature
=========================================
*/

struct Account {
    int accNo;
    int pin;
    float balance;
};

struct Account acc;

// 🔹 LOG FUNCTION
void addLog(char msg[]) {
    FILE *fp = fopen(LOG_FILE, "a");
    if(fp != NULL) {
        fprintf(fp, "%s\n", msg);
        fclose(fp);
    }
}

// 🔹 CREATE ACCOUNT (Member 1)
void createAccount() {
    FILE *fp = fopen(FILE_NAME, "ab");

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accNo);

    printf("Set PIN: ");
    scanf("%d", &acc.pin);

    acc.balance = 0;

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account Created Successfully!\n");
    addLog("Account created (Member 1)");
}

// 🔹 LOGIN (Member 1)
int login() {
    int accNo, pin;
    FILE *fp = fopen(FILE_NAME, "rb");

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    while(fread(&acc, sizeof(acc), 1, fp)) {
        if(acc.accNo == accNo && acc.pin == pin) {
            fclose(fp);
            printf("Login Successful!\n");
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid Account or PIN!\n");
    return 0;
}

// 🔹 UPDATE FILE (Member 1)
void updateFile() {
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *fp2 = fopen("temp.dat", "wb");

    struct Account temp;

    while(fread(&temp, sizeof(temp), 1, fp)) {
        if(temp.accNo == acc.accNo) {
            fwrite(&acc, sizeof(acc), 1, fp2);
        } else {
            fwrite(&temp, sizeof(temp), 1, fp2);
        }
    }

    fclose(fp);
    fclose(fp2);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);
}

// 🔹 FIND ACCOUNT (Member 4)
int findAccount(int accNo, struct Account *result) {
    FILE *fp = fopen(FILE_NAME, "rb");

    while(fread(result, sizeof(struct Account), 1, fp)) {
        if(result->accNo == accNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// 🔹 UPDATE ANY ACCOUNT (Member 4)
void updateAnyAccount(struct Account updated) {
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *fp2 = fopen("temp.dat", "wb");

    struct Account temp;

    while(fread(&temp, sizeof(temp), 1, fp)) {
        if(temp.accNo == updated.accNo) {
            fwrite(&updated, sizeof(updated), 1, fp2);
        } else {
            fwrite(&temp, sizeof(temp), 1, fp2);
        }
    }

    fclose(fp);
    fclose(fp2);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);
}

// 🔹 ATM MENU
void atmMenu() {
    int choice;
    float amount;

    do {
        printf("\n===== ATM MENU =====\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer Money\n");
        printf("5. Change PIN\n");
        printf("6. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {

            // Member 3
            case 1:
                printf("Current Balance: %.2f\n", acc.balance);
                break;

            // Member 2
            case 2:
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);

                if(amount > 0) {
                    acc.balance += amount;
                    updateFile();
                    addLog("Deposit done (Member 2)");
                    printf("Deposit Successful!\n");
                } else {
                    printf("Invalid amount!\n");
                }
                break;

            // Member 2 + 3
            case 3:
                // Edited by Member 3 - UI improvement
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);

                if(amount <= 0) {
                    printf("Invalid amount!\n");
                }
                else if(amount > acc.balance) {
                    printf("Insufficient Balance!\n");
                }
                else {
                    acc.balance -= amount;
                    updateFile();
                    addLog("Withdraw done (Member 2)");
                    printf("Withdraw Successful!\n");
                }
                break;

            // Member 4
            case 4: {
                int targetAccNo;
                struct Account target;

                printf("Enter target account number: ");
                scanf("%d", &targetAccNo);

                if(findAccount(targetAccNo, &target)) {

                    printf("Enter amount: ");
                    scanf("%f", &amount);

                    if(amount > 0 && amount <= acc.balance) {

                        acc.balance -= amount;
                        target.balance += amount;

                        updateFile();
                        updateAnyAccount(target);

                        printf("Transfer Successful!\n");
                        addLog("Transfer done (Member 4)");

                    } else {
                        printf("Invalid transfer amount!\n");
                    }

                } else {
                    printf("Target account not found!\n");
                }
                break;
            }

            // Member 4
            case 5:
                printf("Enter new PIN: ");
                scanf("%d", &acc.pin);

                updateFile();
                printf("PIN Changed Successfully!\n");
                addLog("PIN changed (Member 4)");
                break;

            case 6:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 6);
}

// 🔹 MAIN (Member 1)
int main() {
    int choice;

    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                createAccount();
                break;

            case 2:
                if(login()) {
                    atmMenu();
                }
                break;

            case 3:
                printf("Thank you!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);

    return 0;
}