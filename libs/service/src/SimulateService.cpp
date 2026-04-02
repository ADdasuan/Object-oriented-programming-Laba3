// SimulateService.cpp
#include "SimulateService.h"
#include "ForumService.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>


const int MESSAGE_MAX = 15;
const double MESSAGE_PROBABILITY = 0.4;
const unsigned MIN_BRAIN = 1;
const unsigned MAX_BRAIN = 10;
const float MIN_SIZE = 0.1;
const float MAX_SIZE = 1.0;

SimulateService::SimulateService(std::shared_ptr<Forum> forum,std::shared_ptr<UserService> userService,std::shared_ptr<SectionService> sectionService, std::shared_ptr<ForumService> forumService)
        : forum(forum), userService(userService), sectionService(sectionService), forumService(forumService) {}


int SimulateService::generateMessageCount() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::binomial_distribution<> dist(MESSAGE_MAX, MESSAGE_PROBABILITY);

    return dist(gen);
}

void SimulateService::generateMessages(const std::vector<std::string>& participants) const {
    std::string most_active_user = "";
    int max_messages = 0;

    for(const auto& user_login: participants){
        int messages = generateMessageCount();

        auto user = userService->get_user(user_login);
        if (user) {
            user->set_count_of_message(user->get_count_of_message() + messages);

            if (messages > max_messages) {
                max_messages = messages;
                most_active_user = user_login;
            }

            std::cout << "Напечатал" << user_login << ": " << messages << " сообщений\n";
        }
    }

    if (!most_active_user.empty() && max_messages > 0) {
        std::cout << "Самый активный участник раздела: " << most_active_user
                  << "__" << max_messages << " сообщений__\n";
    }
}


float SimulateService::generateRandomWidth() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(MIN_SIZE, MAX_SIZE);

    return dist(gen);
}

unsigned SimulateService::generateRandomIntelligence() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dist(MIN_BRAIN, MAX_BRAIN);

    return dist(gen);
}


std::string SimulateService::selectRandomParticipant(const std::vector<std::string>& participants) const {
    if (participants.empty()) {
        return "";
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, participants.size() - 1);

    return participants[dist(gen)];
}


void SimulateService::simulateStep(const std::string& section_name) {
    std::cout << "\n=== Симуляция раздела: " << section_name << " ===\n";

    if (!forumService->section_exists(section_name)) {
        std::cout << "Раздел не существует\n";
        return;
    }

    auto participants = forumService->getOnlineUsers(section_name);

    unsigned moderator_sum = forumService->getOnlineModeratorSum(section_name);
    float trolls_sum = forumService->getOnlineTrollInverseSum(section_name);

    auto moderators = forumService->getOnlineModerators(section_name);
    auto trolls = forumService->getOnlineTrolls(section_name);

    if (moderator_sum > trolls_sum && !trolls.empty()) {
            std::string troll_ban = selectRandomParticipant(trolls);
            if (userService->ban_user(troll_ban)) {
                sectionService->del_user_from_section(section_name, troll_ban);
                std::cout << " - Интеллект модераторов больше веса троллей -> Забанен тролль: " << troll_ban << "\n";
                for (const auto& trollLogin : trolls) {
                    userService->increment_troll_success(trollLogin);
                }
            }

    } else if (!trolls.empty()) {
            std::string user_ban = selectRandomParticipant(participants);
            bool is_moderator = userService->is_moderator(user_ban);

            if (userService->ban_user(user_ban)) {
                sectionService->del_user_from_section(section_name, user_ban);
                std::cout << "- Интеллект модераторов не превышает веса тролей -> забанен участник: " << user_ban << "\n";
                if(is_moderator){
                    sectionService->handle_main_moderator_banned(section_name, user_ban);
                }
                std::vector<std::string> based_users;
                for(const auto& login : participants){
                    if(!userService->is_troll(login) && !userService->is_moderator(login)){
                        based_users.push_back(login);
                    }
                }
                if(!based_users.empty()){
                    std::string new_troll = selectRandomParticipant(based_users);
                    float width = generateRandomWidth();

                    if(userService->convert_to_troll(new_troll, width)){
                        std::cout<<"Участник секции"<<new_troll<< " стал троллем"<< width <<"\n";
                    }
                } else{
                    std::cout<<"Нет участников для превращения в нового тролля\n";
                }
            }
    }

    auto current_participants = forumService->getOnlineUsers(section_name);

    std::string most_activ = "";

    int max_messages = 0;

    for (const auto& user_login : current_participants) {
        if (!userService->is_banned(user_login)) {
            int messages = generateMessageCount();
            auto user = userService->get_user(user_login);
            if (user) {
                user->set_count_of_message(user->get_count_of_message() + messages);

                if (messages > max_messages) {
                    max_messages = messages;
                    most_activ = user_login;
                }
            }
        }
    }

    if (!most_activ.empty()) {
        std::cout << "! Самый активный участник секции: " << most_activ << " (" << max_messages << " сообщений)\n";
    }
}

