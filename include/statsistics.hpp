#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <execution>
#include <flat_map>
#include <numeric>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template<BookContainerLike Cont, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const Cont& cont, Comparator comp = {}) {
    std::flat_map<std::string_view, unsigned short int, Comparator> authorHistogram {};
    std::for_each(cont.cbegin(), cont.cend(),
                  [&](const auto& book) { authorHistogram[const_cast<Book&>(book).author]++; });
    return authorHistogram;
}

template<typename It>
    requires BookIterator<It>
auto calculateGenreRatings(It begin, It end) {
    std::flat_map<Genre, std::pair<double, unsigned short>> genreToRatingAndCount {};
    std::for_each(begin, end, [&](const auto& book) {
        genreToRatingAndCount[const_cast<Book&>(book).genre].first += book.rating;
        genreToRatingAndCount[const_cast<Book&>(book).genre].second++;
    });
    std::flat_map<Genre, double> averages {};
    for(const auto& [genre, ratingToCount]: genreToRatingAndCount) {
        averages[const_cast<Genre&>(genre)] = ratingToCount.first / ratingToCount.second;
    }
    return averages;
}

template<BookContainerLike Cont> auto calculateAverageRating(const Cont& cont) {
    if(cont.empty()) {
        return double {};
    }
    auto totalRating = std::accumulate(cont.cbegin(), cont.cend(), double {},
                                       [](double sum, const Book& book) { return sum + book.rating; });
    return totalRating / static_cast<double>(cont.size());
}

template<BookContainerLike Cont>
auto sampleRandomBooks(Cont& cont,
                       size_t num) {  // Если передать const Cont&, то std::sample не скомпилируется.
                                      // Вероятно "под капотом" алгоритм разыменовывает переданный ему итератор.
    std::vector<std::reference_wrapper<const Book>> sample;
    sample.reserve(num);
    std::sample(cont.begin(), cont.end(), std::back_inserter(sample), num, std::mt19937 {std::random_device {}()});
    return sample;
}

template<BookContainerLike Cont, typename Comparator>
auto getTopNBy(Cont& cont, size_t num, Comparator comp = {}) {
    std::sort(cont.begin(), cont.end(), comp);
    if(num >= cont.size()) {
        return std::vector<std::reference_wrapper<const Book>> {cont.cbegin(), cont.cend()};
    }
    return std::vector<std::reference_wrapper<const Book>> {cont.cbegin(), std::next(cont.cbegin(), num)};
}
}  // namespace bookdb
