#pragma once

#include "drm_type.h"
#include <src/database.h>

class SteamDRM : public DRMType
{
    QStringList steamDirectoryList;
	std::vector<Game> steamVector;
    void parseAcf();
    QProgressDialog dialog;

public:
    using DRMType::DRMType;
    void checkExists();
    void findGames();
    std::vector<Game> getGames();
    QButtonGroup* getButtonGroup();
    QWidget* createPane();
    SteamDRM();
};

