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

QButtonGroup* DRMType::getButtonGroup()
{
    return buttonGroup;
}

DRMType::DRMType(QString platformString)
{
    layout = new QGridLayout();
    scrollArea = new QScrollArea();
    viewport = new QWidget();
    buttonGroup = new QButtonGroup();
    buttonGroup->setExclusive(false);
    isInstalled = false;
    rootDir = (".");
    platformLabel = new QLabel(platformString);
    platformLabel->setTextFormat(Qt::TextFormat::RichText);
    descLabel = new QLabel();
    statusLabel = new QLabel();
}
