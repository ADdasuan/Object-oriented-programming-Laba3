#ifndef PROMOTIONSTRATEGY_H
#define PROMOTIONSTRATEGY_H

#include <memory>
#include <string>

class Forum;
/**
 * @file PromotionStrategy.h
 * @brief Базовый интерфейс стратегии назначения модераторов
 */

/**
 * @class PromotionStrategy
 * @brief Абстрактная стратегия назначения нового модератора
 */
class PromotionStrategy {
public:

    virtual ~PromotionStrategy() = default;

    /**
     * @brief Назначить нового модератора в секции
     * @param forum Указатель на форум
     * @param section_name Название секции
     * @return Логин нового модератора или пустая строка при неудаче
     */
    virtual std::string promote_to_moderator(std::shared_ptr<Forum> forum, const std::string& section_name) = 0;
};


#endif
