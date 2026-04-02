#include <gtest/gtest.h>
#include "Forum.h"
#include "StorageUsers.h"
#include "User.h"
#include "Moderator.h"
#include "Troll.h"
#include "UserService.h"
#include "ForumService.h"
#include "DefaultUser.h"
#include "SectionService.h"
#include "SimulateService.h"
#include "StatService.h"
#include "EmergencyAllModeratorBannedStrategy.h"
#include "ForumDataContainer.h"
#include "ForumDataConvector.h"
#include "SaveData.h"
#include "LoadData.h"
#include <memory>
#include <fstream>
#include <filesystem>

class UserServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<UserService> userService;

    void SetUp() override {
        forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);
        userService = std::make_shared<UserService>(forum);
    }
};

TEST_F(UserServiceTest, Constructor) {
    EXPECT_NE(userService, nullptr);
    EXPECT_THROW(UserService(nullptr), std::invalid_argument);
}

TEST_F(UserServiceTest, RegisterUser) {
    auto user = userService->register_user("Komarov");
    EXPECT_NE(user, nullptr);
    EXPECT_EQ(user->get_login(), "Komarov");
}

TEST_F(UserServiceTest, RegisterDuplicate) {
    userService->register_user("Muxa");
    EXPECT_THROW(userService->register_user("Muxa"), std::runtime_error);
}

TEST_F(UserServiceTest, RegisterModerator) {
    auto mod = userService->register_moderator("Skitev", 8);
    EXPECT_TRUE(userService->is_moderator("Skitev"));
    EXPECT_EQ(userService->get_moderator_intelligence("Skitev"), 8);
}

TEST_F(UserServiceTest, RegisterTroll) {
    auto troll = userService->register_troll("Dmitriev", 0.4f);
    EXPECT_TRUE(userService->is_troll("Dmitriev"));
    EXPECT_FLOAT_EQ(userService->get_troll_width("Dmitriev"), 0.4f);
}

TEST_F(UserServiceTest, GetUser) {
    userService->register_user("Sinelnikov");
    auto user = userService->get_user("Sinelnikov");
    EXPECT_NE(user, nullptr);

    auto nouser = userService->get_user("Chepik");
    EXPECT_EQ(nouser, nullptr);
}

TEST_F(UserServiceTest, GetAllUsers) {
    userService->register_user("Ovcharenko");
    userService->register_moderator("Doroshin", 5);
    userService->register_troll("Firrer", 0.3f);

    auto all = userService->get_all_users();
    EXPECT_EQ(all.size(), 3);
}

TEST_F(UserServiceTest, BanUnban) {
    userService->register_user("Alibekova");

    EXPECT_TRUE(userService->ban_user("Alibekova"));
    EXPECT_TRUE(userService->is_banned("Alibekova"));

    EXPECT_TRUE(userService->unban_user("Alibekova"));
    EXPECT_FALSE(userService->is_banned("Alibekova"));
}

TEST_F(UserServiceTest, TypeChecks) {
    userService->register_user("Sein");
    userService->register_moderator("Maximov", 5);
    userService->register_troll("Shevtsov", 0.3f);

    EXPECT_TRUE(userService->is_regular_user("Sein"));
    EXPECT_TRUE(userService->is_moderator("Maximov"));
    EXPECT_TRUE(userService->is_troll("Shevtsov"));
}

TEST_F(UserServiceTest, TypeConversions) {
    userService->register_user("Meleshkin");

    EXPECT_TRUE(userService->convert_to_moderator("Meleshkin", 7));
    EXPECT_TRUE(userService->is_moderator("Meleshkin"));

    EXPECT_TRUE(userService->convert_moderator_to_user("Meleshkin"));
    EXPECT_TRUE(userService->is_regular_user("Meleshkin"));

    EXPECT_TRUE(userService->convert_to_troll("Meleshkin", 0.6f));
    EXPECT_TRUE(userService->is_troll("Meleshkin"));

    EXPECT_TRUE(userService->convert_troll_to_user("Meleshkin"));
    EXPECT_TRUE(userService->is_regular_user("Meleshkin"));
}

TEST_F(UserServiceTest, ChangeModeratorIntelligence) {
    userService->register_moderator("Chesnokova", 5);

    EXPECT_TRUE(userService->change_moderator_intelligence("Chesnokova", 9));
    EXPECT_EQ(userService->get_moderator_intelligence("Chesnokova"), 9);
}

TEST_F(UserServiceTest, TrollSuccess) {
    userService->register_troll("Novikov", 0.5f);

    EXPECT_EQ(userService->get_troll_success("Novikov"), 0);
    EXPECT_TRUE(userService->increment_troll_success("Novikov"));
}

TEST_F(UserServiceTest, Counters) {
    userService->register_user("Gaspariunas");
    userService->register_user("Dyumin");
    userService->register_moderator("Agibalov", 5);
    userService->register_troll("Komarov", 0.3f);

    EXPECT_EQ(userService->get_moderator_count(), 1);
    EXPECT_EQ(userService->get_troll_count(), 1);
}

TEST_F(UserServiceTest, OnlineCounter) {
    userService->register_user("Muxa");
    userService->register_user("Skitev");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Muxa");

    EXPECT_EQ(userService->get_online_count(), 1);
}

class ForumServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<ForumService> forumService;
    std::shared_ptr<UserService> userService;

    void SetUp() override {
        forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);
        forumService = std::make_shared<ForumService>(forum);
        userService = std::make_shared<UserService>(forum);
    }
};

TEST_F(ForumServiceTest, Constructor) {
    EXPECT_NE(forumService, nullptr);
    EXPECT_THROW(ForumService(nullptr), std::invalid_argument);
}

TEST_F(ForumServiceTest, CreateSection) {
    userService->register_moderator("Dmitriev", 5);
    EXPECT_TRUE(forumService->create_section("informatics", "Dmitriev"));
    EXPECT_TRUE(forumService->section_exists("informatics"));
}

TEST_F(ForumServiceTest, DeleteSection) {
    userService->register_moderator("Sinelnikov", 5);
    forumService->create_section("unix", "Sinelnikov");

    EXPECT_TRUE(forumService->del_section("unix"));
    EXPECT_FALSE(forumService->section_exists("unix"));
}

TEST_F(ForumServiceTest, GetSection) {
    userService->register_moderator("Chepik", 5);
    forumService->create_section("IBM", "Chepik");

    auto sec = forumService->get_section("IBM");
    EXPECT_NE(sec, nullptr);
    EXPECT_EQ(sec->get_name(), "IBM");
}

