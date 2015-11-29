#include "DatabaseHelper.h"
using namespace std;

DatabaseHelper::DatabaseHelper() {
    if (sqlite3_open(DB_NAME, &conn)) {
        cerr << "Could not open database connection: " << sqlite3_errmsg(conn) << endl;
    }
    else {
#ifdef DEBUG
        print_debug("Opened database connection");
#endif
    }
    initialize_db();
}

DatabaseHelper::~DatabaseHelper() {
    sqlite3_close(conn);
#ifdef DEBUG
    print_debug("Closed database connection");
#endif
}

void DatabaseHelper::initialize_db() {
    int ret;
    char *err_msg;
    ret = sqlite3_exec(conn, CREATE_TRANSACTIONS_QUERY, 0, 0, &err_msg);
    if (ret) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    else {
#ifdef DEBUG
        print_debug("Successfully created Transactions table.");
#endif
    }
    ret = sqlite3_exec(conn, CREATE_DEBTS_QUERY, 0, 0, &err_msg);
    if (ret) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    else {
#ifdef DEBUG
        print_debug("Successfully created Debts table.");
#endif
    }
}

void DatabaseHelper::add_transaction(std::string payer, std::string debtor,
                double amount, std::string description) {
    int ret;
    sqlite3_stmt *stmt;
    char *query = "INSERT INTO Transactions (Payer, Debtor, Amount,"\
                  "Description) VALUES (?, ?, ?, ?)";
    ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
    if(ret == SQLITE_OK) {
        // bind query arguments
        sqlite3_bind_text(stmt, 1, payer.c_str(), payer.length(), NULL);
        sqlite3_bind_text(stmt, 2, debtor.c_str(), debtor.length(), NULL);
        sqlite3_bind_double(stmt, 3, amount);
        sqlite3_bind_text(stmt, 4, description.c_str(), description.length(), NULL);

        // commit query
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    else {
        cerr << "Error preparing query for adding transaction" << endl;
    }
}
