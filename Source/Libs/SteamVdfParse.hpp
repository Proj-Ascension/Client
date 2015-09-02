#pragma once

#include <boost/property_tree/ptree.hpp>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_map>

#define MAGIC_VALUE 0x07564426

/** Steam VDF Parsing. 
 * A bunch of utility functions to parse the Steam VDF binary format
 */
namespace SteamVdfParse
{

namespace pt = boost::property_tree;
    
/** Function to convert an istream object into a 32-bit little endian integer.
 * \param stream The stream to convert
 * \return The converted integer
 */
inline uint32_t read32_le(std::istream& stream)
{
    return static_cast<uint32_t>((stream.get()) | (stream.get() << 8) | (stream.get() << 16) |
                                 (stream.get() << 24));
}

/** Function to convert an istream object into a 64-bit little endian integer.
 * \param stream The stream to convert
 * \return The converted integer
 */
inline uint64_t read64_le(std::istream& stream)
{
    return static_cast<uint64_t>(
        (stream.get()) | (stream.get() << 8) | (stream.get() << 16) | (stream.get() << 24) |
        ((uint64_t)stream.get() << 32) | ((uint64_t)stream.get() << 40) |
        ((uint64_t)stream.get() << 48) | ((uint64_t)stream.get() << 56));
}

/** Function to convert an istream object into a std::string.
 * \param stream The stream to convert
 * \return The converted string
 */
inline std::string readString(std::istream& stream)
{
    std::string str;
    std::getline(stream, str, '\0');
    return str;
}

/** Struct containing information about a game, as well as a property tree of
 * the parsed sections.
 */
struct GameHeader
{
    uint32_t appID; /**< Steam ID of the game */
    uint32_t size; /**< Size on disk */
    uint32_t infoState;  /**< State of the game, 1-unavailable, 2-available */
    uint32_t lastUpdated; /**< UNIX time of the last update */
    uint64_t accessToken; /**< Token needed to auth services (if needed) */
    uint8_t sha[20]; /**< Checksum of the fs */
    uint32_t changeNumber; /**< ID of the latest change on the game repo */
    pt::ptree pt; /**< Unordered sections of the games in appinfo */
};

/** Take an istream object and a flag to check if the current node is the root
 * node, as well as the key name, and translate the bytes into a property tree.
 * \param input The stream to translate
 * \param root A flag to check if the current node is root
 * \param name Name of the key to use in the section
 * \return The parsed property tree
 * \sa parseGame(), parseVdf()
 */
inline pt::ptree parseSection(std::istream& input, bool root, std::string name)
{
    pt::ptree section; 

    while (true)
    {
        uint8_t valueType = input.get();

        if (valueType == 0x08)
        {
            if (root)
            {
                input.get();
            }
            break;
        }

        std::string key = readString(input);

        switch (valueType)
        {
            case 0x00:  // None
            {
                section.add_child(key, parseSection(input, false, key));
                break;
            }
            case 0x01:  // string
            {
                section.put(key, readString(input));
                break;
            }
            case 0x02:  // int
            {
                section.put(key, read32_le(input));
                break;
            }
            case 0x03:  // float
            {
                std::cerr << "Floats are not implemented." << std::endl;
                break;
            }
            case 0x04:  // ptr
            {
                std::cerr << "Pointers are not implemented." << std::endl;
                break;
            }
            case 0x05:  // Wide string
            {
                std::cerr << "Wide strings are not implemented." << std::endl;
                break;
            }
            case 0x06:  // Colour
            {
                std::cerr << "Colours are not implemented." << std::endl;
                break;
            }
            case 0x07:  // 64 bit int
            {
                section.put(key, read64_le(input));
                break;
            }
            default:
                std::cerr << "Unknown type: " << (int)valueType << std::endl;
        }
    }

    return section;
}

/** Take an istream object, file expected, and convert the block of bytes to a
 * GameHeader type.
 * \param input The stream to translate
 * \return A GameHeader of the section
 * \sa parseSection(), parseVdf()
 */
inline GameHeader parseGame(std::istream& input)
{
    GameHeader game;
    game.appID = read32_le(input);
    if (game.appID == 0)
    {
        return game;
    }

    game.size = read32_le(input);
    game.infoState = read32_le(input);
    game.lastUpdated = read32_le(input);
    game.accessToken = read64_le(input);

    for (int i = 0; i < 20; i++)
    {
        game.sha[i] = input.get();
    }

    game.changeNumber = read32_le(input);

    while (true)
    {
        uint8_t sectionID = input.get();
        if (sectionID == 0x00)
        {
            break;
        }

        input.get();  // read the 00 before name

        std::string name = readString(input);
        pt::ptree section = parseSection(input, true, name);
        game.pt.add_child(name, section);
    }

    return game;
}

/** Takes the location of an appinfo file and handles the parsing if the file is
 * correct.
 * \param location Location of the appinfo file
 * \return A full map of each game, with the key being the game's Steam ID
 * \sa parseSection(), parseVdf()
 */
inline std::unordered_map<int, GameHeader> parseVdf(std::string location)
{
    std::ifstream input(location, std::ifstream::binary);

    uint32_t magic = read32_le(input);
    uint32_t universe = read32_le(input);  // Parse this later

    std::unordered_map<int, GameHeader> games;

    if (magic != MAGIC_VALUE)
    {
        std::cerr << "Invalid magic value!" << std::endl;
        return games;
    }

    while (!input.eof())
    {
        GameHeader game = parseGame(input);
        if (game.appID == 0)
        {
            break;
        }
        games.insert(std::make_pair(game.appID, game));
    }

    return games;
}
}

