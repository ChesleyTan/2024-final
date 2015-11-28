#include <iostream>
#include <string>
using namespace std;

const char *PROMPT = ">> ";
int main() {
    string line;
    bool isEof = false;
    while (!isEof) {
        cout << PROMPT;
        getline(cin, line);
        isEof = cin.eof();
        if (isEof) break;
        cout << "You entered: " << line << endl;
    }
    return 0;
}
