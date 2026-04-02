#ifndef ITROLL_H
#define ITROLL_H

/**
 * @file ITroll.h
 * @brief Интерфейс тролля
 */

/**
 * @interface ITroll
 * @brief Интерфейс для троллей форума
 */
class ITroll{
public:
    virtual ~ITroll() = default;

    /**
     * @brief Получить толщину тролля
     * @return Толщина [0.1-1.0]
     */
    virtual float get_width() const = 0;

    /**
     * @brief Установить толщину тролля
     * @param new_width Новая толщина [0.1-1.0]
     * @throws std::invalid_argument если значение вне диапазона
     */
    virtual void set_width(float new_width) = 0;

    /**
     * @brief Получить успешность тролля
     * @return Количество успешных банов
     */
    virtual size_t get_success() const = 0;

    /**
     * @brief Установить успешность тролля
     * @param new_success Новое значение успешности
     */
    virtual void set_success(size_t new_success) = 0;

};

#endif
