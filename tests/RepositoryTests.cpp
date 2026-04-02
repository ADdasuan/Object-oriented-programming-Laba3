#include <gtest/gtest.h>
#include "Section.h"
#include "StorageUsers.h"
#include "Forum.h"
#include "User.h"
#include "Moderator.h"
#include "Troll.h"
#include <memory>
#include <stdexcept>

TEST(SectionTest, Constructor) {
    Section section("informatics", "Komarov");

    EXPECT_EQ(section.get_name(), "informatics");
    EXPECT_EQ(section.get_login_initial_moderator(), "Komarov");

    EXPECT_TRUE(section.is_participant_exist("Komarov"));
    EXPECT_FALSE(section.section_is_empty());
}

TEST(SectionTest, ConstructorEmptyName) {
    EXPECT_THROW(Section("", "Muxa"), std::invalid_argument);
}

TEST(SectionTest, AddParticipant) {
    Section section("unix", "Skitev");

    EXPECT_NO_THROW(section.add_participant("Dmitriev"));
    EXPECT_TRUE(section.is_participant_exist("Dmitriev"));

    EXPECT_NO_THROW(section.add_participant("Sinelnikov"));
    EXPECT_TRUE(section.is_participant_exist("Sinelnikov"));

    EXPECT_EQ(section.get_participant().size(), 3);
}

TEST(SectionTest, AddParticipantEmptyLogin) {
    Section section("IBM", "Chepik");

    EXPECT_THROW(section.add_participant(""), std::invalid_argument);
}

TEST(SectionTest, AddParticipantDuplicate) {
    Section section("Data Sience", "Ovcharenko");

    section.add_participant("Doroshin");
    EXPECT_EQ(section.get_participant().size(), 2);

    section.add_participant("Doroshin");
    EXPECT_EQ(section.get_participant().size(), 2);
}

TEST(SectionTest, DeleteParticipant) {
    Section section("Theory of automata", "Firrer");
    section.add_participant("Alibekova");
    section.add_participant("Sein");

    EXPECT_EQ(section.get_participant().size(), 3);

    EXPECT_NO_THROW(section.del_participant("Alibekova"));
    EXPECT_FALSE(section.is_participant_exist("Alibekova"));
    EXPECT_EQ(section.get_participant().size(), 2);

    EXPECT_NO_THROW(section.del_participant("Sein"));
    EXPECT_FALSE(section.is_participant_exist("Sein"));
    EXPECT_EQ(section.get_participant().size(), 1);
}

TEST(SectionTest, DeleteNonExistentParticipant) {
    Section section("Circuit design", "Maximov");

    EXPECT_THROW(section.del_participant("Novikov"), std::runtime_error);
}

TEST(SectionTest, DeleteFromEmptySection) {
    Section section("Electronics", "Shevtsov");
    section.del_participant("Shevtsov");

    EXPECT_TRUE(section.section_is_empty());
    EXPECT_THROW(section.del_participant("Meleshkin"), std::runtime_error);
}

TEST(SectionTest, IsParticipantExist) {
    Section section("AiSD", "Chesnokova");
    section.add_participant("Gaspariunas");

    EXPECT_TRUE(section.is_participant_exist("Chesnokova"));
    EXPECT_TRUE(section.is_participant_exist("Gaspariunas"));
    EXPECT_FALSE(section.is_participant_exist("Dyumin"));
    EXPECT_FALSE(section.is_participant_exist("Agibalov"));
}

TEST(SectionTest, IsParticipantExistEmptyLogin) {
    Section section("The assembler", "Komarov");

    EXPECT_THROW(section.is_participant_exist(""), std::invalid_argument);
}

TEST(SectionTest, SectionIsEmpty) {
    Section section("Engineering graphics", "Muxa");

    EXPECT_FALSE(section.section_is_empty());

    section.del_participant("Muxa");
    EXPECT_TRUE(section.section_is_empty());

    section.add_participant("Skitev");
    EXPECT_FALSE(section.section_is_empty());
}

TEST(SectionTest, UpdateModeratorInfo) {
    Section section("The torture chamber", "Dmitriev");
    section.add_participant("Sinelnikov");

    EXPECT_EQ(section.get_login_initial_moderator(), "Dmitriev");

    section.update_moderator_info("Sinelnikov");
    EXPECT_EQ(section.get_login_initial_moderator(), "Sinelnikov");
}

