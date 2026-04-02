#ifndef MODERATOR_H
#define MODERATOR_H

#include "DefaultUser.h"
#include "IModerator.h"

/**
 * @file Moderator.h
 * @brief Класс модератора форума
 */

/**
 * @class Moderator
 * @brief Модератор форума
 */

class Moderator : public DefaultUser , public IModerator{
private:
    unsigned intelligence; /// Разумность модератора [1-10]
public:
    /**
     * @brief Полный конструктор модератора
     * @param login Логин модератора
     * @param date Дата регистрации
     * @param message Количество сообщений
     * @param intelligence Разумность модератора [1-10]
     * @param status Статус модератора (по умолчанию OFFLINE)
     */
    Moderator(const std::string& login, const std::chrono::system_clock::time_point& date, size_t message, unsigned intelligence = 1,
              UserStatus status = UserStatus::OFFLINE);

    /**
     * @brief Упрощенный конструктор модератора
     * @param login Логин модератора
     * @param intelligence Разумность [1-10] - по умолчанию 1
     */
    Moderator(const std::string& login, unsigned int intelligence = 1);

    /**
     * @brief Получить разумность модератора
     * @return Значение разумности [1-10]
     */
    unsigned get_intelligence() const override {return intelligence;}

    /**
     * @brief Установить разумность модератора
     * @param brain Новое значение разумности [1-10]
     * @throws std::invalid_argument если значение вне допустимого диапазона
     */
    void set_intelligence(unsigned brain) override;

};


#endif