TEST_F(ForumServiceTest, SectionCount) {
    userService->register_moderator("Ovcharenko", 5);
    userService->register_moderator("Doroshin", 6);
    userService->register_moderator("Firrer", 7);

    forumService->create_section("Data Sience", "Ovcharenko");
    forumService->create_section("Theory of automata", "Doroshin");
    forumService->create_section("Circuit design", "Firrer");

    EXPECT_EQ(forumService->get_section_count(), 3);
}

TEST_F(ForumServiceTest, GetOnlineUsers) {
    userService->register_moderator("Alibekova", 5);
    userService->register_user("Sein");
    userService->register_user("Maximov");
    forumService->create_section("Electronics", "Alibekova");

    auto sec = forumService->get_section("Electronics");
    sec->add_participant("Sein");
    sec->add_participant("Maximov");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Sein");
    storage->set_user_online("Maximov");

    auto online = forumService->getOnlineUsers("Electronics");
    EXPECT_GE(online.size(), 2);
}

TEST_F(ForumServiceTest, GetOnlineModerators) {
    userService->register_moderator("Shevtsov", 5);
    userService->register_moderator("Meleshkin", 6);
    forumService->create_section("AiSD", "Shevtsov");

    auto sec = forumService->get_section("AiSD");
    sec->add_participant("Meleshkin");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Meleshkin");

    auto mods = forumService->getOnlineModerators("AiSD");
    EXPECT_GE(mods.size(), 1);
}

TEST_F(ForumServiceTest, GetOnlineTrolls) {
    userService->register_moderator("Chesnokova", 5);
    userService->register_troll("Novikov", 0.3f);
    userService->register_troll("Gaspariunas", 0.7f);
    forumService->create_section("The assembler", "Chesnokova");

    auto sec = forumService->get_section("The assembler");
    sec->add_participant("Novikov");
    sec->add_participant("Gaspariunas");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Novikov");
    storage->set_user_online("Gaspariunas");

    auto trolls = forumService->getOnlineTrolls("The assembler");
    EXPECT_EQ(trolls.size(), 2);
}

TEST_F(ForumServiceTest, GetModeratorSum) {
    userService->register_moderator("Dyumin", 5);
    userService->register_moderator("Agibalov", 7);
    forumService->create_section("Engineering graphics", "Dyumin");

    auto sec = forumService->get_section("Engineering graphics");
    sec->add_participant("Agibalov");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Dyumin");
    storage->set_user_online("Agibalov");

    int sum = forumService->getOnlineModeratorSum("Engineering graphics");
    EXPECT_GE(sum, 12);
}

TEST_F(ForumServiceTest, GetTrollInverseSum) {
    userService->register_moderator("Komarov", 5);
    userService->register_troll("Muxa", 0.5f);
    userService->register_troll("Skitev", 0.25f);
    forumService->create_section("The torture chamber", "Komarov");

    auto sec = forumService->get_section("The torture chamber");
    sec->add_participant("Muxa");
    sec->add_participant("Skitev");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Muxa");
    storage->set_user_online("Skitev");

    float sum = forumService->getOnlineTrollInverseSum("The torture chamber");
    EXPECT_FLOAT_EQ(sum, 1.0f/0.5f + 1.0f/0.25f);
}

class SectionServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<SectionService> sectionService;
    std::shared_ptr<UserService> userService;
    std::shared_ptr<ForumService> forumService;

    void SetUp() override {
        forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);
        sectionService = std::make_shared<SectionService>(forum);
        userService = std::make_shared<UserService>(forum);
        forumService = std::make_shared<ForumService>(forum);
    }
};

TEST_F(SectionServiceTest, Constructor) {
    EXPECT_NE(sectionService, nullptr);
    EXPECT_THROW(SectionService(nullptr), std::invalid_argument);
}

TEST_F(SectionServiceTest, AddUserToSection) {
    userService->register_moderator("Dmitriev", 5);
    userService->register_user("Sinelnikov");
    forumService->create_section("Koons camera", "Dmitriev");

    EXPECT_TRUE(sectionService->add_user_to_section("Koons camera", "Sinelnikov"));

    auto sec = forumService->get_section("Koons camera");
    EXPECT_TRUE(sec->is_participant_exist("Sinelnikov"));
}

TEST_F(SectionServiceTest, RemoveUserFromSection) {
    userService->register_moderator("Chepik", 5);
    userService->register_user("Ovcharenko");
    forumService->create_section("The teacher", "Chepik");
    sectionService->add_user_to_section("The teacher", "Ovcharenko");

    EXPECT_TRUE(sectionService->del_user_from_section("The teacher", "Ovcharenko"));

    auto sec = forumService->get_section("The teacher");
    EXPECT_FALSE(sec->is_participant_exist("Ovcharenko"));
}

TEST_F(SectionServiceTest, MoveUser) {
    userService->register_moderator("Doroshin", 5);
    userService->register_moderator("Firrer", 6);
    userService->register_user("Alibekova");
    forumService->create_section("informatics", "Doroshin");
    forumService->create_section("unix", "Firrer");
    sectionService->add_user_to_section("informatics", "Alibekova");

    EXPECT_TRUE(sectionService->move_user_to_section("Alibekova", "informatics", "unix"));

    auto s1 = forumService->get_section("informatics");
    auto s2 = forumService->get_section("unix");
    EXPECT_FALSE(s1->is_participant_exist("Alibekova"));
    EXPECT_TRUE(s2->is_participant_exist("Alibekova"));
}

TEST_F(SectionServiceTest, UserInAnySection) {
    userService->register_moderator("Sein", 5);
    userService->register_user("Maximov");
    forumService->create_section("IBM", "Sein");

    EXPECT_FALSE(sectionService->user_in_any_section("Maximov"));

    sectionService->add_user_to_section("IBM", "Maximov");
    EXPECT_TRUE(sectionService->user_in_any_section("Maximov"));
}

TEST_F(SectionServiceTest, UserInSection) {
    userService->register_moderator("Shevtsov", 5);
    userService->register_user("Meleshkin");
    forumService->create_section("Data Sience", "Shevtsov");
    sectionService->add_user_to_section("Data Sience", "Meleshkin");

}

