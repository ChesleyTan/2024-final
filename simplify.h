#ifndef SIMPLIFY_H_
#define SIMPLIFY_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>

#include "DatabaseHelper.h"
#include "util.h"

void simplify(std::string payer, std::string debtor, double val);
bool sort_by(const std::pair<std::string, double> &a,
        const std::pair<std::string, double> &b);
void assign(DatabaseHelper &db);
void load_debts(DatabaseHelper &db);
int load_debts_callback();

#endif /* SIMPLIFY_H_ */
