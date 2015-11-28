#include "main.h"
using namespace std;

const char *PROMPT = ">> ";
int main() {
    string line;
    bool is_eof = false;
    cout << "== Expense/Debt Tracker by Chesley Tan, Qingjun Wang, and Shaoke Xu ==" << endl;
    cout << "Enter `help` for a list of commands" << endl;
    while (!is_eof) {
        cout << PROMPT;
        getline(cin, line);
        is_eof = cin.eof(); // Stop when EOF is reached
        if (is_eof) break;
        trim_trailing_whitespace(line);
        if (line == "") continue; // Ignore empty lines
#ifdef DEBUG
        cout << "You entered: " << line << endl;
#endif
        if (line == "help") {
#ifdef DEBUG
            cout << "Got help command" << endl;
#endif
        }
        else if (line == "add") {
#ifdef DEBUG
            cout << "Got add command" << endl;
            handle_add_cmd();
#endif
        }
        else if (line == "log") {
#ifdef DEBUG
            cout << "Got log command" << endl;
#endif
        }
        else if (line == "summary") {
#ifdef DEBUG
            cout << "Got summary command" << endl;
#endif
        }
        else {
#ifdef DEBUG
            cout << "Unrecognized command: " << line << endl;
#endif
        }
    }
    return 0;
}

void handle_add_cmd() {
    // TODO validate inputs
    string payer, debtor, description;
    double amount;
    cout << "Payer: ";
    cin >> payer;
    cout << "Debtor: ";
    cin >> debtor;
    cout << "Amount: ";
    cin >> amount;
    cout << "Description: ";
    cin >> description;
#ifdef DEBUG
    cout << "Payer: " << payer << endl;
    cout << "Debtor: " << debtor << endl;
    cout << "Amount: " << std::fixed << std::setprecision(2) << amount << endl;
    cout << "Description: " << description << endl;
#endif
    flush_cin();
}

void trim_trailing_whitespace(string &s) {
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
}

void flush_cin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
