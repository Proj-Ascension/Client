#include "drm_type.h"

bool DRMType::getIsInstalled()
{
    return this->isInstalled;
}

void DRMType::setIsInstalled()
{
    this->isInstalled = true;
}

QDir DRMType::getRootDir()
{
    return this->rootDir;
}

void DRMType::setRootDir(const QDir& rootDir)
{
    this->rootDir = rootDir;
}

QWidget* DRMType::createPane()
{
    return new QWidget();
}

QLabel *DRMType::getStatusLabel()
{
    return this->statusLabel;
}

QLabel *DRMType::getDescLabel()
{
    return this->descLabel;
}

QLabel *DRMType::getPlatformLabel()
{
    return this->platformLabel;
}

DRMType::DRMType(QString platformString)
{
        this->platformLabel = new QLabel(platformString);
        platformLabel->setTextFormat(Qt::TextFormat::RichText);
        this->descLabel = new QLabel();
        this->statusLabel = new QLabel();
}
