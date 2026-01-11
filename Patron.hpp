#ifndef PATRON_HPP
#define PATRON_HPP

#include <string>
#include <ostream>

// Simple data record for one bar patron.
struct Patron {
    std::string name;
    int age = 0;
    int deposit = 0;
    int drinks = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Patron& p) {
    os << "[name: " << p.name
       << "; age: " << p.age
       << "; deposit: " << p.deposit
       << "; drinks: " << p.drinks
       << "]";
    return os;
}

#endif // PATRON_HPP
