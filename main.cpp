#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "PatronList.hpp"

using std::string;
using std::vector;

static inline string ltrim(string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    return s;
}
static inline string rtrim(string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
    return s;
}
static inline string trim(string s) { return rtrim(ltrim(std::move(s))); }
static inline string lower(string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// Split a command line into distinct tokens
// EX: add name="Sofia Bee" age=54 -> ["add", "name=\"Sofia Bee\"", "age=54"]
static vector<string> split_tokens(const string& line) {
    vector<string> out;
    string cur;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '"') {
            in_quotes = !in_quotes;
            cur.push_back(ch);
        } else if (std::isspace(static_cast<unsigned char>(ch)) && !in_quotes) {
            if (!cur.empty()) { out.push_back(cur); cur.clear(); }
        } else {
            cur.push_back(ch);
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

static void print_help() {
    std::cout << "Commands:\n"
              << "  add [index] name=<str> age=<int> deposit=<int> drinks=<int>\n"
              << "  remove <attr>=<value>           (attr: name|age|deposit|drinks)\n"
              << "  sort <attr>                     (attr: name|age|deposit|drinks)\n"
              << "  print\n"
              << "  clear\n"
              << "  help\n"
              << "  quit\n";
}

static bool parse_kv(const string& tok, string& key, string& val) {
    auto pos = tok.find('=');
    if (pos == string::npos) return false;
    key = lower(trim(tok.substr(0, pos)));
    val = trim(tok.substr(pos + 1));
    if (!val.empty() && val.front()=='"' && val.back()=='"' && val.size()>=2) {
        val = val.substr(1, val.size()-2);
    }
    return !key.empty() && !val.empty();
}

static bool is_uint(const string& s) {
    if (s.empty()) return false;
    return std::all_of(s.begin(), s.end(), [](unsigned char ch){ return std::isdigit(ch); });
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    PatronList list;
    std::cout << "Welcome to the Bar Patron Manager. Type 'help' for commands.\n";

    string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        line = trim(line);
        if (line.empty()) continue;

        auto toks = split_tokens(line);
        if (toks.empty()) continue;

        string cmd = lower(toks[0]);

        if (cmd == "help") { print_help(); continue; }
        if (cmd == "quit" || cmd == "exit") break;
        if (cmd == "print") { list.print(std::cout); std::cout << "\n"; continue; }
        if (cmd == "clear") { list.clear(); std::cout << "OK (cleared)\n"; continue; }

        if (cmd == "sort") {
            if (toks.size() < 2) { std::cout << "Usage: sort <name|age|deposit|drinks>\n"; continue; }
            string attr = lower(toks[1]);
            if (attr!="name" && attr!="age" && attr!="deposit" && attr!="drinks") {
                std::cout << "Unknown attribute: " << attr << "\n";
                continue;
            }
            list.sort_by(attr);
            std::cout << "OK (sorted by " << attr << ")\n";
            continue;
        }

        if (cmd == "remove") {
            if (toks.size() < 2) { std::cout << "Usage: remove <attr>=<value>\n"; continue; }
            string key, val;
            if (!parse_kv(toks[1], key, val)) {
                std::cout << "Usage: remove <attr>=<value>\n"; continue;
            }
            list.remove_if([&](const Patron& p){
                if (key=="name") return p.name == val;
                if (key=="age") return std::to_string(p.age) == val;
                if (key=="deposit") return std::to_string(p.deposit) == val;
                if (key=="drinks") return std::to_string(p.drinks) == val;
                return false;
            });
            std::cout << "OK (removed matching)\n";
            continue;
        }

        if (cmd == "add") {
            size_t tokIdx = 1;
            size_t index = list.size();
            if (tokIdx < toks.size() && is_uint(toks[tokIdx])) {
                try {
                    index = static_cast<size_t>(std::stoull(toks[tokIdx]));
                } catch (...) {
                    std::cout << "Invalid index.\n"; 
                    continue;
                }
                ++tokIdx;
            }

            Patron p;
            for (; tokIdx < toks.size(); ++tokIdx) {
                string key, val;
                if (!parse_kv(toks[tokIdx], key, val)) continue;
                try {
                    if (key=="name") p.name = val;
                    else if (key=="age") p.age = std::stoi(val);
                    else if (key=="deposit") p.deposit = std::stoi(val);
                    else if (key=="drinks") p.drinks = std::stoi(val);
                } catch (...) {
                    std::cout << "Invalid number for '" << key << "'.\n";
                    continue;
                }
            }

            if (p.name.empty()) {
                std::cout << "Missing name. Usage: add [index] name=\"...\" age=.. deposit=.. drinks=..\n";
                continue;
            }
            list.insert(index, p);
            std::cout << "OK (added at " << index << ")\n";
            continue;
        }

        std::cout << "Unknown command. Type 'help'.\n";
    }

    std::cout << "Bye.\n";
    return 0;
}
