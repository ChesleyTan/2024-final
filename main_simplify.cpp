//
//  main.cpp
//  Simplify

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
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
    
    while (head_ptr < tail_ptr) {
        double val1 = debt_vector[head_ptr].second;
        double val2 = debt_vector[tail_ptr].second;
        double new_debt = 0;
        if (val1 < 0 && val2 > 0) {
            if (-val1 > val2) {
                tail_ptr--;
                debt_vector[head_ptr].second += val2;
                new_debt = val2;
            } else {
                head_ptr++;
                debt_vector[tail_ptr].second += val1;
                new_debt = -val1;
            }
            Node *data = new Node(debt_vector[head_ptr].first, debt_vector[tail_ptr].first, new_debt);
            // TODO: put data to the SQL
        } else { // the sum of all values should = 1
            cerr << "App Crashes: " << endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    // read data from the table
    // TODO
    
    
    return 0;
}
