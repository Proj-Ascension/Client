#pragma once

#include <tuple>
#include <QSqlDatabase>
#include <QSqlQuery>

struct Game
{
    unsigned int id;
    QString gameName;
    QString gameDirectory;
    QString executablePath;
};

class Database
{
public:
    Database();
    bool init();
    bool reset();

    bool addGame(QString gameName, QString gameDirectory, QString executablePath);
    bool removeGameById(unsigned int id);
    bool removeGameByName(QString name);
    Game getGameById(unsigned int id);
    Game getGameByName(QString name);
    std::tuple<bool, Game> isExists(QString name);
    QList<Game> getGames();
    unsigned int getGameCount();
private:
    QSqlDatabase db;
};
