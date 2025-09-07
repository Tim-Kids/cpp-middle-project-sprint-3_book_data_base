#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

using namespace std::string_view_literals;

constexpr Genre GenreFromString(std::string_view s) {
    // Ваш код здесь
    if(s == "Fiction"sv) {
        return Genre::Fiction;
    }
    else if(s == "NonFiction") {
        return Genre::NonFiction;
    }
    else if(s == "SciFi") {
        return Genre::SciFi;
    }
    else if(s == "Biography") {
        return Genre::Biography;
    }
    else if(s == "Mystery") {
        return Genre::Mystery;
    }
    else {
        return Genre::Unknown;
    }
}

constexpr std::string StringFromGenre(Genre g) {
    // Ваш код здесь
    switch(g) {
        case Genre::Fiction:
            return "Fiction";
        case Genre::Mystery:
            return "Mystery";
        case Genre::NonFiction:
            return "NonFiction";
        case Genre::SciFi:
            return "SciFi";
        case Genre::Biography:
            return "Biography";
        case Genre::Unknown:
            return "Unknown";
        default:
            throw std::logic_error {"Unsupported bookdb::Genre"};
    }
}

struct Book {
    std::string title;
    std::string_view author;  // Экономия памяти - ссылаемся на оригиналы строк в AuthorContainer.

    int year;
    Genre genre;
    double rating;
    int read_count;

    explicit constexpr Book(std::string title, std::string_view author, int year, Genre genre, double rating,
                            int read_count):
        title(std::move(title)),
        author(author),
        year(year),
        genre(genre),
        rating(rating),
        read_count(read_count) {}

    explicit constexpr Book(std::string title, std::string_view author, int year, std::string_view genre, double rating,
                            int read_count):
        title(std::move(title)),
        author(author),
        year(year),
        genre(GenreFromString(genre)),
        rating(rating),
        read_count(read_count) {}
};
}  // namespace bookdb

namespace std {
template<> struct formatter<bookdb::Genre, char> {
    template<typename FormatContext> auto format(const bookdb::Genre g, FormatContext& fc) const {
        std::string genre_str;

        using bookdb::Genre;
        switch(g) {
            case Genre::Fiction:
                genre_str = "Fiction";
                break;
            case Genre::Mystery:
                genre_str = "Mystery";
                break;
            case Genre::NonFiction:
                genre_str = "NonFiction";
                break;
            case Genre::SciFi:
                genre_str = "SciFi";
                break;
            case Genre::Biography:
                genre_str = "Biography";
                break;
            case Genre::Unknown:
                genre_str = "Unknown";
                break;
            default:
                throw logic_error {"Unsupported bookdb::Genre"};
        }

        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
};

template<> struct formatter<bookdb::Book, char> {
    template<typename FormatContext> auto format(const bookdb::Book& book, FormatContext& fc) const {
        return format_to(fc.out(), "{}", book.title);
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
};

}  // namespace std
