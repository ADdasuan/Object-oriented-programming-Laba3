#include "UserService.h"
#include <memory>
#include "Troll.h"
#include "Moderator.h"


UserService::UserService(std::shared_ptr<Forum> forum_ptr) : forum(forum_ptr) {
    if (!forum) {
        throw std::invalid_argument("Forum cannot be null");
    }
}

std::shared_ptr<IUser> UserService::get_user(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }
    return storage->get_user(login);
}

std::shared_ptr<IUser> UserService::register_user(const std::string& login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    if (storage->find_user(login)) {
        throw std::runtime_error("User already exists");
    }

    auto new_user = std::make_shared<User>(login);
    storage->add_user(new_user);
    return new_user;
}

std::shared_ptr<IUser> UserService::register_moderator(const std::string& login, unsigned intelligence) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    if (storage->find_user(login)) {
        throw std::runtime_error("User already exists");
    }

    auto new_moderator = std::make_shared<Moderator>(login, intelligence);
    storage->add_user(new_moderator);
    return new_moderator;
}

std::shared_ptr<IUser> UserService::register_troll(const std::string& login, float width) {
    if (login.empty()) {
        throw std::invalid_argument("Login cannot be empty");
    }

    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    if (storage->find_user(login)) {
        throw std::runtime_error("User already exists");
    }

    auto new_troll = std::make_shared<Troll>(login, width);
    storage->add_user(new_troll);

    return new_troll;
}

bool UserService::ban_user(const std::string& user_login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;

    if(!storage->find_user(user_login)){
        std::cout<<"Пользователя с таким логином не существует, увы\n";
        return false;
    }

    if(storage->is_banned(user_login)){
        std::cout<<"Этот пользователь уже забанен\n";
        return false;
    }

    bool was_moderator = is_moderator(user_login);

    bool ban_result = storage->ban_user(user_login);

    if (!ban_result) {
        return false;
    }

    return ban_result;
}

bool UserService::unban_user(const std::string& user_login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->unban_user(user_login);
}

bool UserService::change_moderator_intelligence(const std::string& login, unsigned intelligence) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->change_moderator_intelligence(login, intelligence);
}

[[maybe_unused]] bool UserService::change_troll_width(const std::string& login, float size) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->change_troll_width(login, size);
}

bool UserService::is_moderator(const std::string& username) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->is_moderator(username);
}

bool UserService::is_troll(const std::string& username) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->is_troll(username);
}

bool UserService::is_regular_user(const std::string& username) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->is_regular_user(username);
}

bool UserService::increment_troll_success(const std::string& login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->increment_troll_success(login);
}

std::vector<std::shared_ptr<IUser>> UserService::get_all_users() const {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        return {};
    }

    return storage->get_all_users();
}

size_t UserService::get_user_count() const {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        return 0;
    }

    auto all_users = storage->get_all_users();
    return all_users.size();
}

size_t UserService::get_online_count() const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0;

    size_t count = 0;
    auto all_users = storage->get_all_users();

    for (const auto& user : all_users) {
        if (user->get_status() == UserStatus::ONLINE) {
            count++;
        }
    }

    return count;
}

bool UserService::is_online(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->is_online(login);
}

bool UserService::is_banned(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return false;
    return storage->is_banned(login);
}

size_t UserService::get_moderator_count() const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0;

    size_t count = 0;
    auto all_users = storage->get_all_users();

    for (const auto& user : all_users) {
        if (std::dynamic_pointer_cast<IModerator>(user)) {
            count++;
        }
    }

    return count;
}

size_t UserService::get_troll_count() const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0;

    size_t count = 0;
    auto all_users = storage->get_all_users();

    for (const auto& user : all_users) {
        if (std::dynamic_pointer_cast<ITroll>(user)) {
            count++;
        }
    }

    return count;
}

bool UserService::convert_to_moderator(const std::string& user_login, unsigned intelligence) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    auto user = storage->get_user(user_login);
    if (!user) {
        throw std::runtime_error("User '" + user_login + "' not found");
    }

    auto existing_moderator = std::dynamic_pointer_cast<IModerator>(user);
    if (existing_moderator) {
        existing_moderator->set_intelligence(intelligence);
        return true;
    }

    auto troll = std::dynamic_pointer_cast<ITroll>(user);
    if (troll) {
        throw std::runtime_error("Cannot convert troll to moderator (moderators don't troll)");
    }

    auto new_moderator = std::make_shared<Moderator>(user->get_login(),user->get_date(),
            user->get_count_of_message(),
            intelligence,
            user->get_status()
    );

    try {
        return storage->update_user(user_login, new_moderator);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to convert user to moderator: " + std::string(e.what()));
    }
}

bool UserService::convert_moderator_to_user(const std::string& moderator_login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    auto user = storage->get_user(moderator_login);
    if (!user) {
        throw std::runtime_error("User '" + moderator_login + "' not found");
    }

    auto moderator = std::dynamic_pointer_cast<IModerator>(user);
    if (!moderator) {
        throw std::runtime_error("User '" + moderator_login + "' is not a moderator");
    }

    auto new_user = std::make_shared<User>(
            user->get_login(),
            user->get_date(),
            user->get_count_of_message(),
            user->get_status()
    );

    try {
        return storage->update_user(moderator_login, new_user);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to convert moderator to user: " + std::string(e.what()));
    }
}

bool UserService::convert_to_troll(const std::string& user_login, float width) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    auto user = storage->get_user(user_login);
    if (!user) {
        throw std::runtime_error("User '" + user_login + "' not found");
    }

    auto existing_troll = std::dynamic_pointer_cast<ITroll>(user);
    if (existing_troll) {
        existing_troll->set_width(width);
        return true;
    }

    auto moderator = std::dynamic_pointer_cast<IModerator>(user);
    if (moderator) {
        throw std::runtime_error("Cannot convert moderator to troll (moderators don't troll)");
    }

    auto new_troll = std::make_shared<Troll>(
            user->get_login(),
            user->get_date(),
            width,
            user->get_status()
    );
    new_troll->set_count_of_message(user->get_count_of_message());

    try {
        return storage->update_user(user_login, new_troll);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to convert user to troll: " + std::string(e.what()));
    }
}

bool UserService::convert_troll_to_user(const std::string& troll_login) {
    auto storage = forum->get_ptr_storage();
    if (!storage) {
        throw std::runtime_error("Storage not available");
    }

    auto user = storage->get_user(troll_login);
    if (!user) {
        throw std::runtime_error("User '" + troll_login + "' not found");
    }

    auto troll = std::dynamic_pointer_cast<ITroll>(user);
    if (!troll) {
        throw std::runtime_error("User '" + troll_login + "' is not a troll");
    }

    auto new_user = std::make_shared<User>(user->get_login(),user->get_date(),user->get_count_of_message(),user->get_status());

    try {
        return storage->update_user(troll_login, new_user);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to convert troll to user: " + std::string(e.what()));
    }
}

float UserService::get_troll_width(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0.0f;
    return storage->get_troll_width(login);
}

int UserService::get_troll_success(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0;
    return storage->get_troll_success(login);
}

unsigned UserService::get_moderator_intelligence(const std::string& login) const {
    auto storage = forum->get_ptr_storage();
    if (!storage) return 0;
    return storage->get_moderator_intelligence(login);
}