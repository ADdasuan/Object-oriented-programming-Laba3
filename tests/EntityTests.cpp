#include <gtest/gtest.h>
#include "DefaultUser.h"
#include "User.h"
#include "Moderator.h"
#include "Troll.h"
#include <chrono>
#include <stdexcept>

TEST(DefaultUserTest, SimpleConstructor) {
    DefaultUser user("Komarov");

    EXPECT_EQ(user.get_login(), "Komarov");
    EXPECT_EQ(user.get_count_of_message(), 0);
    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);

    auto now = std::chrono::system_clock::now();
    auto user_date = user.get_date();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - user_date);
    EXPECT_LE(std::abs(diff.count()), 1);
}

TEST(DefaultUserTest, FullConstructor) {
    auto date = std::chrono::system_clock::now();
    DefaultUser user("Muxa", date, 42, UserStatus::ONLINE);

    EXPECT_EQ(user.get_login(), "Muxa");
    EXPECT_EQ(user.get_date(), date);
    EXPECT_EQ(user.get_count_of_message(), 42);
    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);
}

TEST(DefaultUserTest, Getters) {
    auto date = std::chrono::system_clock::now();
    DefaultUser user("Skitev", date, 100, UserStatus::BANNED);

    EXPECT_EQ(user.get_login(), "Skitev");
    EXPECT_EQ(user.get_date(), date);
    EXPECT_EQ(user.get_count_of_message(), 100);
    EXPECT_EQ(user.get_status(), UserStatus::BANNED);
}

TEST(DefaultUserTest, SetLogin) {
    DefaultUser user("Dmitriev");

    user.set_login("Sinelnikov");
    EXPECT_EQ(user.get_login(), "Sinelnikov");

    user.set_login("Chepik");
    EXPECT_EQ(user.get_login(), "Chepik");
}

TEST(DefaultUserTest, SetDate) {
    DefaultUser user("Ovcharenko");

    auto old_date = std::chrono::system_clock::now() - std::chrono::hours(24);
    auto new_date = std::chrono::system_clock::now();

    user.set_date(old_date);
    EXPECT_EQ(user.get_date(), old_date);

    user.set_date(new_date);
    EXPECT_EQ(user.get_date(), new_date);
}

TEST(DefaultUserTest, SetCountOfMessage) {
    DefaultUser user("Doroshin");

    EXPECT_EQ(user.get_count_of_message(), 0);

    user.set_count_of_message(10);
    EXPECT_EQ(user.get_count_of_message(), 10);

    user.set_count_of_message(999);
    EXPECT_EQ(user.get_count_of_message(), 999);

    user.set_count_of_message(0);
    EXPECT_EQ(user.get_count_of_message(), 0);
}

TEST(DefaultUserTest, SetStatus) {
    DefaultUser user("Firrer");

    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);

    user.set_status(UserStatus::ONLINE);
    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);

    user.set_status(UserStatus::BANNED);
    EXPECT_EQ(user.get_status(), UserStatus::BANNED);

    user.set_status(UserStatus::OFFLINE);
    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);
}

TEST(DefaultUserTest, MultipleStateChanges) {
    DefaultUser user("Alibekova");
    auto new_date = std::chrono::system_clock::now();

    user.set_login("Sein");
    user.set_count_of_message(50);
    user.set_status(UserStatus::ONLINE);
    user.set_date(new_date);

    EXPECT_EQ(user.get_login(), "Sein");
    EXPECT_EQ(user.get_count_of_message(), 50);
    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);
    EXPECT_EQ(user.get_date(), new_date);
}

TEST(UserTest, SimpleConstructor) {
    User user("Maximov");

    EXPECT_EQ(user.get_login(), "Maximov");
    EXPECT_EQ(user.get_count_of_message(), 0);
    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);
}

TEST(UserTest, FullConstructor) {
    auto date = std::chrono::system_clock::now();
    User user("Shevtsov", date, 77, UserStatus::ONLINE);

    EXPECT_EQ(user.get_login(), "Shevtsov");
    EXPECT_EQ(user.get_date(), date);
    EXPECT_EQ(user.get_count_of_message(), 77);
    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);
}

