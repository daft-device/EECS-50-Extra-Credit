#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Window & Page Setup */
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    /* Create function connections */
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::StartBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//======================================================================================

/*
 * Summary   : This function will execute the RestaurantMenuWindow dialog.
 * Parameters: void
 * Return    : void
 */
void MainWindow::StartBtnClicked()
{
    RestaurantMenuWindow menuDlg(this);
    menuDlg.exec();
}

//======================================================================================
