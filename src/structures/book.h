#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

struct book {
    std::vector<int> authors;
    int bestsellers_rank;
    std::vector<int> categories;
    std::string edition;
    int id;
    std::string isbn10;
    std::string isbn13;
    float rating;
    long rating_count;
    std::string title;
};

#endif
