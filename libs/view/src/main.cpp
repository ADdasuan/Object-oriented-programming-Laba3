#include <iostream>
#include <memory>
//#include "StorageUsers.h"
#include "Forum.h"
#include "dialog.h"

int main() {
    try {
        auto forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);
        DialogController controller(forum);
        controller.run();

    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
