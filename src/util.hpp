#pragma once
#include <iostream>
#include <QDirIterator>

#include <boost/property_tree/info_parser.hpp>

namespace pt = boost::property_tree;
namespace Util
{

    /** Debug function to print out a ptree */

    inline void printTree(pt::ptree &pt, int level)
    {
        if (pt.empty())
        {
            std::cerr << "\""<< pt.data()<< "\"";
        }
        else
        {
            auto indent = [&](int lvl){ std::string s; for (int i=0; i<lvl; i++) s += "  "; return s; };
            if (level) std::cerr << std::endl;
            std::cerr << indent(level) << "{" << std::endl;
            for (pt::ptree::iterator pos = pt.begin(); pos != pt.end();)
            {
                std::cerr << indent(level+1) << "\"" << pos->first << "\": ";
                printTree(pos->second, level + 1);
                ++pos;
                if (pos != pt.end())
                {
                    std::cerr << ",";
                }
                std::cerr << std::endl;
            }
            std::cerr << indent(level) << " }";
        }
    }
    /** For a given dir, find every file contained within. It's up to the implementer of this function to insure that the
     * QDir has the relevant filters.
     * \param dir Directory to search in.
     */
    inline QStringList recursiveFindFiles(QDir dir)
    {
        QStringList dirList;
        QDirIterator it(dir, QDirIterator::Subdirectories);

        while (it.hasNext())
        {
            QDir cur = it.next();
            bool found = false;
            for (auto foundIter : dirList)
            {
                if (QDir(foundIter).dirName() == cur.dirName())
                {
                    found = true;
                    break;
                }
            }
            if (!found && cur.dirName() != QDir::currentPath())
            {
                dirList.append(cur.filePath(""));
            }
        }

        return dirList;
    }
}