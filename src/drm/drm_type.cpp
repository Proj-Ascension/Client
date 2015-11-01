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