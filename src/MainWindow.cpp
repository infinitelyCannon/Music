#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TrackItem.h"
#include "Player.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QCoreApplication>
#include <cmath>
#include <string>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new Player(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openMusic);
   /*
	connect(player, &Player::curPos, this, &MainWindow::updatePosition);
    connect(player, &Player::endOfPlayback, this, &MainWindow::endOfPlayback);
    connect(ui->playButton, &QPushButton::clicked, [this] {
        QList<QListWidgetItem *> selected = ui->listWidget->selectedItems();
        ui->progressSlider->setMaximum(player->play(((TrackItem *)selected[0])->getPath()));
    });
    connect(ui->pauseButton, &QPushButton::clicked, player, &Player::pause);
    connect(ui->resumeButton, &QPushButton::clicked, player, &Player::resume);
    connect(ui->stopButton, &QPushButton::clicked, player, &Player::stop);*/
    connect(QApplication::instance(), &QApplication::aboutToQuit, player, &Player::shouldQuit);
    //connect(ui->volumeSlider, &QSlider::valueChanged, player, &Player::setVolume);

    player->start();
}

void MainWindow::updatePosition(double position, double total)
{
    QVariant *positionMin = new QVariant((int)position / 60);
    QVariant *positionSec = new QVariant((int)std::fmod(position, 60.0));
    QVariant *totalMin = new QVariant((int)total / 60);
    QVariant *totalSec = new QVariant((int)std::fmod(total, 60.0));

    ui->progressSlider->setValue(position);
    ui->progressText->setText(positionMin->toString() + ":" + positionSec->toString() + "/" + totalMin->toString() + ":" + totalSec->toString());
}

void MainWindow::endOfPlayback()
{
    ui->progressSlider->setValue(0);
    ui->progressText->setText("0:00/0:00");
}

void MainWindow::volumeChanged(int value)
{
    //player->setVolume(value);
}

void MainWindow::openMusic()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Load Music File",
                                                    QDir::homePath(),
                                                    "Music Files (*.mp3 *.ogg *.wav *.aiff *.multi)");
    if (!filename.isEmpty())
    {
        TrackItem *item = new TrackItem(ui->listWidget, filename);
        ui->listWidget->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    player->quit();
    player->deleteLater();
}