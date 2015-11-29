#ifndef DATABASEHELPER_H_
#define DATABASEHELPER_H_

#include <iostream>
#include <sqlite3.h>
#include "util.h"

static const char *DB_NAME = "transactions.db";
static const char *CREATE_QUERY = "CREATE TABLE IF NOT EXISTS Transactions ("\
                                  "Payer TEXT NOT NULL,"\
                                  "Debtor TEXT NOT NULL,"\
                                  "Amount REAL,"\
                                  "Description TEXT)";

class DatabaseHelper {
    public:
        DatabaseHelper();
        virtual ~DatabaseHelper();
        void initialize_db();
    private:
        sqlite3 *conn;
};

#endif /* DATABASEHELPER_H_ */
