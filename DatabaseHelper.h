#ifndef DATABASEHELPER_H_
#define DATABASEHELPER_H_

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>
#include "util.h"

static const char *DB_NAME = "transactions.db";
static const char *CREATE_TRANSACTIONS_QUERY =
                                  "CREATE TABLE IF NOT EXISTS Transactions ("\
                                  "Payer TEXT NOT NULL,"\
                                  "Debtor TEXT NOT NULL,"\
                                  "Amount REAL,"\
                                  "Description TEXT)";
static const char *CREATE_DEBTS_QUERY =
                                  "CREATE TABLE IF NOT EXISTS Debts ("\
                                  "Payer TEXT NOT NULL,"\
                                  "Debtor TEXT NOT NULL,"\
                                  "Amount REAL)";

class DatabaseHelper {
    public:
        DatabaseHelper();
        virtual ~DatabaseHelper();
        void initialize_db();
        void add_transaction(std::string payer, std::string debtor,
                double amount, std::string description);
        void add_debt(std::string payer, std::string debtor,
                double amount);
    private:
        sqlite3 *conn;
};

#endif /* DATABASEHELPER_H_ */
