#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TrackItem.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	player = new Player(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openMusic);
	connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::beginPlayback);
	connect(ui->volumeSlider, &QSlider::valueChanged, player, &Player::setVolume);
	connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playPause);
	connect(ui->nextButton, &QPushButton::clicked, player, &Player::skipNext);
	connect(ui->prevButton, &QPushButton::clicked, player, &Player::skipBack);
	connect(ui->progressSlider, &QAbstractSlider::sliderReleased, [this]()
	{
		int position = ui->progressSlider->value();
		player->changePosition(position);
	});
	
	connect(player, &Player::beginPlayback, this, &MainWindow::playbackStarted);
	connect(player, &Player::updateUI, this, &MainWindow::updatePosition);
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
    //connect(QApplication::instance(), &QApplication::aboutToQuit, player, &Player::shouldQuit);
    //connect(ui->volumeSlider, &QSlider::valueChanged, player, &Player::setVolume);

    //player->start();
}

void MainWindow::beginPlayback(QListWidgetItem *item)
{
	TrackItem *track = (TrackItem *) item;
	std::vector<TrackInfo> playlist;
	int i = 0;
	QListWidgetItem *temp = ui->listWidget->item(i);

	while(temp != nullptr)
	{
		TrackItem *tempTrack = ( TrackItem *) temp;
		TrackInfo info = tempTrack->getInfo();
		playlist.push_back(info);
		temp = ui->listWidget->item(++i);
	}

	QIcon icon(":/icons/pause.png");

	ui->trackLabel->setText(track->getName());
	ui->playButton->setIcon(icon);
	player->play(ui->listWidget->row(item), playlist);
}

void MainWindow::playPause(bool /*value*/)
{
	player->pause();

	if(player->isPaused())
	{
		QIcon icon(":/icons/play.png");
		ui->playButton->setIcon(icon);
	}
	else
	{
		QIcon icon(":/icons/pause.png");
		ui->playButton->setIcon(icon);
	}
}

void MainWindow::updatePosition(unsigned int position)
{
	std::string len = FormatMilli(position);
	QString str(len.c_str());

    ui->progressSlider->setValue(position);
    ui->progressText->setText(str);
}

void MainWindow::endOfPlayback()
{
    ui->progressSlider->setValue(0);
    ui->progressText->setText("0:00");
}

std::string MainWindow::FormatMilli(unsigned int milli)
{
	unsigned int secs, mins, hours, secVal;
	std::string result = "", hourStr, minStr, secStr;

	secVal = milli / 1000;
	hours = secVal / 3600;
	mins = (secVal - (hours * 3600)) / 60;
	secs = secVal - (hours * 3600) - (mins * 60);

	hourStr = (hours < 10 ? "0" : "") + std::to_string(hours);
	minStr = (mins < 10 && hourStr.compare("00") == 0 ? "0" : "") + std::to_string(mins);
	secStr = (secs < 10 ? "0" : "") + std::to_string(secs);

	result += (hourStr.compare("00") == 0 ? "" : hourStr + ":") + minStr + ":" + secStr;
	return result;
}

void MainWindow::openMusic()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                    "Load Music File",
                                                    QDir::homePath(),
                                                    "Music Files (*.mp3 *.ogg *.wav *.aiff *.multi *.flac)");
    if (!filenames.isEmpty())
    {
		for(int i = 0; i < filenames.size(); i++)
		{
			TrackItem *item = new TrackItem(ui->listWidget, filenames.at(i));
			ui->listWidget->addItem(item);
		}
    }
}

void MainWindow::playbackStarted(unsigned int length, QString name)
{
	std::string len = FormatMilli(length);
	QString str(len.c_str());
	
	ui->playButton->setEnabled(true);
	ui->prevButton->setEnabled(true);
	ui->nextButton->setEnabled(true);
	ui->progressSlider->setMaximum(length);
	ui->lengthText->setText(str);
	ui->trackLabel->setText(name);
}

void MainWindow::reportError(std::string msg)
{
	QString str(msg.c_str());
	qWarning() << str;
}

MainWindow::~MainWindow()
{
    delete ui;
}