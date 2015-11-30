#include "simplify.h"
using std::string;
using namespace std;

std::unordered_map<string, double> debtor_map;

// add debt to debtor_map
void simplify(string payer, string debtor, double val) {
    if (debtor_map.find(payer) != debtor_map.end()) {
        debtor_map.at(payer) -= val;
    }
    else {
        debtor_map.insert({payer, -val});
    }

    if (debtor_map.find(debtor) != debtor_map.end()) {
        debtor_map.at(debtor) += val;
    }
    else {
        debtor_map.insert({debtor, val});
    }
}

bool sort_by(const pair<string, double> &a, const pair<string, double> &b) {
    return a.second < b.second;
}

// distribute the debts between debtors
void assign(DatabaseHelper &db) {
    // sort map by value: make use of vector
    vector<pair<string, double>> debt_vector;

    for (auto &x : debtor_map) {
        debt_vector.push_back(make_pair(x.first, x.second));
    }

    int head_ptr = 0;
    int tail_ptr = (int) debt_vector.size() - 1;

    // sort debt_vector in ascending order
    sort(debt_vector.begin(), debt_vector.end(), sort_by);

    for (auto &x : debt_vector) {
        cout << x.first << ", " << x.second << endl;
    }

    cout << "head: " << head_ptr << endl;
    cout << "tail: " << tail_ptr << endl;
    cout << "transaction after simplification: " << endl;

    while (head_ptr < tail_ptr) {
        double val1 = debt_vector[head_ptr].second;
        double val2 = debt_vector[tail_ptr].second;
        double new_debt = 0;
        Node *data;
        if (val1 == 0) {
            head_ptr++;
            continue;
        }
        if (val2 == 0) {
            tail_ptr--;
            continue;
        }
        if (val1 < 0 && val2 > 0) {
            if (-val1 > val2) {
                new_debt = val2;
                data = new Node(debt_vector[tail_ptr].first,
                        debt_vector[head_ptr].first, new_debt);
                tail_ptr--;
                debt_vector[head_ptr].second += val2;
            }
            else {
                new_debt = -val1;
                data = new Node(debt_vector[tail_ptr].first,
                        debt_vector[head_ptr].first, new_debt);
                head_ptr++;
                debt_vector[tail_ptr].second += val1;
            }
            db.add_debt(data->getPayerName(), data->getDebtorName(),
                    data->getVal());
            delete data;
        }
        else { // no more debts need to be resolved
            return;
        }
    }
}

int load_debts_callback(void *data, int argc, char **argv,
        char **colNames) {
    double amount = atof(argv[2]);
    simplify(argv[0], argv[1], amount);
    return 0;
}

void load_debts(DatabaseHelper &db) {
    char *err_msg;
    const char *query = "SELECT Payer, Debtor, Amount "
                        "FROM Debts";
    int ret = sqlite3_exec(db.get_conn(), query, load_debts_callback, NULL,
            &err_msg);
    if (ret != SQLITE_OK) {
        cerr << "SQL Error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}
