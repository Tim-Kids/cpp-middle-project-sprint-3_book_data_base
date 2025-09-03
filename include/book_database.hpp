#pragma once

#include <print>
#include <string>
#include <vector>
#include <deque>
#include <flat_map>
#include <string_view>
#include <unordered_set>
#include <boost/container/flat_set.hpp>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template<BookContainerLike BookContainer = std::deque<Book>> class BookDatabase {
    public:
    using value_type      = typename BookContainer::value_type;
    using reference       = typename BookContainer::value_type&;
    using const_reference = const typename BookContainer::value_type&;
    using difference_type = typename std::ptrdiff_t;
    using iterator        = typename BookContainer::iterator;
    using const_iterator  = typename BookContainer::const_iterator;
    using size_type       = std::size_t;

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash>;

    BookDatabase() = default;

    template<typename T> constexpr BookDatabase(std::initializer_list<T> books) {
        std::for_each(books.begin(), books.end(), [&](auto&& book) {
            auto [it, _] = authors_.emplace(books.author);
            book.author  = *it;
            books_.push_back(std::forward<Book>(book));
        });
    }

    iterator begin() {
        return books_.begin();
    }

    const_iterator cbegin() const {
        return books_.cbegin();
    }

    iterator end() {
        return books_.end();
    }

    const_iterator cend() const {
        return books_.cend();
    }

    size_type size() const {
        return books_.size();
    }

    bool empty() const {
        return books_.empty();
    }

    reference operator[](size_t id) {
        return books_[id];
    }

    const_reference operator[](size_t id) const {
        return books_.at(id);
    }

    template<typename T = Book>
    void PushBack(T&& book) {
        auto [it, _] = authors_.emplace(book.author);
        book.author = *it;
        books_.push_back(std::forward<T>(book));
    }

    void EmplaceBack(std::string_view title, std::string_view author, int year, Genre genre, double rating, int pages) {
        auto [it, _] = authors_.emplace(author);
        books_.emplace_back(std::string {title}, *it, year, genre, rating, pages);
    }

    std::span<const Book> GetBooks() const {
        return books_;
    }

    auto& GetAuthors() const {
        return authors_;
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    private:
    BookContainer books_;
    AuthorContainer authors_; // Хранит оригиналы строк.
};

} // namespace bookdb

namespace std {
template<> struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template<typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>>& db, FormatContext& fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());
        format_to(fc.out(), "Books:\n");
        for(const bookdb::Book& book: db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }
        format_to(fc.out(), "Authors:\n");
        for(const auto& author: db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin(); // Просто игнорируем пользовательский формат
    }
};

template<> struct formatter<std::flat_map<std::string_view, unsigned short int, bookdb::TransparentStringLess>> {
    template<typename FormatContext>
    auto format(const std::flat_map<std::string_view, unsigned short int, bookdb::TransparentStringLess>& histogram,
                FormatContext& fc) const {
        for(const auto& [author, freq]: histogram) {
            format_to(fc.out(), "{} -> {}\n", author, freq);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin(); // Просто игнорируем пользовательский формат
    }
};

template<> struct formatter<std::flat_map<bookdb::Genre, double>> {
    template<typename FormatContext>
    auto format(const std::flat_map<bookdb::Genre, double>& genreRating, FormatContext& fc) const {
        for(const auto& [genre, rating]: genreRating) {
            format_to(fc.out(), "{} -> {}\n", bookdb::StringFromGenre(genre), static_cast<float>(rating));
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin(); // Просто игнорируем пользовательский формат
    }
};

} // namespace std
