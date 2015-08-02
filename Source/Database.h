#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

//! Game type
/*! 
 * Defines a game type. 
 * Members correspond to a field within the database.
*/
typedef struct
{
    unsigned int id; /*!< Unique identifier for each game. */
    QString gameName;  /*!< Name to display within the client. */
    QString gameDirectory; /*! Directory to use, for loading save files & configuration. */
    QString executablePath; /*! Path to the executable. */
} Game;

//! Database class
/*! Wrapper class to manage the internal game database.
 * Also has a simple interface to find games by certain conditions.
*/
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
