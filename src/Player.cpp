#include "Player.h"
#include <iostream>
#include <string>
#include <QtCore/QDebug>

Player::Player(QObject *parent) : QObject(parent)
{

    tick = new QTimer(this);
    connect(tick, &QTimer::timeout, this, &Player::update);

	mAudio = new Mx3(32, FMOD_INIT_NORMAL, 0);
	errorDelegate = SA::delegate<void(std::string)>::create<Player, &Player::reportError>(this);
	mAudio->BindErrorCallback(errorDelegate);
}

void Player::errorReceiver(std::string msg)
{
	emit reportError(msg);
}

bool Player::isPaused()
{
	return mAudio->isPaused();
}

bool Player::isPlaying()
{
	return mAudio->isPlaying();
}

void Player::play(QString filepath, QVector<QString> queue)
{
	mQueue = queue;
	for(int i = 0; i < mQueue.length(); i++)
	{
		if(mQueue[i].compare(filepath) == 0)
		{
			nowPlaying = i;
			break;
		}
	}

	QString name = filepath.mid(filepath.lastIndexOf('/') + 1);
	playingInfo.name = name;

	mAudio->play(filepath.toStdString());
	mLength = mAudio->getLength();
	playingInfo.length = mLength;
	tick->start(PLAYER_UPDATE_RATE_MS);
	emit beginPlayback(mLength, name);
}

void Player::pause()
{
	mAudio->pause();
}

void Player::skipBack()
{
	unsigned int position;
	if(mQueue.isEmpty())
		return;
	position = mAudio->getPosition();
	if(mQueue.length() == 1 || position <= 1500)
	{
		mAudio->changeTimePosition(0);
		return;
	}

	nowPlaying = (nowPlaying - 1) % mQueue.length();
	QString path = mQueue[nowPlaying];
	QString name = path.mid(path.lastIndexOf('/') + 1);
	playingInfo.name = name;

	mAudio->stop();
	mAudio->play(mQueue[nowPlaying].toStdString());
	mLength = mAudio->getLength();
	emit beginPlayback(mLength, playingInfo.name);
}

void Player::skipNext()
{
	if(mQueue.isEmpty())
		return;

	if(mQueue.length() == 1)
	{
		mAudio->changeTimePosition(0);
		return;
	}

	nowPlaying = (nowPlaying + 1) % mQueue.length();
	QString path = mQueue[nowPlaying];
	QString name = path.mid(path.lastIndexOf('/') + 1);
	playingInfo.name = name;

	mAudio->stop();
	mAudio->play(mQueue[nowPlaying].toStdString());
	mLength = mAudio->getLength();
	emit beginPlayback(mLength, playingInfo.name);
}

void Player::stop()
{
	mAudio->stop();
	tick->stop();
}

void Player::changePosition(unsigned int position)
{
	if(mAudio->isPlaying() || mAudio->isPaused())
		mAudio->changeTimePosition(position);
}

void Player::setVolume(int value)
{
	float vol = (float) value / 100.0f;
	if(mAudio->isPlaying() || mAudio->isPaused())
		mAudio->setGlobalVolume(vol);
}

void Player::update()
{
	emit updateUI(mAudio->getPosition());
}

Player::~Player()
{
    delete mAudio;
}