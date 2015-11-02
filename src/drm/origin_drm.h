#pragma once

#include "drm_type.h"

class OriginDRM : public DRMType
{
public:
    void checkOriginExists();
    using DRMType::DRMType;
    OriginDRM();
};