TEST(SectionTest, UpdateModeratorInfoNonParticipant) {
    Section section("Koons camera", "Chepik");

    std::string old = section.get_login_initial_moderator();
    section.update_moderator_info("Ovcharenko");

    EXPECT_EQ(section.get_login_initial_moderator(), old);
}

TEST(SectionTest, Getters) {
    Section section("The teacher", "Doroshin");
    section.add_participant("Firrer");
    section.add_participant("Alibekova");

    EXPECT_EQ(section.get_name(), "The teacher");
    EXPECT_EQ(section.get_login_initial_moderator(), "Doroshin");

    auto participants = section.get_participant();
    EXPECT_EQ(participants.size(), 3);
    EXPECT_TRUE(std::find(participants.begin(), participants.end(), "Doroshin") != participants.end());
    EXPECT_TRUE(std::find(participants.begin(), participants.end(), "Firrer") != participants.end());
    EXPECT_TRUE(std::find(participants.begin(), participants.end(), "Alibekova") != participants.end());
}

TEST(StorageUsersTest, AddUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Sein");

    EXPECT_NO_THROW(storage.add_user(user));
    EXPECT_TRUE(storage.find_user("Sein"));
    EXPECT_EQ(storage.size(), 1);
}

TEST(StorageUsersTest, AddDuplicateUser) {
    StorageUsers storage;
    auto user1 = std::make_shared<User>("Maximov");
    auto user2 = std::make_shared<User>("Maximov");

    storage.add_user(user1);
    EXPECT_THROW(storage.add_user(user2), std::invalid_argument);
    EXPECT_EQ(storage.size(), 1);
}

TEST(StorageUsersTest, AddDifferentUserTypes) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Shevtsov");
    auto mod = std::make_shared<Moderator>("Meleshkin", 5);
    auto troll = std::make_shared<Troll>("Chesnokova", 0.5f);

    EXPECT_NO_THROW(storage.add_user(user));
    EXPECT_NO_THROW(storage.add_user(mod));
    EXPECT_NO_THROW(storage.add_user(troll));

    EXPECT_EQ(storage.size(), 3);
    EXPECT_TRUE(storage.is_regular_user("Shevtsov"));
    EXPECT_TRUE(storage.is_moderator("Meleshkin"));
    EXPECT_TRUE(storage.is_troll("Chesnokova"));
}

TEST(StorageUsersTest, GetUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Novikov");
    storage.add_user(user);

    auto retrieved = storage.get_user("Novikov");
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->get_login(), "Novikov");

    auto nonexistent = storage.get_user("Gaspariunas");
    EXPECT_EQ(nonexistent, nullptr);
}

TEST(StorageUsersTest, DeleteUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Dyumin");
    storage.add_user(user);

    EXPECT_TRUE(storage.find_user("Dyumin"));
    EXPECT_NO_THROW(storage.del_user("Dyumin"));
    EXPECT_FALSE(storage.find_user("Dyumin"));
    EXPECT_EQ(storage.size(), 0);
}

TEST(StorageUsersTest, DeleteNonExistentUser) {
    StorageUsers storage;

    EXPECT_THROW(storage.del_user("Agibalov"), std::invalid_argument);
}

TEST(StorageUsersTest, FindUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Komarov");
    storage.add_user(user);

    EXPECT_TRUE(storage.find_user("Komarov"));
    EXPECT_FALSE(storage.find_user("Muxa"));
}

TEST(StorageUsersTest, GetAllUsers) {
    StorageUsers storage;
    storage.add_user(std::make_shared<User>("Skitev"));
    storage.add_user(std::make_shared<Moderator>("Dmitriev", 5));
    storage.add_user(std::make_shared<Troll>("Sinelnikov", 0.5f));

    auto all = storage.get_all_users();
    EXPECT_EQ(all.size(), 3);
}

TEST(StorageUsersTest, UpdateUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Chepik");
    storage.add_user(user);

    auto updated = std::make_shared<User>("Chepik");
    updated->set_count_of_message(100);

    EXPECT_TRUE(storage.update_user("Chepik", updated));

    auto retrieved = storage.get_user("Chepik");
    EXPECT_EQ(retrieved->get_count_of_message(), 100);
}

