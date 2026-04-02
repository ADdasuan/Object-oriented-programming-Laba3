#include <iostream>
#include <string>
#include <chrono>
#include "UserStatus.h"


#ifndef IUSER_H
#define IUSER_H


/**
 * @file IUser.h
 * @brief Интерфейс базового пользователя
 */

/**
 * @interface IUser
 * @brief Базовый интерфейс для всех типов пользователей
 */
class IUser{
public:
    virtual ~IUser() = default;

    /**
     * @brief Получить логин пользователя
     * @return Логин в виде строки
     */
    virtual std::string get_login() const = 0;

    /**
     * @brief Получить дату регистрации
     * @return Временная точка регистрации
     */
    virtual std::chrono::system_clock::time_point get_date() const = 0;

    /**
     * @brief Получить количество сообщений
     * @return Количество написанных сообщений
     */
    virtual size_t get_count_of_message() const = 0;

    /**
     * @brief Получить текущий статус
     * @return Статус пользователя
     */
    virtual UserStatus get_status() const = 0;


    /**
     * @brief Установить новый логин
     * @param new_login Новый логин
     */
    virtual void set_login(std::string new_login) = 0;


    /**
     * @brief Установить количество сообщений
     * @param count Количество сообщений
     */
    virtual void set_count_of_message(size_t count) = 0;


    /**
     * @brief Установить статус пользователя
     * @param status Новый статус
     */
    virtual void set_status(UserStatus status) = 0;


    /**
     * @brief Установить дату регистрации
     * @param new_date Новая дата
     */
    virtual void set_date(std::chrono::system_clock::time_point new_date) = 0;

};



#endif