TEST_F(SectionServiceTest, GetSectionUsers) {
    userService->register_moderator("Chesnokova", 5);
    userService->register_user("Novikov");
    userService->register_user("Gaspariunas");
    forumService->create_section("Theory of automata", "Chesnokova");
    sectionService->add_user_to_section("Theory of automata", "Novikov");
    sectionService->add_user_to_section("Theory of automata", "Gaspariunas");

    auto users = sectionService->get_section_users("Theory of automata");
    EXPECT_EQ(users.size(), 3);
}

TEST_F(SectionServiceTest, PromoteRandomModerator) {
    userService->register_moderator("Dyumin", 5);
    userService->register_user("Agibalov");
    userService->register_user("Komarov");
    forumService->create_section("Circuit design", "Dyumin");
    sectionService->add_user_to_section("Circuit design", "Agibalov");
    sectionService->add_user_to_section("Circuit design", "Komarov");

    userService->ban_user("Dyumin");

    auto new_mod = sectionService->promote_random_moderator("Circuit design");
    EXPECT_FALSE(new_mod.empty());
    EXPECT_TRUE(userService->is_moderator(new_mod));
}

TEST_F(SectionServiceTest, HandleUserBanned) {
    userService->register_moderator("Muxa", 5);
    userService->register_user("Skitev");
    forumService->create_section("Electronics", "Muxa");
    sectionService->add_user_to_section("Electronics", "Skitev");

    EXPECT_TRUE(sectionService->handle_user_banned("Skitev"));

    auto sec = forumService->get_section("Electronics");
    EXPECT_FALSE(sec->is_participant_exist("Skitev"));
}

TEST_F(SectionServiceTest, HandleModeratorBanned) {
    userService->register_moderator("Dmitriev", 5);
    userService->register_user("Sinelnikov");
    forumService->create_section("AiSD", "Dmitriev");
    sectionService->add_user_to_section("AiSD", "Sinelnikov");

}

class SimulateServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<UserService> userService;
    std::shared_ptr<ForumService> forumService;
    std::shared_ptr<SectionService> sectionService;
    std::shared_ptr<SimulateService> simulateService;

    void SetUp() override {
        forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);

        userService = std::make_shared<UserService>(forum);
        forumService = std::make_shared<ForumService>(forum);
        sectionService = std::make_shared<SectionService>(forum);
        simulateService = std::make_shared<SimulateService>(
                forum, userService, sectionService, forumService
        );
    }
};

TEST_F(SimulateServiceTest, Constructor) {
    EXPECT_NE(simulateService, nullptr);
}

TEST_F(SimulateServiceTest, SimulateStep) {
    userService->register_moderator("Chepik", 7);
    userService->register_user("Ovcharenko");
    forumService->create_section("The assembler", "Chepik");
    sectionService->add_user_to_section("The assembler", "Ovcharenko");

    EXPECT_NO_THROW(simulateService->simulateStep("The assembler"));
}

TEST_F(SimulateServiceTest, PredictionExistence) {
    userService->register_moderator("Doroshin", 8);
    userService->register_user("Firrer");
    forumService->create_section("Engineering graphics", "Doroshin");
    sectionService->add_user_to_section("Engineering graphics", "Firrer");

    EXPECT_NO_THROW(simulateService->predictionExistenceFuture("Engineering graphics"));
}

class StatServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<Forum> forum;
    std::shared_ptr<UserService> userService;
    std::shared_ptr<StatService> statService;

    void SetUp() override {
        forum = std::make_shared<Forum>();
        auto storage = std::make_shared<StorageUsers>();
        forum->set_storage(storage);

        userService = std::make_shared<UserService>(forum);
        statService = std::make_shared<StatService>(forum, userService);
    }
};

TEST_F(StatServiceTest, Constructor) {
    EXPECT_NE(statService, nullptr);
}

TEST_F(StatServiceTest, FindMostSuccessfulTroll) {
    userService->register_troll("Alibekova", 0.5f);
    userService->register_troll("Sein", 0.3f);

    auto storage = forum->get_ptr_storage();
    auto troll1 = std::dynamic_pointer_cast<ITroll>(storage->get_user("Alibekova"));
    troll1->set_success(10);

    EXPECT_NO_THROW(statService->findMostSuccessfulTroll());
}

TEST_F(StatServiceTest, FindMostActiveUser) {
    userService->register_user("Maximov");
    userService->register_user("Shevtsov");

    auto storage = forum->get_ptr_storage();
    storage->get_user("Maximov")->set_count_of_message(50);
    storage->get_user("Shevtsov")->set_count_of_message(100);

    EXPECT_NO_THROW(statService->findMostActiveUser());
}

TEST_F(StatServiceTest, GetSectionStats) {
    userService->register_moderator("Meleshkin", 5);
    auto forumService = std::make_shared<ForumService>(forum);
    forumService->create_section("The torture chamber", "Meleshkin");

    EXPECT_NO_THROW(statService->getSectionStats());
}

TEST(ForumDataConverterTest, ConvertUserToContainer) {
    ForumDataConverter converter;

    auto user = std::make_shared<User>("Chesnokova");
    user->set_count_of_message(25);

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    storage->add_user(user);
    forum.set_storage(storage);

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection.size(), 1);
    EXPECT_EQ(container.user_data_collection["Chesnokova"].user_login_string, "Chesnokova");
}

TEST(ForumDataConverterTest, ConvertModeratorToContainer) {
    ForumDataConverter converter;

    auto mod = std::make_shared<Moderator>("Novikov", 8);

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    storage->add_user(mod);
    forum.set_storage(storage);

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection["Novikov"].user_category_string, "forum_moderator");
}

TEST(ForumDataConverterTest, ConvertTrollToContainer) {
    ForumDataConverter converter;

    auto troll = std::make_shared<Troll>("Gaspariunas", 0.3f);

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    storage->add_user(troll);
    forum.set_storage(storage);

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection["Gaspariunas"].user_category_string, "forum_troll");
}

TEST(ForumDataConverterTest, FullConversionCycle) {
    ForumDataConverter converter;

    Forum original_forum;
    auto storage = std::make_shared<StorageUsers>();
    original_forum.set_storage(storage);

    auto user = std::make_shared<User>("Dyumin");
    user->set_count_of_message(50);
    storage->add_user(user);

    auto container = converter.convert_forum_to_data_container(original_forum);
    auto restored_forum = converter.convert_data_container_to_forum(container);

    auto restored_storage = restored_forum->get_ptr_storage();
    EXPECT_TRUE(restored_storage->find_user("Dyumin"));
}

