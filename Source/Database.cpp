#include "Database.h"

#include <QDebug>

Database::Database() : db(QSqlDatabase::addDatabase("QSQLITE"))
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
    createQuery.exec("CREATE TABLE IF NOT EXISTS games(ID INTEGER PRIMARY KEY ASC, GAMENAME TEXT NOT NULL, GAMEDIRECTORY TEXT NOT NULL, GAMEEXECUTABLE TEXT NOT NULL);");

    return true;
}

bool Database::reset()
{
    QSqlQuery query(db);
    return  query.exec("DROP TABLES *");
}

bool Database::addGame(QString gameName, QString gameDirectory, QString executablePath)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO GAMES(GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE) VALUES (:gameName, :gameDirectory, :executablePath);");
    query.bindValue(":gameName", gameName);
    query.bindValue(":gameDirectory", gameDirectory);
    query.bindValue(":executablePath", executablePath);
    return query.exec();
}

bool Database::removeGameById(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM GAMES WHERE id = :id;");
    query.bindValue(":id", id);
    return query.exec();
}

Game Database::getGameById(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE FROM GAMES WHERE ID = :id;");
    query.bindValue(":ID", id);
    query.exec();

    if (!query.next())
    {
        return {0}; // TODO: ERROR HANDLING
    }

    QString name = query.value(0).toString();
    QString path = query.value(1).toString();
    QString exe = query.value(2).toString();

    return {id, name, path, exe};
}

Game Database::getGameByName(QString name)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMEDIRECTORY, GAMEEXECUTABLE FROM GAMES WHERE GAMENAME = :NAME;");
    query.bindValue(":NAME", name);
    query.exec();

    if (!query.next())
    {
        return {0}; // TODO: ERROR HANDLING
    }

    unsigned int id = query.value(0).toInt();
    QString path = query.value(1).toString();
    QString exe = query.value(2).toString();

    Game game = {id, name, path, exe};
    return game;
}

QList<Game> Database::getGames()
{
    QList<Game> games;
    QSqlQuery query;
    query.exec("SELECT ID, GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE FROM GAMES;");
    while(query.next())
    {
        unsigned int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();

        games.append({id, name, path, exe});
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
