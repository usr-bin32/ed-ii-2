#include "balanced/red_black.h"
#include "balanced/btree.h"
#include "hashing/hash_table.h"

#include <string>
#include <iostream>

int main() {
    hash_table<std::string> table(100);
    std::string s = "s";
    table.insert(5, std::move(s));
    table.insert(3, "Ana");
    std::cout << *table.lookup(3) << std::endl;
}