TEST(StorageUsersTest, UpdateUserWrongLogin) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Ovcharenko");
    storage.add_user(user);

    auto updated = std::make_shared<User>("Doroshin");
    EXPECT_FALSE(storage.update_user("Ovcharenko", updated));
}

TEST(StorageUsersTest, UserTypeChecks) {
    StorageUsers storage;
    storage.add_user(std::make_shared<User>("Firrer"));
    storage.add_user(std::make_shared<Moderator>("Alibekova", 5));
    storage.add_user(std::make_shared<Troll>("Sein", 0.5f));

    EXPECT_TRUE(storage.is_regular_user("Firrer"));
    EXPECT_FALSE(storage.is_moderator("Firrer"));
    EXPECT_FALSE(storage.is_troll("Firrer"));

    EXPECT_FALSE(storage.is_regular_user("Alibekova"));
    EXPECT_TRUE(storage.is_moderator("Alibekova"));
    EXPECT_FALSE(storage.is_troll("Alibekova"));

    EXPECT_FALSE(storage.is_regular_user("Sein"));
    EXPECT_FALSE(storage.is_moderator("Sein"));
    EXPECT_TRUE(storage.is_troll("Sein"));
}

TEST(StorageUsersTest, StatusChecks) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Maximov");
    user->set_status(UserStatus::OFFLINE);
    storage.add_user(user);

    EXPECT_FALSE(storage.is_online("Maximov"));
    EXPECT_FALSE(storage.is_banned("Maximov"));

    storage.set_user_online("Maximov");
    EXPECT_TRUE(storage.is_online("Maximov"));
    EXPECT_FALSE(storage.is_banned("Maximov"));

    storage.ban_user("Maximov");
    EXPECT_FALSE(storage.is_online("Maximov"));
    EXPECT_TRUE(storage.is_banned("Maximov"));
}

TEST(StorageUsersTest, BanUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Shevtsov");
    user->set_status(UserStatus::ONLINE);
    storage.add_user(user);

    EXPECT_TRUE(storage.ban_user("Shevtsov"));

    auto retrieved = storage.get_user("Shevtsov");
    EXPECT_EQ(retrieved->get_status(), UserStatus::BANNED);

    EXPECT_FALSE(storage.ban_user("Shevtsov"));
}

TEST(StorageUsersTest, UnbanUser) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Meleshkin");
    user->set_status(UserStatus::BANNED);
    storage.add_user(user);

    EXPECT_TRUE(storage.unban_user("Meleshkin"));

    auto retrieved = storage.get_user("Meleshkin");
    EXPECT_EQ(retrieved->get_status(), UserStatus::OFFLINE);

    EXPECT_FALSE(storage.unban_user("Meleshkin"));
}

TEST(StorageUsersTest, SetOnlineOffline) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Chesnokova");
    storage.add_user(user);

    EXPECT_TRUE(storage.set_user_online("Chesnokova"));
    EXPECT_EQ(storage.get_user("Chesnokova")->get_status(), UserStatus::ONLINE);

    EXPECT_TRUE(storage.set_user_offline("Chesnokova"));
    EXPECT_EQ(storage.get_user("Chesnokova")->get_status(), UserStatus::OFFLINE);
}

TEST(StorageUsersTest, PromoteToModerator) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Novikov");
    user->set_count_of_message(50);
    storage.add_user(user);

    EXPECT_TRUE(storage.is_regular_user("Novikov"));
    EXPECT_TRUE(storage.promote_to_moderator("Novikov", 7));

    EXPECT_TRUE(storage.is_moderator("Novikov"));
    EXPECT_FALSE(storage.is_regular_user("Novikov"));

    EXPECT_EQ(storage.get_user("Novikov")->get_count_of_message(), 50);
    EXPECT_EQ(storage.get_moderator_intelligence("Novikov"), 7);
}

TEST(StorageUsersTest, TrollCannotBePromotedToModerator) {
    StorageUsers storage;
    auto troll = std::make_shared<Troll>("Gaspariunas", 0.3f);
    storage.add_user(troll);

    EXPECT_FALSE(storage.promote_to_moderator("Gaspariunas", 5));
    EXPECT_TRUE(storage.is_troll("Gaspariunas"));
}

