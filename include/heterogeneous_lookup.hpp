#pragma once

#include <algorithm>
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(const Book& lhs, std::string_view title) const {
        return lhs.title < std::string {title};
    }
    bool operator()(const Book& lhs, const std::string& author) const {
        return lhs.author < author;
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(const Book& lhs, std::string_view title) const {
        return lhs.title == std::string {title};
    }
    bool operator()(const Book& lhs, const std::string& author) const {
        return lhs.author == author;
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    size_t operator()(const Book& book) const {
        return std::hash<int>()(book.rating) + 37 * std::hash<int>()(book.year) +
               37 * 37 * std::hash<int>()(book.read_count);
    }
};

}  // namespace bookdb
