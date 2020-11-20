#include <string>

struct author {
    std::string name;
    int occurrences = 0;

    // operações de comparação para os algoritmos de ordenação

    bool operator>(const author &a2);
    bool operator>=(const author &a2);

    bool operator<(const author &a2);
    bool operator<=(const author &a2);

    bool operator==(const author &a2);
    bool operator!=(const author &a2);
};