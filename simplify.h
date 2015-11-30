#ifndef SIMPLIFY_H_
#define SIMPLIFY_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "DatabaseHelper.h"
#include "Node.h"

void simplify(string payer, string debtor, double val);
bool sort_by(const std::pair<string, double> &a,
        const std::pair<string, double> &b);
void assign(DatabaseHelper &db);
void load_debts(DatabaseHelper &db);
int load_debts_callback();

#endif /* SIMPLIFY_H_ */
