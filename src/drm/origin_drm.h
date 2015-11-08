#pragma once

#include "drm_type.h"

class OriginDRM : public DRMType
{
    pt::ptree originTree;
    QList<QButtonGroup*> buttonGroupVector;

public:
    void checkOriginExists();
    using DRMType::DRMType;
    QWidget* createPane(QWidget* parent);
    void findGames();
    pt::ptree getGames();
    QList<QButtonGroup*> getButtonGroupVector();
    OriginDRM();
};
