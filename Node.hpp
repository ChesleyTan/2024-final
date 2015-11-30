//
//  Node.hpp
//  Simplify


#include <string>
using std::string;

class Node {
public:
    // define the member functions
    Node(string, string, double);
    
private:
    // define the member variables
    string payer_name;
    string debtor_name;
    double val;
};
