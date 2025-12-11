#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->mdpckbtn, &QPushButton::clicked, this, [this]() {
        QString modpckName = QFileDialog::getOpenFileName(this, "Select modpack file", QString(), "*.html");
        if (!modpckName.isEmpty()) {
            input = modpckName;
            ui->mdpckPath->setText(modpckName);
        }
    });

    connect(ui->outbtn, &QPushButton::clicked, this, [this](){
        QString outfileName = QFileDialog::getOpenFileName(this, "Select output file", QString(), "*.txt");
        if(!outfileName.isEmpty()){
            output = outfileName;
            ui->outPath->setText(outfileName);
        }
    });

    //TODO: Find a better way to handle the python script. 
    connect(ui->extractBtn, &QPushButton::clicked, this, [this](){
        QProcess process;
        QStringList args;
        args << "extract.py" << "--input" << input << "--output" << output;
        process.start("python3", args);

        QString stdoutOutput = process.readAllStandardOutput();
        QString stderrOutput = process.readAllStandardError();
        int exitCode = process.exitCode();

        if (!process.waitForFinished()) {
            qDebug() << "Process timed out or failed to start!";
            qDebug() << "Exit code:" << exitCode;
            qDebug() << "STDOUT:" << stdoutOutput;
            qDebug() << "STDERR:" << stderrOutput;
            return;
        }

        qDebug() << "Exit code:" << exitCode;
        qDebug() << "STDOUT:" << stdoutOutput;
        qDebug() << "STDERR:" << stderrOutput;

        if (exitCode == 0) {
            qDebug() << "Script ran successfully!";
            QMessageBox::information(this, "Success!", "Successfully exported mod ID's!");
        } else {
            qDebug() << "Script failed!";
            QMessageBox::critical(this, "Error!", stderrOutput);
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
