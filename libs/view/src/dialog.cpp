
#include "dialog.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>

DialogController::DialogController(const std::shared_ptr<Forum>& forum)
        : forum(forum),
          userService(std::make_shared<UserService>(forum)),
          sectionService(std::make_shared<SectionService>(forum)),
          forumService(std::make_shared<ForumService>(forum)),
          simulateService(std::make_shared<SimulateService>(forum, userService, sectionService, forumService)),
          statService(forum, userService){

}

void DialogController::waitForEnter() const {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string DialogController::readString(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int DialogController::readInt(const std::string& prompt, int min, int max) const {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите целое число\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < min || value > max) {
            std::cout << "Ошибка: число должно быть в диапазоне ["
                      << min << ", " << max << "]\n";
            continue;
        }

        break;
    }
    return value;
}

float DialogController::readFloat(const std::string& prompt, float min, float max) const {
    float value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < min || value > max) {
            std::cout << "Ошибка: число должно быть в диапазоне ["
                      << min << ", " << max << "]\n";
            continue;
        }

        break;
    }
    return value;
}

void DialogController::show_main_menu() const {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "       СИСТЕМА УПРАВЛЕНИЯ ФОРУМОМ       \n";
    std::cout << "========================================\n";
    std::cout << "1. Управление форумом\n";
    std::cout << "2. Управление пользователями\n";
    std::cout << "3. Управление разделами\n";
    std::cout << "4. Симуляция работы форума\n";
    std::cout << "5. Статистика и аналитика\n";
    std::cout << "6. Предсказание будущего\n";
    std::cout << "7. Сохранить/Загрузить состояние\n";
    std::cout << "0. Выход\n";
    std::cout << "========================================" << std::endl;
}

void DialogController::run() {

    while (true) {
        show_main_menu();
        int choice = readInt("", 0, 7);
        handle_main_menu_choice(choice);

        if (choice == 0) {
            std::cout << "\nДо свидания!\n";
            break;
        }
    }
}


void DialogController::handle_main_menu_choice(int choice) {
    switch (choice) {
        case 1: manageForumMenu(); break;
        case 2: manageUsersMenu(); break;
        case 3: manageSectionsMenu(); break;
        case 4: simulationMenu(); break;
        case 5: statisticsMenu(); break;
        case 6: predictionMenu(); break;
        case 7:saveLoadMenu(); break;
    }
}

void DialogController::manageForumMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== УПРАВЛЕНИЕ ФОРУМОМ ===\n";
        std::cout << "1. Создать новый раздел\n";
        std::cout << "2. Удалить раздел\n";
        std::cout << "3. Показать все разделы\n";
        std::cout << "4. Информация о форуме\n";
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Выберите опцию: ";

        int choice = readInt("", 0, 4);

        switch (choice) {
            case 1: createSectionDialog(); break;
            case 2: deleteSectionDialog(); break;
            case 3: showAllSections(); break;
            case 4: showForumStatistics(); break;
            case 0: return;
        }
    }
}

