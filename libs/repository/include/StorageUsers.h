#ifndef STORAGEUSERS_H
#define STORAGEUSERS_H

#include "TablePattern.h"
#include "IUser.h"
#include "IModerator.h"
#include "ITroll.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @file StorageUsers.h
 * @brief Хранилище всех пользователей форума
 */

/**
 * @class StorageUsers
 * @brief Управляет всеми пользователями форума
 */
class StorageUsers {
private:
    Table<std::string, std::shared_ptr<IUser>> data;/// Упорядоченная таблица пользователей логин + пользователь

public:
    StorageUsers() = default;

    /**
     * @brief Получить пользователя по логину
     * @param login Логин пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    std::shared_ptr<IUser> get_user(const std::string& login) const;

    /**
     * @brief Добавить нового пользователя
     * @param user Указатель на пользователя
     * @throws std::invalid_argument если пользователь с таким логином уже существует
     */
    void add_user(std::shared_ptr<IUser> user);

    /**
     * @brief Удалить пользователя
     * @param login Логин пользователя
     * @throws std::invalid_argument если пользователь не найден
     */
    void del_user(const std::string& login);

    /**
     * @brief Проверить существование пользователя
     * @param login Логин для проверки
     * @return true если пользователь существует
     */
    bool find_user(const std::string& login);

    /**
     * @brief Найти пользователя и получить указатель
     * @param login Логин пользователя
     * @return Указатель на пользователя или nullptr
     */
    std::shared_ptr<IUser> find_user_ptr(const std::string& login);

    /**
     * @brief Получить список всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    std::vector<std::shared_ptr<IUser>> get_all_users();


    /**
     * @brief Обновить данные пользователя
     * @param login Логин пользователя
     * @param new_user Новый объект пользователя
     * @return true если обновление успешно
     */
    bool update_user(const std::string& login, std::shared_ptr<IUser> new_user);

    /**
     * @brief Проверить, является ли пользователь модератором
     * @param login Логин пользователя
     * @return true если пользователь - модератор
     */
    bool is_moderator(const std::string& login) const;

    /**
     * @brief Проверить, является ли пользователь троллем
     * @param login Логин пользователя
     * @return true если пользователь - тролль
     */
    bool is_troll(const std::string& login) const;

    /**
     * @brief Проверить, онлайн ли пользователь
     * @param login Логин пользователя
     * @return true если статус ONLINE
     */
    bool is_online(const std::string& login) const;


    /**
     * @brief Проверить, забанен ли пользователь
     * @param login Логин пользователя
     * @return true если статус BANNED
     */
    bool is_banned(const std::string& login) const;

    /**
     * @brief Забанить пользователя
     * @param login Логин пользователя
     * @return true если операция успешна
     */
    bool ban_user(const std::string& login);

    /**
     * @brief Разбанить пользователя
     * @param login Логин пользователя
     * @return true если операция успешна
     */
    bool unban_user(const std::string& login);

    /**
     * @brief Изменить разумность модератора
     * @param login Логин модератора
     * @param intelligence Новое значение разумности [1-10]
     * @return true если операция успешна
     */
    bool change_moderator_intelligence(const std::string& login, unsigned intelligence);

    /**
     * @brief Изменить толщину тролля
     * @param login Логин тролля
     * @param size Новая толщина [0.1-1.0]
     * @return true если операция успешна
     */
    bool change_troll_width(const std::string& login, float size);

    /**
     * @brief Увеличить успешность тролля
     * @param login Логин тролля
     * @return true если операция успешна
     */
    bool increment_troll_success(const std::string& login);

    /**
     * @brief Преобразовать обычного пользователя в модератора
     * @param login Логин пользователя
     * @param intelligence Разумность нового модератора
     * @return true если преобразование успешно
     */
    bool promote_to_moderator(const std::string& login, unsigned intelligence = 5);

    /**
     * @brief Перевести пользователя в статус ONLINE
     * @param login Логин пользователя
     * @return true если операция успешна
     */
    bool set_user_online(const std::string& login);

    /**
    * @brief Перевести пользователя в статус OFLINE
    * @param login Логин пользователя
    * @return true если операция успешна
    */
    bool set_user_offline(const std::string& login);

    /**
     * @brief Преобразовать пользователя в тролля
     * @param login Логин пользователя
     * @param width Толщина тролля
     * @return true если преобразование успешно
     */
    bool convert_to_troll(const std::string& login, float width = 0.5f);

    /**
     * @brief Преобразовать модератора/тролля в обычного пользователя
     * @param login Логин пользователя
     * @return true если преобразование успешно
     */
    bool convert_to_regular_user(const std::string& login);

    /**
     * @brief Проверить, является ли пользователь обычным
     * @param login Логин пользователя
     * @return true если пользователь - не модератор и не тролль
     */
    bool is_regular_user(const std::string& login) const;

    /**
     * @brief Получить толщину тролля
     * @param login Логин тролля
     * @return Толщина или 0.0f если не тролль
     */
    float get_troll_width(const std::string& login) const;

    /**
     * @brief Получить успешность тролля
     * @param login Логин тролля
     * @return Успешность или 0 если не тролль
     */
    int get_troll_success(const std::string& login) const;

    /**
     * @brief Получить разумность модератора
     * @param login Логин модератора
     * @return Разумность или 0 если не модератор
     */
    unsigned get_moderator_intelligence(const std::string& login) const;

    /**
     * @brief Получить размер хранилища
     * @return Общее количество пользователей
     */
    size_t size() const;

    /**
     * @brief Проверить, пусто ли хранилище
     * @return true если нет пользователей
     */
    bool empty() const;

    /**
     * @brief Очистить хранилище
     */
    void clear();

    /**
     * @brief Подсчитать пользователей онлайн
     * @return Количество пользователей со статусом ONLINE
     */
    size_t get_online_count() const;

    /**
     * @brief Подсчитать модераторов
     * @return Количество модераторов
     */
    size_t get_moderator_count() const;

    /**
     * @brief Подсчитать троллей
     * @return Количество троллей
     */
    size_t get_troll_count() const;

    /**
     * @brief Получить общее количество пользователей
     * @return Общее количество
     */
    size_t get_total_count() const;

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
};

#endif