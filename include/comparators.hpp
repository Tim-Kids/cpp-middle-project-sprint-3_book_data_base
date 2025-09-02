#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByYear {
    bool operator()(const Book& lhs, const Book& rhs) {
        return lhs.year < rhs.year;
    }
};
struct LessByTitle {
    bool operator()(const Book& lhs, const Book& rhs) {
        return lhs.title < rhs.title;
    }
};
struct LessByAuthor {
    bool operator()(const Book& lhs, const Book& rhs) {
        return lhs.author < rhs.author;
    }
};
struct MoreByRating {
    bool operator()(const Book& lhs, const Book& rhs) {
        return lhs.rating > rhs.rating;
    }
};
struct LessByPopularity {
    bool operator()(const Book& lhs, const Book& rhs) {
        return lhs.read_count > rhs.read_count;
    }
};

}  // namespace bookdb::comp