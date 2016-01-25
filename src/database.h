#pragma once

#include <QSqlQuery>
#include <QtWidgets>

/** Game type.
 * Defines a game type. 
 * Members correspond to a field within the database.
 */
typedef struct
{
    unsigned int id;  /**< DB ID of the game. */
    QString gameName; /**< Name of the game to display */
    QString gameDirectory; /**< Working directory of the game */
    QString executablePath; /**< Path to the executable */
    QString arguments; /**< Arguments to pas to the executable */
} Game;

typedef std::vector<Game> GameList;

/** Database class.
 * Wrapper class to manage the internal game database.
 * Also has a simple interface to find games by certain conditions.
 */
class Database
{
public:
    static Database& getInstance();
    static Database& getInstance(QString name);
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
    unsigned int getGameCount() const;

private:
    Database();
    Database(QString name);
    QSqlDatabase db;
    Database(Database const&) = delete;
    void operator=(Database const&) = delete;
};
