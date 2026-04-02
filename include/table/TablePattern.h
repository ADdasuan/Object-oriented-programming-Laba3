#ifndef TABLEPATTERN_H
#define TABLEPATTERN_H

#include <vector>
#include <concepts>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <memory>
#include <iterator>
#include <type_traits>

/**
 * @file TablePattern.h
 * @brief Упорядоченная ассоциативная таблица с бинарным поиском
 */

/**
 * @class Table
 * @brief Контейнер "ключ-значение" с автоматической сортировкой
 * @tparam Key Тип ключа (должен поддерживать сравнение)
 * @tparam Value Тип значения
 * @tparam Compare Компаратор для сравнения ключей (по умолчанию std::less<Key>)
 */
template <typename Key, typename Value, typename Compare = std::less<Key>>
class Table {
private:
    std::vector<std::pair<Key, Value>> table; /// Упорядоченный вектор пар (ключ, значение)

    Compare compare; /// Компаратор для сортировки

    /**
     * @brief Найти позицию для вставки/поиска ключа (бинарный поиск)
     * @param key Ключ для поиска
     * @return Индекс позиции в векторе
     */
    size_t find_position(const Key& key) const {
        size_t left = 0;
        size_t right = table.size();

        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (compare(table[mid].first,key)) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }

public:

    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<Key, Value>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using key_compare = Compare;

    /**
     * @class Iterator_Table
     * @brief Итератор произвольного доступа для таблицы
     * @tparam IsConst true - константный итератор, false - неконстантный
     * @note Итератор инвалидируется при изменении таблицы (вставке/удалении элементов)
     * @warning end()-итератор нельзя разыменовывать
     */
    template<bool IsConst>
    class Iterator_Table{
    private:
        /// @brief Тип вектора в зависимости от константности
        using vector_type = std::conditional_t<IsConst, const std::vector<std::pair<Key, Value>>, std::vector<std::pair<Key, Value>>>;

        /// @brief Тип пары ключ-значение
        using pair_type = std::conditional_t<IsConst, const std::pair<Key, Value>, std::pair<Key, Value>>;

        /// @brief Базовый итератор вектора
        using iterator_type = std::conditional_t<IsConst,typename vector_type::const_iterator,typename vector_type::iterator>;

        iterator_type iterator; /// Текущая позиция в контейнере
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = pair_type;
        using difference_type = std::ptrdiff_t;
        using pointer = pair_type*;
        using reference = pair_type&;

        /**
         * @brief Конструктор по умолчанию
         */
        Iterator_Table() = default;

        /**
         * @brief Конструктор из внутреннего итератора
         * @param it Итератор std::vector
         */
        explicit Iterator_Table(iterator_type it) : iterator(it) {}

        /**
         * @brief Конструктор копирования с преобразованием iterator -> const_iterator
         * @tparam OtherConst Константность источника
         * @param other Итератор для копирования
         */
        template<bool OtherConst, typename = std::enable_if_t<IsConst || !OtherConst>>
        Iterator_Table(const Iterator_Table<OtherConst>& other) : iterator(other.base()) {}

        /**
         * @brief Получить базовый итератор
         * @return Внутренний итератор std::vector
         */
        iterator_type base() const{return iterator;}


        /**
         * @brief Разыменование итератора
         * @return Ссылка на пару (ключ, значение)
         */
        pointer operator->() const {return &*iterator;}

        /**
         * @brief Разыменование итератора
         * @return Ссылка на пару (ключ, значение)
         */
        reference operator*() const {return *iterator;}

        /**
         * @brief Индексация (произвольный доступ)
         * @param n Смещение относительно текущей позиции
         * @return Ссылка на пару на позиции it[n]
         */
        reference operator[](difference_type n) const {return iterator[n];}

        /**
         * @brief Префиксный инкремент (++it)
         * @return Ссылка на текущий итератор
         */

        Iterator_Table& operator++() { ++iterator; return *this; }

        /**
         * @brief Постфиксный инкремент (it++)
         * @return Копия итератора до инкремента
         */

        Iterator_Table operator++(int) { Iterator_Table tmp = *this; ++iterator; return tmp; }

        /**
         * @brief Префиксный декремент (--it)
         * @return Ссылка на текущий итератор
         */
        Iterator_Table& operator--() { --iterator; return *this; }

