#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"

using namespace bookdb;

Book make_book(std::string title, std::string author, int year, Genre genre, double rating, int pages) {
    return Book {std::move(title), std::move(author), year, genre, rating, pages};
}

// --- Basic construction & empty state ---
TEST(BookDatabaseAPI, StartsEmptyAndIteratorsAreValid) {
    BookDatabase db;
    EXPECT_EQ(std::distance(db.begin(), db.end()), 0);
    EXPECT_TRUE(db.begin() == db.end());
}

// --- EmplaceBack should add elements and preserve data ---
TEST(BookDatabaseAPI, EmplaceBackAddsBooksAndSizeIncreases) {
    BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);

    EXPECT_EQ(std::distance(db.begin(), db.end()), 2);

    auto it        = db.begin();
    const Book& b1 = *it++;
    const Book& b2 = *it;

    EXPECT_EQ(b1.title, "1984");
    EXPECT_EQ(b1.author, "George Orwell");
    EXPECT_EQ(b1.year, 1949);
    EXPECT_EQ(b1.genre, Genre::SciFi);
    EXPECT_DOUBLE_EQ(b1.rating, 4.0);

    EXPECT_EQ(b2.title, "Animal Farm");
    EXPECT_EQ(b2.author, "George Orwell");
}

// --- Iterators must be random-access compatible with std::sort ---
TEST(BookDatabaseAPI, IteratorsAllowStdSort) {
    BookDatabase db;
    db.EmplaceBack("C", "B", 2000, Genre::Fiction, 4.1, 100);
    db.EmplaceBack("A", "C", 2001, Genre::Fiction, 4.2, 110);
    db.EmplaceBack("B", "A", 1999, Genre::Fiction, 4.3, 120);

    std::sort(db.begin(), db.end(), [](const Book& lhs, const Book& rhs) { return lhs.title < rhs.title; });

    auto it = db.begin();
    EXPECT_EQ(it->title, "A");
    ++it;
    EXPECT_EQ(it->title, "B");
    ++it;
    EXPECT_EQ(it->title, "C");
}

// --- Author search with std::find_if ---
TEST(BookDatabaseAPI, CanFindAuthorWithStdAlgorithms) {
    BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);

    auto it = std::find_if(db.begin(), db.end(), [](const Book& b) { return b.author == "George Orwell"; });
    ASSERT_TRUE(it != db.end());
    EXPECT_EQ(it->title, "1984");
}

// --- Optional: GetBooks/GetAuthors if implemented later ---
TEST(BookDatabaseAPI, OptionalViewsIfProvided) {
    BookDatabase db;
    db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.2, 688);
     auto books_view = db.GetBooks();
     EXPECT_GE(books_view.size(), 1u);
}

// ================= Additional tests: Filters & Statistics =================
#include "filters.hpp"
#include "statsistics.hpp"

/*TEST(FiltersAPI, Predicates_Composition_Smoke) {
    GTEST_SKIP() << "Enable when filters predicates are implemented.";
    // // If predicates live in bookdb namespace (per current headers):
    // auto by_year  = YearBetween(1990, 2010);
    // auto good     = RatingAbove(4.2);
    // auto fantasy  = GenreIs(Genre::Fiction); // adjust to your Genre names
    // auto combo    = all_of(by_year, good, fantasy);
    // BookDatabase db;
    // db.EmplaceBack("A", "Au", 1995, Genre::Fiction, 4.5, 300);
    // db.EmplaceBack("B", "Au", 1985, Genre::Fiction, 4.9, 300);
    // int hits = std::count_if(db.begin(), db.end(), combo);
    // EXPECT_EQ(hits, 1);
}*/

/*TEST(FiltersAPI, FilterBooks_Helper_ReturnsMatches) {
    GTEST_SKIP() << "Enable when filterBooks(It,It,Pred) is implemented.";
    // BookDatabase db;
    // db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.2, 688);
    // db.EmplaceBack("Sea", "John", 2018, Genre::Adventure, 3.7, 220);
    // auto p = any_of(GenreIs(Genre::SciFi), RatingAbove(4.0));
    // auto matches = filterBooks(db.begin(), db.end(), p);
    // ASSERT_EQ(matches.size(), 1u);
    // EXPECT_EQ(matches[0].get().title, "Dune");
}*/

TEST(StatsAPI, BuildAuthorHistogramFlat_Smoke) {
     BookDatabase db;
     db.EmplaceBack("A", "X", 2000, Genre::Fiction, 4.1, 120);
     db.EmplaceBack("B", "X", 2001, Genre::Fiction, 4.2, 130);
     db.EmplaceBack("C", "Y", 2002, Genre::Fiction, 4.3, 140);
     auto hist = buildAuthorHistogramFlat(db);
     EXPECT_EQ(hist["X"], 2u);
     EXPECT_EQ(hist["Y"], 1u);
}

TEST(StatsAPI, CalculateAverageRating_Smoke) {
     BookDatabase db;
     db.EmplaceBack("A", "X", 2000, Genre::Fiction, 5.0, 120);
     db.EmplaceBack("B", "Y", 2001, Genre::Fiction, 3.0, 130);
     double avg = calculateAverageRating(db);
     EXPECT_NEAR(avg, 4.0, 1e-12);
}

TEST(StatsAPI, CalculateGenreRatings_Smoke) {
     BookDatabase db;
     db.EmplaceBack("A", "X", 2000, Genre::Fiction, 4.0, 120);
     db.EmplaceBack("B", "Y", 2001, Genre::SciFi,   5.0, 130);
     db.EmplaceBack("C", "Z", 2002, Genre::SciFi,   3.0, 140);
     auto per_genre = calculateGenreRatings(db.begin(), db.end());
     EXPECT_NEAR(per_genre[Genre::Fiction], 4.0, 1e-12);
     EXPECT_NEAR(per_genre[Genre::SciFi],   4.0, 1e-12);
}

