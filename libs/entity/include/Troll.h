
#ifndef TROLL_H
#define TROLL_H

#include "DefaultUser.h"
#include "ITroll.h"

/**
 * @file Troll.h
 * @brief Класс тролля форума
 */

/**
 * @brief Класс тролля
 *
 * Представляет тролля на форуме с характеристиками толщины и успешности.
 */

class Troll : public DefaultUser , public ITroll{
private:
    float width;  /// Коэффициент толщины тролля [0.1, 1.0]
    size_t success_count; /// Количество забаненных пользователей в присутствии тролля
public:
     /**
     * @brief Полный конструктор тролля
     * @param login Логин тролля
     * @param date Дата регистрации
     * @param size Коэффициент толщины
     * @param status Статус тролля
     */
    Troll(const std::string& login, const std::chrono::system_clock::time_point& date, float size, UserStatus status);

    /**
     * @brief Упрощенный конструктор тролля
     * @param login Логин тролля
     * @param size Коэффициент толщины
     */
    Troll(const std::string& login, float size);

    /**
     * @brief Получить толщину тролля
     * @return Коэффициент толщины [0.1-1.0]
     */
    float get_width() const override {return width;}

    /**
    * @brief Установить толщину тролля
    * @param new_width Новая толщина [0.1-1.0]
    * @throws std::invalid_argument если значение вне допустимого диапазона
    */
    void set_width(float new_width) override;

    /**
     * @brief Получить успешность тролля
     * @return Количество пользователей, забаненных в его присутствии
     */
    size_t get_success() const override {return success_count;}

    /**
     * @brief Установить успешность тролля
     * @param success Новое значение успешности
     */
    void set_success(size_t success) override {success_count = success;}

    ~Troll() = default;
};


#endif
