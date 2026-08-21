#include <stdio.h>
#include <stdlib.h>

#define DATA_FILE "account.dat"

typedef struct {
    int accountNo;
    char name[50];
    double balance;
} Account;

void createAccount() {
    Account acc;
    FILE *fp = fopen(DATA_FILE, "ab");

    if (fp == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &acc.accountNo);
    printf("Enter name: ");
    scanf(" %49[^\n]", acc.name);
    printf("Enter initial deposit: ");
    scanf("%lf", &acc.balance);

    if (acc.balance < 0)
        acc.balance = 0;

    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    printf("Account created successfully.\n");
}

void deposit() {
    int no, found = 0;
    double amount;
    Account acc;
    FILE *fp = fopen(DATA_FILE, "rb+");

    if (fp == NULL) {
        printf("No account data found.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &no);
    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        fclose(fp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accountNo == no) {
            acc.balance += amount;
            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);
    printf(found ? "Deposit successful.\n" : "Account not found.\n");
}

void withdraw() {
    int no, found = 0;
    double amount;
    Account acc;
    FILE *fp = fopen(DATA_FILE, "rb+");

    if (fp == NULL) {
        printf("No account data found.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &no);
    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        fclose(fp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accountNo == no) {
            found = 1;
            if (amount <= acc.balance) {
                acc.balance -= amount;
                fseek(fp, -(long)sizeof(Account), SEEK_CUR);
                fwrite(&acc, sizeof(Account), 1, fp);
                printf("Withdrawal successful.\n");
            } else {
                printf("Insufficient balance.\n");
            }
            break;
        }
    }

    fclose(fp);
    if (!found)
        printf("Account not found.\n");
}

void balanceEnquiry() {
    int no, found = 0;
    Account acc;
    FILE *fp = fopen(DATA_FILE, "rb");

    if (fp == NULL) {
        printf("No account data found.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &no);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accountNo == no) {
            printf("Account Holder: %s\n", acc.name);
            printf("Balance: %.2lf\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("Account not found.\n");
}

int main() {
    int choice;

    do {
        printf("\n--- BANKING SYSTEM ---\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Balance Enquiry\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: balanceEnquiry(); break;
            case 5: printf("Thank you.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}