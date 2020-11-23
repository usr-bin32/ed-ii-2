#include "balanced/red_black.h"
#include "balanced/btree.h"

int main() {
    btree<int> b(1);
    b.insert(2);
    b.search(2);
    b.walk();
    b.remove(2);
}
