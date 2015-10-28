#ifndef PROJECTASCENSION_DRM_TYPE_H
#define PROJECTASCENSION_DRM_TYPE_H

#include <QDir>

class DRMType
{
    QDir rootDir;
    bool isInstalled;
public:
    QDir getRootDir();
    QDir setRootDir(const QDir);

    bool getIsInstalled();
};

class SteamDRM : public DRMType
{
    void checkSteamExists();
};

class OriginDRM : public DRMType
{
    void checkOriginExists();
};

class UplayDRM : public DRMType
{
    void checkUplayExists();
};


#endif //PROJECTASCENSION_DRM_TYPE_H