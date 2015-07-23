#pragma once

#include <cstdint>
#include <boost/any.hpp>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

#define MAGIC_VALUE 0x07564426

namespace SteamVdfParse
{
uint32_t read32_le(std::istream& stream)
{
    return static_cast<uint32_t>((stream.get()) | (stream.get() << 8) | (stream.get() << 16) |
                                 (stream.get() << 24));
}

uint64_t read64_le(std::istream& stream)
{
    return static_cast<uint64_t>(
        (stream.get()) | (stream.get() << 8) | (stream.get() << 16) | (stream.get() << 24) |
        ((uint64_t)stream.get() << 32) | ((uint64_t)stream.get() << 40) |
        ((uint64_t)stream.get() << 48) | ((uint64_t)stream.get() << 56));
}

std::string readString(std::istream& stream)
{
    std::string str;
    std::getline(stream, str, '\0');
    return str;
}

struct Section
{
    std::string name;
    std::unordered_map<std::string, boost::any> kv;

    template <typename Type>
    Type get(std::string key)
    {
        try
        {
            return boost::any_cast<Type>(kv[key]);
        }
        catch (const boost::bad_any_cast& e)
        {
            std::cerr << "Cannot cast value at " << key
                      << " to the wanted type." << std::endl;
            throw e;
        }
    }
};

struct Game
{
    uint32_t appID;
    uint32_t size;
    uint32_t infoState;  // 1-unavailable, 2-available
    uint32_t lastUpdated;
    uint64_t accessToken;
    uint8_t sha[20];
    uint32_t changeNumber;
    std::unordered_map<std::string, Section> sections;
};

Section parseSection(std::istream& input, bool root, std::string name)
{
    Section section;

    section.name = name;

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

        boost::any value;

        switch (valueType)
        {
            case 0x00:  // None
            {
                value = parseSection(input, false, key);
                break;
            }
            case 0x01:  // string
            {
                value = readString(input);
                break;
            }
            case 0x02:  // int
            {
                value = read32_le(input);
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
                value = read64_le(input);
                break;
            }
            default:
                std::cerr << "Unknown type: " << (int)valueType << std::endl;
        }

        section.kv.insert(std::make_pair(key, value));
    }

    return section;
}

Game parseGame(std::istream& input)
{
    Game game;
    game.appID = read32_le(input);
    if (game.appID == 0)
    {
        return game;
    }
    std::cout << "Parsing game id: " << game.appID << std::endl;
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
        Section section = parseSection(input, true, name);
        game.sections.insert(std::make_pair(name, section));
    }

    return game;
}

std::unordered_map<int, Game> parseVdf(std::string location)
{
    std::ifstream input(location, std::ifstream::binary);

    uint32_t magic = read32_le(input);
    uint32_t universe = read32_le(input);  // Parse this later

    std::unordered_map<int, Game> games;

    if (magic != MAGIC_VALUE)
    {
        std::cerr << "Invalid magic value!" << std::endl;
        return games;
    }

    while (!input.eof())
    {
        Game game = parseGame(input);
        if (game.appID == 0)
        {
            break;
        }
        games.insert(std::make_pair(game.appID, game));
    }

    return games;
}
}
