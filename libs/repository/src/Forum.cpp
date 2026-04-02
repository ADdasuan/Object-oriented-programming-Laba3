#include "Forum.h"
#include <algorithm>

bool Forum::add_section(const std::string& name_of_section, const std::string& moder_login) {
    if(name_of_section.empty()){
        return false;
    }

    if(moder_login.empty()){
        return false;
    }

    if (sections.find(name_of_section) != sections.end()) {
        return false;
    }

    if (!ptr_storage || !ptr_storage->find_user(moder_login)) {
        std::cerr << "Пользователь '" << moder_login << "' не найден\n";
        return false;
    }

    if (ptr_storage->is_troll(moder_login)) {
        std::cerr << "Тролли не могут быть модераторами\n";
        return false;
    }

    if (!ptr_storage->is_moderator(moder_login)) {
        if (!ptr_storage->promote_to_moderator(moder_login, 5)) {
            std::cerr << "Не удалось преобразовать пользователя в модератора\n";
            return false;
        }
        std::cout << "Пользователь '" << moder_login
                  << "' преобразован в модератора (разумность: 5)\n";
    }

    if (!ptr_storage->set_user_online(moder_login)) {
        std::cerr << "Не удалось установить статус Online\n";
        return false;
    }

    try {
        sections.emplace(name_of_section, Section(name_of_section, moder_login));
        std::cout << "Раздел '" << name_of_section << "' успешно создан\n";
        std::cout << "Модератор: " << moder_login << " статус - online)\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Траблы при создании раздела: " << e.what() << "\n";
        return false;
    }
}

bool Forum::del_section(const std::string& name_of_section) {
    if (name_of_section.empty()) {
        return false;
    }

    auto it = sections.find(name_of_section);
    if (it == sections.end()) {
        return false;
    }

    auto participants = it->second.get_participant();

    if (ptr_storage) {
        for (const auto& login : participants) {
            ptr_storage->set_user_offline(login);
        }
    }

    sections.erase(it);
    std::cout << "Раздел '" << name_of_section << "' удален\n";
    return true;
}

bool Forum::section_exist(const std::string& name) const{
    return sections.find(name) != sections.end();
}

Section* Forum::find_section(const std::string& name) {
    auto it = sections.find(name);
    return it != sections.end() ? &it->second : nullptr;
}

bool Forum::user_exists_in_storage(const std::string& login) const {
    return ptr_storage && ptr_storage->find_user(login);
}

bool Forum::user_in_any_section(const std::string& moder_login){
    if (moder_login.empty()) return false;

    auto all_sections = get_all_sections();

    for (const auto& [name, section] : all_sections) {
        if (section.is_participant_exist(moder_login)) {
            return true;
        }
    }
    return false;
}