TEST(UserTest, InheritanceFromDefaultUser) {
    User user("Meleshkin");
    auto new_date = std::chrono::system_clock::now();

    user.set_login("Chesnokova");
    user.set_count_of_message(25);
    user.set_status(UserStatus::BANNED);
    user.set_date(new_date);

    EXPECT_EQ(user.get_login(), "Chesnokova");
    EXPECT_EQ(user.get_count_of_message(), 25);
    EXPECT_EQ(user.get_status(), UserStatus::BANNED);
    EXPECT_EQ(user.get_date(), new_date);
}

TEST(UserTest, MultipleUsersCreation) {
    User user1("Novikov");
    User user2("Gaspariunas");
    User user3("Dyumin");

    EXPECT_EQ(user1.get_login(), "Novikov");
    EXPECT_EQ(user2.get_login(), "Gaspariunas");
    EXPECT_EQ(user3.get_login(), "Dyumin");

    user1.set_count_of_message(10);
    user2.set_count_of_message(20);

    EXPECT_EQ(user1.get_count_of_message(), 10);
    EXPECT_EQ(user2.get_count_of_message(), 20);
    EXPECT_EQ(user3.get_count_of_message(), 0);
}

TEST(UserTest, StatusTransitions) {
    User user("Agibalov");

    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);

    user.set_status(UserStatus::ONLINE);
    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);

    user.set_status(UserStatus::BANNED);
    EXPECT_EQ(user.get_status(), UserStatus::BANNED);

    user.set_status(UserStatus::OFFLINE);
    EXPECT_EQ(user.get_status(), UserStatus::OFFLINE);
}

TEST(ModeratorTest, SimpleConstructorDefaultIntelligence) {
    Moderator mod("Komarov");

    EXPECT_EQ(mod.get_login(), "Komarov");
    EXPECT_EQ(mod.get_intelligence(), 1);
    EXPECT_EQ(mod.get_count_of_message(), 0);
    EXPECT_EQ(mod.get_status(), UserStatus::OFFLINE);
}

TEST(ModeratorTest, SimpleConstructorCustomIntelligence) {
    Moderator mod("Muxa", 7);

    EXPECT_EQ(mod.get_login(), "Muxa");
    EXPECT_EQ(mod.get_intelligence(), 7);
}

TEST(ModeratorTest, FullConstructor) {
    auto date = std::chrono::system_clock::now();
    Moderator mod("Skitev", date, 50, 8, UserStatus::ONLINE);

    EXPECT_EQ(mod.get_login(), "Skitev");
    EXPECT_EQ(mod.get_date(), date);
    EXPECT_EQ(mod.get_count_of_message(), 50);
    EXPECT_EQ(mod.get_intelligence(), 8);
    EXPECT_EQ(mod.get_status(), UserStatus::ONLINE);
}

TEST(ModeratorTest, SetIntelligenceValid) {
    Moderator mod("Dmitriev");

    for (unsigned i = 1; i <= 10; ++i) {
        EXPECT_NO_THROW(mod.set_intelligence(i));
        EXPECT_EQ(mod.get_intelligence(), i);
    }
}

TEST(ModeratorTest, SetIntelligenceTooHigh) {
    Moderator mod("Sinelnikov", 5);

    EXPECT_THROW(mod.set_intelligence(11), std::invalid_argument);
    EXPECT_THROW(mod.set_intelligence(100), std::invalid_argument);
    EXPECT_NO_THROW(mod.set_intelligence(9));

    EXPECT_EQ(mod.get_intelligence(), 9);
}

TEST(ModeratorTest, IntelligenceBoundaryValues) {
    Moderator mod("Chepik");

    EXPECT_NO_THROW(mod.set_intelligence(1));
    EXPECT_EQ(mod.get_intelligence(), 1);

    EXPECT_NO_THROW(mod.set_intelligence(10));
    EXPECT_EQ(mod.get_intelligence(), 10);

    EXPECT_THROW(mod.set_intelligence(0),std::invalid_argument);
    EXPECT_NO_THROW(mod.set_intelligence(8));
    EXPECT_THROW(mod.set_intelligence(11), std::invalid_argument);
}

