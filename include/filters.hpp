#pragma once

#include <algorithm>
#include <functional>
#include <type_traits>
#include <iterator>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

using BookFilter = std::function<bool(const bookdb::Book&)>;

[[nodiscard]] inline BookFilter YearBetween(short int yearBegin, short int yearEnd) {
    return [yearBegin, yearEnd](const Book& book) noexcept {
        return book.year >= std::min(yearBegin, yearEnd) && book.year <= std::max(yearBegin, yearEnd);
    };
}

[[nodiscard]] inline BookFilter RatingAbove(double rating) {
    return [rating](const Book& book) noexcept {
        return book.rating > rating;
    };
}

[[nodiscard]] inline BookFilter GenreIs(Genre genre) {
    return [genre](const Book& book) noexcept {
        return book.genre == genre;
    };
}

template<BookPredicate... Predicate> [[nodiscard]] auto all_of(Predicate&&... pred) {
    // Передаем предикаты корректно, через std::forward<>. Иначе можно получить dangling reference, если передать как
    // [&].
    return [... p = std::decay_t<Predicate>(std::forward<Predicate>(pred))](const Book& book) noexcept {
        return (p(book) && ...);
    };
}

template<BookPredicate... Predicate> [[nodiscard]] auto any_of(Predicate&&... pred) {
    // Передаем предикаты корректно, через std::forward<>. Иначе можно получить dangling reference, если передать как
    // [&].
    return [... p = std::decay_t<Predicate>(std::forward<Predicate>(pred))](const Book& book) noexcept {
        return (p(book) || ...);
    };
}

template<BookIterator It, BookPredicate... Predicate>
    requires(sizeof...(Predicate) > 0)  // Гарантируем вызов хотя бы с одним предикатом.
auto filterBooks(It begin, It end, Predicate&&... pred) {
    return filterBooks(begin, end, all_of(std::forward<Predicate>(pred)...));
}

template<BookIterator It, BookPredicate Predicate> auto filterBooks(It begin, It end, Predicate&& pred) {
    using Ref = std::reference_wrapper<const Book>;
    std::vector<Ref> refs;
    refs.reserve(std::distance(begin, end));
    std::for_each(begin, end, [&refs, p = std::forward<Predicate>(pred)](const Book& book) {
        if(p(book)) {
            refs.emplace_back(book);
        }
    });
    return refs;
}
}  // namespace bookdb
