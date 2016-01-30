#include "database.h"

/** Database constructor
 * Constructs the local database.
 * Currently no interface to handle remote databases, just creates one in the
 * current working directory.
 */
Database::Database()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setHostName("localhost");
    db.setDatabaseName("ascension.db");
}

/** Database constructor
 * Constructs the local database.
 * Currently no interface to handle remote databases, just creates one in the
 * current working directory.
 */
Database::Database(QString name)
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setHostName("localhost");
    db.setDatabaseName(name);
}

/** Initialize the actual database, if it hasn't been done already.
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
    return createQuery.exec("CREATE TABLE IF NOT EXISTS games(ID INTEGER PRIMARY KEY ASC, GAMENAME TEXT NOT NULL, GAMEDIRECTORY TEXT NOT NULL, GAMEEXECUTABLE TEXT NOT NULL, ARGUMENTS TEXT NOT NULL);");
}

/** Remove every table in the database.
 * \return Success/failure of the operation.
*/
bool Database::reset()
{
    if (!db.open())
    {
        return false;
    }
    QSqlQuery query(db);
    return query.exec("DROP TABLE IF EXISTS games") && QFile::remove(db.databaseName());
}

/** Add a game to the database and repopulate the games list.
 * \param gameName The name of the game.
 * \param gameDirectory Working directory of the game.
 * \param executablePath The location of the executable on the filesystem.
 * \param arguments List of arguments to launch with.
 * \return Success/failure of the operation.
*/
bool Database::addGame(QString gameName, QString gameDirectory, QString executablePath, QString arguments)
{
    if (!db.open())
    {
        return false;
    }
    if (!std::get<0>(isExistant(gameName)))
    {
        QSqlQuery query(db);
        query.prepare("INSERT OR IGNORE INTO GAMES(GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS) VALUES (:gameName, :gameDirectory, :executablePath, :arguments);");
        query.bindValue(":gameName", gameName);
        query.bindValue(":gameDirectory", gameDirectory);
        query.bindValue(":executablePath", executablePath);
        query.bindValue(":arguments", arguments);
        return query.exec();
    }
    else
    {
        return false;
    }
}

/** Add games to the database and repopulate the games list.
 * \param games GameList of games to add.
 * \return Success/failure of the operation.
*/
void Database::addGames(GameList games)
{
    if (!db.open())
    {
        return;
    }
    for (auto& game : games)
    {
        addGame(game.gameName, game.gameDirectory, game.executablePath, game.arguments);
    }
}

/** Remove a game from the database by their ID.
 * \param id ID of the game to remove.
 * \return Success/failure of the operation.
*/
bool Database::removeGameById(unsigned int id)
{
    if (!db.open())
    {
        return false;
    }
    if (std::get<0>(isExistant(id)))
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM games WHERE ID = :id;");
        query.bindValue(":id", id);
        return query.exec();
    }
    else
    {
        return false;
    }
}

/** Remove a game from the database by their name.
 * \param name Name of the game to remove.
*/
bool Database::removeGameByName(QString name)
{
    if (!db.open())
    {
        return false;
    }
    if (std::get<0>(isExistant(name)))
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM GAMES WHERE GAMENAME = :name;");
        query.bindValue(":name", name);
        return query.exec();
    }
    else
    {
        return false;
    }
}

/** Wrapper to access the Game object from the ID.
 * \param id ID to find.
 * \return A Game object, empty upon failure.
*/
Game Database::getGameById(unsigned int id)
{
    return std::get<1>(isExistant(id));
}

/** Wrapper to access the Game object from the name.
 * \param id ID to find.
 * \return A Game object, empty upon failure.
*/
Game Database::getGameByName(QString name)
{
    return std::get<1>(isExistant(name));
}

/** Perform a query to find a specific game in the database by their ID. Unsafe at the
 * moment.
 * \param id ID of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
std::pair<bool, Game> Database::isExistant(unsigned int id)
{
    if (!db.open())
    {
        return std::make_pair(false, Game{});
    }
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
    query.exec();

    if (query.next())
    {
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();
        QString args = query.value(4).toString();

        return std::make_pair(true, Game {id, name, path, exe, args});
    }
    else
    {
        return std::make_pair(false, Game{});
    }
}

/** Perform a query to find a specific game by their name (soon to be
 * deprecated).
 *
 * \param name Name of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
std::pair<bool, Game> Database::isExistant(QString name)
{
    if (!db.open())
    {
        return std::make_pair(false, Game{});
    }
    QSqlQuery query(db);
    query.prepare("SELECT ID, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
    query.exec();
    if (query.next())
    {
        unsigned int id = query.value(0).toUInt();
        QString path = query.value(1).toString();
        QString exe = query.value(2).toString();
        QString args = query.value(3).toString();

        return std::make_pair(true, Game {id, name, path, exe, args});
    }
    else
    {
        return std::make_pair(false, Game{});
    }
}

/** Perform a query to find every game in the database.
 * \return A QList of Game objects containing everything in the database.
*/
QList<Game> Database::getGames()
{

    if (!db.open())
    {
        return QList<Game>{};
    }
    QList<Game> games;
    QSqlQuery query;
    query.exec("SELECT ID, GAMENAME, GAMEDIRECTORY, GAMEEXECUTABLE, ARGUMENTS FROM GAMES;");
    while (query.next())
    {
        unsigned int id = query.value(0).toUInt();
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();
        QString args = query.value(4).toString();

        games.append({id, name, path, exe, args});
    }
    return games;
}

/** Queries the database to find the number of games.
 * \return Total number of games stored so far.
*/
unsigned int Database::getGameCount()
{
    if (!db.open())
    {
        return 0;
    }
    QSqlQuery query(db);
    query.exec("SELECT count() FROM GAMES;");
    if (!query.next())
    {
        return 0;
    }

    return query.value(0).toUInt();
}

/** Get the current instance, else create it */
Database &Database::getInstance()
{
    static Database instance;
    return instance;
}

/** Get the current instance, else create it with a path */
Database& Database::getInstance(QString name)
{
    static Database instance(name);
    return instance;
}

bool Database::open() {
    return db.open();
}
