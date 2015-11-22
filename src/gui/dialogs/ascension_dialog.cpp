#include "ascension_dialog.h"

#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>

/** AscensionDialog constructor
 * Constructs the base UI for Dialog, with the UI elements needed for Project Ascension.
 * This constructor does not provide any QPushButtons or QLabels for the dialog content.
 * \param title String to be displayed as the title on the window.
 */
AscensionDialog::AscensionDialog(QString title)
    : QDialog(),
      p(new QSettings("palette.ini", QSettings::IniFormat))
{
    setObjectName("ascensionDialog");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(title);
    setWindowModality(Qt::ApplicationModal);

    // Main panel layout
    QGridLayout* mainGridLayout = new QGridLayout;
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Main panel scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("dialogScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainGridLayout->addWidget(scrollArea);

    // Core widget
    QWidget* coreWidget = new QWidget(scrollArea);
    coreWidget->setObjectName("coreWidget");
    coreWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(coreWidget);

    // Vertical layout #1
    QVBoxLayout* verticalLayout1 = new QVBoxLayout;
    verticalLayout1->setSpacing(0);
    verticalLayout1->setMargin(0);
    verticalLayout1->setAlignment(Qt::AlignHCenter);
    coreWidget->setLayout(verticalLayout1);

    // Accent border
    QLabel* accentBorder = new QLabel(coreWidget);
    accentBorder->setObjectName("accentBorder");
    accentBorder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    accentBorder->setMaximumHeight(3);
    accentBorder->setStyleSheet("border-top: 2px solid " + p->value("Accent/MediumAccent").toString() +
                                ";border-bottom: 1px solid" + p->value("Accent/DarkAccent").toString() + ";");
    accentBorder->adjustSize();
    verticalLayout1->addWidget(accentBorder);

    // Backdrop widget - vertical layout #3
    QWidget* mainPanelBackdrop = new QWidget(coreWidget);
    mainPanelBackdrop->setObjectName("mainPanelBackdrop");
    mainPanelBackdrop->setStyleSheet("QWidget#mainPanelBackdrop {background-color: " +
                                     p->value("Primary/DarkestBase").toString() + ";}");
    verticalLayout1->addWidget(mainPanelBackdrop);

    // Vertical layout #3
    QVBoxLayout* verticalLayout3 = new QVBoxLayout;
    verticalLayout3->setSpacing(0);
    verticalLayout3->setMargin(0);
    verticalLayout3->setAlignment(Qt::AlignHCenter);
    mainPanelBackdrop->setLayout(verticalLayout3);

    // Horizontal layout #1 - Top bar
    QHBoxLayout* horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->setSpacing(0);
    horizontalLayout1->setMargin(0);
    verticalLayout3->addLayout(horizontalLayout1);

    // Horizontal layout #2 - Dialog Title
    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setSpacing(0);
    horizontalLayout2->setContentsMargins(12, 2, 0, 0);
    horizontalLayout2->setAlignment(Qt::AlignLeft);
    horizontalLayout1->addLayout(horizontalLayout2);

    // Title Label
    QLabel* label = new QLabel(title, coreWidget);
    label->setStyleSheet("color: " + p->value("Primary/SubText").toString() + ";");
    label->setFont(QFont("SourceSansPro", 9, QFont::DemiBold));
    horizontalLayout2->addWidget(label);

    // Horizontal layout #3 - window controls
    QHBoxLayout* horizontalLayout3 = new QHBoxLayout;
    horizontalLayout3->setSpacing(0);
    horizontalLayout3->setContentsMargins(0, 9, 7, 7);
    horizontalLayout1->addLayout(horizontalLayout3);

    horizontalLayout3->addStretch();

    // Window controls
    // Close
    QPushButton* pushButtonClose = new QPushButton("", coreWidget);
    pushButtonClose->setObjectName("pushButtonClose");
    horizontalLayout3->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonClose()));

    // Content
    content = new QWidget(coreWidget);
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->setStyleSheet("QPushButton {"
                           "color: " + p->value("Primary/LightText").toString() + "; "
                           "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                           "border: none; margin: 0px; padding: 4px 0 4px 0;} "
                           "QPushButton:hover {"
                           "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                           "QPushButton[default='true'] {"
                           "background-color: " + p->value("Accent/MediumAccent").toString() + ";} "
                           "QPushButton[default='true']:hover {"
                           "background-color: " + p->value("Accent/LightAccent").toString() + ";} "
                           "QLabel {"
                           "color: " + p->value("Primary/LightText").toString() + ";} ");

    verticalLayout3->addWidget(content);
}

AscensionDialog::~AscensionDialog()
{
    delete p;
}

void AscensionDialog::pushButtonClose()
{
    this->reject();
}

void AscensionDialog::mousePressEvent(QMouseEvent* evt)
{
    oldWindowPos = evt->globalPos();
    if (evt->pos().y() < 32)
    {
        if (evt->button() == Qt::LeftButton)
        {
            dragging = true;
        }
    }
}

void AscensionDialog::mouseReleaseEvent(QMouseEvent* evt)
{
    dragging = false;
}

void AscensionDialog::mouseMoveEvent(QMouseEvent* evt)
{
    const QPoint c_delta = evt->globalPos() - oldWindowPos;
    if (dragging)
    {
        dragging = true;
        move(x() + c_delta.x(), y() + c_delta.y());
        oldWindowPos = evt->globalPos();
    }
}

/** Function to create a AscensionDialog with a basic QLabel showing the message and Cancel and Return buttons.
 * \param title The title of the dialog
 * \param message The message to show on the dialog
 * \return The value of AscensionDialog::exec(), true if accepted, false if rejected.
 */
bool AscensionDialog::showConfirmDialog(QString title, QString message) {
    AscensionDialog* dialog = new AscensionDialog(title);
    QWidget* content = dialog->content;

    QGridLayout* gridLayout = new QGridLayout;
    content->setLayout(gridLayout);

    QFont font = QFont("SourceSansPro", 9);

    QLabel* label = new QLabel(message, content);
    label->setFont(font);
    label->setAlignment(Qt::AlignTop);
    label->setWordWrap(true);
    gridLayout->addWidget(label, 0, 0);

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    gridLayout->addLayout(horizontalLayout, 1, 0);

    QPushButton* cancelBtn = new QPushButton("Cancel", content);
    cancelBtn->setContentsMargins(8, 8, 8, 8);
    cancelBtn->setFont(font);
    connect(cancelBtn, SIGNAL(clicked()), dialog, SLOT(reject()));
    horizontalLayout->addWidget(cancelBtn);

    QPushButton* confirmBtn = new QPushButton("Confirm", content);
    confirmBtn->setContentsMargins(8, 8, 8, 8);
    confirmBtn->setFont(font);
    confirmBtn->setProperty("default", true);
    connect(confirmBtn, SIGNAL(clicked()), dialog, SLOT(accept()));
    horizontalLayout->addWidget(confirmBtn);

    bool accepted = dialog->exec();
    delete dialog;
    return accepted;
}
