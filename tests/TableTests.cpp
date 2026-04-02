#include <gtest/gtest.h>
#include <string>
#include <memory>
#include "TablePattern.h"

TEST(TableTest, InsertAndFind) {
    Table<std::string, int> table;

    table.insert("Komarov", 1);
    table.insert("Muxa", 2);
    table.insert("Skitev", 3);

    EXPECT_EQ(*table.find("Komarov"), 1);
    EXPECT_EQ(*table.find("Muxa"), 2);
    EXPECT_EQ(*table.find("Skitev"), 3);
    EXPECT_EQ(table.find("Non-existent"), nullptr);
}

TEST(TableTest, InsertDuplicate) {
    Table<std::string, int> table;

    table.insert("Dmitriev", 1);
    EXPECT_THROW(table.insert("Dmitriev", 2), std::invalid_argument);
}

TEST(TableTest, Delete) {
    Table<std::string, std::string> table;

    table.insert("Sinelnikov", "moderator");
    table.insert("Chepik", "user");

    table.delet("Sinelnikov");

    EXPECT_EQ(table.find("Sinelnikov"), nullptr);
    EXPECT_NE(table.find("Chepik"), nullptr);
    EXPECT_EQ(table.size(), 1);
}

TEST(TableTest, InsertOrAssign) {
    Table<std::string, float> table;

    table.insert_or_assign("Ovcharenko", 0.5f);
    EXPECT_FLOAT_EQ(*table.find("Ovcharenko"), 0.5f);

    table.insert_or_assign("Ovcharenko", 0.7f);
    EXPECT_FLOAT_EQ(*table.find("Ovcharenko"), 0.7f);
}

TEST(TableTest, KeyExists) {
    Table<std::string, int> table;

    table.insert("Doroshin", 1);

    EXPECT_TRUE(table.key_exist("Doroshin"));
    EXPECT_FALSE(table.key_exist("Firrer"));
}

TEST(TableTest, SizeAndEmpty) {
    Table<std::string, int> table;

    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);

    table.insert("Alibekova", 1);

    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 1);
}

TEST(TableTest, Clear) {
    Table<std::string, int> table;

    table.insert("Sein", 1);
    table.insert("Maximov", 2);

    table.clear();

    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(TableTest, GetAllKeys) {
    Table<std::string, int> table;

    table.insert("Shevtsov", 1);
    table.insert("Meleshkin", 2);
    table.insert("Chesnokova", 3);

    auto keys = table.get_all_keys();

    EXPECT_EQ(keys.size(), 3);
    EXPECT_EQ(keys[0], "Chesnokova");
    EXPECT_EQ(keys[1], "Meleshkin");
    EXPECT_EQ(keys[2], "Shevtsov");
}

TEST(TableTest, Iterator) {
    Table<std::string, int> table;

    table.insert("Novikov", 1);
    table.insert("Gaspariunas", 2);
    table.insert("Dyumin", 3);

    int sum = 0;
    for (const auto& [key, value] : table) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}

TEST(TableTest, ConstIterator) {
    Table<std::string, int> table;
    table.insert("Agibalov", 5);

    const auto& const_table = table;
    auto it = const_table.begin();

    EXPECT_EQ(it->first, "Agibalov");
    EXPECT_EQ(it->second, 5);
}

TEST(TableTest, ReverseIterator) {
    Table<std::string, int> table;

    table.insert("Komarov", 1);
    table.insert("Muxa", 2);
    table.insert("Skitev", 3);

    std::vector<std::string> reverse_order;
    for (auto it = table.rbegin(); it != table.rend(); ++it) {
        reverse_order.push_back(it->first);
    }

    EXPECT_EQ(reverse_order.size(), 3);
    EXPECT_EQ(reverse_order[0], "Skitev");
    EXPECT_EQ(reverse_order[1], "Muxa");
    EXPECT_EQ(reverse_order[2], "Komarov");
}

TEST(TableTest, IteratorOperations) {
    Table<std::string, int> table;
    table.insert("Test1", 1);
    table.insert("Test2", 2);
    table.insert("Test3", 3);

    auto it = table.begin();

    EXPECT_EQ((*it).first, "Test1");
    EXPECT_EQ((*it).second, 1);

    ++it;
    EXPECT_EQ(it->first, "Test2");

    it++;
    EXPECT_EQ(it->first, "Test3");

    --it;
    EXPECT_EQ(it->first, "Test2");

    auto it2 = it + 1;
    EXPECT_EQ(it2->first, "Test3");

    auto it3 = it2 - 1;
    EXPECT_EQ(it3->first, "Test2");

    EXPECT_EQ(it2 - it, 1);
}

TEST(TableTest, InitializerListConstructor) {
    Table<std::string, int> table = {
            {"Komarov", 1},
            {"Muxa", 2},
            {"Skitev", 3}
    };

    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(*table.find("Komarov"), 1);
    EXPECT_EQ(*table.find("Muxa"), 2);
    EXPECT_EQ(*table.find("Skitev"), 3);
}


TEST(TableMethodsTest, SizeAndEmpty) {
    Table<std::string, int> table;

    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);

    // Добавляем один элемент
    table.insert("key1", 100);
    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 1);

    // Добавляем еще элементы
    table.insert("key2", 200);
    table.insert("key3", 300);
    EXPECT_EQ(table.size(), 3);

    // Удаляем элемент
    table.delet("key1");
    EXPECT_EQ(table.size(), 2);

    // Очищаем
    table.clear();
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

