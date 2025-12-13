#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

//TODO: Improve GUI
//TODO: Build the project to AppImage
//TODO: Make the window non-reszable
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
        else{
            QMessageBox::critical(this, "Modpack file error!", "Please choose the modpack file!");
        }
    });

    connect(ui->outbtn, &QPushButton::clicked, this, [this](){
        QString outfileName = QFileDialog::getOpenFileName(this, "Select output file", QString(), "*.txt");
        if(!outfileName.isEmpty()){
            output = outfileName;
            ui->outPath->setText(outfileName);
        }
        else{
            QMessageBox::critical(this, "Output file error!", "Please choose the output file!");
        }
    });

    connect(ui->extractBtn, &QPushButton::clicked, this, [this](){
        QProcess process;
        QStringList args;
        args << "--input" << input << "--output" << output;
        process.start("./extract", args);

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

        //FIXME: When output/input or both files aren't provided the qDebug tells that everything went well.
        qDebug() << "Exit code:" << exitCode;
        qDebug() << "STDOUT:" << stdoutOutput;
        qDebug() << "STDERR:" << stderrOutput;

        if (exitCode == 0) {
            if(input.isEmpty() || output.isEmpty()){
                qDebug() << "Missing I/O files!";
                QMessageBox::critical(this, "I/O files error!", "The input and output files are requried!");
            }
            else{
                qDebug() << "Script ran successfully!";
                QMessageBox::information(this, "Success!", "Successfully exported mod ID's!");
            }
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