        /**
         * @brief Постфиксный декремент (it--)
         * @return Копия итератора до декремента
         */
        Iterator_Table operator--(int) { Iterator_Table tmp = *this; --iterator; return tmp; }

        /**
         * @brief Сложение итератора с числом (n + it)
         * @param n Смещение
         * @param it Итератор
         * @return Новый итератор, смещенный на n
         */
        friend Iterator_Table operator+(difference_type n, const Iterator_Table& it) {return it + n;}

        /**
         * @brief Составное сложение (it += n)
         * @param n Смещение
         * @return Ссылка на текущий итератор
         */
        Iterator_Table& operator+=(difference_type n) { iterator += n; return *this; }

        /**
         * @brief Составное вычитание (it -= n)
         * @param n Смещение
         * @return Ссылка на текущий итератор
         */
        Iterator_Table& operator-=(difference_type n) { iterator-= n; return *this; }

        /**
         * @brief Сложение итератора с числом (it + n)
         * @param n Смещение
         * @return Новый итератор, смещенный на n
         */
        Iterator_Table operator+(difference_type n) const { return Iterator_Table(iterator + n); }

        /**
         * @brief Вычитание числа из итератора (it - n)
         * @param n Смещение
         * @return Новый итератор, смещенный на -n
         */
        Iterator_Table operator-(difference_type n) const { return Iterator_Table(iterator - n); }

        /**
         * @brief Разность итераторов (it1 - it2)
         * @param other Другой итератор
         * @return Расстояние между итераторами
         */
        difference_type operator-(const Iterator_Table& other) const { return iterator - other.iterator; }

        /**
         * @brief Равенство итераторов
         */
        template<bool OtherConst>
        bool operator==(const Iterator_Table<OtherConst>& other) const { return iterator == other.base(); }

        /**
         * @brief Неравенство итераторов
         */
        template<bool OtherConst>
        bool operator!=(const Iterator_Table<OtherConst>& other) const { return iterator != other.base(); }

        /**
         * @brief Меньше (it1 < it2)
         */
        template<bool OtherConst>
        bool operator<(const Iterator_Table<OtherConst>& other) const { return iterator < other.base(); }

        /**
         * @brief Меньше или равно (it1 <= it2)
         */
        template<bool OtherConst>
        bool operator<=(const Iterator_Table<OtherConst>& other) const { return iterator <= other.base(); }

        /**
        * @brief Больше (it1 > it2)
        */
        template<bool OtherConst>
        bool operator>(const Iterator_Table<OtherConst>& other) const { return iterator > other.base(); }

        /**
         * @brief Больше или равно (it1 >= it2)
         */
        template<bool OtherConst>
        bool operator>=(const Iterator_Table<OtherConst>& other) const { return iterator >= other.base(); }
    };

    using iterator = Iterator_Table<false>;
    using const_iterator = Iterator_Table<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Итератор на начало
     * @return Итератор на первый элемент
     */
    iterator begin() noexcept { return iterator(table.begin()); }

    /**
     * @brief Итератор на конец
     * @return Итератор за последним элементом
     */
    iterator end() noexcept { return iterator(table.end()); }

    /**
     * @brief Константный итератор на начало
     */
    const_iterator begin() const noexcept { return const_iterator(table.begin()); }

    /**
     * @brief Константный итератор на конец
     */
    const_iterator end() const noexcept { return const_iterator(table.end()); }

    /**
     * @brief Константный итератор на начало (явный)
     */
    const_iterator cbegin() const noexcept { return const_iterator(table.cbegin()); }

    /**
     * @brief Константный итератор на конец (явный)
     */
    const_iterator cend() const noexcept { return const_iterator(table.cend()); }

    /**
     * @brief Обратный итератор на начало (конец таблицы)
     */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    /**
     * @brief Обратный итератор на конец (начало таблицы)
     */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    /**
     * @brief Константный обратный итератор на начало
     */
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     * @brief Константный обратный итератор на конец
     */
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    /**
     * @brief Конструктор по умолчанию
     *
     * Создает пустую таблицу.
     */
    Table() = default;

    /**
     * @brief Конструктор из списка инициализации
     * @param initialization Список пар {ключ, значение}
     */
    Table(std::initializer_list<std::pair<Key, Value>> initialization) {
        for (const auto& pair : initialization) {
            insert(pair.first, pair.second);
        }
    }


