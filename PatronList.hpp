#ifndef PATRONLIST_HPP
#define PATRONLIST_HPP

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <vector>
#include "Patron.hpp"

struct Node {
    Patron data;
    Node*  next;
    Node() : data(), next(nullptr) {}
    explicit Node(const Patron& p) : data(p), next(nullptr) {}
};

class PatronList {
public:
    PatronList() = default;
    ~PatronList();

    PatronList(const PatronList&) = delete;
    PatronList& operator=(const PatronList&) = delete;

    void clear();
    void insert(std::size_t idx, const Patron& p);
    void remove_if(const std::function<bool(const Patron&)>& pred);
    std::vector<Patron> to_vector() const;
    void sort_by(const std::string& attr);
    void print(std::ostream& os) const;

    std::size_t size() const { return n; }

private:
    Node* head = nullptr;
    std::size_t n = 0;
};

#endif // PATRONLIST_HPP
