#include "Database.h"

#include <QDebug>

Database::Database()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setHostName("localhost");
    db.setDatabaseName("ascension.db");
}

bool Database::init()
{
    bool status = db.open();
    if (!status)
    {
        qDebug("Couldn't connect to the database!");
        return false;
    }

    QSqlQuery createQuery(db);
    createQuery.exec("CREATE TABLE IF NOT EXISTS games(ID INTEGER PRIMARY KEY ASC, GAMENAME TEXT NOT NULL, GAMEDIRECTORY TEXT NOT NULL, GAMEEXECUTABLE TEXT NOT NULL, ARGUMENTS TEXT NOT NULL);");

    return true;
}

bool Database::reset()
{
    QSqlQuery query(db);
    return query.exec("DROP TABLES *");
}

bool Database::addGame(QString gameName, QString gameDirectory, QString executablePath, QString arguments)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO GAMES(GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS) VALUES (:gameName, :gameDirectory, :executablePath, :arguments);");
    query.bindValue(":gameName", gameName);
    query.bindValue(":gameDirectory", gameDirectory);
    query.bindValue(":executablePath", executablePath);
    query.bindValue(":arguments", arguments);
    return query.exec();
}

bool Database::removeGameById(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
    return query.exec();
}

bool Database::removeGameByName(QString name)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
    return query.exec();
}

Game Database::getGameById(unsigned int id) { return std::get<1>(isExistant(id)); }

Game Database::getGameByName(QString name) { return std::get<1>(isExistant(name)); }

std::tuple<bool, Game> Database::isExistant(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
    query.exec();

    if (query.next())
    {
        QString name = query.value(0).toString();
        QString path = query.value(1).toString();
        QString exe = query.value(2).toString();
        QString args = query.value(3).toString();

        Game game = {id, name, path, exe, args};
        return std::make_tuple(true, game);
    }
    else
    {
        Game game;
        return std::make_tuple(false, game);
    }
}
std::tuple<bool, Game> Database::isExistant(QString name)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
    query.exec();
    if (query.next())
    {
        unsigned int id = query.value(0).toInt();
        QString path = query.value(1).toString();
        QString exe = query.value(2).toString();
        QString args = query.value(3).toString();

        Game game = {id, name, path, exe, args};
        return std::make_tuple(true, game);
    }
    else
    {
        Game game;
        return std::make_tuple(false, game);
    }
}

QList<Game> Database::getGames()
{
    QList<Game> games;
    QSqlQuery query;
    query.exec("SELECT ID, GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES;");
    while (query.next())
    {
        unsigned int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();
        QString args = query.value(4).toString();

        games.append({id, name, path, exe, args});
    }
    return games;
}

unsigned int Database::getGameCount()
{
    QSqlQuery query(db);
    query.exec("SELECT count() FROM GAMES;");
    if (!query.next())
    {
        return 0;
    }

    return query.value(0).toInt();
}
