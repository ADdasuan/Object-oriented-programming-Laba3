#ifndef FORUMSERVICE_H
#define FORUMSERVICE_H

#include "Forum.h"

/**
 * @file ForumService.h
 * @brief Сервис для работы с форумом в целом
 */

/**
 * @class ForumService
 * @brief Управляет операциями с разделами и общей статистикой форума
 * Отвечает за создание/удаление разделов, получение информации
 * о пользователях разделов
 */
class ForumService{
private:
    std::shared_ptr<Forum> forum; /// Указатель на объект форум
public:

    ForumService() = default;


    /**
     * @brief Конструктор с инициализацией форума
     * @param forum Указатель на форум
     * @throws std::invalid_argument если forum == nullptr
     */
    ForumService(std::shared_ptr<Forum> forum);

    /**
     * @brief создать новая секция
     * @param name_of_section название секции
     * @param moder_login логин модератора секции
     * @return true если секция  создана
     */
    bool create_section(const std::string& name_of_section,const std::string& moder_login);

    /**
     * @brief Удалить секцию
     * @param name_of_section название секции
     * @return true если раздел удален
     */
    bool del_section(const std::string& name_of_section);

    /**
     * @brief Получить указатель на секцию
     * @param name_of_section название секции
     * @return Указатель на секцию или nullptr
     */
    Section* get_section(const std::string& name_of_section);

    /**
     * @brief Получить указатель на секцию const
     * @param name_of_section название секции
     * @return Константный указатель на секцию или nullptr
     */
    const Section* get_section_const(const std::string& name_of_section) const;

    /**
     * @brief Проверить существование секции
     * @param name_of_section название секции
     * @return true если секция существует
     */
    bool section_exists(const std::string& name_of_section) const;

    /**
     * @brief Проверить, все ли модераторы забанены
     * @param section_name название секции
     * @return true если все модераторы забанены
     */
    bool are_all_moderators_banned(const std::string& section_name) const;

    /**
     * @brief Проверить наличие активных модераторов в секции
     * @param section_name Название секции
     * @return true если есть активные модераторы
     */
    bool exist_active_moderators(const std::string& section_name) const;

    /**
     * @brief Вычислить суммарную разумность модераторов для расчета бана
     * @param section_name Название секции
     * @return Сумма intelligence всех онлайн модераторов
     */
    int getOnlineModeratorSum(const std::string& section_name) const;

    /**
     * @brief Вычислить обратную сумму толщины троллей на вероятность бана
     * @param section_name Название секции
     * @return Сумма 1:width всех онлайн троллей
     */
    float getOnlineTrollInverseSum(const std::string& section_name) const;

    /**
     * @brief Получить всех онлайн пользователей секции
     * @param section_name название секции
     * @return Вектор логинов пользователей со статусом ONLINE
     */
    std::vector<std::string> getOnlineUsers(const std::string& section_name) const;

    /**
     * @brief Получить всех онлайн модераторов секции
     * @param section_name Название секции
     * @return Вектор логинов модераторов со статусом ONLINE
     */
    std::vector<std::string> getOnlineModerators(const std::string& section_name) const;

    /**
     * @brief Получить всех онлайн троллей секции
     * @param section_name Название секции
     * @return Вектор логинов троллей со статусом ONLINE
     */
    std::vector<std::string> getOnlineTrolls(const std::string& section_name) const;

    /**
     * @brief Получить количество секций на форуме
     * @return Количество секций
     */
    size_t get_section_count() const;

    /**
     * @brief Получить количество пользователей в секции
     * @param name_of_section Название секции
     * @return Количество юзеров секции
     */
    size_t get_user_count_in_section(const std::string& name_of_section) const;

};

#endif
