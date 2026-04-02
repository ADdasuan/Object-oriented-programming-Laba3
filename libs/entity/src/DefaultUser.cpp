#include "DefaultUser.h"

DefaultUser::DefaultUser(const std::string& login, const std::chrono::system_clock::time_point& date, size_t count_of_message, UserStatus status) : login(login), date(date),count_of_message(count_of_message),
                         status(status){
}

DefaultUser::DefaultUser(const std::string& login) : login(login), date(std::chrono::system_clock::now()),
                        count_of_message(0), status(UserStatus::OFFLINE){

}