TEST(StatsAPI, SampleRandomBooks_Smoke) {
     BookDatabase db;
     for (int i = 0; i < 10; ++i)
         db.EmplaceBack("T"+std::to_string(i), "A", 2000+i, Genre::Fiction, 4.0, 100+i);
     auto sample = sampleRandomBooks(db, 3);
     EXPECT_EQ(sample.size(), 3u);
     for (auto& ref : sample) {
         const Book* ptr = &ref.get();
         bool found = std::any_of(db.begin(), db.end(), [&](const Book& b){ return &b == ptr; });
         EXPECT_TRUE(found);
     }
}

TEST(StatsAPI, GetTopNBy_Smoke) {
     BookDatabase db;
     db.EmplaceBack("Avg", "A", 2000, Genre::Fiction, 4.0, 100);
     db.EmplaceBack("Top", "B", 2001, Genre::Fiction, 5.0, 100);
     db.EmplaceBack("Low", "C", 1999, Genre::Fiction, 3.0, 100);
     auto top2 = getTopNBy(db, 2, comp::MoreByRating{});
     ASSERT_EQ(top2.size(), 2u);
     EXPECT_EQ(top2[0].get().title, "Top");
     EXPECT_EQ(top2[1].get().title, "Avg");
}



TEST(StatsAPI, CalculateGenreRatings_ReturnsAveragesPerPresentGenre) {
    BookDatabase db;
    // Two Fantasy books: avg = (4.0 + 5.0)/2 = 4.5
    db.EmplaceBack("F1", "A", 2001, Genre::Fiction, 4.0, 100);
    db.EmplaceBack("F2", "B", 2002, Genre::Fiction, 5.0, 120);
    // One SciFi book: avg = 3.5
    db.EmplaceBack("S1", "C", 1999, Genre::SciFi, 3.5, 300);
    // No NonFiction inserted ? it should NOT appear in the map

    auto m = calculateGenreRatings(db.begin(), db.end());

    // Only genres that occurred should be present
    // (exact size depends on how many appeared)
    ASSERT_GE(m.size(), 2u);
    ASSERT_TRUE(m.contains(Genre::Fiction));
    ASSERT_TRUE(m.contains(Genre::SciFi));

    // Check averages
    EXPECT_NEAR(m.at(Genre::Fiction), 4.5, 1e-9);
    EXPECT_NEAR(m.at(Genre::SciFi), 3.5, 1e-9);

    // Absent genres should not be present
    EXPECT_FALSE(m.contains(Genre::NonFiction));
}

// ---------------- Stats: calculateAverageRating ----------------

TEST(StatsAPI, CalculateAverageRatingEmptyIsZero) {
    BookDatabase db;
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 0.0);
}

TEST(StatsAPI, CalculateAverageRatingComputesMean) {
    BookDatabase db;
    db.EmplaceBack("A", "aa", 2000, Genre::Fiction, 4.0, 100);
    db.EmplaceBack("B", "bb", 2001, Genre::Fiction, 5.0, 120);
    db.EmplaceBack("C", "cc", 2002, Genre::Fiction, 3.0, 140);
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 4.0);
}

// ---------------- Filters: filterBooks ----------------
/*
#include "filters.hpp"
using namespace bookdb::filters;

TEST(FiltersAPI, FilterBooks_ComposesAndReturnsRefs) {
    BookDatabase db;
    db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.2, 688);
    db.EmplaceBack("The Dragon Reborn", "Robert Jordan", 1991, Genre::Fantasy, 4.6, 624);
    db.EmplaceBack("Sea Stories", "John Doe", 2018, Genre::Adventure, 3.7, 220);

    auto hits = filterBooks(db.begin(), db.end(),
                            GenreIs(Genre::Fantasy),
                            RatingAtLeast(4.5));
    ASSERT_EQ(hits.size(), 1u);
    EXPECT_EQ(hits[0].get().title, "The Dragon Reborn");
}
*/

// ---------------- Stats: getTopNBy (default comparator = MoreByRating) ----------------
TEST(StatsAPI, GetTopNBy_DefaultComp_ReturnsSortedTopN) {
    BookDatabase db;
    db.EmplaceBack("L", "A", 2000, Genre::Fiction, 4.1, 100);
    db.EmplaceBack("H", "B", 2000, Genre::Fiction, 4.9, 100);
    db.EmplaceBack("M", "C", 2000, Genre::Fiction, 4.5, 100);
    db.EmplaceBack("Q", "D", 2000, Genre::Fiction, 3.9, 100);

    auto top2 = getTopNBy(db, 2, comp::MoreByRating{});
    ASSERT_EQ(top2.size(), 2u);
    // Ordered best?worse
    EXPECT_EQ(top2[0].get().title, "H"); // 4.9
    EXPECT_EQ(top2[1].get().title, "M"); // 4.5
}

TEST(StatsAPI, GetTopNBy_ZeroOrSmallEdgeCases) {
    BookDatabase db;
    EXPECT_TRUE(getTopNBy(db, 0, comp::LessByTitle{}).empty());   // N=0
    db.EmplaceBack("Only", "A", 2000, Genre::Fiction, 4.2, 100);
    auto top5 = getTopNBy(db, 5, comp::LessByTitle{});
    ASSERT_EQ(top5.size(), 1u);
    EXPECT_EQ(top5[0].get().title, "Only");
}