// ==================== clear() ====================
TEST(TableMethodsTest, Clear) {
    Table<std::string, int> table;

    // Очистка пустой таблицы
    table.clear();
    EXPECT_TRUE(table.empty());

    // Очистка заполненной таблицы
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);

    EXPECT_EQ(table.size(), 3);
    table.clear();

    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);

    // Проверяем что элементы удалены
    EXPECT_EQ(table.find("a"), nullptr);
    EXPECT_EQ(table.find("b"), nullptr);
    EXPECT_EQ(table.find("c"), nullptr);

    // Можно ли использовать после очистки
    table.insert("new", 42);
    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 1);
}

// ==================== max_size() ====================
TEST(TableMethodsTest, MaxSize) {
    Table<std::string, int> table1;
    Table<int, double> table2;
    Table<std::vector<int>, std::string> table3;

    // Проверяем что метод существует и возвращает значение
    EXPECT_GT(table1.max_size(), 0);
    EXPECT_GT(table2.max_size(), 0);
    EXPECT_GT(table3.max_size(), 0);

    // Проверяем что max_size >= size()
    table1.insert("test", 1);
    EXPECT_GE(table1.max_size(), table1.size());

    // Может ли таблица быть очень большой (теоретически)
    EXPECT_LE(table1.size(), table1.max_size());
}

// ==================== get_all_keys() ====================
TEST(TableMethodsTest, GetAllKeys) {
    Table<std::string, int> table;

    // Пустая таблица
    auto empty_keys = table.get_all_keys();
    EXPECT_TRUE(empty_keys.empty());
    EXPECT_EQ(empty_keys.size(), 0);

    // Один элемент
    table.insert("single", 1);
    auto one_key = table.get_all_keys();
    EXPECT_EQ(one_key.size(), 1);
    EXPECT_EQ(one_key[0], "single");

    // Несколько элементов (вставляем в случайном порядке)
    table.insert("zebra", 2);
    table.insert("apple", 3);
    table.insert("banana", 4);
    table.insert("carrot", 5);

    auto all_keys = table.get_all_keys();

    // Проверяем количество
    EXPECT_EQ(all_keys.size(), 5);

    // Проверяем сортировку
    std::vector<std::string> expected = {"apple", "banana", "carrot", "single", "zebra"};
    EXPECT_EQ(all_keys, expected);

    // Проверяем что возвращается копия, а не ссылка
    table.delet("apple");
    auto keys_after_delete = table.get_all_keys();
    EXPECT_EQ(all_keys.size(), 5); // Оригинальный вектор не изменился
    EXPECT_EQ(keys_after_delete.size(), 4);
}

