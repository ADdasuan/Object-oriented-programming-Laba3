#include "Troll.h"
#include <stdexcept>

void Troll::set_width(float new_width){
    if(new_width < 0.1f || new_width > 1.0f){
        throw std::invalid_argument("Width was been in [0.1 ; 1.0]");
    }
    width = new_width;
}

Troll::Troll(const std::string& login,
             const std::chrono::system_clock::time_point& date,
      float size, UserStatus status) : DefaultUser(login, date, 0, status), width(0.1), success_count(0){
    set_width(size);
}

Troll::Troll(const std::string& login, float size) : DefaultUser(login),
                                                     width(0.1),success_count(0){
    set_width(size);
}

