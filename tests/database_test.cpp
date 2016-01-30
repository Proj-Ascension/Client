#include <gtest/gtest.h>
#include "../src/database.h"

class DatabaseTest : public ::testing::Test {
public:
    Game testGame;
    QList<Game> gameList;
    QFile* dbFile;

protected:
    virtual void SetUp() {
        dbFile = new QFile("ascensionTest.db");
        testGame = {1, QString("Test Game"), QString("."), QString("test.exe"), QString("args")};
        gameList << testGame;
        Database::getInstance("ascensionTest.db").init();
    }

    virtual void TearDown() {
        Database::getInstance("ascensionTest.db").reset();
    }
};

bool populate() {
    return Database::getInstance("ascensionTest.db").addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
}


TEST_F(DatabaseTest, DatabaseTestAddGame) {
    EXPECT_TRUE(populate());
    EXPECT_FALSE(populate());
}

TEST_F(DatabaseTest, DatabaseTestGetGameCount) {
    EXPECT_TRUE(populate());
    EXPECT_EQ(Database::getInstance("ascensionTest.db").getGameCount(), 1);
    Database::getInstance("ascensionTest.db").addGame(QString("Half-Life 3"), QString("."), QString("hl3.exe"), QString(""));
    EXPECT_EQ(Database::getInstance("ascensionTest.db").getGameCount(), 2);
}

TEST_F(DatabaseTest, DatabaseTestGetGameById) {
    EXPECT_TRUE(populate());
    EXPECT_EQ(Database::getInstance("ascensionTest.db").getGameById(1), testGame);
    EXPECT_EQ(Database::getInstance("ascensionTest.db").getGameById(2), Game{});
}

TEST_F(DatabaseTest, DatabaseTestGetGames) {
    EXPECT_TRUE(populate());
    EXPECT_EQ(Database::getInstance("ascensionTest.db").getGames(), gameList);
}

TEST_F(DatabaseTest, DatabaseTestRemoveGameById) {
    EXPECT_FALSE(Database::getInstance("ascensionTest.db").removeGameById(1));
    EXPECT_TRUE(populate());
    EXPECT_TRUE(Database::getInstance("ascensionTest.db").removeGameById(1));
}

TEST_F(DatabaseTest, DatabaseTestRemoveGameByName) {
    EXPECT_FALSE(Database::getInstance("ascensionTest.db").removeGameByName("Test Game"));
    EXPECT_TRUE(populate());
    EXPECT_TRUE(Database::getInstance("ascensionTest.db").removeGameByName("Test Game"));
}