TEST(InsertRangeTest, BasicRange) {
    Table<std::string, int> table;

    // Вставляем из vector
    std::vector<std::pair<std::string, int>> vec = {
            {"c", 3},
            {"a", 1},
            {"b", 2}
    };

    table.insert_range(vec.begin(), vec.end());

    EXPECT_EQ(table.size(), 3);

    // Проверяем сортировку
    auto keys = table.get_all_keys();
    EXPECT_EQ(keys[0], "a");
    EXPECT_EQ(keys[1], "b");
    EXPECT_EQ(keys[2], "c");

    // Проверяем значения
    EXPECT_EQ(*table.find("a"), 1);
    EXPECT_EQ(*table.find("b"), 2);
    EXPECT_EQ(*table.find("c"), 3);
}

TEST(InsertRangeTest, EmptyRange) {
    Table<std::string, int> table;

    table.insert("existing", 100);

    // Пустой диапазон не должен ничего менять
    std::vector<std::pair<std::string, int>> empty;
    table.insert_range(empty.begin(), empty.end());

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(*table.find("existing"), 100);
}

TEST(InsertRangeTest, SingleElementRange) {
    Table<std::string, int> table;

    std::vector<std::pair<std::string, int>> single = {{"only", 42}};
    table.insert_range(single.begin(), single.end());

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(*table.find("only"), 42);
}

// ==================== begin() / end() ====================
TEST(IteratorTest, BeginEnd) {
    Table<std::string, int> table;

    // Пустая таблица
    EXPECT_EQ(table.begin(), table.end());

    // Добавляем элементы
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);

    // Проверяем begin()
    auto it_begin = table.begin();
    EXPECT_EQ(it_begin->first, "a");
    EXPECT_EQ(it_begin->second, 1);

    // Проверяем end() через цикл
    int sum = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        sum += it->second;
    }
    EXPECT_EQ(sum, 6);

    // Range-based for loop
    sum = 0;
    for (const auto& [key, value] : table) {
        sum += value;
    }
    EXPECT_EQ(sum, 6);
}

// ==================== const begin() / const end() ====================
TEST(IteratorTest, ConstBeginEnd) {
    Table<std::string, int> table;
    table.insert("x", 10);
    table.insert("y", 20);

    // Используем константную ссылку
    const auto& const_table = table;

    // Проверяем const begin()
    auto cit = const_table.begin();
    EXPECT_EQ(cit->first, "x");
    EXPECT_EQ(cit->second, 10);

    // Проверяем const end()
    int count = 0;
    for (auto it = const_table.begin(); it != const_table.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 2);

    // Const range-based for loop
    count = 0;
    for (const auto& [key, value] : const_table) {
        ++count;
        EXPECT_TRUE(key == "x" || key == "y");
    }
    EXPECT_EQ(count, 2);
}


// ==================== cbegin() / cend() ====================
TEST(IteratorTest, CbeginCend) {
    Table<std::string, int> table;
    table.insert("first", 100);
    table.insert("second", 200);

    // Проверяем cbegin()
    auto cit = table.cbegin();
    EXPECT_EQ(cit->first, "first");
    EXPECT_EQ(cit->second, 100);

    // Проверяем весь диапазон
    std::vector<std::string> keys;
    for (auto it = table.cbegin(); it != table.cend(); ++it) {
        keys.push_back(it->first);
    }
    EXPECT_EQ(keys.size(), 2);
    EXPECT_EQ(keys[0], "first");
    EXPECT_EQ(keys[1], "second");

    // cbegin/cend должны быть const итераторы
    static_assert(std::is_same_v<decltype(table.cbegin()),
            Table<std::string, int>::const_iterator>);
}

