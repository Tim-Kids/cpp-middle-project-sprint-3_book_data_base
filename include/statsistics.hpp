#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <execution>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, unsigned short int, Comparator> authorHistogram {};
    std::for_each(cont.GetBooks().cbegin(), cont.GetBooks().cend(), [&](const auto& book) {
        authorHistogram[const_cast<Book&>(book).author]++;
    });
    return authorHistogram;
}

}  // namespace bookdb


