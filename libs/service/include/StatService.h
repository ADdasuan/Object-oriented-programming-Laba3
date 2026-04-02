// StatService.h
#ifndef STATSERVICE_H
#define STATSERVICE_H

#include <memory>
#include <string>
#include "Forum.h"
#include "UserService.h"

/**
 * @class StatService
 * @brief Сервис для сбора и вывода статистики по форуму
 */

/**
 * @class StatService
 * @brief Собирает и выводит статистику по форуму
 */
class StatService {
private:
    std::shared_ptr<Forum> forum; /// Указатель на форум
    std::shared_ptr<UserService> userService; ///< Сервис пользователей для доступа к данным

public:
    /**
     * @brief Конструктор StatService
     * @param forum Общий объект форума
     * @param userService Сервис пользователей
     */
    explicit StatService(std::shared_ptr<Forum> forum, std::shared_ptr<UserService> userService);

    /**
     * @brief Находит и выводит информацию о самом успешном тролле
     */
    void findMostSuccessfulTroll();

    /**
     * @brief Находит и выводит информацию о самом активном писателе
     */
    void findMostActiveUser();

    /**
     * @brief Выводит общую статистику по всем разделам форума
     *
     * Включает:
     * - Название раздела
     * - Количество участников
     * - Главного модератора
     * - Распределение по типам пользователей
     * - Количество онлайн пользователей
     * - Уровень активности
     */
    void getSectionStats();
};

#endif
