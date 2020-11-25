#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>

struct author {
    std::string name;
    int occurrences = 0;

    bool operator>(const author &a2);
    bool operator>=(const author &a2);

    bool operator<(const author &a2);
    bool operator<=(const author &a2);

    bool operator==(const author &a2);
    bool operator!=(const author &a2);
};

#endif