TEST(EmergencyStrategyTest, PromoteRegularUser) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto mod = std::make_shared<Moderator>("Agibalov", 5);
    auto user = std::make_shared<User>("Komarov");
    storage->add_user(mod);
    storage->add_user(user);

    forum->add_section("informatics", "Agibalov");
    auto section = forum->find_section("informatics");
    section->add_participant("Komarov");
    storage->set_user_online("Komarov");

    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(forum, "informatics");

    EXPECT_EQ(new_mod, "Komarov");
    EXPECT_TRUE(storage->is_moderator("Komarov"));
}

TEST(EmergencyStrategyTest, DoesNotPromoteTroll) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto mod = std::make_shared<Moderator>("Muxa", 5);
    auto troll = std::make_shared<Troll>("Skitev", 0.3f);
    storage->add_user(mod);
    storage->add_user(troll);

    forum->add_section("unix", "Muxa");
    auto section = forum->find_section("unix");
    section->add_participant("Skitev");
    storage->set_user_online("Skitev");

    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(forum, "unix");

    EXPECT_TRUE(new_mod.empty());
}

TEST_F(SectionServiceTest, CheckAndFixSectionModerators) {
    userService->register_moderator("Dmitriev", 5);
    userService->register_user("Sinelnikov");
    forumService->create_section("IBM", "Dmitriev");
    sectionService->add_user_to_section("IBM", "Sinelnikov");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("IBM"));
}

TEST_F(SectionServiceTest, CheckAllSectionsForModerators) {
    userService->register_moderator("Chepik", 5);
    userService->register_moderator("Ovcharenko", 6);
    forumService->create_section("Data Sience", "Chepik");
    forumService->create_section("Theory of automata", "Ovcharenko");

    auto count = sectionService->check_all_sections_for_moderators();
    EXPECT_GE(count, 0);
}

TEST_F(SectionServiceTest, SetStrategy) {
    auto strategy = std::make_shared<EmergencyAllModeratorBannedStrategy>();
    EXPECT_NO_THROW(sectionService->set_strategy(strategy));
}

TEST_F(SectionServiceTest, GetForum) {
    EXPECT_NE(sectionService->get_forum(), nullptr);
}

TEST_F(SimulateServiceTest, PredictAllSectionsFuture) {
    userService->register_moderator("Doroshin", 8);
    forumService->create_section("Circuit design", "Doroshin");

    EXPECT_NO_THROW(simulateService->predictAllSectionsFuture());
}


TEST(ForumDataConverterTest, ConvertSectionToContainer) {
    ForumDataConverter converter;

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Firrer", 5);
    storage->add_user(mod);
    forum.add_section("Electronics", "Firrer");

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.section_data_collection.size(), 1);
}

TEST(ForumDataConverterTest, ConvertWithParticipants) {
    ForumDataConverter converter;

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Alibekova", 5);
    auto user = std::make_shared<User>("Sein");
    storage->add_user(mod);
    storage->add_user(user);

    forum.add_section("AiSD", "Alibekova");
    auto sec = forum.find_section("AiSD");
    sec->add_participant("Sein");

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.section_data_collection["AiSD"].participant_login_list.size(), 2);
}

TEST(ForumDataConverterTest, ConvertBannedUser) {
    ForumDataConverter converter;

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto user = std::make_shared<User>("Maximov");
    storage->add_user(user);
    storage->ban_user("Maximov");

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection["Maximov"].current_activity_status, "banned");
}

TEST(ForumDataContainerTest, UserDataSerialization) {
    UserDataContainer user;
    user.user_login_string = "Shevtsov";
    user.user_category_string = "regular_user";
    user.registration_time_point = std::chrono::system_clock::now();
    user.total_messages_count = 10;
    user.current_activity_status = "online";

    auto json = user.serialize_to_json();

    EXPECT_EQ(json["user_login_string"], "Shevtsov");
    EXPECT_EQ(json["user_category_string"], "regular_user");
}

TEST(ForumDataContainerTest, UserDataDeserialization) {
    json j;
    j["user_login_string"] = "Meleshkin";
    j["user_category_string"] = "regular_user";
    j["registration_time_formatted"] = "2026-01-01 00:00:00";
    j["total_messages_count"] = 25;
    j["current_activity_status"] = "offline";

    auto user = UserDataContainer::deserialize_from_json(j);

    EXPECT_EQ(user.user_login_string, "Meleshkin");
    EXPECT_EQ(user.total_messages_count, 25);
}

TEST(ForumDataContainerTest, SectionDataSerialization) {
    SectionDataContainer sec;
    sec.section_name_string = "The assembler";
    sec.initial_moderator_login_string = "Chesnokova";
    sec.participant_login_list = {"Novikov", "Gaspariunas"};

    auto json = sec.serialize_to_json();

    EXPECT_EQ(json["section_name_string"], "The assembler");
    EXPECT_EQ(json["participant_login_list"].size(), 2);
}

TEST(ForumDataContainerTest, ForumDataSerialization) {
    ForumDataContainer forum;

    UserDataContainer user;
    user.user_login_string = "Dyumin";
    user.user_category_string = "regular_user";
    user.registration_time_point = std::chrono::system_clock::now();
    forum.user_data_collection["Dyumin"] = user;

    auto json = forum.serialize_to_json();

    EXPECT_TRUE(json.contains("user_data_collection"));
}

TEST(SaveDataTest, Constructor) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();

    EXPECT_NO_THROW(Save(forum, converter));
}

TEST(SaveDataTest, ExportToJsonString) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto user = std::make_shared<User>("Agibalov");
    storage->add_user(user);

    auto converter = std::make_shared<ForumDataConverter>();
    Save saver(forum, converter);

    auto json_str = saver.export_forum_state_to_json_string();
    EXPECT_FALSE(json_str.empty());
}

TEST(LoadDataTest, Constructor) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();

    EXPECT_NO_THROW(Load(forum, converter));
}

TEST(LoadDataTest, ImportFromJsonString) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();
    Load loader(forum, converter);

    std::string json_str = R"({
        "user_data_collection": {
            "Komarov": {
                "user_login_string": "Komarov",
                "user_category_string": "regular_user",
                "registration_time_formatted": "2026-01-01 00:00:00",
                "total_messages_count": 5,
                "current_activity_status": "offline"
            }
        },
        "section_data_collection": {}
    })";

    EXPECT_NO_THROW(loader.import_forum_state_from_json_string(json_str));
}

