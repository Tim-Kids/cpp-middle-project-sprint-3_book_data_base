#pragma once

#include <print>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <boost/container/flat_set.hpp>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template<BookContainerLike BookContainer = std::vector<Book>>
// template <BookContainerLike BookContainer = boost::container::flat_set<Book>>
class BookDatabase {
    public:
    // Type aliases
    using value_type      = BookContainer::value_type;
    using reference       = BookContainer::value_type&;
    using const_reference = const BookContainer::value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator        = BookContainer::iterator;
    using const_iterator  = BookContainer::const_iterator;
    using size_type       = std::size_t;

    //    using AuthorContainer = BookContainer /* Ваш код здесь */;
    using AuthorContainer = std::unordered_set<std::string>;

    // API
    BookDatabase() = default;
    explicit BookDatabase(std::initializer_list<Book> lst) {
        std::for_each(lst.begin(), lst.end(), [&](auto&& book) {
            authors_.emplace(book.author);
            books_.emplace_back(std::move(book));
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

    template<typename T = Book> void PushBack(T&& book) {
        books_.push_back(std::forward<T>(book));
        authors_.emplace(books_.back().author);
    }

    template<typename... Args> void EmplaceBack(Args... args) {
        books_.emplace_back(args...);
    }
    const BookContainer& getBooks() const {
        return books_;
    }
    const AuthorContainer& getAuthors() const {
        return authors_;
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template<> struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template<typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>>& db, FormatContext& fc) const {
        /*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        */
        return fc.out();
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
