#pragma once

#include <QDir>
#include <QWidget>

class DRMType
{
    QDir rootDir;
    bool isInstalled = false;

public:
    QDir getRootDir();
    void setRootDir(const QDir&);

    bool getIsInstalled();
    void setIsInstalled();

    virtual QWidget isExists();
};