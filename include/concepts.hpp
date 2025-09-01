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

template<typename S, typename I>
concept BookSentinel = true;

template<typename P>
concept BookPredicate = true;

template<typename C>
concept BookComparator = std::same_as<C, comp::LessByYear> || std::same_as<C, comp::LessByAuthor> ||
                         std::same_as<C, comp::LessByTitle> || std::same_as<C, comp::MoreByRating>;

}  // namespace bookdb
