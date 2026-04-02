// StatService.cpp
#include "StatService.h"
#include "ForumService.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <cmath>
#include <chrono>

StatService::StatService(std::shared_ptr<Forum> forum, std::shared_ptr<UserService> userService)
        : forum(forum), userService(userService) {}

void StatService::findMostSuccessfulTroll() {
    std::cout << "\n===Самый успешный тролль ===\n";

    auto all_users = userService->get_all_users();
    std::string most_success;
    float best_ration = 0.0;

    for (const auto& user : all_users) {
        std::string login = user->get_login();

        if (userService->is_troll(login) && !userService->is_banned(login)) {
                float success = userService->get_troll_success(login);
                float width = userService->get_troll_width(login);
                float successRate = success * (1.0 / width);

                if (successRate > best_ration) {
                    best_ration = successRate;
                    most_success = login;
                }
        }
    }

    if (!most_success.empty()) {
        std::cout << "Логин: " << most_success<< "\n";
        std::cout << "  Забанено пользователей : " << userService->get_troll_success(most_success) << "\n";
        std::cout << "  Толщина: " << userService->get_troll_width(most_success) << "\n";

    } else {
        std::cout << "На форуме нет активных троллей\n";
    }
}

void StatService::findMostActiveUser() {
    std::cout << "\n=== Поиск самого активного писателя ===\n";

    auto allUsers = userService->get_all_users();
    std::string most_active;
    size_t max_messages = 0;

    for (const auto& user : allUsers) {
        std::string login = user->get_login();
        if (!userService->is_banned(login)) {
            size_t messages = user->get_count_of_message();

            if (messages > max_messages) {
                max_messages = messages;
                most_active = login;
            }
        }
    }

    if (!most_active.empty()) {
        std::cout << "Самый активный Юзер:\n";
        std::cout << " Логин: " << most_active<< "\n";

        if (userService->is_moderator(most_active)) {
            std::cout << "Тип: Модератор \n";
        } else if (userService->is_troll(most_active)) {
            std::cout << "Тип: тролль \n";
        } else {
            std::cout << "Тип: обычный пользователь\n";
        }


    } else {
        std::cout << "На форуме нет активных писателей\n";
    }
}

void StatService::getSectionStats() {
    std::cout << "\n=== Статистика по разделам форума ===\n";

    auto allSections = forum->get_all_sections();

    if (allSections.empty()) {
        std::cout << "На форуме нет разделов\n";
        return;
    }

    for (const auto& [sectionName, section] : allSections) {
        std::cout << "~~~ Раздел: " << sectionName << "~~~~~~\n";
        std::cout << "  Модератор: " << section.get_login_initial_moderator() << "\n";

        auto participants = section.get_participant();
        std::cout << "  Участников: " << participants.size() << "\n";

        int moderator_count = 0;
        int troll_count = 0;
        int based_count = 0;
        size_t total_messages = 0;

        for (const auto& login : participants) {
                if (userService->is_moderator(login)) moderator_count++;
                else if (userService->is_troll(login)) troll_count++;
                else based_count++;

                auto user = userService->get_user(login);
                if (user) {
                    total_messages += user->get_count_of_message();
                }
        }

        std::cout << "  Модераторы: " << moderator_count << "\n";
        std::cout << "  Тролли: " << troll_count << "\n";
        std::cout << "  Обычные пользователи: " << based_count << "\n";
        std::cout << "  Всего сообщений: " << total_messages << "\n";

        if (participants.size() > 0) {
            float avg_messages = static_cast<float>(total_messages) / participants.size();
            std::cout << "  Среднее сообщений на участника: " << avg_messages << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "=== Конец статистики ===\n\n";
}
