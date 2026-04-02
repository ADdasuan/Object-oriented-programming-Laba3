#ifndef EMERGENCYALLMODERATORBANNEDSTRATEGY_H
#define EMERGENCYALLMODERATORBANNEDSTRATEGY_H

#include "PromotionStrategy.h"
/**
 * @file EmergencyAllModeratorBannedStrategy.h
 * @brief Экстренная стратегия при бане всех модераторов
 */

/**
 * @class EmergencyAllModeratorBannedStrategy
 * @brief Стратегия назначения модератора в экстренной ситуации
 */
class EmergencyAllModeratorBannedStrategy : public PromotionStrategy {
public:
    /**
     * @brief Назначает модератором случайного базового юзера
     * @param forum Указатель на форум
     * @param section_name Название раздела
     * @return строку std::string
     */
    std::string promote_to_moderator(std::shared_ptr<Forum> forum, const std::string& section_name) override;

};

#endif
