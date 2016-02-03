#include <lib/auto_test.hpp>
#include "../src/database.h"

class DatabaseTest : public QObject
{
    Q_OBJECT
private:
    QString dbPath;
    Game testGame;
    QList<Game> list;

private slots:
    void init();
    void testAddGames();
    void testGetGameCount();
    void testGetGameById();
    void testGetGamesList();
    void testRemoveGameById();
    void testRemoveGameByName();
    void testResetDatabase();
    void cleanup();
};

void DatabaseTest::init()
{
    QTemporaryDir dir;
    dbPath = dir.isValid() ? dir.path() + "ascensionTest.db" : QDir::tempPath() + "ascensionTest.db";

    testGame = {1, QString("Test Game"), QString("."), QString("test.exe"), QString("args")};
    Game hl3 = {2, QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")};
    list = QList<Game>{testGame, hl3};


    QVERIFY(Database::getInstance(dbPath).init());
}

void DatabaseTest::testAddGames()
{
    QVERIFY(Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args")));
    QVERIFY(!Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args")));

    QVERIFY(Database::getInstance(dbPath).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")));
    QVERIFY(!Database::getInstance(dbPath).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString("")));
}

void DatabaseTest::testGetGameCount()
{
    QVERIFY(Database::getInstance(dbPath).getGameCount() == 0);
    Database::getInstance(dbPath).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString(""));
    QVERIFY(Database::getInstance(dbPath).getGameCount() == 1);
}

void DatabaseTest::testGetGameById()
{
    QVERIFY(Database::getInstance(dbPath).getGameById(1) == Game{});
    Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
    QVERIFY(Database::getInstance(dbPath).getGameById(1) == testGame);
}

void DatabaseTest::testGetGamesList()
{
    QVERIFY(Database::getInstance(dbPath).getGames() == QList<Game>{});

    Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
    Database::getInstance(dbPath).addGame(QString("Half-Life 3"), QString("."), QString("hl2.exe"), QString(""));
    QVERIFY(Database::getInstance(dbPath).getGames() == list);
}

void DatabaseTest::testRemoveGameById()
{
    QVERIFY(!Database::getInstance(dbPath).removeGameById(1));
    Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
    QVERIFY(Database::getInstance(dbPath).removeGameById(1));
}

void DatabaseTest::testRemoveGameByName()
{
    QVERIFY(!Database::getInstance(dbPath).removeGameByName("Test Game"));
    Database::getInstance(dbPath).addGame(QString("Test Game"), QString("."), QString("test.exe"), QString("args"));
    QVERIFY(Database::getInstance(dbPath).removeGameByName("Test Game"));
}

void DatabaseTest::testResetDatabase()
{
    QVERIFY(Database::getInstance(dbPath).reset());
}

void DatabaseTest::cleanup()
{
    Database::getInstance(dbPath).reset();
    QFile(dbPath).remove();
}

DECLARE_TEST(DatabaseTest)
#include "database_test.moc"