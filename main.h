#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>
#include "DatabaseHelper.h"
#include "util.h"

void trim_trailing_whitespace(std::string &s);
void handle_help_cmd();
void handle_add_cmd();
void handle_log_cmd();
void handle_summary_cmd();
void handle_analytics_cmd();
void flush_cin();

#endif /* MAIN_H_ */
