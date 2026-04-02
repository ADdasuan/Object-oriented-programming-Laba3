#ifndef SECTION_H
#define SECTION_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


/**
 * @file Section.h
 * @brief Класс раздела форума
 */

/**
 * @class Section
 * @brief Представляет раздел на форуме
 */
class Section{
private:
    std::string name; /// Название раздела
    std::string initial_moderator_login; /// Логин главного модератора раздела
    std::vector<std::string> participant_logins; /// Список логинов всех участников раздела

    /**
     * @brief Поиск логина в списке участников
     * @param found_login Логин для поиска
     * @return true если логин найден
     */
    bool find_login(const std::string &found_login) const{
        if(participant_logins.empty()){
            return false;
        }
        return std::find(participant_logins.begin(), participant_logins.end(), found_login) != participant_logins.end();
    }

public:
    /**
     * @brief Конструктор раздела
     * @param name Название раздела
     * @param moderator_login Логин главного модератора
     * @throws std::invalid_argument если название пустое
     */
    Section(const std::string& name, const std::string& moderator_login);


    /**
     * @brief Обновить информацию о главном модераторе
     * @param new_moderator_login Логин нового главного модератора
     *
     * Новый модератор должен быть участником раздела.
     */
    void update_moderator_info(const std::string& new_moderator_login) {
        if (std::find(participant_logins.begin(), participant_logins.end(), new_moderator_login) != participant_logins.end()) {
            initial_moderator_login = new_moderator_login;
        }
    }

    /**
     * @brief Получить название раздела
     * @return Название раздела
     */
    std::string get_name() const {return name;}

    /**
     * @brief Получить логин главного модератора
     * @return Логин главного модератора
     */
    std::string get_login_initial_moderator() const {return initial_moderator_login;}

    /**
     * @brief Получить список всех участников раздела
     * @return Вектор логинов участников
     */
    const std::vector<std::string> get_participant() const {return participant_logins;}

    /**
     * @brief Установить новое название раздела
     * @param new_name Новое название
     * @throws std::invalid_argument если название пустое
     */
    void set_name(std::string& new_name);

    /**
     * @brief Установить нового главного модератора
     * @param login Логин модератора
     * @throws std::invalid_argument если логин пустой
     */
    void set_initial_moderator_login(std::string login);

    /**
     * @brief Добавить участника в раздел
     * @param login Логин нового участника
     * @throws std::invalid_argument если логин пустой
     */
    void add_participant(const std::string& login);

    /**
     * @brief Удалить участника из раздела
     * @param login Логин участника
     * @throws std::invalid_argument если логин пустой
     * @throws std::runtime_error если раздел пустой или участник не найден
     */
    void del_participant(const std::string& login);

    /**
     * @brief Проверить наличие участника в разделе
     * @param login Логин для проверки
     * @return true если участник есть в разделе
     * @throws std::invalid_argument если логин пустой
     */
    bool is_participant_exist(const std::string& login) const;


    /**
     * @brief Проверить, пуст ли раздел
     * @return true если в разделе нет участников
     */
    bool section_is_empty() const { return participant_logins.empty(); }

    ~Section() = default;
};

#endif
