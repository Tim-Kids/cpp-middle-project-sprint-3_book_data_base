#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByYear {
    bool operator()(const Book& lhs, const Book& rhs) {
        if(lhs.year == rhs.year) {
            return lhs.title < rhs.title;
        }
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
        if(lhs.rating == rhs.rating) {
            return lhs.title < rhs.title;
        }
        return lhs.rating > rhs.rating;
    }
};

struct MoreByPopularity {
    bool operator()(const Book& lhs, const Book& rhs) {
        if(lhs.read_count == rhs.read_count) {
            return lhs.title < rhs.title;
        }
        return lhs.read_count > rhs.read_count;
    }
};

}  // namespace bookdb::comp
