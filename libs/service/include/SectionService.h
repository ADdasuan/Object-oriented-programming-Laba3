#ifndef SECTIONSERVICE_H
#define SECTIONSERVICE_H

#include "Forum.h"
#include "PromotionStrategy.h"

/**
 * @file SectionService.h
 * @brief Сервис для управления участниками секции
 */

/**
 * @class SectionService
 * @brief Управляет добавлением/удалением пользователей в секции
 */
class SectionService {
private:
    std::shared_ptr<Forum> forum; /// Указатель на форум
    std::shared_ptr<PromotionStrategy> currentStrategy; /// Стратегия назначения модераторов
public:

    /**
     * @brief Получить указатель на форум
     * @return Указатель на объект форума
     */
    std::shared_ptr<Forum> get_forum() const { return forum; }

    /**
     * @brief Конструктор с инициализацией форума
     * @param forum_ptr Указатель на форум
     * @throws std::invalid_argument если forum_ptr == nullptr
     */
    SectionService(std::shared_ptr<Forum> forum_ptr);

    /**
     * @brief Добавить пользователя в секцию
     * @param section_name Название секции
     * @param login Логин пользователя
     * @return true если добавление успешно
     */
    bool add_user_to_section(const std::string& section_name, const std::string& login);

    /**
     * @brief Удалить пользователя из секции
     * @param section_name Название секции
     * @param login Логин пользователя
     * @return true если удаление успешно
     */
    bool del_user_from_section(const std::string& section_name, const std::string& login);

    /**
     * @brief Переместить пользователя из одной секции в другую
     * @param login Логин пользователя
     * @param from_section Исходный секция
     * @param to_section Целевой секция
     * @return true если перемещение успешно
     */
    bool move_user_to_section(const std::string& login, const std::string& from_section, const std::string& to_section);


    /**
     * @brief Проверить, состоит ли пользователь в какой-то секции
     * @param login Логин пользователя
     * @return true если пользователь в какой-то секции
     */
    bool user_in_any_section(const std::string& login);

    /**
     * @brief Получить список участников секции
     * @param section_name Название секции
     * @return Вектор логинов участников
     */
    std::vector<std::string> get_section_users(const std::string& section_name);

    /**
     * @brief Проверить и восстановить наличие модераторов в секции
     * @param section_name Название секции
     * @return true если модераторы есть или успешно назначены
     */
    bool check_and_fix_section_moderators(const std::string& section_name);


    /**
     * @brief Проверить все секции форума на наличие модераторов
     * @return Количество исправленных секции
     */
    size_t check_all_sections_for_moderators();

    /**
     * @brief Назначить случайного пользователя модератором
     * @param section_name Название раздела
     * @return Логин нового модератора или пустая строка при ошибке
     */
    std::string promote_random_moderator(const std::string& section_name);

    /**
     * @brief Обработать бан пользователя
     * @param login Логин забаненного пользователя
     * @return true если обработка успешна
     */
    bool handle_user_banned(const std::string& login);

    /**
     * @brief Обработать бан главного модератора
     * @param section_name Название секции
     * @param banned_moderator Логин забаненного модератора
     * @return true если обработка успешна
     */
    bool handle_main_moderator_banned(const std::string& section_name, const std::string& banned_moderator);

    /**
     * @brief Установить стратегию назначения модераторов
     * @param strategy Новая стратегия
     */
    void set_strategy(std::shared_ptr<PromotionStrategy> strategy);
};

#endif