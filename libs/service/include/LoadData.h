#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <memory>
#include <string>
#include "Forum.h"
#include "ForumDataConvector.h"

/**
 * @file LoadData.h
 * @brief Сервис загрузки состояния форума
 */

/**
 * @class Load
 * @brief Загружает состояние форума из файла или JSON строки
 * @endcode
 */
class Load {
private:
    std::shared_ptr<Forum> forum_pointer; /// Указатель на форум для загрузки
    std::shared_ptr<ForumDataConverter> data_converter_pointer; /// Конвертер для преобразования

public:
    /**
    * @brief Конструктор сервиса загрузки
    * @param forum_instance Указатель на форум
    * @param converter_instance Указатель на конвертер
    */
    Load(std::shared_ptr<Forum> forum_instance,
         std::shared_ptr<ForumDataConverter> converter_instance);

    /**
     * @brief Загрузить состояние форума из файла
     * @param file_path_string Путь к файлу для загрузки
     * @throws std::runtime_error при ошибке загрузки или парсинга
     */
    void load_forum_state_from_file(const std::string& file_path_string);


    /**
     * @brief Импортировать состояние форума из JSON-строки
     * @param json_content_string JSON-строка с данными
     * @throws std::runtime_error при ошибке парсинга
     */
    void import_forum_state_from_json_string(const std::string& json_content_string);

private:
    /**
     * @brief Прочитать содержимое файла целиком
     * @param file_path_string Путь к файлу
     * @return Содержимое файла в виде строки
     * @throws std::runtime_error если не удалось открыть файл
     */
    std::string read_entire_file_content(const std::string& file_path_string) const;
};

#endif
