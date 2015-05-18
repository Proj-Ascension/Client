#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

typedef struct
{
    unsigned int id;
    QString gameName;
    QString gameDirectory;
    QString executablePath;
} Game;

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
    QList<Game> getGames();
    unsigned int getGameCount();
private:
    QSqlDatabase db;
};
