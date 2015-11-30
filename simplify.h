#ifndef SIMPLIFY_H_
#define SIMPLIFY_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "Node.h"

void simplify(string payer, string debtor, double val);
bool sort_by(const std::pair<string, double> &a,
        const std::pair<string, double> &b);
void assign();

#endif /* SIMPLIFY_H_ */
