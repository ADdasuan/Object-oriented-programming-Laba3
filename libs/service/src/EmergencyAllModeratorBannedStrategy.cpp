#include "EmergencyAllModeratorBannedStrategy.h"

#include "Forum.h"
#include "Section.h"
#include "StorageUsers.h"
#include "Moderator.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>

#define DEFAULT_BRAIN 5

std::string EmergencyAllModeratorBannedStrategy::promote_to_moderator(
        std::shared_ptr<Forum> forum,
        const std::string& section_name) {

    if (!forum) {
        std::cerr << "Forum is null in EmergencyAllModeratorBannedStrategy" << std::endl;
        return "";
    }

    auto storage = forum->get_ptr_storage();
    if (!storage) {
        std::cerr << "Storage is null in EmergencyAllModeratorBannedStrategy" << std::endl;
        return "";
    }

    auto* section = forum->find_section(section_name);
    if (!section) {
        std::cerr << "Section '" << section_name << "' not found" << std::endl;
        return "";
    }

    auto participants = section->get_participant();
    if (participants.empty()) {
        std::cerr << "Section '" << section_name << "' is empty, no one to promote" << std::endl;
        return "";
    }

    std::vector<std::string> eligible_users;

    for (const auto& login : participants) {
        try {
            auto user = storage->get_user(login);
            if (!user) {
                std::cerr << "User '" << login << "' not found in storage" << std::endl;
                continue;
            }

            if (user->get_status() == UserStatus::BANNED) {
                continue;
            }

            if (storage->is_moderator(login)) {
                continue;
            }

            if (storage->is_troll(login)) {
                continue;
            }

            eligible_users.push_back(login);

        } catch (const std::exception& e) {
            std::cerr << "checking user '" << login << "': " << e.what() << std::endl;
        }
    }

    if (eligible_users.empty()) {
        std::cerr << "No eligible users for promotion in section '" << section_name << "'" << std::endl;
        std::cerr << "  Total participants: " << participants.size() << std::endl;
        std::cerr << "  Reasons:" << std::endl;

        for (const auto& login : participants) {
            auto user = storage->get_user(login);
            if (!user) {
                std::cerr << "  - " << login << ": not found in storage" << std::endl;
                continue;
            }

            std::string reason;
            if (user->get_status() == UserStatus::BANNED) {
                reason = "banned";
            } else if (storage->is_moderator(login)) {
                reason = "already moderator";
            } else if (storage->is_troll(login)) {
                reason = "troll (moderators don't troll)";
            } else {
                reason = "eligible (but not selected?)";
            }
            std::cerr << "  - " << login << ": " << reason << std::endl;
        }

        return "";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(eligible_users.size()) - 1);

    std::string selected_login = eligible_users[dist(gen)];

    auto user_to_promote = storage->get_user(selected_login);
    if (!user_to_promote) {
        std::cerr << "Selected user '" << selected_login << "' not found" << std::endl;
        return "";
    }

    std::cout << "=== EMERGENCY PROMOTION ===" << std::endl;
    std::cout << "Section: " << section_name << std::endl;
    std::cout << "Promoting user: " << selected_login << std::endl;
    std::cout << "User type before: Ordinary user" << std::endl;

    try {
        auto new_moderator = std::make_shared<Moderator>(
                user_to_promote->get_login(),
                user_to_promote->get_date(),
                user_to_promote->get_count_of_message(),
                DEFAULT_BRAIN,
                user_to_promote->get_status()
        );

        if (storage->update_user(selected_login, new_moderator)) {
            std::cout << "SUCCESS: User '" << selected_login << "' promoted to moderator" << std::endl;
            std::cout << "New moderator intelligence: 5 (default)" << std::endl;

            if (!storage->set_user_online(selected_login)) {
                std::cerr << "Failed to set ONLINE status for new moderator" << std::endl;
            }

            if (section) {
                section->update_moderator_info(selected_login);
                std::cout << "Section moderator updated to: " << selected_login << std::endl;
            }

            if (storage->is_moderator(selected_login)) {
                std::cout << "User is now a moderator" << std::endl;
            } else {
                std::cerr << "User was updated but is_moderator() returns false" << std::endl;
            }

            return selected_login;
        } else {
            std::cerr << "Failed to update user in storage" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "creating moderator: " << e.what() << std::endl;
    }

    return "";
}