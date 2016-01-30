#pragma once

#include "game.h"

#include <QtSql>



/** Database class.
 * Wrapper class to manage the internal game database.
 * Also has a simple interface to find games by certain conditions.
 */
class Database
{
public:
    static Database& getInstance();
    static Database& getInstance(QString name);
    bool open();
    bool init();
    bool reset();

    bool addGame(QString gameName, QString gameDirectory, QString executablePath, QString arguments);
    void addGames(GameList games);
    bool removeGameById(unsigned int id);
    bool removeGameByName(QString name);
    Game getGameById(unsigned int id);
    Game getGameByName(QString name);
    std::pair<bool, Game> isExistant(unsigned int id);
    std::pair<bool, Game> isExistant(QString name);
    QList<Game> getGames();
    unsigned int getGameCount();

private:
    Database();
    Database(QString name);
    QSqlDatabase db;
    Database(Database const&) = delete;
    void operator=(Database const&) = delete;
};