void DialogController::createSectionDialog() {
    clearScreen();
    std::cout << "=== СОЗДАНИЕ НОВОГО РАЗДЕЛА ===\n";

    std::string name = readString("Введите название раздела: ");
    std::string moderatorLogin = readString("Введите логин модератора: ");

    try {
        if (forumService->create_section(name, moderatorLogin)) {
            std::cout << "Раздел '" << name << "' успешно создан\n";
        } else {
            std::cout << "Не удалось создать раздел\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::deleteSectionDialog() {
    clearScreen();
    std::cout << "=== УДАЛЕНИЕ РАЗДЕЛА ===\n";

    std::string name = readString("Введите название раздела для удаления: ");

    try {
        if (forumService->del_section(name)) {
            std::cout << "Раздел '" << name << "' успешно удален\n";
        } else {
            std::cout << "Раздел не найден или не может быть удален\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::showAllSections() {
   // clearScreen();
    std::cout << "=== ВСЕ РАЗДЕЛЫ ФОРУМА ===\n";

    auto sections = forum->get_all_sections();
    if (sections.empty()) {
        std::cout << "На форуме нет разделов\n";
    } else {
        int i = 1;
        for (const auto& [name, section] : sections) {
            std::cout << i++ << ". " << name << " (модератор: "
                      << section.get_login_initial_moderator()
                      << ", участников: " << section.get_participant().size() << ")\n";
        }
    }

    //waitForEnter();
}

void DialogController::manageUsersMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== УПРАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯМИ ===\n";
        std::cout << "1. Зарегистрировать пользователя\n";
        std::cout << "2. Зарегистрировать модератора\n";
        std::cout << "3. Зарегистрировать тролля\n";
        std::cout << "4. Забанить пользователя\n";
        std::cout << "5. Разбанить пользователя\n";
        std::cout << "6. Изменить тип пользователя\n";
        std::cout << "7. Показать информацию о пользователе\n";
        std::cout << "8. Показать всех пользователей\n";
        std::cout << "0. Назад в главное меню\n";

        std::cout << "Выберите опцию: ";
        int choice = readInt("", 0, 8);

        switch (choice) {
            case 1: registerUserDialog(); break;
            case 2: registerModeratorDialog(); break;
            case 3: registerTrollDialog(); break;
            case 4: ban_user_dialog(); break;
            case 5: unban_user_dialog(); break;
            case 6: change_user_type_dialog(); break;
            case 7: show_user_info_dialog(); break;
            case 8: showAllUsers(); break;
            case 0: return;
        }
    }
}

void DialogController::registerUserDialog() {
    clearScreen();
    std::cout << "=== РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ ===\n";

    std::string login = readString("Введите логин: ");

    try {
        auto user = userService->register_user(login);
        if (user) {
            std::cout << "Пользователь '" << login << "' успешно зарегистрирован\n";
        } else {
            std::cout << "Не удалось зарегистрировать пользователя\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::registerModeratorDialog() {
    clearScreen();
    std::cout << "=== РЕГИСТРАЦИЯ МОДЕРАТОРА ===\n";

    std::string login = readString("Введите логин: ");
    int intelligence = readInt("Введите разумность (1-10): ", 1, 10);

    try {
        auto moderator = userService->register_moderator(login, intelligence);
        if (moderator) {
            std::cout << "Модератор '" << login << "' успешно зарегистрирован\n";
        } else {
            std::cout << "Не удалось зарегистрировать модератора\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::registerTrollDialog() {
    clearScreen();
    std::cout << "=== РЕГИСТРАЦИЯ ТРОЛЛЯ ===\n";

    std::string login = readString("Введите логин: ");
    float width = readFloat("Введите толщину (0.1-1.0): ", 0.1f, 1.0f);

    try {
        auto troll = userService->register_troll(login, width);
        if (troll) {
            std::cout << "Тролль '" << login << "' успешно зарегистрирован\n";
        } else {
            std::cout << "Не удалось зарегистрировать тролля\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::ban_user_dialog() {
    clearScreen();
    std::cout << "=== БАНИТЬ ПОЛЬЗОВАТЕЛЯ ===\n";

    std::string login = readString("Введите логин пользователя: ");

    try {
        // Проверяем, в каком разделе пользователь
        std::string user_section;
        auto all_sections = forum->get_all_sections();
        for (const auto& [section_name, section] : all_sections) {
            auto participants = section.get_participant();
            if (std::find(participants.begin(), participants.end(), login) != participants.end()) {
                user_section = section_name;
                break;
            }
        }

        if (!user_section.empty()) {
            std::cout << "Пользователь находится в разделе: " << user_section << "\n";
        }

        bool is_moderator = userService->is_moderator(login);
        if (is_moderator && !user_section.empty()) {
            // Проверяем, является ли главным модератором
            auto* section = forum->find_section(user_section);
            if (section && section->get_login_initial_moderator() == login) {
                std::cout << "Внимание: это главный модератор раздела!\n";
            }
        }

        if (userService->ban_user(login)) {
            std::cout << "Пользователь '" << login << "' забанен\n";
            if (!user_section.empty()) {
                sectionService->handle_user_banned(login);
            }

        } else {
            std::cout << "Не удалось забанить пользователя\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::show_user_info_dialog() {
    clearScreen();
    std::cout << "=== ИНФОРМАЦИЯ О ПОЛЬЗОВАТЕЛЕ ===\n";

    std::string login = readString("Введите логин пользователя: ");

    auto user = userService->get_user(login);
    if (!user) {
        std::cout << "Пользователь не найден\n";
        waitForEnter();
        return;
    }

    std::cout << "\n=== ДАННЫЕ ПОЛЬЗОВАТЕЛЯ ===\n";
    std::cout << "Логин: " << user->get_login() << "\n";
    std::cout << "Дата регистрации: " << formatTimePoint(user->get_date()) << "\n";
    std::cout << "Сообщений: " << user->get_count_of_message() << "\n";

    std::cout << "Статус: ";
    switch (user->get_status()) {
        case UserStatus::ONLINE: std::cout << " онлайн "; break;
        case UserStatus::OFFLINE: std::cout << " оффлайн "; break;
        case UserStatus::BANNED: std::cout << " забанен "; break;
    }
    std::cout << "\n";

    if (userService->is_moderator(login)) {
        std::cout << "Тип: Модератор\n";
        auto moderator = std::dynamic_pointer_cast<IModerator>(user);
        if (moderator) {
            std::cout << "Разумность: " << moderator->get_intelligence() << "\n";
        }
    } else if (userService->is_troll(login)) {
        std::cout << "Тип: Тролль\n";
        auto troll = std::dynamic_pointer_cast<ITroll>(user);
        if (troll) {
            std::cout << "Толщина: " << troll->get_width() << "\n";
            std::cout << "Успешность: " << troll->get_success() << " банов\n";
        }
    } else {
        std::cout << "Тип: Обычный пользователь\n";
    }

    waitForEnter();
}

void DialogController::manageSectionsMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== УПРАВЛЕНИЕ РАЗДЕЛАМИ ===\n";
        std::cout << "1. Добавить пользователя в раздел\n";
        std::cout << "2. Удалить пользователя из раздела\n";
        std::cout << "3. Переместить пользователя между разделами\n";
        std::cout << "4. Показать участников раздела\n";
        std::cout << "5. Показать статистику раздела\n";
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Выберите опцию: ";

        int choice = readInt("", 0, 5);

        switch (choice) {
            case 1: addUserToSectionDialog(); break;
            case 2: removeUserFromSectionDialog(); break;
            case 3: moveUserToSectionDialog(); break;
            case 4: showSectionUsersDialog(); break;
            case 5: showSectionStateDialog(); break;
            case 0: return;
        }
    }
}

void DialogController::addUserToSectionDialog() {
    clearScreen();
    std::cout << "=== ДОБАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯ В РАЗДЕЛ ===\n";

    std::string sectionName = readString("Введите название раздела: ");
    std::string userLogin = readString("Введите логин пользователя: ");

    try {
        if (sectionService->add_user_to_section(sectionName, userLogin)) {
            std::cout << "Пользователь '" << userLogin
                      << "' добавлен в раздел '" << sectionName << "'\n";
        } else {
            std::cout << "Не удалось добавить пользователя в раздел\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::simulationMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== СИМУЛЯЦИЯ РАБОТЫ ФОРУМА ===\n";
        std::cout << "1. Выполнить один шаг симуляции для раздела\n";
        std::cout << "2. Запустить полную симуляцию\n";
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Выберите опцию: ";

        int choice = readInt("", 0, 4);

        switch (choice) {
            case 1: simulateStepDialog(); break;
            case 2: runFullSimulation(); break;
            case 0: return;
        }
    }
}

void DialogController::simulateStepDialog() {
    clearScreen();
    std::cout << "=== ШАГ СИМУЛЯЦИИ РАЗДЕЛА ===\n";

    std::string sectionName = readString("Введите название раздела: ");

    simulateService->simulateStep(sectionName);

    waitForEnter();
}

void DialogController::predictSectionFutureDialog() {
    clearScreen();
    std::cout << "=== ПРЕДСКАЗАНИЕ БУДУЩЕГО РАЗДЕЛА ===\n";

    std::string sectionName = readString("Введите название раздела: ");

    simulateService->predictionExistenceFuture(sectionName);

    waitForEnter();
}

void DialogController::statisticsMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== СТАТИСТИКА И АНАЛИТИКА ===\n";
        std::cout << "1. Самый успешный тролль\n";
        std::cout << "2. Самый активный юзер (по сообщениям)\n";
        std::cout << "3. Статистика разделам\n";
        std::cout << "Выберите опцию: ";

        int choice = readInt("", 0, 5);

        switch (choice) {
            case 1:
                statService.findMostSuccessfulTroll();
                waitForEnter();
                break;
            case 2:
                statService.findMostActiveUser();
                waitForEnter();
                break;
            case 3:
                statService.getSectionStats();
                waitForEnter();
                break;
            case 0: return;
        }
    }
}

void DialogController::showMostSuccessfulTroll() {
    clearScreen();
    std::cout << "=== ПОИСК САМОГО УСПЕШНОГО ТРОЛЛЯ ===\n";
    statService.findMostSuccessfulTroll();
    waitForEnter();
}

void DialogController::showMostActiveUser() {
    clearScreen();
    std::cout << "=== ПОИСК САМОГО АКТИВНОГО ПИСАТЕЛЯ ===\n";
    statService.findMostActiveUser();
    waitForEnter();
}

void DialogController::showForumStatistics() {
    clearScreen();
    std::cout << "=== ОБЩАЯ СТАТИСТИКА ФОРУМА ===\n";
    statService.getSectionStats();
    waitForEnter();
}

void DialogController::clearScreen() const {

}

std::string DialogController::formatTimePoint(const std::chrono::system_clock::time_point& tp) const {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time_t);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}


void DialogController::saveStateDialog() {
    clearScreen();
    std::cout << "=== СОХРАНЕНИЕ СОСТОЯНИЯ ===\n";
    std::string filename = readString("Введите имя файла для сохранения в формате.json: ");

    if (filename.empty()) {
        std::cout << " Имя файла не может быть пустым\n";
        waitForEnter();
        return;
    }

    if (filename.find(".json") == std::string::npos) {
        filename += ".json";
    }

    try {
        auto converter = std::make_shared<ForumDataConverter>();
        Save saver(forum, converter);

        saver.save_forum_state_to_file(filename);

        std::cout << "\n Состояние форума успешно сохранено в файл: " << filename << "\n";

        auto storage = forum->get_ptr_storage();
        if (storage) {
            std::cout << "\nСохранено:\n";
            std::cout << "  - Пользователей: " << storage->size() << "\n";
            std::cout << "  - Разделов: " << forum->get_all_sections().size() << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "\n Ошибка при сохранении: " << e.what() << "\n";
    }
    waitForEnter();
}

void DialogController::loadStateDialog() {
    clearScreen();
    std::cout << "=== ЗАГРУЗКА СОСТОЯНИЯ ===\n";
    std::string filename = readString("Введите имя файла для загрузки: ");

    if (filename.empty()) {
        std::cout << "Имя файла не может быть пустым\n";
        waitForEnter();
        return;
    }

    std::cout << "\n Текущее состояние форума будет заменено!\n";

    try {
        auto converter = std::make_shared<ForumDataConverter>();
        Load loader(forum, converter);

        loader.load_forum_state_from_file(filename);

        std::cout << "\n Состояние форума успешно загружено из файла: " << filename << "\n";

        auto storage = forum->get_ptr_storage();
        if (storage) {
            std::cout << "\nЗагружено:\n";
            std::cout << "  - Пользователей: " << storage->size() << "\n";
            std::cout << "  - Модераторов: " << storage->get_moderator_count() << "\n";
            std::cout << "  - Троллей: " << storage->get_troll_count() << "\n";
            std::cout << "  - Разделов: " << forum->get_all_sections().size() << "\n";
            std::cout << "  - Онлайн: " << storage->get_online_count() << "\n";
        }

        userService = std::make_shared<UserService>(forum);
        sectionService = std::make_shared<SectionService>(forum);
        forumService = std::make_shared<ForumService>(forum);
        simulateService = std::make_shared<SimulateService>(forum, userService, sectionService, forumService);
        statService = StatService(forum, userService);

        std::cout << "\n Все сервисы обновлены\n";

    } catch (const std::exception& e) {
        std::cout << "\n Ошибка при загрузке: " << e.what() << "\n";
    }

    waitForEnter();
}



void DialogController::predictionMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== ПРЕДСКАЗАНИЕ БУДУЩЕГО И АНАЛИЗА ===\n";
        std::cout << "1. Предсказать будущее раздела\n";
        std::cout << "2. Предсказать для всех разделов с многопотоком\n";
        std::cout << "0. Назад в главное меню\n";

        int choice = readInt("Выберите опцию: ", 0, 2);

        switch (choice) {
            case 1: {
                std::string sectionName = readString("Введите название раздела: ");
                simulateService->predictionExistenceFuture(sectionName);
                waitForEnter();
                break;
            }
            case 2:
                simulateService->predictAllSectionsFuture();
                waitForEnter();
                break;
            case 0:
                return;
        }
    }
}



void DialogController::showAllUsers() {
    clearScreen();
    std::cout << "=== ВСЕ ПОЛЬЗОВАТЕЛИ ФОРУМА ===\n";

    auto allUsers = userService->get_all_users();
    if (allUsers.empty()) {
        std::cout << "На форуме нет пользователей\n";
    } else {
        std::cout << std::left << std::setw(20) << " Логин "
                  << std::setw(15) << " Тип "
                  << std::setw(12) << " Статус "
                  << std::setw(10) << " Сообщений " << "\n";
        std::cout << std::string(57, '-') << "\n";

        for (const auto& user : allUsers) {
            std::cout << std::left << std::setw(20) << user->get_login();

            std::string type;
            if (userService->is_moderator(user->get_login())) {
                type = "Модератор";
            } else if (userService->is_troll(user->get_login())) {
                type = "Тролль";
            } else {
                type = "Пользователь";
            }
            std::cout << std::setw(15) << type;

            std::string status;
            switch (user->get_status()) {
                case UserStatus::ONLINE: status = " онлайн "; break;
                case UserStatus::OFFLINE: status = " оффлайн "; break;
                case UserStatus::BANNED: status = " забанен "; break;
            }
            std::cout << std::setw(12) << status;

            std::cout << std::setw(10) << user->get_count_of_message();

            std::cout << "\n";
        }
    }

    waitForEnter();
}

void DialogController::unban_user_dialog() {
    clearScreen();
    std::cout << "=== РАЗБАН ПОЛЬЗОВАТЕЛЯ ===\n";

    std::string login = readString("Введите логин пользователя: ");

    try {
        if (userService->unban_user(login)) {
            std::cout << "Пользователь '" << login << "' успешно разбанен\n";
        } else {
            std::cout << "Не удалось разбанить пользователя. Возможно:\n";
            std::cout << "   - Пользователь не найден или не был забанен\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    std::cout << "Тотальный разбан\n";

    waitForEnter();
}

void DialogController::change_user_type_dialog() {
    clearScreen();
    std::cout << "=== ИЗМЕНЕНИЕ ТИПА ПОЛЬЗОВАТЕЛЯ ===\n";

    std::string login = readString("Введите логин пользователя: ");

    std::cout << "\nВыберите новый тип:\n";
    std::cout << "1. Модератор\n";
    std::cout << "2. Тролль\n";
    std::cout << "0. Отмена\n";

    int choice = readInt("Ваш выбор: ", 0, 3);

    if (choice == 0) {
        std::cout << "Отменено\n";
        waitForEnter();
        return;
    }

    try {
        bool success = false;
        switch (choice) {
            case 1:
                if (userService->convert_to_moderator(login, 5)) {
                    std::cout << "Пользователь преобразован в модератора\n";
                    success = true;
                }
                break;
            case 2:
                float width = readFloat("Введите толщину тролля (0.1-1.0): ", 0.1f, 1.0f);
                if (userService->convert_to_troll(login, width)) {
                    std::cout << "Пользователь преобразован в тролля\n";
                    success = true;
                }
                break;
        }

        if (!success) {
            std::cout << "Не удалось изменить тип пользователя\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::removeUserFromSectionDialog() {
    clearScreen();
    std::cout << "=== УДАЛЕНИЕ ПОЛЬЗОВАТЕЛЯ ИЗ РАЗДЕЛА ===\n";

    std::string sectionName = readString("Введите название раздела: ");
    std::string userLogin = readString("Введите логин пользователя: ");

    try {
        if (sectionService->del_user_from_section(sectionName, userLogin)) {
            std::cout << "Пользователь '" << userLogin
                      << "' удален из раздела '" << sectionName << "'\n";
        } else {
            std::cout << "Не удалось удалить пользователя из раздела\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::moveUserToSectionDialog() {
    clearScreen();
    std::cout << "=== ПЕРЕМЕЩЕНИЕ ПОЛЬЗОВАТЕЛЯ МЕЖДУ РАЗДЕЛАМИ ===\n";

    std::string userLogin = readString("Введите логин пользователя: ");
    std::string fromSection = readString("Введите исходный раздел: ");
    std::string toSection = readString("Введите целевой раздел: ");

    try {
        sectionService->move_user_to_section(userLogin, fromSection, toSection);
        std::cout << "✓ Пользователь '" << userLogin
                  << "' перемещен из раздела '" << fromSection
                  << "' в раздел '" << toSection << "'\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }

    waitForEnter();
}

void DialogController::showSectionUsersDialog() {
    clearScreen();
    std::cout << "=== УЧАСТНИКИ РАЗДЕЛА ===\n";

    std::string sectionName = readString("Введите название раздела: ");

    auto* section = forum->find_section(sectionName);
    if (!section) {
        std::cout << "Раздел не найден\n";
        waitForEnter();
        return;
    }

    auto participants = section->get_participant();
    std::cout << "\nУчастники раздела '" << sectionName << "' ("
              << participants.size() << "):\n";

    if (participants.empty()) {
        std::cout << "Раздел пуст\n";
    } else {
        for (size_t i = 0; i < participants.size(); ++i) {
            std::cout << i + 1 << ". " << participants[i] << "\n";
        }
    }

    waitForEnter();
}

void DialogController::runFullSimulation() {
    clearScreen();
    std::cout << "=== ПОЛНАЯ СИМУЛЯЦИЯ РАБОТЫ ФОРУМА ===\n";

    std::cout << "Сколько шагов симуляции выполнить? ";
    int steps = readInt("", 1, 100);

    auto sections = forum->get_all_sections();
    if (sections.empty()) {
        std::cout << "На форуме нет разделов для симуляции\n";
        waitForEnter();
        return;
    }

    for (int step = 1; step <= steps; ++step) {
        std::cout << "\n=== ШАГ " << step << " ИЗ " << steps << " ===\n";

        for (const auto& [sectionName, section] : sections) {
            std::cout << "\n--- Раздел: " << sectionName << " ---\n";
            simulateService->simulateStep(sectionName);
        }

        if (step < steps) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
    }

    std::cout << "\nПолная симуляция завершена!\n";
    waitForEnter();
}

void DialogController::showSectionStateDialog() {
    clearScreen();
    std::cout << "=== СОСТОЯНИЕ РАЗДЕЛА ===\n";

    std::string sectionName = readString("Введите название раздела: ");

    auto* section = forum->find_section(sectionName);
    if (!section) {
        std::cout << "Раздел не найден\n";
        waitForEnter();
        return;
    }

    auto participants = section->get_participant();

    std::cout << "\nРаздел: " << sectionName << "\n";
    std::cout << "Главный модератор: " << section->get_login_initial_moderator() << "\n";
    std::cout << "Всего участников: " << participants.size() << "\n";

    waitForEnter();
}

void DialogController::saveLoadMenu() {
    while (true) {
        clearScreen();
        std::cout << "=== СОХРАНЕНИЕ И ЗАГРУЗКА ===\n";
        std::cout << "1. Сохранить состояние\n";
        std::cout << "2. Загрузить состояние\n";
        std::cout << "0. Назад в главное меню\n";

        int choice = readInt("Выберите опцию: ", 0, 2);

        switch (choice) {
            case 1: saveStateDialog(); break;
            case 2: loadStateDialog(); break;
            case 0: return;
        }
    }
}

void DialogController::checkAllSectionsForModerators() {
    clearScreen();
    std::cout << "=== ПРОВЕРКА РАЗДЕЛОВ НА НАЛИЧИЕ МОДЕРАТОРОВ ===\n";

    size_t fixed = sectionService->check_all_sections_for_moderators();
    std::cout << "\nПроверка завершена. Исправлено разделов: " << fixed << "\n";

    waitForEnter();
}