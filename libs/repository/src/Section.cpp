#include "Section.h"

Section::Section(const std::string& name, const std::string& moderator_login) : name(name), initial_moderator_login(moderator_login){
    if(name.empty()){
        throw std::invalid_argument("Name of section dont be a empty");
    }

    participant_logins.push_back(moderator_login);
}

void Section::set_name(std::string& new_name) {
    if(!new_name.empty()){
        name = new_name;
    }
    throw std::invalid_argument("Name of section is empty - mistakes");
}

void Section::set_initial_moderator_login(std::string login){
    if(!login.empty()){
        initial_moderator_login = login;
    }
    throw std::invalid_argument("Login is empty - mistakes");
}

void Section::add_participant(const std::string& login){
    if (login.empty()) {
        throw std::invalid_argument("Login is empty - mistake");
    }
    if (!find_login(login)) {
        participant_logins.push_back(login);
    }
}

void Section::del_participant(const std::string& login){
    if (login.empty()) {
        throw std::invalid_argument("Login is empty - mistake");
    }

    if (participant_logins.empty()) {
        throw std::runtime_error("Section is empty");
    }

    auto result = std::find(participant_logins.begin(), participant_logins.end(), login);

    if (result == participant_logins.end()) {
        throw std::runtime_error("Login not found in section");
    }

    participant_logins.erase(result);
}

bool Section::is_participant_exist(const std::string& login) const{
    if(login.empty()){
        throw std::invalid_argument("Login is empty - mistake");
    }
    if(participant_logins.empty()){
        return false;
    }
    return std::find(participant_logins.begin(), participant_logins.end(),login) != participant_logins.end();

}




