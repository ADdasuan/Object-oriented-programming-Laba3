#include "User.h"

User::User(const std::string& login, const std::chrono::system_clock::time_point& date, size_t message_count, UserStatus status)
        : DefaultUser(login, date, message_count, status) {
}

User::User(const std::string& login): DefaultUser(login) {
}



