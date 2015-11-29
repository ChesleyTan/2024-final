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
    char *err_msg;
    if (sqlite3_exec(conn, CREATE_QUERY, 0, 0, &err_msg)) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
    else {
#ifdef DEBUG
        print_debug("Successfully created table.");
#endif
    }
}
