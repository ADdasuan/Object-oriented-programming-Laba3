
#ifndef DIALOGCONTROLLER_H
#define DIALOGCONTROLLER_H

#include <memory>
#include <string>
#include "Forum.h"
#include "StorageUsers.h"
#include "ForumService.h"
#include "UserService.h"
#include "SectionService.h"
#include "SimulateService.h"
#include "StatService.h"
#include "SaveData.h"
#include "LoadData.h"
#include <cfloat>

/**
 * @class DialogController
 * @brief Контроллер для управления диалоговым интерфейсом форума
 * 
 * Отвечает за отображение меню, обработку пользовательского ввода
 * и координацию работы всех сервисов форума.
 */
class DialogController {
private:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<ForumService> forumService;
    std::shared_ptr<UserService> userService;
    std::shared_ptr<SectionService> sectionService;
    std::shared_ptr<SimulateService> simulateService;
    StatService statService;

    /**
     * @brief Отображает главное меню
     */
    void show_main_menu() const;

    /**
     * @brief Обрабатывает выбор пользователя в главном меню
     * @param choice Выбор пользователя
     */
    void handle_main_menu_choice(int choice);

    /**
     * @brief Ожидает нажатия клавиши Enter для продолжения
     */
    void waitForEnter() const;

    /**
     * @brief Читает строку от пользователя
     * @param prompt Приглашение для ввода
     * @return Введенная строка
     */
    std::string readString(const std::string& prompt) const;

    /**
     * @brief Читает целое число от пользователя
     * @param prompt Приглашение для ввода
     * @param min Минимальное значение
     * @param max Максимальное значение
     * @return Введенное число
     */
    int readInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) const;

    /**
     * @brief Читает дробное число от пользователя
     * @param prompt Приглашение для ввода
     * @param min Минимальное значение
     * @param max Максимальное значение
     * @return Введенное число
     */
    float readFloat(const std::string& prompt, float min = -FLT_MAX, float max = FLT_MAX) const;

    /**
     * @brief Очищает экран (кроссплатформенная версия)
     */
    void clearScreen() const;

public:
    /**
     * @brief Конструктор DialogController
     * @param forum Объект форума
     */
    explicit DialogController(const std::shared_ptr<Forum>& forum);

    /**
     * @brief Запускает главный цикл диалогового интерфейса
     */
    void run();
    void manageForumMenu();
    void manageUsersMenu();
    void manageSectionsMenu();
    void simulationMenu();
    void statisticsMenu();

private:
    void createSectionDialog();
    void deleteSectionDialog();
    void showAllSections();

    void registerUserDialog();
    void registerModeratorDialog();
    void registerTrollDialog();
    void ban_user_dialog();
    void unban_user_dialog();
    void change_user_type_dialog();
    void show_user_info_dialog();
    void showAllUsers();

    void addUserToSectionDialog();
    void removeUserFromSectionDialog();
    void moveUserToSectionDialog();
    void showSectionUsersDialog();

    void simulateStepDialog();
    void predictSectionFutureDialog();
    void runFullSimulation();

    void showMostSuccessfulTroll();
    void showMostActiveUser();
    void showSectionStateDialog();
    void showForumStatistics();

    void saveStateDialog();
    void loadStateDialog();

    void predictionMenu();

    void saveLoadMenu();
    void checkAllSectionsForModerators();


    std::string formatTimePoint(const std::chrono::system_clock::time_point& tp) const;

};

#endif