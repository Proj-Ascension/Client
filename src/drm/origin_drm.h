#pragma once

#include "drm_type.h"

class OriginDRM : public DRMType
{
    static QDir DRMType::rootDir;
    static bool DRMType::isInstalled;

public:
    void checkOriginExists();
    using DRMType::DRMType;
    OriginDRM();
};