TEST(StorageUsersTest, ConvertToTroll) {
    StorageUsers storage;
    auto user = std::make_shared<User>("Dyumin");
    user->set_count_of_message(30);
    storage.add_user(user);

    EXPECT_TRUE(storage.convert_to_troll("Dyumin", 0.6f));

    EXPECT_TRUE(storage.is_troll("Dyumin"));
    EXPECT_FALSE(storage.is_regular_user("Dyumin"));
    EXPECT_FLOAT_EQ(storage.get_troll_width("Dyumin"), 0.6f);
}

TEST(StorageUsersTest, ModeratorCannotBecomeTroll) {
    StorageUsers storage;
    auto mod = std::make_shared<Moderator>("Agibalov", 8);
    storage.add_user(mod);

    EXPECT_FALSE(storage.convert_to_troll("Agibalov", 0.5f));
    EXPECT_TRUE(storage.is_moderator("Agibalov"));
}

TEST(StorageUsersTest, ConvertModeratorToRegularUser) {
    StorageUsers storage;
    auto mod = std::make_shared<Moderator>("Komarov", 6);
    mod->set_count_of_message(100);
    storage.add_user(mod);

    EXPECT_TRUE(storage.convert_to_regular_user("Komarov"));

    EXPECT_TRUE(storage.is_regular_user("Komarov"));
    EXPECT_FALSE(storage.is_moderator("Komarov"));
    EXPECT_EQ(storage.get_user("Komarov")->get_count_of_message(), 100);
}

TEST(StorageUsersTest, ConvertTrollToRegularUser) {
    StorageUsers storage;
    auto troll = std::make_shared<Troll>("Muxa", 0.4f);
    troll->set_count_of_message(50);
    storage.add_user(troll);

    EXPECT_TRUE(storage.convert_to_regular_user("Muxa"));

    EXPECT_TRUE(storage.is_regular_user("Muxa"));
    EXPECT_FALSE(storage.is_troll("Muxa"));
    EXPECT_EQ(storage.get_user("Muxa")->get_count_of_message(), 50);
}

TEST(StorageUsersTest, ChangeModeratorIntelligence) {
    StorageUsers storage;
    auto mod = std::make_shared<Moderator>("Skitev", 5);
    storage.add_user(mod);

    EXPECT_TRUE(storage.change_moderator_intelligence("Skitev", 9));
    EXPECT_EQ(storage.get_moderator_intelligence("Skitev"), 9);

    auto user = std::make_shared<User>("Dmitriev");
    storage.add_user(user);
    EXPECT_FALSE(storage.change_moderator_intelligence("Dmitriev", 5));
}

TEST(StorageUsersTest, ChangeTrollWidth) {
    StorageUsers storage;
    auto troll = std::make_shared<Troll>("Sinelnikov", 0.3f);
    storage.add_user(troll);

    EXPECT_TRUE(storage.change_troll_width("Sinelnikov", 0.8f));
    EXPECT_FLOAT_EQ(storage.get_troll_width("Sinelnikov"), 0.8f);

    auto user = std::make_shared<User>("Chepik");
    storage.add_user(user);
    EXPECT_FALSE(storage.change_troll_width("Chepik", 0.5f));
}

TEST(StorageUsersTest, IncrementTrollSuccess) {
    StorageUsers storage;
    auto troll = std::make_shared<Troll>("Ovcharenko", 0.5f);
    storage.add_user(troll);

    EXPECT_EQ(storage.get_troll_success("Ovcharenko"), 0);

    EXPECT_TRUE(storage.increment_troll_success("Ovcharenko"));
    EXPECT_EQ(storage.get_troll_success("Ovcharenko"), 0);

    EXPECT_TRUE(storage.increment_troll_success("Ovcharenko"));
    EXPECT_EQ(storage.get_troll_success("Ovcharenko"), 0);
}

