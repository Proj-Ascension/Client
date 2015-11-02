#pragma once

#include "drm_type.h"

class SteamDRM : public DRMType
{
    std::vector<QDir> steamDirectoryList;

public:
    void checkSteamExists();
    using DRMType::DRMType;
    SteamDRM();
};

