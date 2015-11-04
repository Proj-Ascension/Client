#include "origin_drm.h"
#include <QStandardPaths>

void OriginDRM::checkOriginExists()
{
    QDir originRoot;
    QDir originFolder;
#if defined(_WIN32) || defined(_WIN64)
    originRoot = QDir(qgetenv("APPDATA").append("/Origin"));
#else
    originRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Origin/");
#endif

    if (originRoot.exists())
    {
        pt::ptree originTree;
        read_xml(originRoot.filePath("local.xml").toLocal8Bit().constData(), originTree);


        for (auto &xmlIter : originTree.get_child("Settings"))
        {
            if (xmlIter.second.get<std::string>("<xmlattr>.key") == "DownloadInPlaceDir")
            {
                originFolder = QString::fromStdString(xmlIter.second.get<std::string>("<xmlattr>.value"));
                break;
            }
        }

        if (originFolder == QDir("."))
        {
            originFolder = QDir("C:\\Program Files (x86)\\Origin Games\\");
        }
    }

    if (originFolder.filePath("").trimmed() != "" && originFolder.exists() && originFolder != QDir("."))
    {
        this->setRootDir(originFolder);
        this->setIsInstalled();
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Origin found in " + originFolder.filePath(""));
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Origin not found. Verify installation and try again.");
    }
}

OriginDRM::OriginDRM() : DRMType("<b>Origin</b>"){}