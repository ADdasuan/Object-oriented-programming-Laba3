
#ifndef FORUMDATACONVERTER_H
#define FORUMDATACONVERTER_H

#include <memory>
#include "ForumDataContainer.h"
#include "Forum.h"

/**
 * @file ForumDataConverter.h
 * @brief Конвертер между объектами форума и контейнерами данных
 */


/**
 * @class ForumDataConverter
 * @brief Преобразует объекты Forum в контейнеры и обратно
 *
 * Поддерживает двойное преобразование:
 * - Forum в ForumDataContainer - для сохранения
 * - ForumDataContainer в Forum - для загрузки
 */
class ForumDataConverter {
public:
    ForumDataConverter() = default;

    /**
     * @brief Конвертирует Forum в ForumDataContainer
     * @param forum_instance Объект форума для конвертации
     * @return Контейнер с данными форума
     */
    ForumDataContainer convert_forum_to_data_container(const Forum& forum_instance) const;

    /**
     * @brief Конвертирует ForumDataContainer в Forum
     * @param data_container Контейнер с данными
     * @return Указатель на восстановленный объект форума
     */
    std::shared_ptr<Forum> convert_data_container_to_forum(const ForumDataContainer& data_container) const;

public: //было привайт
    /**
     * @brief Конвертировать пользователя в контейнер
     * @param user_pointer Указатель на пользователя
     * @return Контейнер с данными пользователя
     * @throws std::invalid_argument если user_pointer == nullptr
     */
    UserDataContainer convert_user_to_data_container(std::shared_ptr<IUser> user_pointer) const;

    /**
     * @brief Конвертировать контейнер в объект пользователя
     * @param user_data Контейнер с данными пользователя
     * @return Указатель на созданный объект User/Moderator/Troll
     */
    std::shared_ptr<IUser> convert_data_container_to_user(const UserDataContainer& user_data) const;

    /**
     * @brief Конвертировать раздел в контейнер
     * @param section_instance Объект раздела
     * @return Контейнер с данными раздела
     */
    SectionDataContainer convert_section_to_data_container(const Section& section_instance) const;

    /**
     * @brief Конвертировать контейнер в объект раздела
     * @param section_data Контейнер с данными раздела
     * @return Объект Section
     */
    Section convert_data_container_to_section(const SectionDataContainer& section_data) const;
};

#endif