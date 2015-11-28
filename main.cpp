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
        is_eof = cin.eof();
        if (is_eof) break;
        trim_trailing_whitespace(line);
        cout << "You entered: " << line << endl;
        if (line == "help") {
            cout << "Got help command" << endl;
        }
        else if (line == "add") {
            cout << "Got add command" << endl;
        }
        else if (line == "log") {
            cout << "Got log command" << endl;
        }
        else if (line == "summary") {
            cout << "Got summary command" << endl;
        }
        else {
            cout << "Unrecognized command: " << line << endl;
        }
    }
    return 0;
}

void trim_trailing_whitespace(string &s) {
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
}
