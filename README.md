# Interactive Patron Manager (C++)

A command-line patron management application written in C++. Manages a list of patrons using a **singly linked list** and supports interactive commands for adding, removing, sorting, and displaying records.

---

## Features

- Custom singly linked list implementation (no STL containers for storage)
- Interactive command-driven interface
- Insert patrons at any index (including O(1) insertion at head)
- Remove patrons based on attribute matching
- Stable sorting by multiple attributes
- Quoted string parsing for names with spaces
- Proper cleanup using RAII and destructors

---

## Build

```bash
g++ -std=c++17 -Wall -Wextra -O2 main.cpp PatronList.cpp -o patronmgr
```

---

## Run

```bash
./patronmgr
```

---

## Available Commands

```
add [index] name="<string>" age=<int> deposit=<int> drinks=<int>
remove <attr>=<value>             # attr: name | age | deposit | drinks
sort <attr>                       # attr: name | age | deposit | drinks
print
clear
help
quit
```

---

## Example Session

```
> add name="Sofia" age=54 deposit=1000 drinks=6
> add 0 name="Kate" age=27 deposit=3456 drinks=7
> print

[name: Kate; age: 27; deposit: 3456; drinks: 7]
[name: Sofia; age: 54; deposit: 1000; drinks: 6]

> sort deposit
> remove drinks=7
> print

[name: Sofia; age: 54; deposit: 1000; drinks: 6]
> quit
Bye.
```

---
