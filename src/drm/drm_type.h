#pragma once

#include <QDir>
#include <QWidget>
#include <QLabel>

#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

namespace pt = boost::property_tree;

class DRMType
{
    QDir rootDir;
    bool isInstalled;

protected:
    QLabel* statusLabel;
    QLabel* descLabel;
    QLabel* platformLabel;

public:
    QDir getRootDir();
    void setRootDir(const QDir &);

    bool DRMType::getIsInstalled();
    void setIsInstalled();

    QLabel* getStatusLabel();
    QLabel* getDescLabel();
    QLabel* getPlatformLabel();

    virtual QWidget* createPane();
    DRMType(QString platformString);
};