// ==================== rbegin() / rend() ====================
TEST(IteratorTest, ReverseBeginEnd) {
    Table<std::string, int> table;
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);

    // Проверяем rbegin()
    auto rit = table.rbegin();
    EXPECT_EQ(rit->first, "c");
    EXPECT_EQ(rit->second, 3);

    // Проходим в обратном порядке
    std::vector<std::string> reverse_keys;
    for (auto rit = table.rbegin(); rit != table.rend(); ++rit) {
        reverse_keys.push_back(rit->first);
    }

    EXPECT_EQ(reverse_keys.size(), 3);
    EXPECT_EQ(reverse_keys[0], "c");
    EXPECT_EQ(reverse_keys[1], "b");
    EXPECT_EQ(reverse_keys[2], "a");

    // Проверяем операторы reverse итератора
    ++rit; // Теперь rit указывает на 'b' (после инкремента от 'c')
    --rit; // Вернулись на 'c'
    EXPECT_EQ(rit->first, "c");
}



// ==================== crbegin() / crend() ====================
TEST(IteratorTest, ConstReverseBeginEnd) {
    Table<std::string, int> table;
    table.insert("one", 1);
    table.insert("two", 2);
    table.insert("three", 3);

    const auto& const_table = table;

    // Проверяем crbegin()
    auto crit = const_table.crbegin();
    EXPECT_EQ(crit->first, "two"); // "two" после "three" в сортировке
    EXPECT_EQ(crit->second, 2);

    // Проходим в обратном порядке константно
    int sum = 0;
    std::vector<std::string> const_reverse_keys;
    for (auto crit = const_table.crbegin(); crit != const_table.crend(); ++crit) {
        const_reverse_keys.push_back(crit->first);
        sum += crit->second;
    }

    EXPECT_EQ(sum, 6);
    EXPECT_EQ(const_reverse_keys.size(), 3);
    EXPECT_EQ(const_reverse_keys[0], "two");
    EXPECT_EQ(const_reverse_keys[1], "three");
    EXPECT_EQ(const_reverse_keys[2], "one");

    static_assert(std::is_same_v<decltype(const_table.crbegin()),
            Table<std::string, int>::const_reverse_iterator>);
}

TEST(SingleMethodTest, Size) {
    Table<std::string, int> table;
    EXPECT_EQ(table.size(), 0);

    table.insert("a", 1);
    EXPECT_EQ(table.size(), 1);

    table.insert("b", 2);
    table.insert("c", 3);
    EXPECT_EQ(table.size(), 3);

    table.delet("b");
    EXPECT_EQ(table.size(), 2);
}


TEST(SingleMethodTest, InsertRange) {
    Table<std::string, int> table;

    std::vector<std::pair<std::string, int>> data = {
            {"zebra", 3},
            {"apple", 1},
            {"banana", 2}
    };

    table.insert_range(data.begin(), data.end());

    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(table.begin()->first, "apple");
    EXPECT_EQ(table.begin()->second, 1);
}


TEST(SingleMethodTest, MaxSize) {
    Table<std::string, int> table;
    size_t max = table.max_size();
    EXPECT_GT(max, 0);
    EXPECT_GE(max, table.size());
}


TEST(SingleMethodTest, Empty) {
    Table<std::string, int> table;
    EXPECT_TRUE(table.empty());

    table.insert("test", 1);
    EXPECT_FALSE(table.empty());

    table.clear();
    EXPECT_TRUE(table.empty());
}

// ==================== clear() ====================
TEST(SingleMethodTest, Clear) {
    Table<std::string, int> table;

    table.insert("x", 1);
    table.insert("y", 2);
    table.insert("z", 3);

    EXPECT_EQ(table.size(), 3);
    EXPECT_FALSE(table.empty());

    table.clear();

    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.find("x"), nullptr);
}