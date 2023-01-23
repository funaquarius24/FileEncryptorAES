#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QFileDialog>
#include <QNetworkInterface>
#include <QFileInfo>
#include <QSslEllipticCurve>
#include <QSslConfiguration>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>

#include <qaesencryption.h>

// Delete
#include <QScreen>

QNetworkInterface intf;
int te = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager, &QNetworkAccessManager::finished, this, &MainWindow::onManagerFinished);


    setWindowTitle(tr("Aqua Font Encryptor"));


}

void MainWindow::on_browseButton_clicked()
{
    std::cout << "Hello" << std::endl;
//    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("TTF font Files (*.ttf *.otf)"));
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("text Files (*.*)"));

    if(fileNames.length() < 1){
        return;
    }

    ui->filePathLineEdit->setText(fileNames[0]);

    bool fileExists = QFileInfo::exists(fileNames[0]) && QFileInfo(fileNames[0]).isFile();

    if(!fileExists){
        ui->infoLabel->setText("Error: The file does not exist or is a folder.");
    }

}

void MainWindow::on_saveFolderBrowseButton_clicked()
{
    QString directory =
            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath()));

    if (directory.isEmpty()) {
        return;

    }
    else
    {
        QDir directoryoj(directory);
        if(!directoryoj.isEmpty())
        {
            ui->infoLabel->setText("Warning: Folder is not empty.");
        }

    }

    ui->saveFolderPathLineEdit->setText(directory);


}

void MainWindow::on_encryptButton_clicked()
{
    QString file_path = ui->filePathLineEdit->text();

    QString saveFolderPath = ui->saveFolderPathLineEdit->text();

    std::string filePathString = file_path.toStdString();
    std::string base_filename = filePathString.substr(filePathString.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    bool noDot = int(p) == -1;
    std::string file_without_extension;
    std::string file_extension;
    std::string file_path_no_extension;
    if(!noDot){
        file_without_extension = base_filename.substr(0, p);
        file_extension = base_filename.substr(p, base_filename.length());
        std::string::size_type const loc(filePathString.find_last_of('.'));
        file_path_no_extension = filePathString.substr(0, loc);
    }
    else{
        file_without_extension = base_filename;
        file_extension = "";
    }


    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CFB);

    QDir inputFileDir(file_path);
    QDir outputDir(saveFolderPath);

    QDir tedir(saveFolderPath);
    qDebug() << "Path";
    qDebug() << tedir.absoluteFilePath("fhddfi");

    QFile textFile(file_path);
    QByteArray input;
    if (textFile.open(QFile::ReadOnly)){
        input = textFile.readAll();
    }
    else
        ui->infoLabel->setText("File not found.");

    QString key("depend purity silly luggage trick ugly hunt track differ certain identify coconut misery danger seek pelican crouch ugly hole spatial roast youth unveil receive");
    QByteArray iv;

    quint8 iv_16[16]     = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    for (int i=0; i<16; i++)
       iv.append(iv_16[i]);

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);

    QByteArray encodedText = encryption.encode(input, hashKey, iv);

    QString outFileName = file_path_no_extension.c_str();
    outFileName += "enc";
    outFileName += file_extension.c_str();

    QFile encodedFile(outputDir.absoluteFilePath(inputFileDir.dirName()));
    if(encodedFile.open(QIODevice::WriteOnly)) {
        encodedFile.write(encodedText);
    }
    else
    {
        ui->infoLabel->setText("Failed to write file.");
    }
    encodedFile.close();

    QString hashres = getDataHash(encodedText);

    QString hashFileExtension = ".ini";
    QString base_filenameQ = QString::fromUtf8(file_without_extension.c_str());

    qDebug() << base_filenameQ;

    QString hashFilename = base_filenameQ;
    hashFilename += hashFileExtension;

    QFile hashFile(outputDir.absoluteFilePath(hashFilename));
    if(hashFile.open(QIODevice::WriteOnly))
    {
        hashFile.write(hashres.toUtf8());
        ui->infoLabel->setText("Font successfully encrypted.");
    }
    else
        ui->infoLabel->setText("Failed to write file.");


}

void MainWindow::onManagerFinished(QNetworkReply *reply)
{
    qDebug()<< reply->readAll();
}


MainWindow::~MainWindow()
{
    delete ui;
}

