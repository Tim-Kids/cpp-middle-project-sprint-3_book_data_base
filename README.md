# BookDB - A C++ Book Database Library

This C++ library provides a flexible and efficient in-memory database for managing collections of books. It offers features for storing book metadata, searching, filtering, and performing statistical analysis, with a focus on type safety and performance.

## Table of Contents

- [Features](#features)
- [Dependencies](#dependencies)
- [Building](#building)
- [Usage](#usage)
- [API Overview](#api-overview)
  - [Core Components](#core-components)
  - [Searching and Filtering](#searching-and-filtering)
  - [Statistics](#statistics)
- [Examples](#examples)

## Features

*   **Generic Container Backend**: The core `BookDatabase` class is a template, allowing you to use different standard containers (e.g., `std::vector`, `std::deque`) to store the books, adapting to your performance needs (e.g., fast insertion vs. fast random access).
*   **Efficient Author Lookup**: Uses `std::pmr::unordered_set` with custom transparent hash/equality functors (`TransparentStringHash`, `TransparentStringEqual`) for fast, string-view-based lookup of authors without unnecessary string copies.
*   **Type-Safe Genre Handling**: Employs a strongly typed `enum class Genre` for robust categorization.
*   **Generic Iteration Concepts**: Defines C++20 concepts (`BookContainerLike`, `BookIterator`, `BookSentinel`) to constrain generic algorithms, ensuring they work correctly with book collections.
*   **Powerful Filtering**: Provides a set of composable predicate factories (`YearBetween`, `RatingAbove`, `GenreIs`) and combinators (`all_of`, `any_of`) to create complex filters. The `filterBooks` algorithm applies these filters and returns `std::vector<std::reference_wrapper<const Book>>`.
*   **Statistical Analysis**: Includes functions for common book database statistics:
    *   `calculateAverageRating`: Calculates the mean rating.
    *   `buildAuthorHistogramFlat`: Counts books per author using `std::flat_map`.
    *   `calculateGenreRatings`: Calculates average rating per genre.
    *   `sampleRandomBooks`: Selects a random sample of books.
    *   `getTopNBy`: Finds the top N books based on a custom comparator.
*   **Heterogeneous Comparisons**: Functors like `LessByAuthor`, `LessByTitle`, `LessByYear`, `MoreByRating` enable sorting and comparisons using different criteria.
*   **Standard Integration**: Designed to work seamlessly with standard C++ iterators and algorithms.

## Dependencies

*   **C++ Standard**: C++20 (uses concepts, `std::format`/`std::print`, `std::span`, `std::expected`, modules like `<ranges>`, `<numeric>`)
*   **Standard Library**: Standard C++ library (headers like `<vector>`, `<string>`, `<unordered_set>`, `<algorithm>`, `<flat_map>`, etc.)
*   **GoogleTest** (for testing): Required only if building and running tests.

## Building

This project uses CMake for building.

1.  **Prerequisites**:
    *   C++20 compatible compiler (e.g., GCC 10+, Clang 12+)
    *   CMake (version specified in `CMakeLists.txt`, likely 3.15+)
    *   GoogleTest (if running tests)
2.  **Clone & Configure**:
    ```bash
    git clone <repository_url> # Replace with actual repo URL if applicable
    cd bookdb_project_directory
    mkdir build
    cd build
    cmake .. # Add options like -DCMAKE_BUILD_TYPE=Release if needed
    ```
3.  **Build**:
    ```bash
    cmake --build . # Or use make, ninja, etc., depending on your CMake generator
    ```
4.  **Run Tests** (if configured):
    ```bash
    # Assuming a target named 'tests' or similar is defined
    ctest
    # Or run the test executable directly if built
    # ./tests/BookDB_tests
    ```

## Usage

To use the `bookdb` library in your project:

1.  Include the necessary headers (e.g., `book_database.hpp`, `book.hpp`, `filters.hpp`, `statsistics.hpp`).
2.  Link against the compiled library (if built as one) or ensure the source files are compiled into your project.
3.  Ensure your compiler is set to C++20 standard (and has access to the C++23 `<flat_map>` header or equivalent implementation).

## API Overview

### Core Components

*   **`bookdb::Book`**: A struct holding book metadata (`title`, `author`, `year`, `genre`, `rating`, `pages`). Provides a `toString()` method.
*   **`bookdb::Genre`**: An `enum class` defining book genres (e.g., `Fiction`, `SciFi`, `Mystery`).
*   **`bookdb::BookDatabase<Container>`**: The main database class template.
    *   **Template Parameter**: `Container` (default `std::vector<Book>`) - The underlying container type for storing books.
    *   **Key Methods**:
        *   `EmplaceBack(...)`: Efficiently adds a new book.
        *   `PushBack(const Book&)`: Adds an existing book.
        *   `size()`, `empty()`: Container introspection.
        *   `begin()`, `end()`, `cbegin()`, `cend()`: Standard iterators for the books.
        *   `GetBooks()`: Returns a `std::span` view of the books.
        *   `GetAuthors()`: Returns a `std::span` view of author string views.
        *   `FindByAuthor(key)`: Finds the first book by an author (using transparent lookup).

### Searching and Filtering

*   **Functors** (`bookdb::comp` namespace): `LessByAuthor`, `LessByTitle`, `LessByYear`, `MoreByRating` for comparisons.
*   **Predicate Factories** (`bookdb::filters` namespace):
    *   `YearBetween(int, int)`: Creates a predicate for books within a year range.
    *   `RatingAbove(double)`: Creates a predicate for books above a rating.
    *   `GenreIs(Genre)`: Creates a predicate for books of a specific genre.
*   **Predicate Combinators** (`bookdb::filters` namespace):
    *   `all_of(preds...)`: Combines predicates with logical AND.
    *   `any_of(preds...)`: Combines predicates with logical OR.
*   **Filter Algorithm**:
    *   `bookdb::filterBooks(It first, It last, predicates...)`: Applies predicates to a range of books and returns a vector of `std::reference_wrapper<const Book>` for matches.

### Statistics

*   **`bookdb::stats` namespace** (functions typically take a `BookContainerLike`):
    *   `calculateAverageRating(const Container&)`: Returns the average book rating (`double`).
    *   `buildAuthorHistogramFlat(const Container&, Comparator comp = {})`: Returns a `std::flat_map<std::string_view, unsigned short>` mapping authors to book counts.
    *   `calculateGenreRatings(It begin, It end)`: Returns a `std::flat_map<Genre, double>` mapping genres to average ratings.
    *   `sampleRandomBooks(Container&, size_t k)`: Returns a `std::vector<std::reference_wrapper<Book>>` with `k` randomly selected books.
    *   `getTopNBy(Container&, size_t N, Comparator)`: Returns a `std::vector<std::reference_wrapper<const Book>>` with the top N books based on the comparator.

## Examples

A basic usage example can be found in `main.cpp`. It demonstrates creating a `BookDatabase`, adding books, and calling some statistical functions.

```c++
#include "book_database.hpp"
#include "statsistics.hpp"
#include <vector>
#include <print> // C++23, or use <iostream> and std::cout

using namespace bookdb;

int main() {
    // Create a database using std::vector as the backend
    BookDatabase<std::vector<Book>> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    // ... add more books

    // Calculate statistics
    double avg_rating = calculateAverageRating(db);
    std::print("Average Rating: {}\n", avg_rating);

    auto author_hist = buildAuthorHistogramFlat(db);
    std::print("Books per Author:\n{}\n", author_hist); // Requires formatter specializations

    return 0;
}
