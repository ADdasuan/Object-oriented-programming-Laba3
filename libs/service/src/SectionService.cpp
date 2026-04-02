#include "SectionService.h"
#include "Moderator.h"
#include "ITroll.h"
#include <random>
#include "EmergencyAllModeratorBannedStrategy.h"

SectionService::SectionService(std::shared_ptr<Forum> forum_ptr) : forum(forum_ptr) {
    if (!forum) {
        throw std::invalid_argument("Forum cannot be null");
    }

    currentStrategy = std::make_shared<EmergencyAllModeratorBannedStrategy>();
}

bool SectionService::add_user_to_section(const std::string& section_name, const std::string& login) {
    auto *section = forum->find_section(section_name);
    if (!section) {
        std::cout << "Раздел '" << section_name << "' не найден\n";
        return false;
    }

    auto storage = forum->get_ptr_storage();
    if (!storage) {
        std::cout << "Хранилище не доступно\n";
        return false;
    }

    if (!storage->find_user(login)) {
        std::cout << "Пользователь '" << login << "' не найден\n";
        return false;
    }

    if (storage->is_banned(login)) {
        std::cout << "Пользователь '" << login << "' забанен и не может быть добавлен\n";
        return false;
    }

    if (user_in_any_section(login)) {
        std::cout << "Пользователь '" << login << "' уже состоит в другом разделе\n";
        return false;
    }

    if (section->is_participant_exist(login)) {
        std::cout << "Пользователь '" << login << "' уже состоит в этом разделе\n";
        return false;
    }

    try {
        section->add_participant(login);

        if (!storage->is_online(login)) {
            if (storage->set_user_online(login)) {
                std::cout << "Статус пользователя '" << login << "' изменен на ONLINE\n";
            }
        }

        return true;

    } catch (const std::exception &e) {
        std::cout << "Ошибка при добавлении пользователя: " << e.what() << "\n";
        return false;
    }
}

