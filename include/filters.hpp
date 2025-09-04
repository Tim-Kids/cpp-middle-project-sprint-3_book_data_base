#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

using BookFilter = std::function<bool(const bookdb::Book& book)>;

BookFilter YearBetween(short int yearBegin, short int yearEnd) {
    return [yearBegin, yearEnd](const Book& book) noexcept {
        return book.year >= std::min(yearBegin, yearEnd) && book.year <= std::max(yearBegin, yearEnd);
    };
}
BookFilter RatingAbove(double rating) {
    return [rating](const Book& book) noexcept {
        return book.rating > rating;
    };
}
BookFilter GenreIs(Genre genre) {
    return [genre](const Book& book) noexcept {
        return book.genre == genre;
    };
}

template<BookPredicate... Predicate> auto all_of(Predicate&&... pred) {
    return [&](const Book& book) noexcept {
        return (std::forward<Predicate>(pred)(book) && ...);
    };
}

template<BookPredicate... Predicate> auto any_of(Predicate... pred) {
    return [&](const Book& book) noexcept {
        return (std::forward<Predicate>(pred)(book) || ...);
    };
}

template<BookIterator It, typename Predicate> auto filterBooks(It begin, It end, Predicate pred) {
    std::vector<std::reference_wrapper<const Book>> filtered {};
    std::copy_if(begin, end, std::back_inserter(filtered), pred);
    return filtered;
}
}  // namespace bookdb
