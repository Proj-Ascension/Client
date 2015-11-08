#pragma once

#include "drm_type.h"

class UplayDRM : public DRMType
{
    pt::ptree uplayTree;
    QList<QButtonGroup*> buttonGroupVector;

public:
    void checkUplayExists();
    using DRMType::DRMType;
    void findGames();
    pt::ptree getGames();
    QWidget* createPane();
    QList<QButtonGroup*> getButtonGroupVector();
    UplayDRM();
};