bool SectionService::del_user_from_section(const std::string& section_name, const std::string& login) {
    auto* section = forum->find_section(section_name);
    if (!section) {
        std::cout << "Раздел '" << section_name << "' не найден\n";
        return false;
    }

    if (!section->is_participant_exist(login)) {
        std::cout << "Пользователь не состоит в этом разделе \n";
        return false;
    }

    try {
        auto participants_before = section->get_participant();

        section->del_participant(login);
        std::cout << "Участник '" << login << "' удален из раздела '" << section_name << "'\n";

        auto storage = forum->get_ptr_storage();
        if (!storage) return true;

        storage->set_user_offline(login);
        std::cout << "  Статус пользователя изменен на OFFLINE\n";

        if (storage->is_banned(login)) {
            bool was_moderator = storage->is_moderator(login);

            if (section->section_is_empty()) {
                std::cout << "Секция '" << section_name << "' пуста - удаление...\n";
                forum->del_section(section_name);
                //std::cout << "✓ Раздел '" << section_name << "' удален\n";
            } else if (was_moderator) {
                check_and_fix_section_moderators(section_name);
            }
        } else {
            if (section->section_is_empty()) {
                for (const auto& participant : participants_before) {
                    if (participant != login) {
                        storage->set_user_offline(participant);
                    }
                }
                forum->del_section(section_name);
                std::cout << "Раздел '" << section_name << "' удален (пустой)\n";
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
        return false;
    }
}

bool SectionService::move_user_to_section(const std::string& login, const std::string& from_section, const std::string& to_section) {
    bool is_moderator = false;
    auto storage = forum->get_ptr_storage();
    if (storage) {
        is_moderator = storage->is_moderator(login);
    }

    if (!del_user_from_section(from_section, login)) {
        return false;
    }

    if (!add_user_to_section(to_section, login)) {
        add_user_to_section(from_section, login);
        return false;
    }

    if (is_moderator) {
        check_and_fix_section_moderators(from_section);
    }
    return true;
}

bool SectionService::user_in_any_section(const std::string& login) {
    if (login.empty()) return false;

    auto all_sections = forum->get_all_sections();

    for (const auto& [name, section] : all_sections) {
        if (section.is_participant_exist(login)) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> SectionService::get_section_users(const std::string& section_name) {
    if (section_name.empty()) {
        return {};
    }

    Section* section = forum->find_section(section_name);
    if (!section) {
        return {};
    }

    return section->get_participant();
}

std::string SectionService::promote_random_moderator(const std::string& section_name) {
    if (!currentStrategy) {
        std::cerr << "No promotion strategy set" << std::endl;
        return "";
    }

    return currentStrategy->promote_to_moderator(forum, section_name);
}

bool SectionService::check_and_fix_section_moderators(const std::string& section_name) {
    auto* section = forum->find_section(section_name);
    if (!section) return false;

    auto storage = forum->get_ptr_storage();
    if (!storage) return false;

    bool has_active_moderator = false;
    bool has_based_users = false;
    bool has_troll = false;
    auto participants = section->get_participant();

    for (const auto& login : participants) {
        auto user = storage->get_user(login);
        if (!user) continue;

        if (storage->is_banned(login)) continue;

        if (storage->is_moderator(login)) {
            has_active_moderator = true;
        } else if (storage->is_troll(login)) {
            has_troll = true;
        } else {
            has_based_users = true;
        }
    }

    if (has_active_moderator) {
        return true;
    }

    if (has_based_users) {
        std::cout << "\n[SectionService] В разделе '" << section_name
                  << "' нет активных модераторов!\n";

        std::string new_moderator = promote_random_moderator(section_name);

        if (!new_moderator.empty()) {
            std::cout << "[SectionService] ✓ Назначен новый модератор: "
                      << new_moderator << "\n";
            return true;
        } else {
            std::cout << "[SectionService] ✗ Не удалось назначить модератора\n";
            return false;
        }
    }

    if (has_troll) {
        std::cout << "\n В разделе остались только тролли - удаление раздела\n";

        for (const auto& login : participants) {
            if (storage->is_troll(login) && !storage->is_banned(login)) {
                storage->set_user_offline(login);
            }
        }

        forum->del_section(section_name);
        return true;
    }

    if (participants.empty()) {
        forum->del_section(section_name);
        return true;
    }
    return false;
}

size_t SectionService::check_all_sections_for_moderators() {
    auto all_sections = forum->get_all_sections();
    size_t fixed_count = 0;

    std::cout << "\n=== ПРОВЕРКА ВСЕХ РАЗДЕЛОВ НА НАЛИЧИЕ МОДЕРАТОРОВ ===\n";

    for (const auto& [section_name, section] : all_sections) {
        std::cout << "\n--- Раздел: " << section_name << " ---\n";

        if (check_and_fix_section_moderators(section_name)) {
            fixed_count++;
        }
    }

    std::cout << "\n=== РЕЗУЛЬТАТЫ ПРОВЕРКИ ===\n";
    std::cout << "Проверено разделов: " << all_sections.size() << "\n";
    std::cout << "Исправлено разделов: " << fixed_count << "\n";

    return fixed_count;
}


bool SectionService::handle_user_banned(const std::string& login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;

    std::string user_section;
    auto all_sections = forum->get_all_sections();
    for (const auto& [section_name, section] : all_sections) {
        if (section.is_participant_exist(login)) {
            user_section = section_name;
            break;
        }
    }

    if (user_section.empty()) {
        return true;
    }

    auto* section = forum->find_section(user_section);
    if (!section) return false;

    section->del_participant(login);
    std::cout << "Пользователь '" << login << "' удален из раздела '" << user_section << "'\n";

    std::string main_moderator = section->get_login_initial_moderator();
    bool was_main_moderator = (login == main_moderator);

    if (section->section_is_empty()) {
        forum->del_section(user_section);
        std::cout << "Раздел '" << user_section << "' удален (пустой)\n";
        return true;
    }

    if (was_main_moderator) {
        return handle_main_moderator_banned(user_section, login);
    }

    return true;
}

bool SectionService::handle_main_moderator_banned(const std::string& section_name, const std::string& banned_moderator) {
    auto* section = forum->find_section(section_name);
    if (!section) return false;

    auto storage = forum->get_ptr_storage();
    if (!storage) return false;

    auto participants = section->get_participant();

    std::vector<std::string> active_moderators;
    bool has_regular_users = false;
    bool has_trolls = false;

    for (const auto& login : participants) {
        if (storage->is_banned(login)) continue;

        if (storage->is_moderator(login)) {
            active_moderators.push_back(login);
        } else if (storage->is_troll(login)) {
            has_trolls = true;
        } else {
            has_regular_users = true;
        }
    }

    if (!active_moderators.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, active_moderators.size() - 1);
        std::string new_main = active_moderators[dist(gen)];

        section->set_initial_moderator_login(new_main);
        std::cout << "[SectionService] Главным модератором раздела '" << section_name << "' назначен: " << new_main << "\n";
        return true;
    }

    if (has_regular_users) {
        std::cout << "\n[SectionService] В разделе '" << section_name << "' нет активных модераторов, щас будем нового назначать...\n";

        std::string new_moderator = promote_random_moderator(section_name);

        if (!new_moderator.empty()) {
            section->set_initial_moderator_login(new_moderator);
            std::cout << "[SectionService] Назначен новый модератор: "
                      << new_moderator << "\n";
            return true;
        }
        return false;
    }

    if (has_trolls) {
        std::cout << "\n[SectionService] В разделе '" << section_name << "' остались только тролли! Анлак - удаляется раздел...\n";

        for (const auto& login : participants) {
            if (storage->is_troll(login) && !storage->is_banned(login)) {
                storage->set_user_offline(login);
                std::cout << "  Тролль '" << login << "' переведен в offline\n";
            }
        }

        forum->del_section(section_name);
        return true;
    }

    return false;
}

void SectionService::set_strategy(std::shared_ptr<PromotionStrategy> strategy) {
    currentStrategy = strategy;
}

