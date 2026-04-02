#include "ForumService.h"
#include "Forum.h"
//#include "Moderator.h"
#include "StorageUsers.h"

#include <stdexcept>
#include <algorithm>

ForumService::ForumService(std::shared_ptr<Forum> forum_ptr) : forum(forum_ptr) {
    if (!forum) {
        throw std::invalid_argument("Forum cannot be null");
    }
}

bool ForumService::create_section(const std::string& name_of_section, const std::string& moder_login) {
    if (!forum) {
        return false;
    }

    if (forum->user_in_any_section(moder_login)) {
        std::cout << "Анлак:'" << moder_login<< "' уже состоит в другом разделе\n";
        return false;
    }

    return forum->add_section(name_of_section,moder_login);
}

bool ForumService::del_section(const std::string& name_of_section) {
    if (!forum) {
        return false;
    }
    return forum->del_section(name_of_section);
}

bool ForumService::section_exists(const std::string& name_of_section) const {
    if (!forum) {
        return false;
    }
    return forum->section_exist(name_of_section);
}

std::vector<std::string> ForumService::getOnlineUsers(const std::string& section_name) const {
    std::vector<std::string> result;

    if (!forum) return result;

    auto storage = forum->get_ptr_storage();
    if (!storage) return result;

    const auto* section = get_section_const(section_name);
    if (!section) return result;

    auto participants = section->get_participant();

    for (const auto& login : participants) {
        if (storage->is_online(login)) {
            result.push_back(login);
        }
    }

    return result;
}

std::vector<std::string> ForumService::getOnlineModerators(const std::string& section_name) const {
    std::vector<std::string> result;

    if (!forum) return result;

    auto storage = forum->get_ptr_storage();
    if (!storage) return result;

    const auto* section = get_section_const(section_name);
    if (!section) return result;

    auto participants = section->get_participant();

    for (const auto& login : participants) {
        if (storage->is_online(login) && storage->is_moderator(login)) {
            result.push_back(login);
        }
    }
    return result;
}

std::vector<std::string> ForumService::getOnlineTrolls(const std::string& section_name) const {
    std::vector<std::string> result;

    if (!forum) return result;

    auto storage = forum->get_ptr_storage();
    if (!storage) return result;

    const auto* section = get_section_const(section_name);
    if (!section) return result;

    auto participants = section->get_participant();

    for (const auto& login : participants) {
        if (storage->is_online(login) && storage->is_troll(login)) {
            result.push_back(login);
        }
    }

    return result;
}
int ForumService::getOnlineModeratorSum(const std::string& section_name) const {
    int sum = 0;
    if (!forum) return sum;

    auto storage = forum->get_ptr_storage();
    if (!storage) return sum;

    auto moderators = getOnlineModerators(section_name);

    for (const auto& login : moderators) {
        auto user = storage->get_user(login);
        if (auto moderator = std::dynamic_pointer_cast<IModerator>(user)) {
            sum += moderator->get_intelligence();
        }
    }

    return sum;
}

float ForumService::getOnlineTrollInverseSum(const std::string& section_name) const {
    float sum = 0.0f;
    if (!forum) return sum;

    auto storage = forum->get_ptr_storage();
    if (!storage) return sum;

    auto trolls = getOnlineTrolls(section_name);

    for (const auto& login : trolls) {
        auto user = storage->get_user(login);
        if (auto troll = std::dynamic_pointer_cast<ITroll>(user)) {
            float width = troll->get_width();
            if (width > 0.0f) {
                sum += 1.0f / width;
            }
        }
    }

    return sum;
}

bool ForumService::are_all_moderators_banned(const std::string& section_name) const {
    if (!forum) return false;

    auto storage = forum->get_ptr_storage();
    if (!storage) return false;

    auto moderators = getOnlineModerators(section_name);
    if (moderators.empty()) return false;

    for (const auto& login : moderators) {
        if (!storage->is_banned(login)) {
            return false;
        }
    }

    return true;
}

Section* ForumService::get_section(const std::string& name_of_section) {
    if (!forum) return nullptr;
    return forum->find_section(name_of_section);
}

const Section* ForumService::get_section_const(const std::string& name_of_section) const {
    if (!forum) return nullptr;

    return forum->find_section(name_of_section);
}

size_t ForumService::get_section_count() const {
    if (!forum) return 0;

    auto all_sections = forum->get_all_sections();
    return all_sections.size();
}