TEST(StorageUsersTest, Counters) {
    StorageUsers storage;

    storage.add_user(std::make_shared<User>("Doroshin"));
    storage.add_user(std::make_shared<User>("Firrer"));
    storage.add_user(std::make_shared<Moderator>("Alibekova", 5));
    storage.add_user(std::make_shared<Moderator>("Sein", 6));
    storage.add_user(std::make_shared<Moderator>("Maximov", 7));
    storage.add_user(std::make_shared<Troll>("Shevtsov", 0.3f));
    storage.add_user(std::make_shared<Troll>("Meleshkin", 0.7f));

    EXPECT_EQ(storage.get_total_count(), 7);
    EXPECT_EQ(storage.get_moderator_count(), 3);
    EXPECT_EQ(storage.get_troll_count(), 2);

    storage.set_user_online("Doroshin");
    storage.set_user_online("Alibekova");
    EXPECT_EQ(storage.get_online_count(), 2);

    storage.set_user_online("Shevtsov");
    EXPECT_EQ(storage.get_online_count(), 3);
}

TEST(StorageUsersTest, Clear) {
    StorageUsers storage;
    storage.add_user(std::make_shared<User>("Chesnokova"));
    storage.add_user(std::make_shared<User>("Novikov"));

    EXPECT_EQ(storage.size(), 2);

    storage.clear();
    EXPECT_EQ(storage.size(), 0);
    EXPECT_TRUE(storage.empty());
}

TEST(StorageUsersTest, Iterators) {
    StorageUsers storage;
    storage.add_user(std::make_shared<User>("Gaspariunas"));
    storage.add_user(std::make_shared<User>("Dyumin"));
    storage.add_user(std::make_shared<User>("Agibalov"));

    int count = 0;
    for (auto it = storage.begin(); it != storage.end(); ++it) {
        EXPECT_NE(it->second, nullptr);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(ForumTest, Constructor) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();

    forum.set_storage(storage);
    EXPECT_EQ(forum.get_ptr_storage(), storage);
}

TEST(ForumTest, AddSection) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Komarov", 7);
    storage->add_user(mod);

    EXPECT_TRUE(forum.add_section("informatics", "Komarov"));
    EXPECT_TRUE(forum.section_exist("informatics"));

    EXPECT_EQ(storage->get_user("Komarov")->get_status(), UserStatus::ONLINE);
}

TEST(ForumTest, AddSectionAutoPromote) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto user = std::make_shared<User>("Muxa");
    storage->add_user(user);

    EXPECT_TRUE(storage->is_regular_user("Muxa"));

    EXPECT_TRUE(forum.add_section("unix", "Muxa"));

    EXPECT_TRUE(storage->is_moderator("Muxa"));
    EXPECT_EQ(storage->get_user("Muxa")->get_status(), UserStatus::ONLINE);
}

TEST(ForumTest, AddSectionTrollCannotCreate) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto troll = std::make_shared<Troll>("Skitev", 0.3f);
    storage->add_user(troll);

    EXPECT_FALSE(forum.add_section("IBM", "Skitev"));
    EXPECT_FALSE(forum.section_exist("IBM"));
}

TEST(ForumTest, AddSectionNonExistentUser) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    EXPECT_FALSE(forum.add_section("Data Sience", "Dmitriev"));
}

TEST(ForumTest, AddSectionDuplicate) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Sinelnikov", 5);
    storage->add_user(mod);

    EXPECT_TRUE(forum.add_section("Theory of automata", "Sinelnikov"));

    auto mod2 = std::make_shared<Moderator>("Chepik", 6);
    storage->add_user(mod2);

    EXPECT_FALSE(forum.add_section("Theory of automata", "Chepik"));
}

TEST(ForumTest, DeleteSection) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Ovcharenko", 5);
    storage->add_user(mod);

    forum.add_section("Circuit design", "Ovcharenko");
    EXPECT_TRUE(forum.section_exist("Circuit design"));

    EXPECT_TRUE(forum.del_section("Circuit design"));
    EXPECT_FALSE(forum.section_exist("Circuit design"));

    EXPECT_EQ(storage->get_user("Ovcharenko")->get_status(), UserStatus::OFFLINE);
}

TEST(ForumTest, DeleteNonExistentSection) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    EXPECT_FALSE(forum.del_section("Electronics"));
}

