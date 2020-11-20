#include "author.h"

bool author::operator>(const author &a2) {
    return this->occurrences > a2.occurrences;
}

bool author::operator>=(const author &a2) {
    return this->occurrences >= a2.occurrences;
}

bool author::operator<(const author &a2) {
    return this->occurrences < a2.occurrences;
}

bool author::operator<=(const author &a2) {
    return this->occurrences <= a2.occurrences;
}

bool author::operator==(const author &a2) {
    return this->occurrences == a2.occurrences;
}

bool author::operator!=(const author &a2) {
    return this->occurrences != a2.occurrences;
}
