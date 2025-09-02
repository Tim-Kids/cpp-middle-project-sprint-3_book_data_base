#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <execution>
#include <flat_map>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template<BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T>& cont, Comparator comp = {}) {
    std::flat_map<std::string_view, unsigned short int, Comparator> authorHistogram {};
    std::for_each(cont.GetBooks().cbegin(), cont.GetBooks().cend(),
                  [&](const auto& book) { authorHistogram[const_cast<Book&>(book).author]++; });
    return authorHistogram;
}

template <typename It>
requires BookIterator<It>
auto calculateGenreRatings(It begin, It end) {
    std::flat_map<Genre, std::pair<double, unsigned short>> genreToRatingAndCount{};
    std::for_each(begin, end, [&](const auto& book) {
        genreToRatingAndCount[const_cast<Book&>(book).genre].first += book.rating;
        genreToRatingAndCount[const_cast<Book&>(book).genre].second++;
    });
    std::flat_map<Genre, double> averages{};
    for(const auto& [genre, ratingToCount] : genreToRatingAndCount) {
        averages[const_cast<Genre&>(genre)] = ratingToCount.first / ratingToCount.second;
    }
    return averages;
}

}  // namespace bookdb
