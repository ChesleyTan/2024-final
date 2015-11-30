//
//  Node.cpp
//  Simplify


#include "Node.hpp"

// constructor, create instance of Employee and set first, last name and salary
Node::Node(string payer_name, string debtor_name, double val) {
    this->payer_name = payer_name;
    this->debtor_name = debtor_name;
    this->val = val;
}

string Node::getPayerName() {
    return payer_name;
}

string Node::getDebtorName() {
    return debtor_name;
}

double Node::getVal() {
    return val;
}
