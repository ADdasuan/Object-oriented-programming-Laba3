#ifndef SAVE_H
#define SAVE_H

#include <memory>
#include <string>
#include "Forum.h"
#include "ForumDataConvector.h"

/**
 * @file SaveData.h
 * @brief Сервис сохранения состояния форума
 */

/**
 * @class Save
 * @brief Сохраняет состояние форума в файл или JSON строку
 */
class Save {
private:
    /**
    * @brief Конструктор сервиса сохранения
    * @param forum_instance Указатель на форум
    * @param converter_instance Указатель на конвертер
    */
    std::shared_ptr<Forum> forum_pointer; /// Указатель на форум для сохранения
    std::shared_ptr<ForumDataConverter> data_converter_pointer; /// Конвертер для преобразования

public:
    /**
    * @brief Конструктор сервиса сохранения
    * @param forum_instance Указатель на форум
    * @param converter_instance Указатель на конвертер
    */
    Save(std::shared_ptr<Forum> forum_instance,
         std::shared_ptr<ForumDataConverter> converter_instance);

    /**
    * @brief Сохраняет состояние форума в файл
     * @param file_path_string Путь к файлу для сохранения
     * @throws std::runtime_error при ошибке сохранения
    */
    void save_forum_state_to_file(const std::string& file_path_string);

    /**
    * @brief Экспортирует состояние форума в JSON строку
    * @return JSON-строка с состоянием форума
    * @throws std::runtime_error при ошибке сериализации
    */
    std::string export_forum_state_to_json_string() const;

private:
    /**
    * @brief Записать содержимое в файл
    * @param file_path_string Путь к файлу
    * @param file_content_string Содержимое для записи
    * @throws std::runtime_error если не удалось создать файл
    */
    void write_content_to_file(const std::string& file_path_string,
                              const std::string& file_content_string) const;
};

#endif