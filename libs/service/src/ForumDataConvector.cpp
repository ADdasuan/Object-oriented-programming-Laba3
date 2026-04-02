#include "ForumDataConvector.h"
#include "User.h"
#include "Moderator.h"
#include "Troll.h"
#include "StorageUsers.h"
#include <iostream>

ForumDataContainer ForumDataConverter::convert_forum_to_data_container(
        const Forum& forum_instance) const {

    ForumDataContainer container;

    auto storage = forum_instance.get_ptr_storage();
    if (storage) {
        for (const auto& [login, user_ptr] : *storage) {
            if (user_ptr) {
                container.user_data_collection[login] =
                        convert_user_to_data_container(user_ptr);
            }
        }
    }

    auto sections = forum_instance.get_all_sections();
    for (const auto& [name, section] : sections) {
        container.section_data_collection[name] =
                convert_section_to_data_container(section);
    }

    return container;
}

UserDataContainer ForumDataConverter::convert_user_to_data_container(
        std::shared_ptr<IUser> user_pointer) const {

    if (!user_pointer) {
        throw std::invalid_argument("User pointer is null");
    }

    UserDataContainer container;
    container.user_login_string = user_pointer->get_login();
    container.registration_time_point = user_pointer->get_date();
    container.total_messages_count = user_pointer->get_count_of_message();

    switch (user_pointer->get_status()) {
        case UserStatus::ONLINE:
            container.current_activity_status = "online";
            break;
        case UserStatus::OFFLINE:
            container.current_activity_status = "offline";
            break;
        case UserStatus::BANNED:
            container.current_activity_status = "banned";
            break;
    }

    if (auto moderator = std::dynamic_pointer_cast<IModerator>(user_pointer)) {
        container.user_category_string = "forum_moderator";
        container.moderator_reason_level = moderator->get_intelligence();
    }
    else if (auto troll = std::dynamic_pointer_cast<ITroll>(user_pointer)) {
        container.user_category_string = "forum_troll";
        container.troll_width_value = troll->get_width();
        container.troll_success_score = troll->get_success();
    }
    else {
        container.user_category_string = "regular_user";
    }

    return container;
}

SectionDataContainer ForumDataConverter::convert_section_to_data_container(
        const Section& section_instance) const {

    SectionDataContainer container;
    container.section_name_string = section_instance.get_name();
    container.initial_moderator_login_string =
            section_instance.get_login_initial_moderator();
    container.participant_login_list = section_instance.get_participant();

    return container;
}

std::shared_ptr<Forum> ForumDataConverter::convert_data_container_to_forum(const ForumDataContainer& data_container) const {

    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();

    for (const auto& [login, user_data] : data_container.user_data_collection) {
        try {
            auto user = convert_data_container_to_user(user_data);
            if (user) {
                storage->add_user(user);
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка загрузки пользователя '" << login
                      << "': " << e.what() << std::endl;
        }
    }

    forum->set_storage(storage);

    for (const auto& [name, section_data] : data_container.section_data_collection) {
        try {
            if (!storage->find_user(section_data.initial_moderator_login_string)) {
                std::cerr << "Модератор секции '" << name
                          << "' не найден, пропускаем раздел" << std::endl;
                continue;
            }

            if (forum->add_section(name, section_data.initial_moderator_login_string)) {
                auto* section = forum->find_section(name);
                if (section) {
                    for (const auto& participant_login : section_data.participant_login_list) {
                        if (participant_login != section_data.initial_moderator_login_string) {
                            try {
                                section->add_participant(participant_login);
                                auto user = storage->get_user(participant_login);
                                if (user && user->get_status() == UserStatus::ONLINE) {
                                    storage->set_user_online(participant_login);
                                }
                            } catch (const std::exception& e) {
                                std::cerr << "Не удалось добавить участника '"
                                          << participant_login << "' в раздел '"
                                          << name << "'" << std::endl;
                            }
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка загрузки раздела '" << name
                      << "': " << e.what() << std::endl;
        }
    }

    return forum;
}

std::shared_ptr<IUser> ForumDataConverter::convert_data_container_to_user(
        const UserDataContainer& user_data) const {

    UserStatus status = UserStatus::OFFLINE;
    if (user_data.current_activity_status == "online") {
        status = UserStatus::ONLINE;
    } else if (user_data.current_activity_status == "banned") {
        status = UserStatus::BANNED;
    }

    std::shared_ptr<IUser> user;

    if (user_data.user_category_string == "forum_moderator") {
        user = std::make_shared<Moderator>(user_data.user_login_string,user_data.registration_time_point,user_data.total_messages_count,
                user_data.moderator_reason_level,
                status
        );
    }
    else if (user_data.user_category_string == "forum_troll") {
        auto troll = std::make_shared<Troll>(user_data.user_login_string,user_data.registration_time_point,
                user_data.troll_width_value,
                status
        );
        troll->set_count_of_message(user_data.total_messages_count);
        troll->set_success(user_data.troll_success_score);
        user = troll;
    }
    else {
        user = std::make_shared<User>(user_data.user_login_string, user_data.registration_time_point,
                user_data.total_messages_count,
                status
        );
    }
    return user;
}

Section ForumDataConverter::convert_data_container_to_section(const SectionDataContainer& section_data) const {
    Section section(
            section_data.section_name_string,
            section_data.initial_moderator_login_string
    );

    for (const auto& participant : section_data.participant_login_list) {
        if (participant != section_data.initial_moderator_login_string) {
            section.add_participant(participant);
        }
    }

    return section;
}


