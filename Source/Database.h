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

    bool addGame(QString gameName, QString gameDirectory, QString executablePath);
    bool removeGameById(unsigned int id);
    Game getGameById(unsigned int id);
    QList<Game> getGames();
    unsigned int getGameCount();
private:
    QSqlDatabase db;
};
