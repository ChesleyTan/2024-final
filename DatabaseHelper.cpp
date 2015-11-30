#include "DatabaseHelper.h"
using namespace std;

static int num_rows;

DatabaseHelper::DatabaseHelper() {
    if (sqlite3_open(DB_NAME, &conn)) {
        cerr << "Could not open database connection: " << sqlite3_errmsg(conn)
                << endl;
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
    ret = sqlite3_exec(conn, CREATE_TRANSACTIONS_QUERY, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    else {
#ifdef DEBUG
        print_debug("Successfully created Transactions table.");
#endif
    }
    ret = sqlite3_exec(conn, CREATE_DEBTS_QUERY, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK) {
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
    const char *debtor_str = debtor.c_str();
    const char *payer_str = payer.c_str();
    int debtor_strlen = strlen(debtor_str);
    int payer_strlen = strlen(payer_str);
    int ret;
    sqlite3_stmt *stmt;
    const char *query = "INSERT INTO Transactions (Payer, Debtor, Amount,"
                        "Description) VALUES (?, ?, ?, ?)";
    ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
    if(ret == SQLITE_OK) {
        // bind query arguments
        sqlite3_bind_text(stmt, 1, payer_str, payer_strlen, NULL);
        sqlite3_bind_text(stmt, 2, debtor_str, debtor_strlen, NULL);
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

void DatabaseHelper::add_debt(std::string payer, std::string debtor,
                double amount) {
    const char *debtor_str = debtor.c_str();
    const char *payer_str = payer.c_str();
    int debtor_strlen = strlen(debtor_str);
    int payer_strlen = strlen(payer_str);
    // case 1: debtor already owes payer
    int orientation_case; // which case of payer/debtor already exists
    double existing_debt;
    int ret;
    sqlite3_stmt *stmt;
    const char *query;
    query = "SELECT Amount FROM Debts WHERE Payer = ? AND Debtor = ?";
    ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
    if (ret == SQLITE_OK) {
        // bind query arguments
        sqlite3_bind_text(stmt, 1, payer_str, payer_strlen, NULL);
        sqlite3_bind_text(stmt, 2, debtor_str, debtor_strlen, NULL);

        // commit query
        ret = sqlite3_step(stmt);
    }
    else {
        cerr << "Error preparing query for adding transaction" << endl;
    }
    if (ret == SQLITE_ROW) { // Row exists
        orientation_case = 1;
        existing_debt = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
    }
    // case 2: payer already owes debtor
    else {
        query = "SELECT Amount FROM Debts WHERE Payer = ? AND Debtor = ?";
        ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
        if (ret == SQLITE_OK) {
            // bind query arguments
            sqlite3_bind_text(stmt, 1, debtor_str, debtor_strlen, NULL);
            sqlite3_bind_text(stmt, 2, payer_str, payer_strlen, NULL);

            // commit query
            ret = sqlite3_step(stmt);
        }
        else {
            cerr << "Error preparing query for adding transaction" << endl;
        }
        if (ret == SQLITE_ROW) { // Row exists
            orientation_case = 2;
            existing_debt = sqlite3_column_double(stmt, 0);
            sqlite3_finalize(stmt);
        }
        // case 3: no prior history between payer and debtor
        else {
            orientation_case = 3;
        }
    }
#ifdef DEBUG
    ostringstream out;
#endif
    switch (orientation_case) {
        case 1:
#ifdef DEBUG
            out << debtor << " already owes " << payer << " " << fixed
                    << setprecision(2) << existing_debt;
            print_debug(out.str());
#endif
            query = "UPDATE Debts SET Amount = ? WHERE Payer = ? AND "
                    "Debtor = ?";
            ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
            if (ret == SQLITE_OK) {
                // bind query arguments
                sqlite3_bind_double(stmt, 1, existing_debt + amount);
                sqlite3_bind_text(stmt, 2, payer_str, payer_strlen, NULL);
                sqlite3_bind_text(stmt, 3, debtor_str, debtor_strlen, NULL);

                // commit query
                ret = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            else {
                cerr << "Error preparing query for adding transaction" << endl;
            }
            break;
        case 2:
#ifdef DEBUG
            out << payer << " already owes " << debtor << " " << fixed
                    << setprecision(2) << existing_debt;
            print_debug(out.str());
#endif
            // Case 2a: Perform update
            if (existing_debt > amount) {
                query = "UPDATE Debts SET Amount = ? WHERE Payer = ? AND "
                        "Debtor = ?";
                ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
                if (ret == SQLITE_OK) {
                    // bind query arguments
                    sqlite3_bind_double(stmt, 1, existing_debt - amount);
                    sqlite3_bind_text(stmt, 2, debtor_str, debtor_strlen, NULL);
                    sqlite3_bind_text(stmt, 3, payer_str, payer_strlen, NULL);

                    // commit query
                    ret = sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                else {
                    cerr << "Error preparing query for adding transaction"
                            << endl;
                }
            }
            // Case 2b: Perform deletion and insertion
            else {
                query = "DELETE FROM Debts WHERE Payer = ? AND Debtor = ?";
                ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
                if (ret == SQLITE_OK) {
                    // bind query arguments
                    sqlite3_bind_text(stmt, 1, debtor_str, debtor_strlen, NULL);
                    sqlite3_bind_text(stmt, 2, payer_str, payer_strlen, NULL);

                    // commit query
                    ret = sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                else {
                    cerr << "Error preparing query for adding transaction"
                            << endl;
                }
                query = "INSERT INTO Debts (Payer, Debtor, Amount) VALUES "
                        "(?, ?, ?)";
                ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
                if (ret == SQLITE_OK) {
                    // bind query arguments
                    sqlite3_bind_text(stmt, 1, payer_str, payer_strlen, NULL);
                    sqlite3_bind_text(stmt, 2, debtor_str, debtor_strlen, NULL);
                    sqlite3_bind_double(stmt, 3, amount - existing_debt);

                    // commit query
                    ret = sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                else {
                    cerr << "Error preparing query for adding transaction"
                            << endl;
                }
            }
            break;
        case 3:
#ifdef DEBUG
            print_debug(payer + " and " + debtor + " have no prior debts");
#endif
            query = "INSERT INTO Debts (Payer, Debtor, Amount) VALUES "
                    "(?, ?, ?)";
            ret = sqlite3_prepare(conn, query, strlen(query), &stmt, NULL);
            if (ret == SQLITE_OK) {
                // bind query arguments
                sqlite3_bind_text(stmt, 1, payer_str, payer_strlen, NULL);
                sqlite3_bind_text(stmt, 2, debtor_str, debtor_strlen, NULL);
                sqlite3_bind_double(stmt, 3, amount);

                // commit query
                ret = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            else {
                cerr << "Error preparing query for adding transaction" << endl;
            }
            break;
    }
    clear_zero_debts();
}

void DatabaseHelper::clear_zero_debts() {
    const char *query = "DELETE FROM Debts WHERE Amount = 0";
    char *err_msg;
    int ret = sqlite3_exec(conn, query, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}

int print_transaction_log_callback(void *data, int argc, char **argv,
        char **colNames) {
    double amount = atof(argv[2]);
    cout << argv[1] << " owes " << argv[0] << " $" << fixed << setprecision(2)
            << amount << " (" << argv[3] << ")" << endl;
    return 0;
}

void DatabaseHelper::print_transaction_log() {
    char *err_msg;
    const char *query = "SELECT Payer, Debtor, Amount, Description "
                        "FROM Transactions";
    int ret = sqlite3_exec(conn, query, print_transaction_log_callback, NULL,
            &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}

int print_summary_log_callback(void *data, int argc, char **argv,
        char **colNames) {
    ++num_rows;
    double amount = atof(argv[2]);
    cout << argv[1] << " owes " << argv[0] << " $" << fixed << setprecision(2)
            << amount << endl;
    return 0;
}

void DatabaseHelper::print_summary_log() {
    char *err_msg;
    const char *query = "SELECT Payer, Debtor, Amount "
                        "FROM Debts";
    num_rows = 0;
    int ret = sqlite3_exec(conn, query, print_summary_log_callback, NULL,
            &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    if (num_rows == 0) {
        cout << "There are no debts!" << endl;
    }
}

int print_analytics_largest_debt_callback(void *data, int argc, char **argv,
        char **colNames) {
    ++num_rows;
    double amount = atof(argv[1]);
    cout << argv[0] << " has the largest debt of $" << fixed << setprecision(2)
            << amount << endl;
    return 0;
}

int print_analytics_smallest_debt_callback(void *data, int argc, char **argv,
        char **colNames) {
    ++num_rows;
    double amount = atof(argv[1]);
    cout << argv[0] << " has the smallest debt of $" << fixed << setprecision(2)
            << amount << endl;
    return 0;
}

void DatabaseHelper::print_analytics() {
    char *err_msg;
    const char *query = "SELECT Debtor, SUM(Amount) "
                        "FROM Debts GROUP BY Debtor "
                        "ORDER BY Amount DESC LIMIT 1";
    num_rows = 0;
    int ret = sqlite3_exec(conn, query, print_analytics_largest_debt_callback,
            NULL, &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    if (num_rows == 0) {
        cout << "There are no debts!" << endl;
        return;
    }
    query = "SELECT Debtor, SUM(Amount) "
            "FROM Debts GROUP BY Debtor "
            "ORDER BY Amount LIMIT 1";
    num_rows = 0;
    ret = sqlite3_exec(conn, query, print_analytics_smallest_debt_callback,
            NULL, &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}

void DatabaseHelper::clear_debts() {
    char *err_msg;
    const char *query = "DELETE FROM Debts";
    int ret = sqlite3_exec(conn, query, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}
