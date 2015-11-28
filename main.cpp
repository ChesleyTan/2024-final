#include "main.h"
#include "util.h"
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
        print_debug("You entered: " + line);
#endif
        if (line == "help") {
#ifdef DEBUG
            print_debug("Got help command");
#endif
            handle_help_cmd();
        }
        else if (line == "add") {
#ifdef DEBUG
            print_debug("Got add command");
#endif
            handle_add_cmd();
        }
        else if (line == "log") {
#ifdef DEBUG
            print_debug("Got log command");
#endif
        }
        else if (line == "summary") {
#ifdef DEBUG
            print_debug("Got summary command");
#endif
        }
        else {
            cerr << "Unrecognized command: " << line << endl;
        }
    }
    return 0;
}

void handle_help_cmd() {
    cout << "Commands: " << endl;
    cout << "add" << endl;
    cout << "\t- Add a new transaction" << endl;
    cout << "list" << endl;
    cout << "\t- List all transactions" << endl;
    cout << "summary" << endl;
    cout << "\t- Print a summary of debts" << endl;
}

void handle_add_cmd() {
    string payer, debtor, description;
    double amount;
    bool bad_input = false;
    do {
        if (bad_input) {
            cout << "Invalid input. Please try again." << endl;
        }
        cout << "Payer: ";
        getline(cin, payer);
        bad_input = payer == "";
    } while (bad_input);
#ifdef DEBUG
    print_debug("Payer: " + payer);
#endif
    do {
        if (bad_input) {
            cout << "Invalid input. Please try again." << endl;
        }
        cout << "Debtor: ";
        getline(cin, debtor);
        bad_input = debtor == "" || debtor == payer;
    } while (bad_input);
#ifdef DEBUG
    print_debug("Debtor: " + debtor);
#endif
    do {
        if (bad_input) {
            cout << "Invalid input. Please try again." << endl;
        }
        cout << "Amount: ";
        cin >> amount;
        bad_input = !cin.good() || amount <= 0;
        flush_cin();
    } while (bad_input);
#ifdef DEBUG
    std::ostringstream out;
    out << "Amount: " << std::fixed << std::setprecision(2) << amount;
    print_debug(out.str());
#endif
    cout << "Description: ";
    getline(cin, description);
#ifdef DEBUG
    print_debug("Description: " + description);
#endif
}

void trim_trailing_whitespace(string &s) {
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
}

void flush_cin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
