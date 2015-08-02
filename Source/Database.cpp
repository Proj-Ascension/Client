#include "Database.h"

#include <QDebug>

//! Constructor
/*!
 * Constructs the local database. 
 * Currently no interface to handle remote databases, just creates on in the
 * current working directory.
*/
Database::Database() : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setHostName("localhost");
    db.setDatabaseName("ascension.db");
}

/*! Initialize the actual database, if it hasn't been done already.
 * \return Success/failure of the operation.
*/
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

/*! Remove every table in the database.
 * \return Success/failure of the operation.
*/
bool Database::reset()
{
    QSqlQuery query(db);
    return  query.exec("DROP TABLES *");
}

/*! Add a game to the database and repopulate the games list.
 * \param gameName The name of the game.
 * \param gameDirectory Working directory of the game.
 * \param executablePath The location of the executable on the filesystem.
 * \return Success/failure of the operation.
*/
bool Database::addGame(QString gameName, QString gameDirectory, QString executablePath)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO GAMES(GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE) VALUES (:gameName, :gameDirectory, :executablePath);");
    query.bindValue(":gameName", gameName);
    query.bindValue(":gameDirectory", gameDirectory);
    query.bindValue(":executablePath", executablePath);
    return query.exec();
}

/*! Remove a game from the database by their ID.
 * \param id ID of the game to remove.
 * \return Success/failure of the operation.
*/
bool Database::removeGameById(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
    return query.exec();
}

/*! Remove a game from the database by their name.
 * \param name Name of the game to remove
*/
bool Database::removeGameByName(QString name)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
    return query.exec();
}

/*! Perform a query to find a specific game in the database by their ID. Unsafe at the
 * moment.
 * \param id ID of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
Game Database::getGameById(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
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

/*! Perform a query to find a specific game by their name. Unsafe at the moment.
 * \param name Name of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
Game Database::getGameByName(QString name)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMEDIRECTORY, GAMEEXECUTABLE FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
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

/*! Perform a query to find every game in the database.
 * \return A QList of Game objects containing everything in the database.
*/
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

/*! Queries the database to find the number of games.
 * \return Total number of games stored so far.
*/
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
