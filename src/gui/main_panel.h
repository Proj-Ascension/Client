#pragma once

#include "tab_widget.h"
#include "sidebar.h"
#include "stack/library.h"

#include <QStackedWidget>
#include <QApplication>
#include <QSettings>
#include <QWidget>

/** MainPanel class.
* Class to construct the core content of the launcher window.
* Also moderates interaction between sidebar and stacked content.
*/
class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget* parent = 0);
    ~MainPanel()
    {
        delete p;
    }

public slots:
    virtual void pushButtonMinimize() = 0;
    virtual void pushButtonMaximize() = 0;
    virtual void pushButtonClose() = 0;

protected:
    QStackedWidget* stack;
    void init();

private:
    QSettings* p;
    Sidebar* sidebar;
    TabWidget* activeTab;

    QWidget* home;
    QWidget* community;
    Library* library;
	QWidget* settings;
    QWidget* news;

private slots:
    void setHome()
    {
        if (!(activeTab == sidebar->homeTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(home);
            activeTab = sidebar->homeTab;
            activeTab->toggleActive();
        }
    }
    void setStore()
    {
        if (!(activeTab == sidebar->storeTab))
        {
            activeTab->toggleInactive();
            activeTab = sidebar->storeTab;
            activeTab->toggleActive();
        }
    }
    void setGames()
    {
        if (!(activeTab == sidebar->gamesTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(library);
            activeTab = sidebar->gamesTab;
            activeTab->toggleActive();
        }
    }
    void setCommunity()
    {
        if (!(activeTab == sidebar->communityTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(community);
            activeTab = sidebar->communityTab;
            activeTab->toggleActive();
        }
    }
    void setNews()
    {
        if (!(activeTab == sidebar->newsTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(news);
            activeTab = sidebar->newsTab;
            activeTab->toggleActive();
        }
    }
    void setDownloads()
    {
        if (!(activeTab == sidebar->downloadsTab))
        {
            activeTab->toggleInactive();
            activeTab = sidebar->downloadsTab;
            activeTab->toggleActive();
        }
    }
    void setSettings()
    {
        if (!(activeTab == sidebar->settingsTab))
        {
            activeTab->toggleInactive();
			stack->setCurrentWidget(settings);
            activeTab = sidebar->settingsTab;
            activeTab->toggleActive();
        }
    }
};