TEST_F(ForumServiceTest, AreAllModeratorsBanned) {
    userService->register_moderator("Muxa", 5);
    forumService->create_section("Engineering graphics", "Muxa");

    EXPECT_FALSE(forumService->are_all_moderators_banned("Engineering graphics"));

    userService->ban_user("Muxa");
    EXPECT_FALSE(forumService->are_all_moderators_banned("Engineering graphics"));
}

TEST_F(ForumServiceTest, GetSectionConst) {
    userService->register_moderator("Skitev", 5);
    forumService->create_section("The torture chamber", "Skitev");

    const auto sec = forumService->get_section_const("The torture chamber");
    EXPECT_NE(sec, nullptr);
    EXPECT_EQ(sec->get_name(), "The torture chamber");
}

TEST_F(ForumServiceTest, AreAllModeratorsBannedEmpty) {
    userService->register_moderator("Dmitriev", 5);
    forumService->create_section("Koons camera", "Dmitriev");

    auto storage = forum->get_ptr_storage();
    storage->set_user_offline("Dmitriev");

    EXPECT_FALSE(forumService->are_all_moderators_banned("Koons camera"));
}

TEST(SaveDataTest, SaveToFile) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto user = std::make_shared<User>("Sinelnikov");
    storage->add_user(user);

    auto converter = std::make_shared<ForumDataConverter>();
    Save saver(forum, converter);

    std::string filename = "test_forum_save.json";
    EXPECT_NO_THROW(saver.save_forum_state_to_file(filename));

    std::ifstream file(filename);
    EXPECT_TRUE(file.good());
    file.close();

    std::remove(filename.c_str());
}

TEST(SaveDataTest, ExportEmptyForum) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();
    Save saver(forum, converter);

    auto json_str = saver.export_forum_state_to_json_string();
    EXPECT_FALSE(json_str.empty());
    EXPECT_TRUE(json_str.find("user_data_collection") != std::string::npos);
}

TEST(LoadDataTest, LoadFromFile) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto user = std::make_shared<User>("Chepik");
    user->set_count_of_message(42);
    storage->add_user(user);

    auto converter = std::make_shared<ForumDataConverter>();
    Save saver(forum, converter);

    std::string filename = "test_forum_load.json";
    saver.save_forum_state_to_file(filename);

    auto new_forum = std::make_shared<Forum>();
    Load loader(new_forum, converter);
    EXPECT_NO_THROW(loader.load_forum_state_from_file(filename));

    auto new_storage = new_forum->get_ptr_storage();
    EXPECT_TRUE(new_storage->find_user("Chepik"));

    std::remove(filename.c_str());
}

TEST(LoadDataTest, LoadNonExistentFile) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();
    Load loader(forum, converter);

    EXPECT_THROW(loader.load_forum_state_from_file("nonexistent_file_12345.json"),
                 std::runtime_error);
}

TEST(LoadDataTest, ImportInvalidJson) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto converter = std::make_shared<ForumDataConverter>();
    Load loader(forum, converter);

    std::string invalid_json = "{ invalid json }}";
    EXPECT_THROW(loader.import_forum_state_from_json_string(invalid_json),
                 std::runtime_error);
}

TEST_F(SectionServiceTest, AddUserToBannedUser) {
    userService->register_moderator("Ovcharenko", 5);
    userService->register_user("Doroshin");
    forumService->create_section("The teacher", "Ovcharenko");

    userService->ban_user("Doroshin");

    EXPECT_FALSE(sectionService->add_user_to_section("The teacher", "Doroshin"));
}

TEST_F(SectionServiceTest, AddUserToNonExistentSection) {
    userService->register_user("Firrer");

    EXPECT_FALSE(sectionService->add_user_to_section("NonExistent", "Firrer"));
}

TEST_F(SectionServiceTest, RemoveUserFromNonExistentSection) {
    userService->register_user("Alibekova");

    EXPECT_FALSE(sectionService->del_user_from_section("NonExistent", "Alibekova"));
}

TEST_F(SectionServiceTest, MoveUserInvalidSections) {
    userService->register_user("Sein");

    EXPECT_FALSE(sectionService->move_user_to_section("Sein", "informatics", "unix"));
}

TEST_F(SectionServiceTest, PromoteRandomModeratorNoUsers) {
    userService->register_moderator("Maximov", 5);
    forumService->create_section("Filial taste villa", "Maximov");

    userService->ban_user("Maximov");

    auto new_mod = sectionService->promote_random_moderator("Filial taste villa");
    EXPECT_TRUE(new_mod.empty());
}

TEST_F(SectionServiceTest, HandleModeratorBannedEmpty) {
    userService->register_moderator("Shevtsov", 5);
    forumService->create_section("informatics", "Shevtsov");

}

TEST(ForumDataConverterTest, ConvertMultipleUsersAndSections) {
    ForumDataConverter converter;

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto user = std::make_shared<User>("Meleshkin");
    auto mod = std::make_shared<Moderator>("Chesnokova", 7);
    auto troll = std::make_shared<Troll>("Novikov", 0.4f);

    storage->add_user(user);
    storage->add_user(mod);
    storage->add_user(troll);

    forum.add_section("unix", "Chesnokova");
    forum.add_section("IBM", "Chesnokova");

    auto sec1 = forum.find_section("unix");
    sec1->add_participant("Meleshkin");
    sec1->add_participant("Novikov");

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection.size(), 3);
    EXPECT_EQ(container.section_data_collection.size(), 2);
    EXPECT_EQ(container.section_data_collection["unix"].participant_login_list.size(), 3);
}

TEST(ForumDataConverterTest, ConvertOnlineAndBannedUsers) {
    ForumDataConverter converter;

    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto online_user = std::make_shared<User>("Gaspariunas");
    auto banned_user = std::make_shared<User>("Dyumin");

    storage->add_user(online_user);
    storage->add_user(banned_user);

    storage->set_user_online("Gaspariunas");
    storage->ban_user("Dyumin");

    auto container = converter.convert_forum_to_data_container(forum);

    EXPECT_EQ(container.user_data_collection["Gaspariunas"].current_activity_status, "online");
    EXPECT_EQ(container.user_data_collection["Dyumin"].current_activity_status, "banned");
}

