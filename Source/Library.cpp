#include <QFileDialog>
#include <QProcess>

#include "Library.h"
#include "ui_Library.h"

Library::Library(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
}

Library::~Library()
{
    delete ui;
}

void Library::on_testLaunch_clicked()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Select Executable");
    dialog.setFileMode(QFileDialog::ExistingFile);


    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        QString file = files.at(0);

        runProcess(file);
    }
}

void Library::runProcess(QString location) {
    // TODO: Implement some threading
    QProcess process(this);
    process.start("\"" + location + "\"");
    process.waitForFinished(-1);
}