TEST(ModeratorTest, ConstructorWithInvalidIntelligence) {
    auto date = std::chrono::system_clock::now();

    EXPECT_THROW(Moderator("Ovcharenko", date, 0, 0, UserStatus::OFFLINE),std::invalid_argument);
    EXPECT_THROW(Moderator("Doroshin", date, 0, 15, UserStatus::OFFLINE),std::invalid_argument);
}

TEST(ModeratorTest, InheritanceFromDefaultUser) {
    Moderator mod("Firrer", 6);
    auto new_date = std::chrono::system_clock::now();

    mod.set_login("Alibekova");
    mod.set_count_of_message(100);
    mod.set_status(UserStatus::ONLINE);
    mod.set_date(new_date);

    EXPECT_EQ(mod.get_login(), "Alibekova");
    EXPECT_EQ(mod.get_count_of_message(), 100);
    EXPECT_EQ(mod.get_status(), UserStatus::ONLINE);
    EXPECT_EQ(mod.get_date(), new_date);

    EXPECT_EQ(mod.get_intelligence(), 6);
}

TEST(ModeratorTest, MultipleModeratorsWithDifferentIntelligence) {
    Moderator mod1("Sein", 3);
    Moderator mod2("Maximov", 7);
    Moderator mod3("Shevtsov", 10);

    EXPECT_EQ(mod1.get_intelligence(), 3);
    EXPECT_EQ(mod2.get_intelligence(), 7);
    EXPECT_EQ(mod3.get_intelligence(), 10);

    mod1.set_intelligence(5);
    EXPECT_EQ(mod1.get_intelligence(), 5);
    EXPECT_EQ(mod2.get_intelligence(), 7);
}

TEST(TrollTest, SimpleConstructor) {
    Troll troll("Meleshkin", 0.5f);

    EXPECT_EQ(troll.get_login(), "Meleshkin");
    EXPECT_FLOAT_EQ(troll.get_width(), 0.5f);
    EXPECT_EQ(troll.get_success(), 0);
    EXPECT_EQ(troll.get_count_of_message(), 0);
    EXPECT_EQ(troll.get_status(), UserStatus::OFFLINE);
}

TEST(TrollTest, FullConstructor) {
    auto date = std::chrono::system_clock::now();
    Troll troll("Chesnokova", date, 0.3f, UserStatus::ONLINE);

    EXPECT_EQ(troll.get_login(), "Chesnokova");
    EXPECT_EQ(troll.get_date(), date);
    EXPECT_FLOAT_EQ(troll.get_width(), 0.3f);
    EXPECT_EQ(troll.get_success(), 0);
    EXPECT_EQ(troll.get_status(), UserStatus::ONLINE);
}

TEST(TrollTest, SetWidthValid) {
    Troll troll("Novikov", 0.5f);

    EXPECT_NO_THROW(troll.set_width(0.1f));
    EXPECT_FLOAT_EQ(troll.get_width(), 0.1f);

    EXPECT_NO_THROW(troll.set_width(0.5f));
    EXPECT_FLOAT_EQ(troll.get_width(), 0.5f);

    EXPECT_NO_THROW(troll.set_width(1.0f));
    EXPECT_FLOAT_EQ(troll.get_width(), 1.0f);

    EXPECT_NO_THROW(troll.set_width(0.75f));
    EXPECT_FLOAT_EQ(troll.get_width(), 0.75f);
}

TEST(TrollTest, SetWidthTooLow) {
    Troll troll("Gaspariunas", 0.5f);

    EXPECT_THROW(troll.set_width(0.09f), std::invalid_argument);
    EXPECT_THROW(troll.set_width(0.0f), std::invalid_argument);
    EXPECT_THROW(troll.set_width(-0.5f), std::invalid_argument);

    EXPECT_FLOAT_EQ(troll.get_width(), 0.5f);
}

