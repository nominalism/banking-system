#include <stdio.h>

// Data structure for storing account information
struct Account {
  unsigned int acc_num;
  char name[16];
  char last_name[15];
  double balance;
  double deposit;
  double withdrawal;
  double credit;
};
// Data structure for storing transaction information
struct Transaction {
  unsigned int acc_num;
  char type;
  float value;
  float balan;
};
// Function prototypes
void import();
void register_account(FILE *fPtr);
void transaction(FILE *cPtr, FILE *movPtr);
void extract(FILE *readptr, FILE *extractptr);
void balance(FILE *readptr);
void report(FILE *readptr, FILE *move);
void client_balances(FILE *readptr);
void ler();
int main() {
  // Display options menu
  unsigned int choice;
  FILE *contasPtr;
  FILE *movPtr;
  if (((contasPtr = fopen("accounts.dat", "rb+")) == NULL) ||
      (movPtr = fopen("transactions.dat", "rb+")) == NULL) {
    puts("non-existent file");
  } else {
    do {
      printf("\n--- Banking System ---\n");
      printf("1. Register\n");
      printf("2. Extract \n");
      printf("3. Deposit or withdrawal\n");
      printf("4. All acounts\n");
      printf("5. Transactions Report\n");
      printf("6. Registered accounts\n");
      printf("7. Balance\n");
      printf("8. Import\n");
      printf("9. Exit\n");
      printf("Enter your choice: ");
      scanf("%d", &choice);

      switch (choice) {
      case 1:
        register_account(contasPtr);
        break;
      case 2:
        extract(contasPtr, movPtr);
        break;
      case 3:
        transaction(contasPtr, movPtr);
        break;
      case 4:
        ler();
        break;
      case 5:
        report(contasPtr, movPtr);
        break;
      case 6:
        client_balances(contasPtr);
        break;
      case 7:
        balance(contasPtr);
        break;
      case 8:
        import(contasPtr);
        break;
      case 9:
        printf("Exiting program.\n");
        break;
      default:
        printf("Invalid choice.\n");
        break;
      }
    } while (choice != 9);
  }
}
void register_account(FILE *input_file) {
  unsigned int acc_num;
  printf("%s", " enter account number (1 to 100): ");
  scanf("%u", &acc_num); // conta nova
  fseek(input_file, (acc_num - 1) * sizeof(struct Account),
        SEEK_SET); // seta na conta que esta sendoadicionada
  struct Account client = {0, "", "", 0, 0, 0, 0};
  fread(&client, sizeof(struct Account), 1, input_file);
  if (client.acc_num != 0) {
    printf("\naccount %d already exists\n", client.acc_num);
  } else {
    // Read the account information from the user
    struct Account account;
    printf("name       last_name    limit\n");
    scanf("%14s %14s %lf", client.name, client.last_name, &client.credit);
    client.acc_num = acc_num;
    fseek(input_file, (acc_num - 1) * sizeof(struct Account), SEEK_SET);
    fwrite(&client, sizeof(struct Account), 1, input_file);
  }
}
void ler() {
  struct Account c;
  FILE *file = fopen("accounts.dat", "rb");

  if (file) {
    while (!feof(file)) {
      if (fread(&c, sizeof(struct Account), 1, file))
        printf("\nAccount: %d\nName: %s\nlast_name:%s\n limite: %f\n",
               c.acc_num, c.name, c.last_name, c.credit);
    }
    fclose(file);
  } else
    printf("\error opening file!\n");
}
void transaction(FILE *contasPtr, FILE *movPtr) {
  unsigned int c;

  puts("account code 1 - 100 ");
  scanf("%d", &c);
  fseek(contasPtr, (c - 1) * sizeof(struct Account), SEEK_SET);
  struct Account cli = {0, "", "", 0, 0, 0, 0};
  
  fseek(movPtr, sizeof(struct Transaction), SEEK_END);
  struct Transaction trans = {0, ' ', 0, 0};
  
  fread(&trans, sizeof(struct Transaction), 1, movPtr);
  fread(&cli, sizeof(struct Account), 1, contasPtr);

  if (cli.acc_num == 0) {
    printf("account %d without information.\n", c);
  } else { // altera
    printf("DEPOSIT (1) OU WITHDRAWL (2) SIR?\n");
    printf("\n%-6d %10.2f\n\n", cli.acc_num, cli.balance);
    int operator1;
    scanf("%d", &operator1);
    // faz transação
    double limite1 = cli.balance + cli.credit;
    printf("%-6d%-16s%-16s%10.2f%10.2f\n", cli.acc_num, cli.name, cli.last_name,
           cli.balance, limite1);
    trans.acc_num = cli.acc_num;
    printf("enter the amount: ");
    double transacao; // transacao amount
    scanf("%lf", &transacao);

    if (operator1 == 1) {
      cli.balance += transacao; // update record saldo
      cli.deposit += transacao;
      trans.type = 'D';
      trans.value = transacao;
      trans.balan = cli.balance;

    } else if (operator1 == 2) {
      double limit;
      limit = (cli.balance - transacao) + cli.credit;
      if (limite1 < 0) {
        printf("\b sem limite \n");
        return;
      }
      cli.balance -= transacao; // update record saldo
      cli.withdrawal -= transacao;
      trans.type = 'S';
      trans.value = transacao;
      trans.balan = cli.balance;
    } else {
      puts("Invalid\n");
      return;
    }
    // move the ptr
    fseek(contasPtr, (c - 1) * sizeof(struct Account), SEEK_SET);
    fseek(movPtr, sizeof(struct Transaction), SEEK_END);
    fwrite(&cli, sizeof(struct Account), 1, contasPtr);
    fwrite(&trans, sizeof(struct Transaction), 1, movPtr);
    // atualiza
  }
}
void extract(FILE *readptr, FILE *extractptr) {
  FILE *extratotxt;
  extratotxt = fopen("extrato.txt", "w");

  rewind(readptr);
  rewind(extractptr);

  int o;
  printf("account number: ");
  scanf("%d", &o);

  while (!feof(readptr)) {
    struct Account cli = {0, "", "", 0, 0, 0, 0};
    struct Account save = {0, "", "", 0, 0, 0, 0};
    fread(&cli, sizeof(struct Account), 1, readptr);
    if (cli.acc_num == o) {
      fprintf(extratotxt, "+++++++++++++++++++++++++++++++++++++++++++++\n");
      fprintf(extratotxt,
              "++++++++++++++++ Extract ++++++++++++++++\n"
              "account: %d\n"
              "Name: %-14s\n"
              "opening balance: 0.00\n"
              "+++++++++++++++++++++++++++++++++++++++++++++\n"
              "%6s%14s%14s\n",
              cli.acc_num, cli.name, "Type", "Value", "balance");
      fprintf(extratotxt, "+++++++++++++++++++++++++++++++++++++++++++++\n");
    }
  }
  double final_balance;
  while (!feof(extractptr)) {
    struct Transaction trans = {0, ' ', 0, 0};
   
    fread(&trans, sizeof(struct Transaction), 1, extractptr);
    if (trans.acc_num == o) {
      fprintf(extratotxt, "%6c%14.2lf%14.2lf\n", trans.type, trans.value,
              trans.balan);
      final_balance = trans.balan;
    }
  }

  fprintf(extratotxt, "Final balance %22.2lf\n", final_balance);
  fprintf(extratotxt, "+++++++++++++++++++++++++++++++++++++++++++++\n");
  fclose(extratotxt);
}
void client_balances(FILE *readptr) {
  FILE *fprintf1;
  fprintf1 = fopen("accounts.txt", "w");
  rewind(readptr);
  fprintf(fprintf1,
          "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fprintf(fprintf1, "%-6s%-16s%-16s%10s\n", "account", "Name", "last_name",
          "Limit");
  while (!feof(readptr)) {
    struct Account cli = {0, "", "", 0, 0, 0, 0};
    int result = fread(&cli, sizeof(struct Account), 1, readptr);
    if (result != 0 && cli.acc_num != 0) {
      fprintf(fprintf1, "%5d %-16s%-16s%10.2lf\n", cli.acc_num, cli.name,
              cli.last_name, cli.credit);
    }
  }
  fprintf(fprintf1,
          "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fclose(fprintf1);
}
// saque
void balance(FILE *readptr) {
  FILE *withdrawalTxt;
  withdrawalTxt = fopen("balance.txt", "w");

  rewind(readptr);
  fprintf(withdrawalTxt, "+++++++++++++++++++++++++++++++++++++++++++++++++++++"
                         "+++++++++++++++++++\n");
  fprintf(withdrawalTxt, "%-6s%-16s%-16s%10s%10s%10s\n", "Conta", "Nome",
          "last_name", "deposit", "withdrawl", "balance");

  while (!feof(readptr)) {
    struct Account cli = {0, "", "", 0, 0, 0, 0};
    int result = fread(&cli, sizeof(struct Account), 1, readptr);
    if (result != 0 && cli.acc_num != 0) {
      fprintf(withdrawalTxt, "%5d %-16s%-16s%10.2lf%10.2lf%10.2lf\n",
              cli.acc_num, cli.name, cli.last_name, cli.deposit, cli.withdrawal,
              cli.balance);
    }
  }
  fprintf(
      withdrawalTxt,
      "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fclose(withdrawalTxt);
  fclose(readptr);
}
void report(FILE *readptr, FILE *moveptr) {
  FILE *relTxt;
  relTxt = fopen("report.txt", "w");
  rewind(readptr);
  rewind(moveptr);
  while (!feof(moveptr)) {
    struct Transaction trans = {0, ' ', 0, 0};
    fread(&trans, sizeof(struct Transaction), 1, moveptr);
    struct Account cli = {0, "", "", 0, 0, 0, 0};
    fseek(readptr, (trans.acc_num - 1) * sizeof(struct Account), SEEK_SET);
    fread(&cli, sizeof(struct Transaction), 1, readptr);
    fprintf(relTxt, "%4d%14s%6c%14.2lf\n", trans.acc_num, cli.name, trans.type,
            trans.value);
  }
  fclose(relTxt);
}
// Import accounts from a sequential file and store them in a binary file
// Register a new account

void import(FILE *fPtr) {
  FILE *txt;
  FILE *error;
  txt = fopen("accounts1.txt", "r");
  error = fopen("error.txt", "w");
  if (txt) {
    while (!feof(txt)) {
      unsigned int contaNum;
      fscanf(txt, "%d", &contaNum);
      fseek(fPtr, (contaNum - 1) * sizeof(struct Account), SEEK_SET);
      struct Account cli = {0, "", "", 0.0};
      fread(&cli, sizeof(struct Account), 1, fPtr);
      fscanf(txt, "%s %s %lf ", cli.name, cli.last_name, &cli.credit);
      if (cli.acc_num != 0) {
        printf("conta #%d com informação.\n", cli.acc_num);
        cli.acc_num = contaNum;
        fseek(fPtr, (cli.acc_num - 1) * sizeof(struct Account), SEEK_SET);
        fprintf(error, "%5d %15s %10s %8.2lf Já existe \n", cli.acc_num,
                cli.name, cli.last_name, cli.credit);
      } else {
        cli.acc_num = contaNum;
        fseek(fPtr, (cli.acc_num - 1) * sizeof(struct Account), SEEK_SET);
        fwrite(&cli, sizeof(struct Account), 1, fPtr);
        printf("Pronto\n");
      }
    }

    fclose(txt);
    fclose(error);
  }
}
