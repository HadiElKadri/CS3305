// By Hadi El-Kadri
// Date: Dec. 4, 2020
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Struct account to hold accounts and balances
struct account
{
    int account_number;
    int balance;
};

//Struct transaction to hold transactions and transaction information
struct transaction
{
    int  account1;
    int  account2;
    char type;
    int  amount;
    int  client;
};

//Initialize variables and arrays needed
pthread_mutex_t    a_lock, b_lock;
struct account     array[500];
struct transaction array2[1000];
int                numTransactions = 0;
int                n_threads       = 0;
pthread_t*         threads;

// This function performs the transactions on the account from the clients
void* performTransactions(void* arg)
{
    for (int i = 0; i < numTransactions; ++i) {
        struct transaction* transfer = &array2[i];
        if (transfer->client != (int)arg)
            continue;

        // Entry Region
        pthread_mutex_lock(&a_lock);

        // Transfer
        if (transfer->type == 't') {
            if (array[transfer->account1 - 1].balance - transfer->amount >= 0) {
                array[transfer->account1 - 1].balance -= transfer->amount;
                array[transfer->account2 - 1].balance += transfer->amount;
            }
        }
        // Deposit
        else if (transfer->type == 'd') {
            array[transfer->account1 - 1].balance += transfer->amount;
        }
        // Withdraw
        else {
            if (array[transfer->account1 - 1].balance - transfer->amount >= 0)
                array[transfer->account1 - 1].balance -= transfer->amount;
        }

        // Exit Region
        pthread_mutex_unlock(&a_lock);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    
    // Open file for parsing
    char const* const fileName = argv[1];
    FILE*             file     = fopen(fileName, "r");
    char              line[256];
    int               numAccounts = 0;
    int               count2      = 0;
    int               count4      = 0;
    
    // Get each line of file 
    while (fgets(line, sizeof(line), file)) {
        char  delim[] = " ";
        char* ptr     = strtok(line, delim);
        
        // Check if line is for account and create new account struct filling it in
        // Append to array of accounts
        if (ptr[0] == 'a') {
            struct account new_account;
            int            val         = ptr[1];
            new_account.account_number = val - 48;
            ++numAccounts;
            ptr                 = strtok(NULL, delim);
            ptr                 = strtok(NULL, delim);
            new_account.balance = atoi(ptr);
            array[count2]       = new_account;
            count2++;
        }
        
        // Check if line is for clients and create new transaction struct filling it in
        // Append to array of transactions
        if (ptr[0] == 'c') {
            ++count4;
            int lol = 1;

            while (lol != 0) {
                struct transaction new_transaction;
                new_transaction.account1 = '0';
                new_transaction.account2 = '0';
                new_transaction.client   = count4;

                if (ptr[0] != 't' && ptr[0] != 'd' && ptr[0] != 'w') {
                    ptr = strtok(NULL, delim);
                }

                if (ptr[0] == 't') {
                    new_transaction.type     = ptr[0];
                    ptr                      = strtok(NULL, delim);
                    int val                  = ptr[1];
                    new_transaction.account1 = val - 48;
                    ptr                      = strtok(NULL, delim);
                    int val2                 = ptr[1];
                    new_transaction.account2 = val2 - 48;
                    ptr                      = strtok(NULL, delim);
                    new_transaction.amount   = atoi(ptr);
                }

                if (ptr[0] == 'd' || ptr[0] == 'w') {
                    new_transaction.type     = ptr[0];
                    ptr                      = strtok(NULL, delim);
                    int val                  = ptr[1];
                    new_transaction.account1 = val - 48;
                    ptr                      = strtok(NULL, delim);
                    new_transaction.amount   = atoi(ptr);
                }

                array2[numTransactions] = new_transaction;
                ++numTransactions;

                ptr = strtok(NULL, delim);
                if (ptr == 0)
                    lol = 0;
            }
        }
    }

    // Perform the clients transactions
    n_threads = numTransactions;
    threads   = calloc(n_threads, sizeof(pthread_t));
    for (int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, performTransactions, (void*)i);
    }
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    // Print the accounts information
    for (int i = 0; i < numAccounts; i++) {
        printf("a%d %c %d\r\n", array[i].account_number, 'b', array[i].balance);
        fprintf(file, "a%d %c %d\r\n", array[i].account_number, 'b',
                array[i].balance);
    }
    // Free memory
    free(threads);

    fclose(file);
    return 0;
}
