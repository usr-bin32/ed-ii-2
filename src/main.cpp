#include "balanced/btree.h"
#include "balanced/btree_node.h"
#include "balanced/red_black.h"
#include "hashing/hash_table.h"

#include <iostream>
#include <string>


int main()
{
    btree<std::string> b(2);
  
    int c;
    b.insert(100, "Hello", c);
    b.insert(1, "Darkness", c);
    b.insert(3, "My", c);
    b.insert(4, "Old", c);
    b.insert(0, "Friend", c);

    std::cout << *b.search(100, c) << std::endl;
    std::cout << *b.search(1, c) << std::endl;
    std::cout << *b.search(3, c) << std::endl;
    std::cout << *b.search(4, c) << std::endl;
    std::cout << *b.search(0, c) << std::endl;

    return 0;
 
}
