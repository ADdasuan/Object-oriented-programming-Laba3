#ifndef FORUM_H
#define FORUM_H

#include "StorageUsers.h"
#include "Section.h"
#include <unordered_map>

/**
 * @file Forum.h
 * @brief Главный класс форума
 */

/**
 * @brief Класс форума
 *
 * Хранит данные форума и предоставляет основные операции для работы с ними
 */

class Forum{
private:
    std::shared_ptr<StorageUsers> ptr_storage; /// Хранилище пользователей
    std::unordered_map<std::string, Section> sections; /// Разделы форума
public:
    Forum() = default;
    /**
 * @brief Получить все разделы форума
 * @return Карта разделов форума
 */
    std::unordered_map<std::string, Section> get_all_sections() const {return sections;}

    /**
 * @brief Получить указатель на хранилище пользователей
 * @return Умный указатель на хранилище пользователей
 */
    std::shared_ptr<StorageUsers> get_ptr_storage() const { return ptr_storage; }

 /**
 * @brief Установить хранилище пользователей
 * @param storage Указатель на хранилище пользователей
 */
    void set_storage(std::shared_ptr<StorageUsers> storage) {
        ptr_storage = storage;
    }

    /**
 * @brief Добавить раздел в форум
 * @param name_of_section Название раздела
 * @param moder_login Логин модератора раздела
 * @return true если раздел успешно добавлен, false в противном случае
 */
    bool add_section(const std::string& name_of_section, const std::string& moder_login);

    /**
     * @brief Удалить раздел форума
     * @param name_of_section Название раздела
     * @return true если раздел успешно удален
     */
    bool del_section(const std::string& name_of_section);

    /**
     * @brief Проверить существование раздела
     * @param name Название раздела
     * @return true если раздел существует
     */
    bool section_exist(const std::string& name) const;


    /**
     * @brief Найти раздел по названию
     * @param name Название раздела
     * @return Указатель на раздел или nullptr если не найден
     */
    Section* find_section(const std::string& name);

    /**
     * @brief Проверить существование пользователя в хранилище
     * @param login Логин пользователя
     * @return true если пользователь существует
     */
    bool user_exists_in_storage(const std::string& login) const;

    /**
     * @brief Получить название раздела, в котором находится пользователь
     * @param user_login Логин пользователя
     * @return Название раздела или пустая строка если не найдено
     */
    std::string get_user_section(const std::string& user_login) const;

    /**
     * @brief Проверить наличие активных модераторов в разделе
     * @param section_name Название раздела
     * @return true если есть хотя бы один активный модератор
     */
    bool has_active_moderators(const std::string& section_name) const;

    /**
     * @brief Подсчитать количество активных модераторов в разделе
     * @param section_name Название раздела
     * @return Количество активных модераторов
     */
    size_t count_active_moderators(const std::string& section_name) const;

    /**
     * @brief Подсчитать количество активных модераторов в разделе
     * @param section_name Название раздела
     * @return Количество активных модераторов
     */
    bool user_in_any_section(const std::string& moder_login);

    ~Forum() = default;
};

#endif