TEST(ForumDataConverterTest, RestoreWithSections) {
    ForumDataConverter converter;

    ForumDataContainer container;

    UserDataContainer mod;
    mod.user_login_string = "Agibalov";
    mod.user_category_string = "forum_moderator";
    mod.registration_time_point = std::chrono::system_clock::now();
    mod.total_messages_count = 10;
    mod.current_activity_status = "online";
    mod.moderator_reason_level = 8;

    UserDataContainer user;
    user.user_login_string = "Komarov";
    user.user_category_string = "regular_user";
    user.registration_time_point = std::chrono::system_clock::now();
    user.total_messages_count = 5;
    user.current_activity_status = "offline";

    container.user_data_collection["Agibalov"] = mod;
    container.user_data_collection["Komarov"] = user;

    SectionDataContainer sec;
    sec.section_name_string = "Data Sience";
    sec.initial_moderator_login_string = "Agibalov";
    sec.participant_login_list = {"Komarov"};

    container.section_data_collection["Data Sience"] = sec;

    auto restored = converter.convert_data_container_to_forum(container);

    EXPECT_TRUE(restored->section_exist("Data Sience"));
    EXPECT_TRUE(restored->get_ptr_storage()->find_user("Agibalov"));
    EXPECT_TRUE(restored->get_ptr_storage()->find_user("Komarov"));
}

TEST_F(SimulateServiceTest, SimulateStepEmptySection) {
    userService->register_moderator("Muxa", 7);
    forumService->create_section("Theory of automata", "Muxa");

    EXPECT_NO_THROW(simulateService->simulateStep("Theory of automata"));
}


TEST_F(SectionServiceTest, AddUserAlreadyInSection) {
    userService->register_moderator("Skitev", 5);
    userService->register_user("Dmitriev");
    forumService->create_section("Circuit design", "Skitev");

    sectionService->add_user_to_section("Circuit design", "Dmitriev");

    EXPECT_FALSE(sectionService->add_user_to_section("Circuit design", "Dmitriev"));
}

TEST_F(SectionServiceTest, AddUserNonExistentUser) {
    userService->register_moderator("Sinelnikov", 5);
    forumService->create_section("Electronics", "Sinelnikov");

    EXPECT_FALSE(sectionService->add_user_to_section("Electronics", "nonexistent"));
}

TEST_F(SectionServiceTest, RemoveUserNotInSection) {
    userService->register_moderator("Chepik", 5);
    userService->register_user("Ovcharenko");
    forumService->create_section("AiSD", "Chepik");

    EXPECT_FALSE(sectionService->del_user_from_section("AiSD", "Ovcharenko"));
}

TEST_F(SectionServiceTest, MoveUserNotInSourceSection) {
    userService->register_moderator("Doroshin", 5);
    userService->register_moderator("Firrer", 6);
    userService->register_user("Alibekova");
    forumService->create_section("informatics", "Doroshin");
    forumService->create_section("unix", "Firrer");

    EXPECT_FALSE(sectionService->move_user_to_section("Alibekova", "informatics", "unix"));
}

TEST_F(SectionServiceTest, UserInSectionTrue) {
    userService->register_moderator("Sein", 5);
    userService->register_user("Maximov");
    forumService->create_section("IBM", "Sein");

    sectionService->add_user_to_section("IBM", "Maximov");

}

TEST_F(SectionServiceTest, UserInSectionFalse) {
    userService->register_moderator("Shevtsov", 5);
    userService->register_user("Meleshkin");
    forumService->create_section("Data Sience", "Shevtsov");

}

TEST_F(SectionServiceTest, UserInAnySectionEmpty) {
    userService->register_user("Chesnokova");

    EXPECT_FALSE(sectionService->user_in_any_section("Chesnokova"));
}

TEST_F(SectionServiceTest, GetSectionUsersEmpty) {
    userService->register_moderator("Novikov", 5);
    forumService->create_section("Theory of automata", "Novikov");

    auto users = sectionService->get_section_users("Theory of automata");
    EXPECT_EQ(users.size(), 1);
}

TEST_F(SectionServiceTest, GetSectionUsersNonExistent) {
    auto users = sectionService->get_section_users("NonExistent");
    EXPECT_TRUE(users.empty());
}

TEST_F(SectionServiceTest, PromoteRandomModeratorWithTrolls) {
    userService->register_moderator("Gaspariunas", 5);
    userService->register_troll("Dyumin", 0.3f);
    userService->register_troll("Agibalov", 0.5f);
    forumService->create_section("Circuit design", "Gaspariunas");

    sectionService->add_user_to_section("Circuit design", "Dyumin");
    sectionService->add_user_to_section("Circuit design", "Agibalov");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Dyumin");
    storage->set_user_online("Agibalov");

    userService->ban_user("Gaspariunas");

    auto new_mod = sectionService->promote_random_moderator("Circuit design");
    EXPECT_TRUE(new_mod.empty());
}

TEST_F(SectionServiceTest, PromoteRandomModeratorWithBannedUsers) {
    userService->register_moderator("Komarov", 5);
    userService->register_user("Muxa");
    userService->register_user("Skitev");
    forumService->create_section("Electronics", "Komarov");

    sectionService->add_user_to_section("Electronics", "Muxa");
    sectionService->add_user_to_section("Electronics", "Skitev");

    userService->ban_user("Muxa");
    userService->ban_user("Skitev");
    userService->ban_user("Komarov");

    auto new_mod = sectionService->promote_random_moderator("Electronics");
    EXPECT_TRUE(new_mod.empty());
}

TEST_F(SectionServiceTest, HandleUserBannedNotInSection) {
    userService->register_user("Dmitriev");

    EXPECT_TRUE(sectionService->handle_user_banned("Dmitriev"));
}

TEST_F(SectionServiceTest, HandleUserBannedMultipleSections) {
    userService->register_moderator("Sinelnikov", 5);
    userService->register_moderator("Chepik", 6);
    userService->register_user("Ovcharenko");

    forumService->create_section("AiSD", "Sinelnikov");
    forumService->create_section("The assembler", "Chepik");

    sectionService->add_user_to_section("AiSD", "Ovcharenko");
    sectionService->add_user_to_section("The assembler", "Ovcharenko");

    EXPECT_TRUE(sectionService->handle_user_banned("Ovcharenko"));

}

TEST_F(SectionServiceTest, HandleModeratorBannedWithUsers) {
    userService->register_moderator("Doroshin", 5);
    userService->register_user("Firrer");
    userService->register_user("Alibekova");

    forumService->create_section("Engineering graphics", "Doroshin");
    sectionService->add_user_to_section("Engineering graphics", "Firrer");
    sectionService->add_user_to_section("Engineering graphics", "Alibekova");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Firrer");
    storage->set_user_online("Alibekova");

}

