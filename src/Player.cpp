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

void Player::play(int index, std::vector<TrackInfo> queue)
{
	mQueue = queue;
	QString name(mQueue[index].name.c_str());

	mAudio->play(mQueue[index].path);
	mLength = mAudio->getLength();
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
	if(mQueue.size() == 0)
		return;
	position = mAudio->getPosition();
	if(mQueue.size() == 1 || position <= 1500)
	{
		mAudio->changeTimePosition(0);
		return;
	}

	nowPlaying = (nowPlaying - 1) % mQueue.size();
	QString name(mQueue[nowPlaying].name.c_str());

	mAudio->stop();
	mAudio->play(mQueue[nowPlaying].path);
	mLength = mAudio->getLength();
	emit beginPlayback(mLength, name);
}

void Player::skipNext()
{
	if(mQueue.size() == 0)
		return;

	if(mQueue.size() == 1)
	{
		mAudio->changeTimePosition(0);
		return;
	}

	nowPlaying = (nowPlaying + 1) % mQueue.size();
	QString name(mQueue[nowPlaying].name.c_str());

	mAudio->stop();
	mAudio->play(mQueue[nowPlaying].path);
	mLength = mAudio->getLength();
	emit beginPlayback(mLength, name);
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