
#include "LoadData.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Load::Load(std::shared_ptr<Forum> forum_instance, std::shared_ptr<ForumDataConverter> converter_instance)
        : forum_pointer(forum_instance), data_converter_pointer(converter_instance) {}

void Load::load_forum_state_from_file(const std::string& file_path_string) {
    try {
        std::string json_content = read_entire_file_content(file_path_string);
        import_forum_state_from_json_string(json_content);
        std::cout << "Состояние форума успешно загружено из файла: "<< file_path_string << std::endl;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Ошибка при загрузке состояния: ") + e.what());
    }
}

void Load::import_forum_state_from_json_string(const std::string& json_content_string) {
    if (!data_converter_pointer) {
        throw std::runtime_error("Конвертер данных не инициализирован");
    }

    try {
        ForumDataContainer data_container = ForumDataContainer::deserialize_from_json(json::parse(json_content_string));

        auto new_forum_pointer = data_converter_pointer->convert_data_container_to_forum(data_container);

        *forum_pointer = *new_forum_pointer;
    } catch (const json::exception& json_error) {
        throw std::runtime_error("Ошибка парсинга JSON: " + std::string(json_error.what()));
    }
}

std::string Load::read_entire_file_content(const std::string& file_path_string) const {
    std::ifstream input_file_stream(file_path_string);

    if (!input_file_stream.is_open()) {
        std::string parent_path = "../" + file_path_string;
        input_file_stream.open(parent_path);
    }

    if (!input_file_stream.is_open()) {
        std::string project_root = "../../" + file_path_string;
        input_file_stream.open(project_root);
    }

    if (!input_file_stream.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + file_path_string);
    }

    std::stringstream buffer_stream;
    buffer_stream << input_file_stream.rdbuf();
    input_file_stream.close();

    return buffer_stream.str();
}