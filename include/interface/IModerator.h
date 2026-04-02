#ifndef IMODERATOR_H
#define IMODERATOR_H

/**
 * @file IModerator.h
 * @brief Интерфейс модератора
 */

/**
 * @interface IModerator
 * @brief Интерфейс для модераторов форума
 */

class IModerator{
public:
    virtual ~IModerator() = default;

    /**
     * @brief Получить разумность модератора
     * @return Значение разумности [1-10]
     */
    virtual unsigned get_intelligence() const = 0;

    /**
     * @brief Установить разумность модератора
     * @param brain Новая разумность [1-10]
     * @throws std::invalid_argument если значение вне диапазона
     */
    virtual void set_intelligence(unsigned brain) = 0;

};

#endif
