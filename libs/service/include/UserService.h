
#ifndef USERSERVICE_H
#define USERSERVICE_H


#include "Forum.h"
#include "User.h"

/**
 * @file UserService.h
 * @brief Сервис для работы с пользователями
 */


/**
 * @class UserService
 * @brief Управляет операциями с пользователями форума
 */
class UserService {
private:
    std::shared_ptr<Forum> forum; /// Указатель на главный объект форума
public:
    /**
     * @brief Конструктор сервиса
     * @param forum_ptr Указатель на форум
     * @throws std::invalid_argument если forum_ptr == nullptr
     */
    UserService(std::shared_ptr<Forum> forum_ptr);

    /**
     * @brief Получить пользователя по логину
     * @param user_login Логин пользователя
     * @return Указатель на пользователя или nullptr
     */
    std::shared_ptr<IUser> get_user(const std::string& user_login) const;

    /**
     * @brief Зарегистрировать обычного пользователя
     * @param user_login Логин нового пользователя
     * @return Указатель на созданного пользователя
     * @throws std::runtime_error если пользователь уже существует
     */
    std::shared_ptr<IUser> register_user(const std::string& user_login);

    /**
     * @brief Зарегистрировать модератора
     * @param login Логин модератора
     * @param intelligence Разумность [1-10]
     * @return Указатель на созданного модератора
     * @throws std::runtime_error если пользователь уже существует
     */
    std::shared_ptr<IUser> register_moderator(const std::string& login, unsigned intelligence);

    /**
     * @brief Зарегистрировать тролля
     * @param login Логин тролля
     * @param width Толщина [0.1-1.0]
     * @return Указатель на созданного тролля
     * @throws std::runtime_error если пользователь уже существует
     */
    std::shared_ptr<IUser> register_troll(const std::string& login, float width);


    /**
     * @brief Забанить пользователя
     * @param user_login Логин пользователя
     * @return true если бан успешен
     */
    bool ban_user(const std::string& user_login);

    /**
     * @brief Разбанить пользователя
     * @param user_login Логин пользователя
     * @return true если разбан успешен
     */
    bool unban_user(const std::string& user_login);

    /**
     * @brief Получить список всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    std::vector<std::shared_ptr<IUser>> get_all_users() const;

    /**
     * @brief Увеличить успешность тролля
     * @param login Логин тролля
     * @return true если операция успешна
     */
    bool increment_troll_success(const std::string& login);

    /**
     * @brief Изменить разумность модератора
     * @param moderator_login Логин модератора
     * @param intelligence Новая разумность [1-10]
     * @return true если изменение успешно
     */
    bool change_moderator_intelligence(const std::string& moderator_login, unsigned intelligence);

    bool change_troll_width(const std::string& troll_login, float size);

    /**
     * @brief Проверить, забанен ли пользователь
     * @param login Логин для проверки
     * @return true если статус BANNED
     */
    bool is_banned(const std::string& login) const;

    /**
     * @brief Проверить, онлайн ли пользователь
     * @param login Логин для проверки
     * @return true если статус ONLINE
     */
    bool is_online(const std::string& login) const;

    /**
     * @brief Проверить, модератор ли пользователь
     * @param username Логин для проверки
     * @return true если модератор
     */
    bool is_moderator(const std::string& username) const;

    /**
     * @brief Проверяет, является ли пользователь троллем
     * @param username Логин для проверки
     * @return true если тролль
     */
    bool is_troll(const std::string& username) const;

    /**
    * @brief Преобразовать обычного пользователя в модератора
    * @param user_login Логин пользователя
    * @param intelligence Разумность нового модератора (1-10)
    * @return true если преобразование успешно
    */
    bool convert_to_moderator(const std::string& user_login, unsigned intelligence = 5);

    /**
     * @brief Преобразовать модератора в обычного пользователя
     * @param moderator_login Логин модератора
     * @return true если преобразование успешно
     */
    bool convert_moderator_to_user(const std::string& moderator_login);

    /**
     * @brief Преобразовать обычного пользователя в тролля
     * @param user_login Логин пользователя
     * @param width Толщина тролля (0.1-1.0)
     * @return true если преобразование успешно
     */
    bool convert_to_troll(const std::string& user_login, float width = 0.5f);

    /**
     * @brief Преобразовать тролля в обычного пользователя
     * @param troll_login Логин тролля
     * @return true если преобразование успешно
     */
    bool convert_troll_to_user(const std::string& troll_login);

    /**
     * @brief Проверить, обычный ли пользователь
     * @param username Логин для проверки
     * @return true если обычный пользователь
     */
    bool is_regular_user(const std::string& username) const;

    /**
     * @brief Получить общее количество пользователей
     * @return Количество пользователей
     */
    size_t get_user_count() const;

    /**
     * @brief Получить количество модераторов
     * @return Количество модераторов
     */
    size_t get_moderator_count() const;

    /**
     * @brief Получить количество троллей
     * @return Количество троллей
     */
    size_t get_troll_count() const;

    /**
    * @brief Получить количество пользователей онлайн
    * @return Количество пользователей со статусом ONLINE
    */
    size_t get_online_count() const;

    /**
     * @brief Получить толщину тролля
     * @param login Логин тролля
     * @return Толщина или 0.0f если не тролль
     */
    float get_troll_width(const std::string& login) const;


    /**
     * @brief Получить успешность тролля
     * @param login Логин тролля
     * @return Количество успешных банов или 0 если не тролль
     */
    int get_troll_success(const std::string& login) const;

    /**
     * @brief Получить разумность модератора
     * @param login Логин модератора
     * @return Разумность или 0 если не модератор
     */
    unsigned get_moderator_intelligence(const std::string& login) const;
};

#endif