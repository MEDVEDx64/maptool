#include "mainwindow.h"
#include "replayfile.h"

#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("WAgame Map Patcher");
    setMinimumSize(700, 80);
    setMaximumSize(minimumSize());

    QWidget *wgt = new QWidget(this);
    setCentralWidget(wgt);

    QGridLayout *grid = new QGridLayout(wgt);

    filePathWgt = new QLineEdit(wgt);

    filePickBtn = new QPushButton();
    uploadBtn = new QPushButton();
    downloadBtn = new QPushButton();

    filePickBtn->setText("...");
    uploadBtn->setText("Upload");
    downloadBtn->setText("Download");

    uploadBtn->setEnabled(false);
    downloadBtn->setEnabled(false);

    QLabel *label = new QLabel("For TASing purposes. MEDVEDx64, 2018", this);
    QFont font = label->font();
    font.setPointSize(8);
    label->setFont(font);

    grid->addWidget(filePathWgt, 0, 0, 1, 5);
    grid->addWidget(filePickBtn, 0, 5);
    grid->addWidget(uploadBtn, 1, 2, 1, 2);
    grid->addWidget(downloadBtn, 1, 4, 1, 2);
    grid->addWidget(label, 1, 0, 1, 2);

    wgt->setLayout(grid);

    connect(filePickBtn, &QPushButton::clicked, this, &MainWindow::filePick);
    connect(uploadBtn, &QPushButton::clicked, this, &MainWindow::upload);
    connect(downloadBtn, &QPushButton::clicked, this, &MainWindow::download);

    connect(filePathWgt, &QLineEdit::textChanged, this, &MainWindow::handleFilePathChanged);
}

MainWindow::~MainWindow()
{
}

void MainWindow::filePick()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setNameFilter("W:A replay files (*.WAgame)");
    if(dialog->exec())
    {
        QStringList files = dialog->selectedFiles();
        for(int i = 0; i < files.count(); i++)
        {
            filePathWgt->setText(files[i]);
            break;
        }
    }

    delete dialog;
}

void MainWindow::upload()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setNameFilter("Map files (*.png *.bit *.lev)");
    if(dialog->exec())
    {
        QStringList files = dialog->selectedFiles();
        for(int i = 0; i < files.count(); i++)
        {
            ReplayFile rf(filePathWgt->text());
            if(!rf.upload(files[i]))
            {
                QMessageBox box;
                box.setText("Failed to open replay nor map file");
                box.setIcon(QMessageBox::Critical);
                box.setStandardButtons(QMessageBox::Ok);
                box.exec();
            }

            break;
        }
    }

    delete dialog;
}

void MainWindow::download()
{
}

void MainWindow::handleFilePathChanged(const QString &text)
{
    bool value = text.length() > 0;

    uploadBtn->setEnabled(value);
    downloadBtn->setEnabled(value);
}
