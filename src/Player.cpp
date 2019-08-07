#include "Player.h"
#include <iostream>
#include <string>

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

	mAudio->play(filepath.toStdString());
	mLength = mAudio->getLength();
	tick->start(PLAYER_UPDATE_RATE_MS);
	emit beginPlayback(mLength);
}

void Player::pause()
{
	mAudio->pause();

	if(mAudio->isPaused())
		tick->stop();
	else
		tick->start(PLAYER_UPDATE_RATE_MS);
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
	mAudio->stop();
	mAudio->play(mQueue[nowPlaying].toStdString());
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