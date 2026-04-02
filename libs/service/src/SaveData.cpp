
#include "SaveData.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Save::Save(std::shared_ptr<Forum> forum_instance,std::shared_ptr<ForumDataConverter> converter_instance)
        : forum_pointer(forum_instance),data_converter_pointer(converter_instance) {}

void Save::save_forum_state_to_file(const std::string& file_path_string) {
    try {
        std::string json_content = export_forum_state_to_json_string();
        write_content_to_file(file_path_string, json_content);
        std::cout << "Состояние форума успешно сохранено в файл: " << file_path_string << std::endl;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Ошибка сохранения в файл: ") + e.what());
    }
}

std::string Save::export_forum_state_to_json_string() const {
    if (!forum_pointer || !data_converter_pointer) {
        throw std::runtime_error("Форум или конвертер не инициализированы");
    }

    try{
        ForumDataContainer data_container = data_converter_pointer->convert_forum_to_data_container(*forum_pointer);

        json json_object = data_container.serialize_to_json();
        return json_object.dump(4);
    }catch (const std::exception& e) {
        throw std::runtime_error(std::string("Ошибка сериализации: ") + e.what());
    }

}

void Save::write_content_to_file(const std::string& file_path_string,
                                 const std::string& file_content_string) const {
    std::ofstream output_file_stream(file_path_string);
    if (!output_file_stream.is_open()) {
        throw std::runtime_error("Не удалось создать файл: " + file_path_string);
    }

    output_file_stream << file_content_string;
    output_file_stream.close();
}