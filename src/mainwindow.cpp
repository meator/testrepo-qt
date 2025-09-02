#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , wizard(new ATP::ATPWizard(ATP::ADB | ATP::FASTBOOT, this))
{
    ui->setupUi(this);
    connect(this->ui->select_button, &QAbstractButton::clicked, this->wizard, &QWizard::open);
    // this->setCentralWidget(new QtAndroidToolsPicker::AndroidToolsPicker(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
