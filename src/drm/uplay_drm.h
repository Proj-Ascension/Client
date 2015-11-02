#pragma once

#include "drm_type.h"

class UplayDRM : public DRMType
{
public:
    void checkUplayExists();
    using DRMType::DRMType;
    UplayDRM();
};


