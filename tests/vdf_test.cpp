#include <lib/auto_test.hpp>
#include "../src/libs/steam_vdf_parse.hpp"

class VdfTest : public QObject
{
    Q_OBJECT
   private:
    std::unordered_map<int, SteamVdfParse::GameHeader> map;
    SteamVdfParse::GameHeader gameTest;
   private slots:
    void init();
    void testAppId();
    void testSize();
    void testInfoState();
    void testLastUpdated();
    void testAccessToken();
    void testChangeNumber();
    void testSha();
};

void VdfTest::init()
{
    map = SteamVdfParse::parseVdf("appinfo.vdf");
    gameTest = map.at(208050);
}

void VdfTest::testAppId()
{
    QVERIFY(gameTest.appID == 208050);
}

void VdfTest::testSize()
{
    QVERIFY(gameTest.size == 1239);
}

void VdfTest::testInfoState()
{
    QVERIFY(gameTest.infoState == 2);
}

void VdfTest::testLastUpdated()
{
    QVERIFY(gameTest.lastUpdated == 1439232482);
}

void VdfTest::testAccessToken()
{
    QVERIFY(gameTest.accessToken == 0);
}

void VdfTest::testChangeNumber()
{
    QVERIFY(gameTest.changeNumber == 1127960);
}

void VdfTest::testSha()
{
    int codes[20] = {42, 74, 191, 28, 19, 82, 192, 217, 80, 82, 248, 202, 208, 235, 83, 1, 97, 130, 131, 196};

    bool shaFlag = true;
    for (auto i = 0; i < 20; i++)
    {
        if (int(gameTest.sha[i]) != codes[i])
        {
            shaFlag = false;
            break;
        }
    }
    QVERIFY(shaFlag);
}

DECLARE_TEST(VdfTest)
#include "vdf_test.moc"
