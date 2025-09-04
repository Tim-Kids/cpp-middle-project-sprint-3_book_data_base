#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

using BookFilter = std::function<bool(const bookdb::Book& book)>;

BookFilter YearBetween = [](const bookdb::Book& book){ return true;};
BookFilter RatingAbove = [](const bookdb::Book& book){ return true;};
BookFilter GenreIs = [](const bookdb::Book& book){ return true;};

template<typename... Predicate>
auto all_of(Predicate... pred) {
    return (pred() && ...);
}

template<typename... Predicate>
auto any_of(Predicate... pred) {
    return (pred() || ...);
}


}  // namespace bookdb