void SimulateService::predictionExistenceFuture(const std::string& section_name) {
    std::cout << "\n=== Предсказание будущего для раздела: " << section_name << " ===\n";

    auto participants = forumService->getOnlineUsers(section_name);
    auto moderators_sum = forumService->getOnlineModeratorSum(section_name);
    auto trolls_sum = forumService->getOnlineTrollInverseSum(section_name);


    if (moderators_sum > trolls_sum) {
        std::cout << "Предсказания для раздела: Стабильный состав \n";
    } else {
        std::cout << "Предсказание: Раздел будет опустошен \n";
    }

}

void SimulateService::predictSectionFutureThreadSafe(const std::string& section_name,std::map<std::string, std::string>& results) const {

    auto participants = forumService->getOnlineUsers(section_name);
    auto moderators_sum = forumService->getOnlineModeratorSum(section_name);
    auto trolls_sum = forumService->getOnlineTrollInverseSum(section_name);

    std::string prediction;
    if (moderators_sum > trolls_sum) {
        prediction = "Стабильный (модераторы контролируют ситуацию)";
    } else {
        prediction = "Опустошение (тролли доминируют)";
    }

    std::lock_guard<std::mutex> lock(prediction_mutex);
    results[section_name] = prediction;
}

void SimulateService::predictAllSectionsFuture() {
    std::cout << "\n=== Многопоточное Предсказание для всех разделов форума ===\n";

    auto sections = forum->get_all_sections();

    if (sections.empty()) {
        std::cout << "Нет разделов в форуме\n";
        return;
    }

    unsigned num_threads = std::thread::hardware_concurrency();

    std::cout<<"My Hardware_Concurrency is "<<num_threads<<" O KAK\n";

    if (num_threads == 0) num_threads = 2;

    if (num_threads > sections.size()) {
        num_threads = sections.size();
    }

    std::cout << "Анализ " << sections.size() << " секций в "<< num_threads << " потоках...\n\n";

    std::vector<std::string> section_names;
    section_names.reserve(sections.size());
    for (const auto& [name, section] : sections) {
        section_names.push_back(name);
    }

    std::map<std::string, std::string> results;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    size_t sections_per_thread = (section_names.size() + num_threads - 1) / num_threads;
    int x  = 0;
    for (unsigned i = 0; i < num_threads && i * sections_per_thread < section_names.size(); ++i) {
        size_t start_idx = i * sections_per_thread;
        size_t end_idx = std::min(start_idx + sections_per_thread, section_names.size());

        threads.emplace_back([this, &section_names, &results, start_idx, end_idx, i, &x]() {
            x++;
            std::cout << "Поток " << std::this_thread::get_id() << " обрабатывает разделы "
                      << start_idx + 1 << "-" << end_idx << "\n";

            for (size_t j = start_idx; j < end_idx; ++j) {
                predictSectionFutureThreadSafe(section_names[j], results);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "\nАнализ завершен за " << duration.count() << " мс\n\n";

    std::cout << "Итог предсказаний:\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << std::left << std::setw(30) << "Раздел"<< std::setw(40) << "Прогноз" << "\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& [name, prediction] : results) {
        std::cout << std::setw(30) << name << std::setw(40) << prediction << "\n";
    }

    std::cout << std::string(70, '=') << "\n";

}


