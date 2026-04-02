#ifndef FORUMDATACONTAINER_H
#define FORUMDATACONTAINER_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @file ForumDataContainer.h
 * @brief Контейнеры для сериализации данных форума
 */

/**
 * @struct UserDataContainer
 * @brief Контейнер данных пользователя для сериализации(препобразования)/десериализации(восстановления)
 *
 */
struct UserDataContainer {
    std::string user_login_string; /// Логин пользователя
    std::string user_category_string; /// Тип пользователя
    std::chrono::system_clock::time_point registration_time_point; /// Дата регистрации
    size_t total_messages_count;   ///Количество сообщений напечатанных пользователем
    std::string current_activity_status; /// Статус пользователя

    int moderator_reason_level = 0; /// поле для разумности модератора, если пользователь модератор
    float troll_width_value = 0.0f; /// поле толщины тролля, если пользователь троль
    size_t troll_success_score = 0; /// поле успешности тролля, если юзер троль


    /**
     * @brief Сериализовывает контейнер в JSON
     * @return JSON объект с данными пользователя
     */
    json serialize_to_json() const;

    /**
     * @brief Десериализовывает контейнер из JSON
     * @param json_input JSON объект
     * @return Заполненный контейнер UserDataContainer
     */
    static UserDataContainer deserialize_from_json(const json& json_input);
};


/**
 * @struct SectionDataContainer
 * @brief Контейнер данных секции форума для сериализации/десериализации
 */
struct SectionDataContainer {
    std::string section_name_string; /// имя секции
    std::string initial_moderator_login_string; /// Логин главного модератора секции
    std::vector<std::string> participant_login_list; /// Список логинов всех участников


    /**
     * @brief Сериализовываетконтейнер в JSON
     * @return JSON объект с данными раздела
     */
    json serialize_to_json() const;

    /**
     * @brief Десериализовывает контейнер из JSON
     * @param json_input JSON-объект
     * @return Заполненный контейнер SectionDataContainer
     */
    static SectionDataContainer deserialize_from_json(const json& json_input);
};

/**
 * @struct ForumDataContainer
 * @brief Контейнер всех данных форума для сериализации/десериализации
 * Объединяет данные всех юзеров и секций форума
 * Является корневым контейнером для сохранения и загрузки
 */
struct ForumDataContainer {
    std::map<std::string, UserDataContainer> user_data_collection; /// Хранение пользователей логин-строка + данные
    std::map<std::string, SectionDataContainer> section_data_collection; ///Хранение разделов название + данные

    /**
     * @brief Сериализовать весь форум в JSON
     * @return JSON объект с полным состоянием форума
     */
    json serialize_to_json() const;

    /**
     * @brief Десериализовать форум из JSON
     * @param json_input JSON объект
     * @return Заполненный контейнер ForumDataContainer
     */
    static ForumDataContainer deserialize_from_json(const json& json_input);
};

#endif
