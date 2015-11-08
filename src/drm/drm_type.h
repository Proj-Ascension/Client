#pragma once
#include <src/util.hpp>

#include <QDir>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QButtonGroup>

#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

namespace pt = boost::property_tree;
using namespace Util;

class DRMType
{
protected:
    QDir rootDir;
    bool isInstalled;
    QLabel* statusLabel;
    QLabel* descLabel;
    QLabel* platformLabel;

    QGridLayout* layout;
    QScrollArea* scrollArea;
    QWidget* viewport;

    QButtonGroup* buttonGroup;

public:
    QDir getRootDir();
    void setRootDir(const QDir &);

    bool getIsInstalled();
    void setIsInstalled();

    QLabel* getStatusLabel();
    QLabel* getDescLabel();
    QLabel* getPlatformLabel();

    virtual QButtonGroup* getButtonGroup();
    virtual void findGames() = 0;
    DRMType(QString platformString);
};