#ifndef PLAYER_H
#define PLAYER_H

//#include "AudioEngine.h"
#include <QtCore/QTimer>
#include <QtCore/QThread>

class Player : public QThread
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    ~Player();
    bool isPlaying = false;

protected:
    void run();

private:
    QTimer *t;
    bool willQuit = false;
    void *mAudio;

public slots:
    void shouldQuit();
    void setVolume(int value);

signals:
    void endOfPlayback();
    void curPos(double position, double total);

public:
    double play(QString filepath);
    void pause();
    void resume();
    void stop();
    void signalUpdate();
    void changePosition(int position);
};

#endif // PLAYER_H