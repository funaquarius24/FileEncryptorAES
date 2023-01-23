#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include <QLineEdit>
#include <QLabel>
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLineEdit *filePathLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *licenseCodeLineEdit;
    QLineEdit *macLineEdit;

    QLabel *infoLabel;


    QNetworkAccessManager manager;

private slots:
    void on_browseButton_clicked();
    void on_saveFolderBrowseButton_clicked();
//    void on_submitInstallButton_clicked();
    void on_encryptButton_clicked();

    void onManagerFinished(QNetworkReply *reply);
};
#endif // MAINWINDOW_H
