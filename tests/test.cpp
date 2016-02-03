#include "test.h"
#include "../src/database.h"
#include "../src/libs/steam_vdf_parse.hpp"


TEST_CASE ("Database", "[db]")
{
    QTemporaryDir dir;
    QString path = dir.isValid() ? dir.path() + "ascensionTest.db" : QDir::tempPath() + "ascensionTest.db";

    Game testGame = {1, QString("Test Game"), QString("."), QString("test.exe"), QString("args")};
    Game hl3 = {2, QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")};
    QList<Game> list{testGame, hl3};


    REQUIRE (Database::getInstance(path).init());

    SECTION("Add a test game")
    {
        REQUIRE (Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args")));
        REQUIRE (!Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args")));
    }
    SECTION("Add another test game")
    {
        REQUIRE (Database::getInstance(path).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")));
        REQUIRE (!Database::getInstance(path).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")));
    }
    SECTION("Get game count")
    {
        REQUIRE (Database::getInstance(path).getGameCount() == 0);
        Database::getInstance(path).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString(""));
        REQUIRE (Database::getInstance(path).getGameCount() == 1);
    }
    SECTION("Get game by id")
    {
        REQUIRE (Database::getInstance(path).getGameById(1) == Game{});
        Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
        REQUIRE (Database::getInstance(path).getGameById(1) == testGame);
    }
    SECTION("Get games list")
    {
        REQUIRE (Database::getInstance(path).getGames() == QList<Game>{});

        Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
        Database::getInstance(path).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString(""));
        REQUIRE (Database::getInstance(path).getGames() == list);
    }
    SECTION("Remove game by id")
    {
        REQUIRE (!Database::getInstance(path).removeGameById(1));
        Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
        REQUIRE (Database::getInstance(path).removeGameById(1));
    }
    SECTION("Remove game by name")
    {
        REQUIRE (!Database::getInstance(path).removeGameByName("Test Game"));
        Database::getInstance(path).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
        REQUIRE (Database::getInstance(path).removeGameByName("Test Game"));
    }
    SECTION("Reset db")
    {
        REQUIRE (Database::getInstance(path).reset());
    }
    Database::getInstance(path).reset();
}

TEST_CASE ("VDF Parser", "[vdf]")
{
    // VDF Parse tests
    std::unordered_map<int, SteamVdfParse::GameHeader> map = SteamVdfParse::parseVdf("appinfo.vdf");
    SteamVdfParse::GameHeader gameTest = map.at(208050);

    REQUIRE (gameTest.appID == 208050);
    REQUIRE (gameTest.size == 1239);
    REQUIRE (gameTest.infoState == 2);
    REQUIRE (gameTest.lastUpdated == 1439232482);
    REQUIRE (gameTest.accessToken == 0);
    REQUIRE (gameTest.changeNumber == 1127960);

    // Verify the SHA sum
    int codes[20] = {42,74,-65,28,19,82,-64,-39,80,82,-8,-54,-48,-21,83,1,97,-126,-125,-60};

    bool shaFlag = true;
    for (auto i=0;i<20;i++)
    {
        if (!int(gameTest.sha[i]) == codes[i])
        {
            shaFlag = false;
            break;
        }
    }
    REQUIRE (shaFlag);
}
