#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->mdpckbtn, &QPushButton::clicked, this, [this]() {
        QString modpckName = QFileDialog::getOpenFileName(this, "Open File");
        if (!modpckName.isEmpty()) {
            input = modpckName;
            ui->mdpckPath->setText(modpckName);
        }
    });

    connect(ui->outbtn, &QPushButton::clicked, this, [this](){
        QString outfileName = QFileDialog::getOpenFileName(this, "Open File");
        if(!outfileName.isEmpty()){
            output = outfileName;
            ui->outPath->setText(outfileName);
        }
    });

    //TODO: Rewiev the connect code below for extract button
    connect(ui->extractBtn, &QPushButton::clicked, this, [this](){
        QProcess process;
        process.start("python3", QStringList() << "extract.py" << "--input" << input << "--output" << output);

        if (!process.waitForFinished()) {
            qDebug() << "Process timed out or failed to start!";
            return;
        }

        QString stdoutOutput = process.readAllStandardOutput();
        QString stderrOutput = process.readAllStandardError();

        int exitCode = process.exitCode();

        qDebug() << "Exit code:" << exitCode;
        qDebug() << "STDOUT:" << stdoutOutput;
        qDebug() << "STDERR:" << stderrOutput;

        if (exitCode == 0) {
            qDebug() << "Script ran successfully!";
        } else {
            qDebug() << "Script failed!";
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
