#include "PatronList.hpp"
#include <algorithm>
#include <tuple>
#include <iostream>

PatronList::~PatronList() { clear(); }

void PatronList::clear() {
    Node* cur = head;
    while (cur) {
        Node* nx = cur->next;
        delete cur;
        cur = nx;
    }
    head = nullptr;
    n = 0;
}

void PatronList::insert(std::size_t idx, const Patron& p) {
    if (idx > n) idx = n;
    Node* node = new Node(p);
    if (idx == 0) {
        node->next = head;
        head = node;
    } else {
        Node* prev = head;
        for (std::size_t i = 0; i + 1 < idx; ++i) prev = prev->next;
        node->next = prev->next;
        prev->next = node;
    }
    ++n;
}

void PatronList::remove_if(const std::function<bool(const Patron&)>& pred) {
    Node dummy;
    dummy.next = head;
    Node* prev = &dummy;
    while (prev->next) {
        if (pred(prev->next->data)) {
            Node* del = prev->next;
            prev->next = del->next;
            delete del;
            --n;
        } else {
            prev = prev->next;
        }
    }
    head = dummy.next;
}

std::vector<Patron> PatronList::to_vector() const {
    std::vector<Patron> v;
    v.reserve(n);
    Node* cur = head;
    while (cur) {
        v.push_back(cur->data);
        cur = cur->next;
    }
    return v;
}

void PatronList::sort_by(const std::string& attr) {
    auto v = to_vector();
    auto key = [&](const Patron& p) {
        if (attr == "name")    return std::tuple<std::string,int,int,int>(p.name, p.age, p.deposit, p.drinks);
        if (attr == "age")     return std::tuple<std::string,int,int,int>(std::string(), p.age, 0, 0);
        if (attr == "deposit") return std::tuple<std::string,int,int,int>(std::string(), p.deposit, 0, 0);
        if (attr == "drinks")  return std::tuple<std::string,int,int,int>(std::string(), p.drinks, 0, 0);
        return std::tuple<std::string,int,int,int>(std::string(), 0, 0, 0);
    };
    std::stable_sort(v.begin(), v.end(), [&](const Patron& a, const Patron& b){ return key(a) < key(b); });
    clear();
    for (const auto& p : v) insert(n, p);
}

void PatronList::print(std::ostream& os) const {
    Node* cur = head;
    bool first = true;
    while (cur) {
        if (!first) os << '\n';
        first = false;
        os << cur->data;
        cur = cur->next;
    }
}
