#include "drm_type.h"

/** Getter for isInstalled bool */
bool DRMType::getIsInstalled()
{
    return isInstalled;
}

/** Setter for the isInstalled bool */
void DRMType::setIsInstalled()
{
    isInstalled = true;
}

/** Getter for rootDir */
QDir DRMType::getRootDir()
{
    return rootDir;
}

/** Setter for rootDir
 * \param rootDir const ref to QDir to set
 */
void DRMType::setRootDir(const QDir &rootdir)
{
    rootDir = rootdir;
}

/** Getter for statusLabel */
QLabel* DRMType::getStatusLabel()
{
    return statusLabel;
}

/** Getter for descLabel */
QLabel* DRMType::getDescLabel()
{
    return descLabel;
}

/** Getter for platformLabel */
QLabel* DRMType::getPlatformLabel()
{
    return platformLabel;
}

/** Getter for buttonGroup */
QButtonGroup* DRMType::getButtonGroup()
{
    return buttonGroup;
}

/** DRMType constructor, sets a myriad of variables. Inherited by the other
 * DRMTypes.
 * \param platformString QString of the platform label
 */
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