    /**
     * @brief Деструктор по умолчанию
     */
    ~Table() = default;

    /**
     * @brief Вставить элемент в таблицу
     * @param key Ключ элемента
     * @param value Значение элемента
     * @throws std::invalid_argument если ключ пустой
     * @throws std::invalid_argument если ключ уже существует
     */
    void insert(const Key& key, const Value& value) {
        if (key.empty()) {
            throw std::invalid_argument("Key not can be is empty");
        }

        size_t position = find_position(key);
        if (position < table.size() && table[position].first == key) {
            throw std::invalid_argument("Oh no...This key already exist");
        }
        table.insert(table.begin() + position, {key, value});
    }




    /**
     * @brief Удалить элемент из таблицы
     * @param key Ключ элемента для удаления
     * @throws std::out_of_range если ключ не найден
     */
    void delet(const Key& key) {
        size_t position = find_position(key);
        if (position >= table.size() || table[position].first != key) {
            throw std::out_of_range("This login don't exist");
        }
        table.erase(table.begin() + position);
    }

    /**
     * @brief Найти элемент по ключу
     * @param key Ключ для поиска
     * @return Указатель на значение или nullptr если не найдено
     */
    const Value* find(const Key& key) const{
        size_t position = find_position(key);
        if (position < table.size() && table[position].first == key) {
            return &table[position].second;
        }
        return nullptr;
    }

    /**
     * @brief Проверить существование ключа
     * @param key Ключ для проверки
     * @return true если ключ существует
     */
    bool key_exist(const Key& key) const {
        size_t position = find_position(key);
        return (position < table.size() && table[position].first == key);
    }

    /**
     * @brief Вставить или обновить элемент
     * @param key Ключ элемента
     * @param value Значение элемента
     * @throws std::invalid_argument если ключ пустой
     */
    void insert_or_assign(const Key& key, const Value& value) {
        if (key.empty()) {
            throw std::invalid_argument("Key cannot be empty");
        }

        size_t position = find_position(key);
        if (position < table.size() && table[position].first == key) {
            table[position].second = value;
        } else {
            table.insert(table.begin() + position, {key, value});
        }
    }

    /**
     * @brief Получить количество элементов
     * @return Размер таблицы
     */
    size_t size() const noexcept { return table.size(); }

    /**
     * @brief Создать элемент на месте
     */
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        std::pair<Key, Value> temp_pair(std::forward<Args>(args)...);

        if constexpr (requires { temp_pair.first.empty(); }) {
            if (temp_pair.first.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }
        }

        size_t position = find_position(temp_pair.first);

        if (position < table.size() && table[position].first == temp_pair.first) {
            return {iterator(table.begin() + position), false};
        }
        auto it = table.emplace(table.begin() + position, std::move(temp_pair));
        return {iterator(it), true};
    }

    /**
     * @brief Создать элемент на месте с подсказкой
     */
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        (void)hint;
        return emplace(std::forward<Args>(args)...).first;
    }

    /**
     * @brief Вставить диапазон элементов
     */
    template<std::input_iterator InputIt>
    void insert_range(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            insert(it->first, it->second);
        }
    }

    /**
    * @brief Получить количество элементов
    * @return Максимальный размер таблицы
    */
    size_t max_size() const noexcept { return table.max_size(); }

    /**
     * @brief Проверить, пуста ли таблица
     * @return true если таблица пустая
     */
    bool empty() const noexcept { return table.empty(); }

    /**
     * @brief Очистить таблицу
     */
    void clear() noexcept { table.clear(); }

    /**
     * @brief Получить список всех ключей
     * @return Вектор всех ключей в отсортированном порядке
     */
    std::vector<Key> get_all_keys() const {
        std::vector<Key> keys;
        keys.reserve(table.size());
        for (const auto& pair : table) {
            keys.push_back(pair.first);
        }
        return keys;
    }
};

static_assert(std::is_same<typename std::iterator_traits<Table<int, int>::iterator>::iterator_category,
        std::random_access_iterator_tag>::value,"Table iterator must be random_access_iterator");

static_assert(std::random_access_iterator<Table<int, int>::iterator>);
static_assert(std::ranges::range<Table<int, int>>);

#endif