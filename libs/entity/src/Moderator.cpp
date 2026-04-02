#include "Moderator.h"
#include <stdexcept>

Moderator::Moderator(const std::string& login,
                     const std::chrono::system_clock::time_point& date, size_t message,
                     unsigned intelligence,
                     UserStatus status)
        : DefaultUser(login, date, message, status), intelligence(1) {
    set_intelligence(intelligence);
}

Moderator::Moderator(const std::string& login, unsigned intelligence)
        : DefaultUser(login), intelligence(1) {
    set_intelligence(intelligence);
}

void Moderator::set_intelligence(unsigned brain) {
    if (brain < 1 || brain > 10) {
        throw std::invalid_argument("Brain of moderator was been in [1,10]");
    }
    intelligence = brain;
}
