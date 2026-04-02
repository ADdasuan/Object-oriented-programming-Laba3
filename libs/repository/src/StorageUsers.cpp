#include "StorageUsers.h"
#include "User.h"
#include "Moderator.h"
#include "Troll.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

std::shared_ptr<IUser> StorageUsers::get_user(const std::string& login) const {
    auto ptr = data.find(login);
    if (ptr) {
        return *ptr;
    }
    return nullptr;
}

void StorageUsers::add_user(std::shared_ptr<IUser> user) {
    if (!user) return;

    std::string login = user->get_login();
    if (data.find(login)) {
        throw std::invalid_argument("User with this login already exist");
    }
    data.insert(login, user);
}

void StorageUsers::del_user(const std::string& login) {
    if (!data.find(login)) {
        throw std::invalid_argument("User with this login not found");
    }
    data.delet(login);
}

bool StorageUsers::find_user(const std::string& login) {
    return data.find(login) != nullptr;
}

std::shared_ptr<IUser> StorageUsers::find_user_ptr(const std::string& login) {
    auto user_ptr = data.find(login);
    if (user_ptr) {
        return *user_ptr;
    }
    return nullptr;
}

std::vector<std::shared_ptr<IUser>> StorageUsers::get_all_users() {
    std::vector<std::shared_ptr<IUser>> users;
    users.reserve(data.size());

    for (auto i = data.begin(); i != data.end(); ++i) {
        users.push_back(i->second);
    }
    return users;
}

bool StorageUsers::update_user(const std::string& login, std::shared_ptr<IUser> new_user) {
    if (!data.find(login) || !new_user) {
        return false;
    }

    if (login != new_user->get_login()) {
        return false;
    }

    data.insert_or_assign(login, new_user);
    return true;
}

bool StorageUsers::is_moderator(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return false;

    try {
        auto moderator = std::dynamic_pointer_cast<IModerator>(user);
        return moderator != nullptr;
    } catch (...) {
        return false;
    }
}

bool StorageUsers::is_troll(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return false;

    try {
        auto troll = std::dynamic_pointer_cast<ITroll>(user);
        return troll != nullptr;
    } catch (...) {
        return false;
    }
}

bool StorageUsers::is_online(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return false;
    return user->get_status() == UserStatus::ONLINE;
}

bool StorageUsers::is_banned(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return false;
    return user->get_status() == UserStatus::BANNED;
}

bool StorageUsers::ban_user(const std::string& login) {
    auto user = get_user(login);
    if (!user || user->get_status() == UserStatus::BANNED) {
        return false;
    }

    user->set_status(UserStatus::BANNED);
    return true;
}

bool StorageUsers::unban_user(const std::string& login) {
    auto user = get_user(login);
    if (!user || user->get_status() != UserStatus::BANNED) {
        return false;
    }

    user->set_status(UserStatus::OFFLINE);
    return true;
}

bool StorageUsers::change_moderator_intelligence(const std::string& login, unsigned intelligence) {
    auto user = get_user(login);
    if (!user) return false;

    auto moderator = std::dynamic_pointer_cast<IModerator>(user);
    if (!moderator) return false;

    moderator->set_intelligence(intelligence);
    return true;
}

bool StorageUsers::change_troll_width(const std::string& login, float size) {
    auto user = get_user(login);
    if (!user) return false;

    auto troll = std::dynamic_pointer_cast<ITroll>(user);
    if (!troll) return false;

    troll->set_width(size);
    return true;
}

bool StorageUsers::increment_troll_success(const std::string& login) {
    auto user = get_user(login);
    if (!user) return false;

    auto troll = std::dynamic_pointer_cast<ITroll>(user);
    if (!troll) return false;

    troll->set_success(troll->get_success() + 0.1f);
    return true;
}

bool StorageUsers::promote_to_moderator(const std::string& login, unsigned intelligence) {
    auto user = get_user(login);
    if (!user) {
        return false;
    }

    if (is_moderator(login) || is_troll(login)) {
        return false;
    }

    auto new_moderator = std::make_shared<Moderator>(
            user->get_login(),
            user->get_date(),
            user->get_count_of_message(),
            intelligence,
            user->get_status()
    );
    return update_user(login, new_moderator);
}

bool StorageUsers::set_user_online(const std::string& login) {
    auto user = get_user(login);
    if (!user) {
        return false;
    }

    user->set_status(UserStatus::ONLINE);
    return true;
}

bool StorageUsers::set_user_offline(const std::string& login) {
    auto user = get_user(login);
    if (!user) {
        return false;
    }

    user->set_status(UserStatus::OFFLINE);
    return true;
}

bool StorageUsers::convert_to_troll(const std::string& login, float width) {
    auto user = get_user(login);
    if (!user) {
        return false;
    }

    if (is_moderator(login) || is_troll(login)) {
        return false;
    }

    auto new_troll = std::make_shared<Troll>(
            user->get_login(),
            user->get_date(),
            width,
            user->get_status()
    );

    return update_user(login, new_troll);
}

bool StorageUsers::convert_to_regular_user(const std::string& login) {
    auto user = get_user(login);
    if (!user) {
        return false;
    }

    if (!is_moderator(login) && !is_troll(login)) {
        return false;
    }

    auto new_user = std::make_shared<User>(
            user->get_login(),
            user->get_date(),
            user->get_count_of_message(),
            user->get_status()
    );

    return update_user(login, new_user);
}

bool StorageUsers::is_regular_user(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return false;

    auto moderator = std::dynamic_pointer_cast<IModerator>(user);
    auto troll = std::dynamic_pointer_cast<ITroll>(user);

    return !moderator && !troll;
}

size_t StorageUsers::get_online_count() const {
    size_t count = 0;
    for (const auto& [login, user] : data) {
        if (user && user->get_status() == UserStatus::ONLINE) {
            count++;
        }
    }
    return count;
}

size_t StorageUsers::get_moderator_count() const {
    size_t count = 0;
    for (const auto& [login, user] : data) {
        if (user && std::dynamic_pointer_cast<IModerator>(user)) {
            count++;
        }
    }
    return count;
}

size_t StorageUsers::get_troll_count() const {
    size_t count = 0;
    for (const auto& [login, user] : data) {
        if (user && std::dynamic_pointer_cast<ITroll>(user)) {
            count++;
        }
    }
    return count;
}

size_t StorageUsers::get_total_count() const {
    return data.size();
}

size_t StorageUsers::size() const {
    return data.size();
}

bool StorageUsers::empty() const {
    return data.empty();
}

void StorageUsers::clear() {
    data.clear();
}

float StorageUsers::get_troll_width(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return 0.0f;

    if (is_troll(login)) {
        if (auto troll = std::dynamic_pointer_cast<ITroll>(user)) {
            return troll->get_width();
        }
    }
    return 0.0f;
}

int StorageUsers::get_troll_success(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return 0;

    if (is_troll(login)) {
        if (auto troll = std::dynamic_pointer_cast<ITroll>(user)) {
            return troll->get_success();
        }
    }
    return 0;
}

unsigned StorageUsers::get_moderator_intelligence(const std::string& login) const {
    auto user = get_user(login);
    if (!user) return 0;

    if (is_moderator(login)) {
        if (auto moderator = std::dynamic_pointer_cast<IModerator>(user)) {
            return moderator->get_intelligence();
        }
    }
    return 0;
}