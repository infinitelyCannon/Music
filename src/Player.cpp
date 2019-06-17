#include "Player.h"
#include <iostream>
#include <string>

Player::Player(QObject *parent) : QThread(parent)
{
    mAudio = new AudioEngine();

    t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &Player::signalUpdate);
}

double Player::play(QString filepath)
{
    if (mAudio->isPlaying())
    {
        mAudio->stop();
    }

    mAudio->play(filepath.toStdString());
    t->start(100);
    return mAudio->getLength();
    /*
    if (!(channel = BASS_StreamCreateFile(FALSE, filepath.toStdString().c_str(), 0, 0, BASS_SAMPLE_LOOP)))
    {
        std::cout << "Cannot play file" << std::endl;
        ErrorCheck();
        return 0.0;
    }
    else
    {
        DWORD len = BASS_ChannelGetLength(channel, BASS_POS_BYTE);
        length = BASS_ChannelBytes2Seconds(channel, len);
        BASS_ChannelPlay(channel, TRUE);
        ErrorCheck();
        t->start(100);
        isPlaying = true;
        return length;
    }
    */
}

void Player::pause()
{
    mAudio->pause();
    /* BASS_ChannelPause(channel);
    ErrorCheck();
    t->stop();
    isPlaying = false; */
}

void Player::resume()
{
    mAudio->resume();
    /* if (!BASS_ChannelPlay(channel, FALSE))
    {
        std::cout << "Error resuming" << std::endl;
        ErrorCheck();
    }
    else
    {
        t->start(98);
        isPlaying = true;
    } */
}

void Player::stop()
{
    mAudio->stop();
    /* BASS_ChannelStop(channel);
    ErrorCheck();
    t->stop();
    isPlaying = false; */
}

void Player::signalUpdate()
{
    emit curPos(mAudio->getPosition(), mAudio->getLength());
    /* double position = BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetPosition(channel, BASS_POS_BYTE));
    ErrorCheck();

    if (position < length)
    {
        emit curPos(position, length);
    }
    else
    {
        emit endOfPlayback();
        isPlaying = false;
    } */
}

void Player::changePosition(int position)
{
    mAudio->changeTimePosition(position);
    //BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, position), BASS_POS_BYTE);
}

void Player::setVolume(int value)
{
    mAudio->setGlobalVolume((float)value / 100.0f);
    /* BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, value / 100.0f);
    ErrorCheck(); */
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
    delete mAudio;
}