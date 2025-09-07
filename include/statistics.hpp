#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <string_view>
#include <execution>
#include <flat_map>
#include <numeric>

#include "book_database.hpp"

namespace bookdb {

template<BookContainerLike Cont, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const Cont& cont, Comparator comp = {}) {
    std::flat_map<std::string_view, unsigned short int, Comparator> authorHistogram {};
    std::for_each(cont.begin(), cont.end(), [&](auto& book) { authorHistogram[const_cast<Book&>(book).author]++; });
    return authorHistogram;
}

template<BookIterator It> auto calculateGenreRatings(It begin, It end) {
    std::flat_map<Genre, std::pair<double, unsigned short>> genreToRatingAndCount {};
    std::for_each(begin, end, [&](const auto& book) {
        if(auto it = genreToRatingAndCount.find(const_cast<Book&>(book).genre); it != genreToRatingAndCount.end()) {
            it->second.first += book.rating;
            it->second.second++;
        }
        else {  // Используем способ создания составного объекта прям в мапе. Исключаем накладные расходы на
                // конструкторы/операторы копирвоания/перемещения.
            genreToRatingAndCount.emplace(std::piecewise_construct,
                                          std::forward_as_tuple(const_cast<Book&>(book).genre),
                                          std::forward_as_tuple(book.rating, 1));
        }
    });
    std::flat_map<Genre, double> averages {};
    for(const auto& [genre, ratingToCount]: genreToRatingAndCount) {
        averages.emplace(std::piecewise_construct, std::forward_as_tuple(const_cast<Genre&>(genre)),
                         std::forward_as_tuple(ratingToCount.first / ratingToCount.second));
    }
    return averages;
}

template<BookContainerLike Cont> auto calculateAverageRating(const Cont& cont) {
    if(cont.empty()) {
        return double {};
    }
    auto totalRating = std::accumulate(cont.begin(), cont.end(), double {},
                                       [](double sum, const Book& book) { return sum + book.rating; });
    return totalRating / static_cast<double>(cont.size());
}

template<BookContainerLike Cont> auto sampleRandomBooks(const Cont& cont, size_t num) {
    using Ref = std::reference_wrapper<const Book>;
    std::vector<Ref> refs;
    refs.reserve(cont.size());
    for(const Book& book: const_cast<Cont&>(cont)) {
        refs.emplace_back(book);
    }
    std::vector<Ref> sample {};
    sample.reserve(std::min(num, refs.size()));
    if(num == 0 || refs.empty()) {
        return sample;
    }
    std::sample(refs.begin(), refs.end(), std::back_inserter(sample), num, std::mt19937 {std::random_device {}()});
    return sample;
}

template<BookContainerLike Cont, BookComparator Comparator>
auto getTopNBy(const Cont& cont, size_t num, Comparator comp = {}) {
    using Ref = std::reference_wrapper<const Book>;
    std::vector<Ref> topN;
    topN.reserve(cont.size());
    for(const auto& book: const_cast<Cont&>(cont)) {
        topN.emplace_back(book);
    }
    if(num == 0 || topN.empty()) {
        return std::vector<Ref> {};
    }
    if(topN.size() > num) {
        auto nth = std::next(topN.begin(), num);
        std::nth_element(topN.begin(), nth, topN.end(),
                         [&](const Ref& a, const Ref& b) { return comp(a.get(), b.get()); });
        topN.erase(nth, topN.end());
    }
    std::sort(topN.begin(), topN.end(), [&](const Ref& a, const Ref& b) { return comp(a.get(), b.get()); });
    return topN;
}
}  // namespace bookdb
