# BookDB – A C++ Book Database Library

A modern C++23 library for storing, filtering, and analyzing book collections.  
The project is structured as a reusable library with clear separation of concerns, compile-time safety via concepts, and utilities for filtering, statistics, and formatting.

## Table of Contents

- [Features](#features)  
- [Dependencies](#dependencies)  
- [Building & Testing](#building--testing)  
- [Usage](#usage)  
- [API Overview](#api-overview)  
  - [Core Components](#core-components)  
  - [Filtering](#filtering)  
  - [Statistics](#statistics)  
- [Project Structure](#project-structure)  
- [License](#license)

## Features

- **Book model**: strongly typed `Book` struct with title, author, year, genre, rating, and pages.
- **BookDatabase**: template database class (supports `std::vector` or `std::deque` backend) with author string interning and stable views.
- **Concepts**: C++20 concepts (`BookLike`, `BookPredicate`, `BookContainerLike`, …) ensure safe and expressive templates.
- **Comparators**: ready-to-use functors (`LessByAuthor`, `MoreByRating`, `LessByYear`, …).
- **Filters**: composable predicate factories (`YearBetween`, `RatingAbove`, `GenreIs`), combinators (`all_of`, `any_of`), and `filterBooks`.
- **Statistics**: author histograms, average ratings, genre averages, random sampling, Top-N queries.
- **Formatters**: `std::formatter` specializations for pretty-printing books, databases, histograms, and results with `std::format` / `std::print`.
- **Standard integration**: works seamlessly with iterators, algorithms, and ranges.

## Dependencies

- **C++ standard**: C++23 (requires `<flat_map>`, `<print>`, concepts, spans, ranges).  
- **Build system**: CMake ≥ 3.20  
- **Testing**: GoogleTest (optional, for unit tests).  

## Building & Testing

```bash
git clone <repository_url>
cd bookdb_project
mkdir build && cd build
cmake ..
make -j
ctest     # run tests if GoogleTest is available
```

## Usage

```cpp
#include "book_database.hpp"
#include "filters.hpp"
#include "statistics.hpp"
#include <print>

using namespace bookdb;

int main() {
    BookDatabase<> db;
    db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.2, 688);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fantasy, 4.8, 310);

    auto results = filterBooks(db.begin(), db.end(),
                               YearBetween(1930, 1970),
                               RatingAbove(4.0));
    std::print("Filtered books:\n{}\n", results);

    double avg = calculateAverageRating(db);
    std::print("Average rating: {:.2f}\n", avg);
}
```

## API Overview

### Core Components
- **`Book`** – metadata record.  
- **`Genre`** – strongly typed enum.  
- **`BookDatabase<Container>`** – database with interned authors.  
  - `PushBack`, `EmplaceBack`  
  - `size()`, `empty()`  
  - `begin()`, `end()` iterators  
  - `GetBooks()`, `GetAuthors()`

### Filtering
- **Factories**: `YearBetween`, `RatingAbove`, `GenreIs`  
- **Combinators**: `all_of`, `any_of`  
- **Algorithm**: `filterBooks(It first, It last, preds...)` → `std::vector<std::reference_wrapper<const Book>>`

### Statistics
- `calculateAverageRating`  
- `buildAuthorHistogramFlat`  
- `calculateGenreRatings`  
- `sampleRandomBooks`  
- `getTopNBy`

## Project Structure

- `book.hpp` – Book model and Genre enum  
- `book_database.hpp` – Database container with author interning  
- `comparators.hpp` – Sorting functors  
- `concepts.hpp` – Core concepts  
- `filters.hpp` – Filtering predicates and helpers  
- `heterogeneous_lookup.hpp` – Transparent string hash/equal  
- `statistics.hpp` – Analysis functions  
- `db_test.cpp` – Unit tests  

## License

MIT License – see [LICENSE](LICENSE).

---

📌 **Note:** A `UML.png` diagram illustrating the architecture is included in the root directory.