TEST(TrollTest, SetWidthTooHigh) {
    Troll troll("Dyumin", 0.5f);

    EXPECT_THROW(troll.set_width(1.01f), std::invalid_argument);
    EXPECT_THROW(troll.set_width(2.0f), std::invalid_argument);
    EXPECT_THROW(troll.set_width(10.0f), std::invalid_argument);

    EXPECT_FLOAT_EQ(troll.get_width(), 0.5f);
}

TEST(TrollTest, WidthBoundaryValues) {
    Troll troll("Agibalov", 0.5f);

    EXPECT_NO_THROW(troll.set_width(0.1f));
    EXPECT_FLOAT_EQ(troll.get_width(), 0.1f);

    EXPECT_NO_THROW(troll.set_width(1.0f));
    EXPECT_FLOAT_EQ(troll.get_width(), 1.0f);

    EXPECT_THROW(troll.set_width(0.099f), std::invalid_argument);
    EXPECT_THROW(troll.set_width(1.001f), std::invalid_argument);
}

TEST(TrollTest, ConstructorWithInvalidWidth) {
    auto date = std::chrono::system_clock::now();

    EXPECT_THROW(Troll("Komarov", date, 0.05f, UserStatus::OFFLINE), std::invalid_argument);
    EXPECT_THROW(Troll("Muxa", date, 1.5f, UserStatus::OFFLINE), std::invalid_argument);
    EXPECT_THROW(Troll("Skitev", 0.0f), std::invalid_argument);
}

TEST(TrollTest, SuccessCountManagement) {
    Troll troll("Dmitriev", 0.5f);

    EXPECT_EQ(troll.get_success(), 0);

    troll.set_success(5);
    EXPECT_EQ(troll.get_success(), 5);

    troll.set_success(42);
    EXPECT_EQ(troll.get_success(), 42);

    troll.set_success(0);
    EXPECT_EQ(troll.get_success(), 0);

    troll.set_success(9999);
    EXPECT_EQ(troll.get_success(), 9999);
}

TEST(TrollTest, IncrementSuccess) {
    Troll troll("Sinelnikov", 0.5f);

    EXPECT_EQ(troll.get_success(), 0);

    troll.set_success(troll.get_success() + 1);
    EXPECT_EQ(troll.get_success(), 1);

    troll.set_success(troll.get_success() + 1);
    EXPECT_EQ(troll.get_success(), 2);

    troll.set_success(troll.get_success() + 1);
    EXPECT_EQ(troll.get_success(), 3);
}

TEST(TrollTest, InheritanceFromDefaultUser) {
    Troll troll("Chepik", 0.7f);
    auto new_date = std::chrono::system_clock::now();

    troll.set_login("Ovcharenko");
    troll.set_count_of_message(50);
    troll.set_status(UserStatus::BANNED);
    troll.set_date(new_date);

    EXPECT_EQ(troll.get_login(), "Ovcharenko");
    EXPECT_EQ(troll.get_count_of_message(), 50);
    EXPECT_EQ(troll.get_status(), UserStatus::BANNED);
    EXPECT_EQ(troll.get_date(), new_date);

    EXPECT_FLOAT_EQ(troll.get_width(), 0.7f);
    EXPECT_EQ(troll.get_success(), 0);
}

TEST(TrollTest, MultipleTrollsWithDifferentWidth) {
    Troll troll1("Doroshin", 0.1f);
    Troll troll2("Firrer", 0.5f);
    Troll troll3("Alibekova", 1.0f);

    EXPECT_FLOAT_EQ(troll1.get_width(), 0.1f);
    EXPECT_FLOAT_EQ(troll2.get_width(), 0.5f);
    EXPECT_FLOAT_EQ(troll3.get_width(), 1.0f);

    troll1.set_success(10);
    troll2.set_success(20);

    EXPECT_EQ(troll1.get_success(), 10);
    EXPECT_EQ(troll2.get_success(), 20);
    EXPECT_EQ(troll3.get_success(), 0);
}

