#include "origin_drm.h"
#include <QLineEdit>
#include <QCheckBox>

OriginDRM::OriginDRM() : DRMType("<b>Origin</b>"){}

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

void OriginDRM::findGames()
{
    rootDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = rootDir.entryList();
    int count = 0;

    for (auto i : folderList)
    {
        if (i != "DownloadCache")
        {
            pt::ptree& node = originTree.add("games.game", "");
            QDir dir(rootDir.absoluteFilePath(i));
            dir.setNameFilters(QStringList("*.exe"));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            QStringList test = recursiveFindFiles(dir);
            node.put("name", dir.dirName().toLocal8Bit().constData());
            for (auto exe : test)
            {
                node.add("exes.exe", exe.toLocal8Bit().constData());
            }
            count++;
        }
    }
    originTree.add("games.count", count);

}

pt::ptree OriginDRM::getGames()
{
    return originTree;
}

QWidget* OriginDRM::createPane(QWidget* parent)
{
	viewport = new QWidget(parent);
	scrollArea = new QScrollArea(parent);
	layout = new QGridLayout(parent);
    int row = 0;
    for (pt::ptree::value_type& games : originTree.get_child("games"))
    {
        boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
        if (exeTest)
        {
            QButtonGroup* group = new QButtonGroup();
            QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
            name->setFixedWidth(350);
            QCheckBox* checkBox = new QCheckBox();
            layout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
            row++;
            for (auto& exe : games.second.get_child("exes"))
            {
                checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                group->addButton(checkBox);
                layout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
            }
            buttonGroupVector.push_back(group);
        }
    }
    viewport->setLayout(layout);
    scrollArea->setWidget(viewport);

    return scrollArea;
}

QList<QButtonGroup*> OriginDRM::getButtonGroupVector()
{
    return buttonGroupVector;
}
