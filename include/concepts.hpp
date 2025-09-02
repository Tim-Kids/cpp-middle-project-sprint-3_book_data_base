#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"
#include "comparators.hpp"

namespace bookdb {

template<typename Cont>
concept BookContainerLike = requires(Cont c) {
    typename Cont::value_type;
    { c.begin() } -> std::same_as<typename Cont::iterator>;
    { c.end() } -> std::same_as<typename Cont::iterator>;
    { c.cbegin() } -> std::same_as<typename Cont::const_iterator>;
    { c.cend() } -> std::same_as<typename Cont::const_iterator>;
    { c.size() } -> std::same_as<size_t>;
    { c.empty() } -> std::same_as<bool>;
};

template<typename Iter>
concept BookIterator =
    std::forward_iterator<Iter> || std::random_access_iterator<Iter> || std::bidirectional_iterator<Iter>;

template<typename S, typename It>
concept BookSentinel = std::sentinel_for<S, It>;

template<typename Pred, typename T = Book>
concept BookPredicate = std::predicate<Pred, const T&>;  // Проверит, что тип Pred принимает один аргумент типа const T&
                                                         // и возвращает значение, которое можно привести к bool.

template<typename Comp, typename T = Book>
concept BookComparator =
    std::predicate<Comp, const T&, const T&>;  // Проверит, что тип Comp принимает два аргумента типа const T& и
                                               // возвращает значение, которое можно рпивести к bool.

}  // namespace bookdb