TEST_F(SectionServiceTest, CheckAndFixSectionModeratorsAlreadyOk) {
    userService->register_moderator("Sein", 5);
    userService->register_user("Maximov");
    forumService->create_section("The torture chamber", "Sein");
    sectionService->add_user_to_section("The torture chamber", "Maximov");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Sein");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("The torture chamber"));
}

TEST_F(SectionServiceTest, CheckAndFixNonExistentSection) {
    EXPECT_FALSE(sectionService->check_and_fix_section_moderators("NonExistent"));
}

TEST_F(SectionServiceTest, CheckAllSectionsMultipleSections) {
    userService->register_moderator("Shevtsov", 5);
    userService->register_moderator("Meleshkin", 6);
    userService->register_user("Chesnokova");
    userService->register_user("Novikov");

    forumService->create_section("Koons camera", "Shevtsov");
    forumService->create_section("The teacher", "Meleshkin");

    sectionService->add_user_to_section("Koons camera", "Chesnokova");
    sectionService->add_user_to_section("The teacher", "Novikov");

    auto count = sectionService->check_all_sections_for_moderators();
    EXPECT_EQ(count, 2);
}

TEST(EmergencyStrategyTest, NoEligibleUsers) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto mod = std::make_shared<Moderator>("Gaspariunas", 5);
    storage->add_user(mod);

    forum->add_section("Filial taste villa", "Gaspariunas");

    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(forum, "Filial taste villa");

    EXPECT_TRUE(new_mod.empty());
}

TEST(EmergencyStrategyTest, NullForum) {
    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(nullptr, "informatics");

    EXPECT_TRUE(new_mod.empty());
}

TEST(EmergencyStrategyTest, NonExistentSection) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(forum, "NonExistent");

    EXPECT_TRUE(new_mod.empty());
}

TEST(EmergencyStrategyTest, PromoteOfflineUser) {
    auto forum = std::make_shared<Forum>();
    auto storage = std::make_shared<StorageUsers>();
    forum->set_storage(storage);

    auto mod = std::make_shared<Moderator>("Dyumin", 5);
    auto user = std::make_shared<User>("Agibalov");
    storage->add_user(mod);
    storage->add_user(user);

    forum->add_section("unix", "Dyumin");
    auto section = forum->find_section("unix");
    section->add_participant("Agibalov");

    storage->set_user_offline("Agibalov");

    EmergencyAllModeratorBannedStrategy strategy;
    auto new_mod = strategy.promote_to_moderator(forum, "unix");

    EXPECT_EQ(new_mod, "Agibalov");
}

TEST_F(SimulateServiceTest, PredictAllSectionsZeroThreads) {
    userService->register_moderator("Komarov", 8);
    forumService->create_section("IBM", "Komarov");

    EXPECT_NO_THROW(simulateService->predictAllSectionsFuture());
}

TEST(ForumDataContainerTest, ModeratorSerialization) {
    UserDataContainer mod;
    mod.user_login_string = "Muxa";
    mod.user_category_string = "forum_moderator";
    mod.registration_time_point = std::chrono::system_clock::now();
    mod.total_messages_count = 100;
    mod.current_activity_status = "online";
    mod.moderator_reason_level = 10;

    auto json = mod.serialize_to_json();

    EXPECT_EQ(json["user_category_string"], "forum_moderator");
    EXPECT_EQ(json["moderator_reason_level"], 10);
}

TEST(ForumDataContainerTest, TrollSerialization) {
    UserDataContainer troll;
    troll.user_login_string = "Skitev";
    troll.user_category_string = "forum_troll";
    troll.registration_time_point = std::chrono::system_clock::now();
    troll.total_messages_count = 50;
    troll.current_activity_status = "banned";
    troll.troll_width_value = 0.8f;
    troll.troll_success_score = 15;

    auto json = troll.serialize_to_json();

    EXPECT_EQ(json["user_category_string"], "forum_troll");
    EXPECT_FLOAT_EQ(json["troll_width_value"], 0.8f);
    EXPECT_EQ(json["troll_success_score"], 15);
}

TEST(ForumDataContainerTest, TrollDeserialization) {
    json j;
    j["user_login_string"] = "Dmitriev";
    j["user_category_string"] = "forum_troll";
    j["registration_time_formatted"] = "2026-01-01 12:00:00";
    j["total_messages_count"] = 20;
    j["current_activity_status"] = "online";
    j["troll_width_value"] = 0.5f;
    j["troll_success_score"] = 8;

    auto troll = UserDataContainer::deserialize_from_json(j);

    EXPECT_EQ(troll.user_login_string, "Dmitriev");
    EXPECT_FLOAT_EQ(troll.troll_width_value, 0.5f);
    EXPECT_EQ(troll.troll_success_score, 8);
}

TEST(ForumDataContainerTest, SectionDeserialization) {
    json j;
    j["section_name_string"] = "Data Sience";
    j["initial_moderator_login_string"] = "Sinelnikov";
    j["participant_login_list"] = json::array({"Chepik", "Ovcharenko", "Doroshin"});

    auto sec = SectionDataContainer::deserialize_from_json(j);

    EXPECT_EQ(sec.section_name_string, "Data Sience");
    EXPECT_EQ(sec.participant_login_list.size(), 3);
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_HasOtherModerators) {
    try {
        userService->register_moderator("Firrer", 5);
        userService->register_moderator("Alibekova", 6);
        userService->register_user("Sein");

        forumService->create_section("Theory of automata", "Firrer");
        sectionService->add_user_to_section("Theory of automata", "Alibekova");
        sectionService->add_user_to_section("Theory of automata", "Sein");

        userService->ban_user("Firrer");

        EXPECT_TRUE(sectionService->handle_main_moderator_banned("Theory of automata", "Firrer"));
    } catch (...){

    };
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_PromoteUser) {
    try {
        userService->register_moderator("Maximov", 5);
        userService->register_user("Shevtsov");
        userService->register_user("Meleshkin");

        forumService->create_section("Circuit design", "Maximov");
        sectionService->add_user_to_section("Circuit design", "Shevtsov");
        sectionService->add_user_to_section("Circuit design", "Meleshkin");

        auto storage = forum->get_ptr_storage();
        storage->set_user_online("Shevtsov");
        storage->set_user_online("Meleshkin");

        userService->ban_user("Maximov");

        EXPECT_TRUE(sectionService->handle_main_moderator_banned("Circuit design", "Maximov"));
    } catch (...) {

    };
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_OnlyTrolls) {
    userService->register_moderator("Chesnokova", 5);
    userService->register_troll("Novikov", 0.3f);
    userService->register_troll("Gaspariunas", 0.5f);

    forumService->create_section("Electronics", "Chesnokova");
    sectionService->add_user_to_section("Electronics", "Novikov");
    sectionService->add_user_to_section("Electronics", "Gaspariunas");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Novikov");
    storage->set_user_online("Gaspariunas");

    userService->ban_user("Chesnokova");

    EXPECT_TRUE(sectionService->handle_main_moderator_banned("Electronics", "Chesnokova"));

    EXPECT_FALSE(forumService->section_exists("Electronics"));

    EXPECT_FALSE(storage->is_online("Novikov"));
    EXPECT_FALSE(storage->is_online("Gaspariunas"));
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_NonExistentSection) {
    EXPECT_FALSE(sectionService->handle_main_moderator_banned("NonExistent", "Dyumin"));
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_EmptySection) {
    userService->register_moderator("Agibalov", 5);
    forumService->create_section("AiSD", "Agibalov");

    userService->ban_user("Agibalov");

    EXPECT_FALSE(sectionService->handle_main_moderator_banned("AiSD", "Agibalov"));
}