TEST(TrollTest, InvalidWidthErrorMessage) {
    Troll troll("Sein", 0.5f);

    try {
        troll.set_width(0.05f);
        FAIL() << "Expected std::invalid_argument";
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Width was been in [0.1 ; 1.0]");
    }

    try {
        troll.set_width(1.5f);
        FAIL() << "Expected std::invalid_argument";
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Width was been in [0.1 ; 1.0]");
    }
}

TEST(TrollTest, TrollLifecycle) {
    Troll troll("Maximov", 0.3f);
    EXPECT_EQ(troll.get_status(), UserStatus::OFFLINE);
    EXPECT_EQ(troll.get_success(), 0);

    troll.set_status(UserStatus::ONLINE);
    EXPECT_EQ(troll.get_status(), UserStatus::ONLINE);

    troll.set_count_of_message(10);
    EXPECT_EQ(troll.get_count_of_message(), 10);

    troll.set_success(troll.get_success() + 1);
    EXPECT_EQ(troll.get_success(), 1);

    troll.set_success(troll.get_success() + 1);
    EXPECT_EQ(troll.get_success(), 2);

    troll.set_status(UserStatus::BANNED);
    EXPECT_EQ(troll.get_status(), UserStatus::BANNED);

    EXPECT_FLOAT_EQ(troll.get_width(), 0.3f);
    EXPECT_EQ(troll.get_success(), 2);
    EXPECT_EQ(troll.get_count_of_message(), 10);
}

TEST(EntityIntegrationTest, DifferentUserTypesIndependence) {
    User user("Shevtsov");
    Moderator mod("Meleshkin", 5);
    Troll troll("Chesnokova", 0.5f);

    EXPECT_EQ(user.get_login(), "Shevtsov");
    EXPECT_EQ(mod.get_login(), "Meleshkin");
    EXPECT_EQ(troll.get_login(), "Chesnokova");

    user.set_count_of_message(100);
    EXPECT_EQ(user.get_count_of_message(), 100);
    EXPECT_EQ(mod.get_count_of_message(), 0);
    EXPECT_EQ(troll.get_count_of_message(), 0);
}

TEST(EntityIntegrationTest, PolymorphismThroughIUser) {
    User user("Novikov");
    Moderator mod("Gaspariunas", 7);
    Troll troll("Dyumin", 0.6f);

    IUser* iuser1 = &user;
    IUser* iuser2 = &mod;
    IUser* iuser3 = &troll;

    EXPECT_EQ(iuser1->get_login(), "Novikov");
    EXPECT_EQ(iuser2->get_login(), "Gaspariunas");
    EXPECT_EQ(iuser3->get_login(), "Dyumin");

    iuser1->set_status(UserStatus::ONLINE);
    iuser2->set_status(UserStatus::ONLINE);
    iuser3->set_status(UserStatus::ONLINE);

    EXPECT_EQ(user.get_status(), UserStatus::ONLINE);
    EXPECT_EQ(mod.get_status(), UserStatus::ONLINE);
    EXPECT_EQ(troll.get_status(), UserStatus::ONLINE);
}

TEST(EntityIntegrationTest, ForumWithMultipleUserTypes) {
    std::vector<std::unique_ptr<DefaultUser>> users;

    users.push_back(std::make_unique<User>("Komarov"));
    users.push_back(std::make_unique<User>("Muxa"));
    users.push_back(std::make_unique<Moderator>("Skitev", 8));
    users.push_back(std::make_unique<Moderator>("Dmitriev", 6));
    users.push_back(std::make_unique<Troll>("Sinelnikov", 0.3f));
    users.push_back(std::make_unique<Troll>("Chepik", 0.7f));

    EXPECT_EQ(users.size(), 6);

    EXPECT_EQ(users[0]->get_login(), "Komarov");
    EXPECT_EQ(users[2]->get_login(), "Skitev");
    EXPECT_EQ(users[4]->get_login(), "Sinelnikov");

    for (const auto& u : users) {
        EXPECT_EQ(u->get_status(), UserStatus::OFFLINE);
    }
}