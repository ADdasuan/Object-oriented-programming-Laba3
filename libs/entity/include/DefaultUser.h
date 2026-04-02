#ifndef DEFAULTUSER_H
#define DEFAULTUSER_H

#include <chrono>
#include "IUser.h"
#include "UserStatus.h"

/**
 * @file DefaultUser.h
 * @brief Базовый класс для всех типов пользователей форума
 */

/**
 * @class DefaultUser
 * @brief Базовый класс пользователя форума
 */

class DefaultUser: public IUser{
protected:
    std::string login; /// Уникальный логин пользователя
    std::chrono::system_clock::time_point date; /// Дата и время регистрации
    size_t count_of_message; /// Количество написанных сообщений
    UserStatus status; /// Текущий статус ONLINE/OFFLINE/BANNED
public:
    /**
 * @brief Упрощенный конструктор пользователя
 * @param login Логин пользователя
 */
    explicit DefaultUser(const std::string& login);

    /**
 * @brief Полный конструктор пользователя
 * @param login Логин пользователя
 * @param date Дата регистрации
 * @param message_count Количество сообщений
 * @param status Статус пользователя
 */
    DefaultUser(const std::string& login,const  std::chrono::system_clock::time_point& date, size_t count_of_message = 0, UserStatus status = UserStatus::OFFLINE);

    /**
     * @brief Получить логин пользователя
     * @return Логин в виде строки
     */
    std::string get_login() const override {return login;}

    /**
     * @brief Получить дату регистрации
     * @return Дата регистрации
     */
    std::chrono::system_clock::time_point get_date() const override {return date;}

    /**
     * @brief Получить количество сообщений
     * @return Количество написанных сообщений
     */
    size_t get_count_of_message() const override {return count_of_message;}

    /**
     * @brief Получить текущий статус
     * @return Статус пользователя ONLINE/OFFLINE/BANNED
     */
    UserStatus get_status() const override {return status;}


    /**
     * @brief Установить новый логин
     * @param new_login Новый логин
     */
    void set_login(std::string new_login) override {login = new_login;}

    /**
     * @brief Установить дату регистрации
     * @param new_date Новая дата
     */
    void set_date(std::chrono::system_clock::time_point new_date) override {date = new_date;}

    /**
     * @brief Установить количество сообщений
     * @param count Новое количество сообщений
     */
    void set_count_of_message(size_t count) override { count_of_message = count; }

    /**
     * @brief Установить статус пользователя
     * @param new_status Новый статус
     */
    void set_status(UserStatus new_status) override { status = new_status; }

    ~DefaultUser() = default;

};

#endif