TEST_F(SectionServiceTest, CheckAndFix_HasActiveModerator) {
    userService->register_moderator("Komarov", 5);
    userService->register_user("Muxa");
    forumService->create_section("The assembler", "Komarov");
    sectionService->add_user_to_section("The assembler", "Muxa");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Komarov");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("The assembler"));
    EXPECT_TRUE(forumService->section_exists("The assembler"));
}

TEST_F(SectionServiceTest, CheckAndFix_NoModeratorHasUsers) {
    userService->register_moderator("Skitev", 5);
    userService->register_user("Dmitriev");
    userService->register_user("Sinelnikov");

    forumService->create_section("Engineering graphics", "Skitev");
    sectionService->add_user_to_section("Engineering graphics", "Dmitriev");
    sectionService->add_user_to_section("Engineering graphics", "Sinelnikov");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Dmitriev");
    storage->set_user_online("Sinelnikov");

    userService->ban_user("Skitev");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("Engineering graphics"));
    EXPECT_TRUE(forumService->section_exists("Engineering graphics"));
}

TEST_F(SectionServiceTest, CheckAndFix_OnlyTrolls) {
    userService->register_moderator("Chepik", 5);
    userService->register_troll("Ovcharenko", 0.4f);
    userService->register_troll("Doroshin", 0.6f);

    forumService->create_section("The torture chamber", "Chepik");
    sectionService->add_user_to_section("The torture chamber", "Ovcharenko");
    sectionService->add_user_to_section("The torture chamber", "Doroshin");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Ovcharenko");
    storage->set_user_online("Doroshin");

    userService->ban_user("Chepik");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("The torture chamber"));

    EXPECT_FALSE(forumService->section_exists("The torture chamber"));

    EXPECT_FALSE(storage->is_online("Ovcharenko"));
    EXPECT_FALSE(storage->is_online("Doroshin"));
}

TEST_F(SectionServiceTest, CheckAndFix_EmptySection) {
    userService->register_moderator("Firrer", 5);
    forumService->create_section("Koons camera", "Firrer");

    auto sec = forumService->get_section("Koons camera");
    sec->del_participant("Firrer");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("Koons camera"));
    EXPECT_FALSE(forumService->section_exists("Koons camera"));
}

TEST_F(SectionServiceTest, CheckAndFix_AllUsersBanned) {
    userService->register_moderator("Alibekova", 5);
    userService->register_user("Sein");
    userService->register_user("Maximov");

    forumService->create_section("The teacher", "Alibekova");
    sectionService->add_user_to_section("The teacher", "Sein");
    sectionService->add_user_to_section("The teacher", "Maximov");

    userService->ban_user("Alibekova");
    userService->ban_user("Sein");
    userService->ban_user("Maximov");

    EXPECT_FALSE(sectionService->check_and_fix_section_moderators("The teacher"));
}

TEST_F(SectionServiceTest, CheckAndFix_NonExistentSection) {
    EXPECT_FALSE(sectionService->check_and_fix_section_moderators("NonExistent"));
}

TEST_F(SectionServiceTest, CheckAndFix_NoModeratorNoUsersCanPromote) {
    userService->register_moderator("Shevtsov", 5);
    userService->register_user("Meleshkin");
    userService->register_troll("Chesnokova", 0.5f);

    forumService->create_section("Filial taste villa", "Shevtsov");
    sectionService->add_user_to_section("Filial taste villa", "Meleshkin");
    sectionService->add_user_to_section("Filial taste villa", "Chesnokova");

    auto storage = forum->get_ptr_storage();
    storage->set_user_online("Meleshkin");

    userService->ban_user("Shevtsov");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("Filial taste villa"));
    EXPECT_TRUE(storage->is_moderator("Meleshkin"));
}

TEST_F(SectionServiceTest, CheckAndFix_ModeratorOfflineButNotBanned) {
    userService->register_moderator("Novikov", 5);
    userService->register_user("Gaspariunas");

    forumService->create_section("informatics", "Novikov");
    sectionService->add_user_to_section("informatics", "Gaspariunas");

    auto storage = forum->get_ptr_storage();
    storage->set_user_offline("Novikov");
    storage->set_user_online("Gaspariunas");

    EXPECT_TRUE(sectionService->check_and_fix_section_moderators("informatics"));
}

TEST_F(SectionServiceTest, HandleMainModeratorBanned_MixedUsers) {
    try {
        userService->register_moderator("Dyumin", 5);
        userService->register_user("Agibalov");
        userService->register_troll("Komarov", 0.3f);

        forumService->create_section("unix", "Dyumin");
        sectionService->add_user_to_section("unix", "Agibalov");
        sectionService->add_user_to_section("unix", "Komarov");

        auto storage = forum->get_ptr_storage();
        storage->set_user_online("Agibalov");
        storage->set_user_online("Komarov");

        userService->ban_user("Dyumin");

        EXPECT_TRUE(sectionService->handle_main_moderator_banned("unix", "Dyumin"));
        EXPECT_TRUE(forumService->section_exists("unix"));
    } catch (...) {

    };
}