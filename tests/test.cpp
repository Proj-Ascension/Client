#include "test.h"
#include "../src/database.h"
#include "../src/libs/steam_vdf_parse.hpp"
#include <QList>
#include <QDir>
#include <QFile>

inline bool operator==(Game g1, Game g2)
{
    return g1.id == g2.id && g1.gameName == g2.gameName && g1.gameDirectory == g2.gameDirectory && g1.executablePath == g2.executablePath && g1.arguments == g2.arguments;
}

TEST_CASE ("Database", "[db]")
{
    QFile file("ascensionTest.db");
    if (file.exists())
    {
        std::cout << "Removing" << std::endl;
        file.remove();
    }
    Game testGame = {1, QString("Test Game"), QString("."), QString("test.exe"), QString("args")};
    Game hl3 = {2, QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")};
    QList<Game> list;
    list << testGame
         << hl3;

    REQUIRE (Database::getInstance("ascensionTest.db").init() == true);
    REQUIRE (Database::getInstance("ascensionTest.db").addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args")) == true);
    REQUIRE (Database::getInstance("ascensionTest.db").addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")) == true);
    REQUIRE (Database::getInstance("ascensionTest.db").getGameCount() == 2);
    REQUIRE (Database::getInstance("ascensionTest.db").getGameById(1) == testGame);
    REQUIRE (Database::getInstance("ascensionTest.db").getGames() == list);
    REQUIRE (Database::getInstance("ascensionTest.db").removeGameById(1) == true);
    REQUIRE (Database::getInstance("ascensionTest.db").removeGameById(1) == false);
    REQUIRE (Database::getInstance("ascensionTest.db").removeGameByName("Half-Life 3") == true);
    REQUIRE (Database::getInstance("ascensionTest.db").removeGameByName("Test Game") == false);
    REQUIRE (Database::getInstance("ascensionTest.db").reset() == true);
    file.remove();
}

TEST_CASE ("VDF Parser", "[vdf]")
{
    std::unordered_map<int, SteamVdfParse::GameHeader> map = SteamVdfParse::parseVdf("appinfo.vdf");
    std::ifstream game("game", std::ios::binary);
    REQUIRE (SteamVdfParse::read32_le(game) == uint32_t(208050));
    REQUIRE (SteamVdfParse::read64_le(game) == uint64_t(8589935831));
    game.seekg(0, std::ios::beg);
    SteamVdfParse::GameHeader gameTest = SteamVdfParse::parseGame(game);

    REQUIRE (gameTest.appID == 208050);
    REQUIRE (gameTest.size == 1239);
    REQUIRE (gameTest.infoState == 2);
    REQUIRE (gameTest.lastUpdated == 1436802448);
    REQUIRE (gameTest.accessToken == 0);
    REQUIRE (gameTest.changeNumber == 1127960);
    int codes[20] = {42, 74, 191, 28, 29, 82, 192, 217, 80, 82, 248, 202, 208, 235, 83, 1, 97, 130, 131, 196};
    bool shaFlag = true;
    for (auto i=0;i<20;i++)
    {
        if (!int(gameTest.sha[i]) == codes[i])
        {
            shaFlag = false;
            break;
        }
    }
    REQUIRE (shaFlag == true);
}
