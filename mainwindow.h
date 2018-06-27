#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLineEdit *filePathWgt;

    QPushButton *filePickBtn;
    QPushButton *uploadBtn;
    QPushButton *downloadBtn;

private slots:
    void filePick();
    void upload();
    void download();

    void handleFilePathChanged(const QString &text);
};

#endif // MAINWINDOW_H
