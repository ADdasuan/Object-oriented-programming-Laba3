#include "ForumDataContainer.h"
#include <ctime>
#include <iomanip>
#include <sstream>

json UserDataContainer::serialize_to_json() const {
    json json_output;
    json_output["user_login_string"] = user_login_string;
    json_output["user_category_string"] = user_category_string;

    auto time_converted = std::chrono::system_clock::to_time_t(registration_time_point);
    std::tm time_structure = *std::localtime(&time_converted);
    std::ostringstream time_stream;
    time_stream << std::put_time(&time_structure, "%Y-%m-%d %H:%M:%S");
    json_output["registration_time_formatted"] = time_stream.str();

    json_output["total_messages_count"] = total_messages_count;
    json_output["current_activity_status"] = current_activity_status;

    if (user_category_string == "forum_moderator") {
        json_output["moderator_reason_level"] = moderator_reason_level;
    } else if (user_category_string == "forum_troll") {
        json_output["troll_width_value"] = troll_width_value;
        json_output["troll_success_score"] = troll_success_score;
    }
    return json_output;
}

UserDataContainer UserDataContainer::deserialize_from_json(const json& json_input) {
    UserDataContainer container;
    container.user_login_string = json_input["user_login_string"];
    container.user_category_string = json_input["user_category_string"];

    std::string time_str = json_input["registration_time_formatted"].get<std::string>();
    std::tm time_structure = {};
    std::istringstream time_parser(time_str);
    time_parser >> std::get_time(&time_structure, "%Y-%m-%d %H:%M:%S");
    auto time_converted = std::mktime(&time_structure);
    container.registration_time_point = std::chrono::system_clock::from_time_t(time_converted);

    container.total_messages_count = json_input["total_messages_count"];
    container.current_activity_status = json_input["current_activity_status"];

    if (container.user_category_string == "forum_moderator" &&
        json_input.contains("moderator_reason_level")) {
        container.moderator_reason_level = json_input["moderator_reason_level"];
    } else if (container.user_category_string == "forum_troll") {
        if (json_input.contains("troll_width_value")) {
            container.troll_width_value = json_input["troll_width_value"];
        }
        if (json_input.contains("troll_success_score")) {
            container.troll_success_score = json_input["troll_success_score"];
        }
    }

    return container;
}

json SectionDataContainer::serialize_to_json() const {
    json json_output;
    json_output["section_name_string"] = section_name_string;
    json_output["initial_moderator_login_string"] = initial_moderator_login_string;
    json_output["participant_login_list"] = participant_login_list;
    return json_output;
}

SectionDataContainer SectionDataContainer::deserialize_from_json(const json& json_input) {
    SectionDataContainer container;
    container.section_name_string = json_input["section_name_string"];
    container.initial_moderator_login_string = json_input["initial_moderator_login_string"];
    container.participant_login_list = json_input["participant_login_list"].get<std::vector<std::string>>();
    return container;
}

json ForumDataContainer::serialize_to_json() const {
    json json_output;

    json users_json;
    for (const auto& [login_key, user_data] : user_data_collection) {
        users_json[login_key] = user_data.serialize_to_json();
    }
    json_output["user_data_collection"] = users_json;

    json sections_json;
    for (const auto& [name_key, section_data] : section_data_collection) {
        sections_json[name_key] = section_data.serialize_to_json();
    }
    json_output["section_data_collection"] = sections_json;

    return json_output;
}

ForumDataContainer ForumDataContainer::deserialize_from_json(const json& json_input) {
    ForumDataContainer container;

    if (json_input.contains("user_data_collection")) {
        auto users_json = json_input["user_data_collection"];
        for (auto& [login_key, user_json] : users_json.items()) {
            container.user_data_collection[login_key] = UserDataContainer::deserialize_from_json(user_json);
        }
    }

    if (json_input.contains("section_data_collection")) {
        auto sections_json = json_input["section_data_collection"];
        for (auto& [name_key, section_json] : sections_json.items()) {
            container.section_data_collection[name_key] = SectionDataContainer::deserialize_from_json(section_json);
        }
    }

    return container;
}