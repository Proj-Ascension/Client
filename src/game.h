#pragma once
#include <QString>
#include <vector>

/** Game type.
 * Defines a game type.
 * Members correspond to a field within the database.
 */
struct Game
{
    unsigned int id;  /**< DB ID of the game. */
    QString gameName; /**< Name of the game to display */
    QString gameDirectory; /**< Working directory of the game */
    QString executablePath; /**< Path to the executable */
    QString arguments; /**< Arguments to pas to the executable */
    inline bool operator==(const Game g) const /**< Comparison operator for Games */
    {
        return g.id == id
               && g.gameName == gameName
               && g.gameDirectory == gameDirectory
               && g.executablePath == executablePath
               && g.arguments == arguments;
    }
};

typedef std::vector<Game> GameList;