TEST(ForumTest, DeleteSectionAllUsersOffline) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Doroshin", 5);
    auto user1 = std::make_shared<User>("Firrer");
    auto user2 = std::make_shared<User>("Alibekova");

    storage->add_user(mod);
    storage->add_user(user1);
    storage->add_user(user2);

    forum.add_section("AiSD", "Doroshin");
    auto section = forum.find_section("AiSD");
    section->add_participant("Firrer");
    section->add_participant("Alibekova");

    storage->set_user_online("Firrer");
    storage->set_user_online("Alibekova");

    EXPECT_EQ(storage->get_online_count(), 3);

    forum.del_section("AiSD");

    EXPECT_EQ(storage->get_user("Doroshin")->get_status(), UserStatus::OFFLINE);
    EXPECT_EQ(storage->get_user("Firrer")->get_status(), UserStatus::OFFLINE);
    EXPECT_EQ(storage->get_user("Alibekova")->get_status(), UserStatus::OFFLINE);
}

TEST(ForumTest, FindSection) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod = std::make_shared<Moderator>("Sein", 5);
    storage->add_user(mod);

    forum.add_section("The assembler", "Sein");

    auto section = forum.find_section("The assembler");
    EXPECT_NE(section, nullptr);
    EXPECT_EQ(section->get_name(), "The assembler");

    auto nosection = forum.find_section("Engineering graphics");
    EXPECT_EQ(nosection, nullptr);
}

TEST(ForumTest, UserExistsInStorage) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto user = std::make_shared<User>("Maximov");
    storage->add_user(user);

    EXPECT_TRUE(forum.user_exists_in_storage("Maximov"));
    EXPECT_FALSE(forum.user_exists_in_storage("Shevtsov"));
}

TEST(ForumTest, GetAllSections) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto mod1 = std::make_shared<Moderator>("Meleshkin", 5);
    auto mod2 = std::make_shared<Moderator>("Chesnokova", 6);
    auto mod3 = std::make_shared<Moderator>("Novikov", 7);
    storage->add_user(mod1);
    storage->add_user(mod2);
    storage->add_user(mod3);

    forum.add_section("The torture chamber", "Meleshkin");
    forum.add_section("Koons camera", "Chesnokova");
    forum.add_section("The teacher", "Novikov");

    auto sections = forum.get_all_sections();
    EXPECT_EQ(sections.size(), 3);
    EXPECT_TRUE(sections.find("The torture chamber") != sections.end());
    EXPECT_TRUE(sections.find("Koons camera") != sections.end());
    EXPECT_TRUE(sections.find("The teacher") != sections.end());
}

TEST(RepositoryIntegrationTest, FullForumScenario) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    storage->add_user(std::make_shared<Moderator>("Gaspariunas", 10));
    storage->add_user(std::make_shared<User>("Dyumin"));
    storage->add_user(std::make_shared<User>("Agibalov"));
    storage->add_user(std::make_shared<Troll>("Komarov", 0.3f));

    EXPECT_EQ(storage->size(), 4);

    EXPECT_TRUE(forum.add_section("Filial taste villa", "Gaspariunas"));

    auto section = forum.find_section("Filial taste villa");
    ASSERT_NE(section, nullptr);

    section->add_participant("Dyumin");
    section->add_participant("Agibalov");
    section->add_participant("Komarov");

    storage->set_user_online("Dyumin");
    storage->set_user_online("Agibalov");
    storage->set_user_online("Komarov");

    EXPECT_EQ(section->get_participant().size(), 4);
    EXPECT_EQ(storage->get_online_count(), 4);

    storage->ban_user("Komarov");
    section->del_participant("Komarov");

    EXPECT_EQ(section->get_participant().size(), 3);
    EXPECT_TRUE(storage->is_banned("Komarov"));

    forum.del_section("Filial taste villa");
    EXPECT_FALSE(forum.section_exist("Filial taste villa"));
    EXPECT_EQ(storage->get_online_count(), 0);
}

TEST(RepositoryIntegrationTest, PromoteAndCreateSection) {
    Forum forum;
    auto storage = std::make_shared<StorageUsers>();
    forum.set_storage(storage);

    auto user = std::make_shared<User>("Muxa");
    storage->add_user(user);

    EXPECT_TRUE(storage->is_regular_user("Muxa"));

    storage->promote_to_moderator("Muxa", 6);
    EXPECT_TRUE(storage->is_moderator("Muxa"));

    EXPECT_TRUE(forum.add_section("informatics", "Muxa"));
    EXPECT_TRUE(forum.section_exist("informatics"));
}