
#ifndef SIMULATESERVICE_H
#define SIMULATESERVICE_H

#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "Section.h"
#include "ForumService.h"
#include "SectionService.h"
#include "UserService.h"
#include <map>

/**
 * @file SimulateService.h
 * @brief Сервис симуляции жизни форума
 */

/**
 * @class SimulateService
 * @brief Сервис для симуляции работы форума и предсказания будущего состояния
 */
class SimulateService {
private:
    std::shared_ptr<Forum> forum; /// Указатель на форум
    std::shared_ptr<SectionService> sectionService; /// Сервис пользователей
    std::shared_ptr<UserService> userService; /// Сервис секций
    std::shared_ptr<ForumService> forumService; /// Сервис форума

public:

    mutable std::mutex prediction_mutex;  /// Мьютекс для многопоточных предсказаний

    /**
     * @brief Создает биномиальное количество сообщений для пользователя форума
     * @return Биномиально распределенное количество сообщений
     */
    int generateMessageCount() const;


    /**
     * @brief Назначает количество сообщений для пользователя форума
     * @param Ссылка на вектор с логинами std::string пользователей раздела
    */
    void generateMessages(const std::vector<std::string>& participants) const;

    /**
     * @brief Генерирует случайную толщину тролля в диапазоне [0.1, 1.0]
     * @return Случайное значение толщины
     */
    float generateRandomWidth() const;

    /**
    * @brief Генерирует случайный интеллект в диапазоне [1, 10]
    * @return Случайное значение интеллекта
    */
    unsigned generateRandomIntelligence() const;

    /**
     * @brief Выбрать случайного участника из списка
     * @param participants Список логинов
     * @return Случайный логин или пустая строка если список пуст
     */
    std::string selectRandomParticipant(const std::vector<std::string>& participants) const;

    /**
     * @brief Потокобезопасное предсказание будущего раздела
     * @param section_name имя секции
     * @param results мапа результатов секция в предсказание
     * Используется для многопоточного бенчмарка.
     */
    void predictSectionFutureThreadSafe(const std::string& section_name, std::map<std::string, std::string>& results) const;



public:
     /**
     * @brief Конструктор SimulateService
     * @param forum Общий объект форума
     */
    SimulateService(std::shared_ptr<Forum> forum,std::shared_ptr<UserService> userService,std::shared_ptr<SectionService> sectionService, std::shared_ptr<ForumService> forumService);


    /**
     * @brief Выполняет один этап симуляции для указанного раздела
     * @param section_name Название раздела для симуляции
     */
    void simulateStep(const std::string& section_name);

    /**
     * @brief Предсказывает итоговое состояние раздела при сохранении текущего состава
     * @param section_name Название раздела для анализа
     */
    void predictionExistenceFuture(const std::string& section_name);


    /**
     * @brief Предсказать будущее всех разделов многопоточно
     * @param num_threads Количество потоков если 0 то автоопределение
     */
    void predictAllSectionsFuture();


};
#endif