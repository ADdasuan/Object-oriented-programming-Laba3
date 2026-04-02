#ifndef USER_H
#define USER_H

#include "DefaultUser.h"

/**
 * @file User.h
 * @brief Класс обычного пользователя форума
 */

/**
 * @class User
 * @brief Обычный пользователь без специальных параметров
 */

class User : public DefaultUser{
public:
    /**
     * @brief Полный конструктор обычного пользователя
     * @param login Логин пользователя
     * @param date Дата регистрации
     * @param message_count Количество сообщений
     * @param status Статус пользователя
     */
    User(const std::string& login, const std::chrono::system_clock::time_point& date,
         size_t message_count, UserStatus status);

    /**
     * @brief Упрощенный конструктор
     * @param login Логин пользователя
     * Создает пользователя с текущей датой и статусом OFFLINE
     */
    explicit User(const std::string& login);
};

#endif
