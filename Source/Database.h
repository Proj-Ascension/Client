#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database();
    bool init();
private:
    QSqlDatabase db;
};
