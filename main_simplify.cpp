#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Node.hpp"

using std::string;
using namespace std;

std::unordered_map<string, double> debtor_map;

void simplify(string payer, string debtor, double val) {
    if (debtor_map.find(payer) != debtor_map.end()) {
        debtor_map.at(payer) -= val;
    } else {
        debtor_map.insert({payer, -val});
    }
    
    if (debtor_map.find(debtor) != debtor_map.end()) {
        debtor_map.at(debtor) += val;
    } else {
        debtor_map.insert({debtor, val});
    }
    
}

bool sort_by(const pair<string, double>& a ,const pair<string, double>& b) {
    return a.second < b.second;
}

// distribute the debts between app users
void assign() {
    // sort map by value: make use of vector
    vector<pair<string,double>> debt_vector;
    
    for (auto& x: debtor_map) {
        debt_vector.push_back(make_pair(x.first, x.second));
    }
    
    int head_ptr = 0;
    int tail_ptr = (int)debt_vector.size() - 1;
    
    // get sorted vector, ascending
    sort(debt_vector.begin(), debt_vector.end(), sort_by);
    
    for (auto& x: debt_vector) {
        cout << x.first << ", " << x.second << endl;
    }
    
    cout << "head: " << head_ptr << endl;
    cout << "tail: " << tail_ptr << endl;
    cout << "transction after simplifying: " << endl;
    
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
                data = new Node(debt_vector[head_ptr].first, debt_vector[tail_ptr].first, new_debt);
                tail_ptr--;
                debt_vector[head_ptr].second += val2;
            } else {
                new_debt = -val1;
                data = new Node(debt_vector[head_ptr].first, debt_vector[tail_ptr].first, new_debt);
                head_ptr++;
                debt_vector[tail_ptr].second += val1;
            }
            // TODO: put data to the SQL
            cout << data->getPayerName() << " owes " << data->getDebtorName() << " $" << data->getVal() << endl;
        } else { // the sum of all values should = 1
            cerr << "App Crashes: " << endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    // read data from the table
    // TODO
    
    cout << " ------ test case from user ------ " << endl;
    simplify("A", "B", 5.0);
    simplify("B", "C", 10.0);
    simplify("C", "A", 3.0);
    simplify("C", "D", 8.0);
    simplify("C", "B", 2.0);
    simplify("E", "A", 5.0);
    simplify("B", "E", 2.0);
    simplify("A", "E", 1.0);
    
    for (auto& x: debtor_map) {
        cout << x.first << ", " << x.second << endl;
    }
    cout << " ------ " << endl;
    assign();
    
    return 0;
}
