#include <gtest/gtest.h>

TEST(TestComponentName, SimpleCheck) {
    EXPECT_EQ(1 + 1, 2);
}

// 1. Сгенерировать тесты для базового функционала класса BookDatabase (протестировать API: begin, end, size, empty, [],
// const [], EmplaceBack, GetBooks, GetAuthors, Clear).
// 2. Протестирвоать компараторы с std::sort.
// 3. Протестирвоать buildAuthorHistogramFlat.
