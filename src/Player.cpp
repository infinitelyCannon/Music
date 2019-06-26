#include "Player.h"
#include <iostream>
#include <string>

Player::Player(QObject *parent) : QThread(parent)
{

    t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &Player::signalUpdate);
}

double Player::play(QString filepath)
{
	return 0.0;
}

void Player::pause()
{
    
}

void Player::resume()
{
    
}

void Player::stop()
{
    
}

void Player::signalUpdate()
{
    
}

void Player::changePosition(int position)
{
    
}

void Player::setVolume(int value)
{
    
}

void Player::shouldQuit()
{
    willQuit = true;
}

void Player::run()
{
    //exec();
    while (!willQuit)
        ;
}

Player::~Player()
{
    //delete mAudio;
}