#include "drm_type.h"

bool DRMType::getIsInstalled()
{
    return isInstalled;
}

void DRMType::setIsInstalled()
{
    isInstalled = true;
}

QDir DRMType::getRootDir()
{
    return rootDir;
}

void DRMType::setRootDir(const QDir& rootdir)
{
    rootDir = rootdir;
}

QWidget* DRMType::createPane()
{
    return new QWidget();
}

QLabel *DRMType::getStatusLabel()
{
    return statusLabel;
}

QLabel *DRMType::getDescLabel()
{
    return descLabel;
}

QLabel *DRMType::getPlatformLabel()
{
    return platformLabel;
}

DRMType::DRMType(QString platformString)
{
    isInstalled = false;
    rootDir = (".");
    platformLabel = new QLabel(platformString);
    platformLabel->setTextFormat(Qt::TextFormat::RichText);
    descLabel = new QLabel();
    statusLabel = new QLabel();
}
