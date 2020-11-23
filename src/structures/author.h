struct author {
    int id;
    int occurrences = 0;

    bool operator>(const author &a2);
    bool operator>=(const author &a2);

    bool operator<(const author &a2);
    bool operator<=(const author &a2);

    bool operator==(const author &a2);
    bool operator!=(const author &a2);
};
