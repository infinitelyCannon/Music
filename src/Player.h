#ifndef PLAYER_H
#define PLAYER_H

#include "Mx3.hpp"
#include <QtCore/QTimer>
#include <QtCore/QVector>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    ~Player();
    bool isPlaying = false;

private:
    QTimer *tick;
    Mx3 *mAudio;
	unsigned int mLength;
	SA::delegate<void(std::string)> errorDelegate;
	const static int PLAYER_UPDATE_RATE_MS = 20;
	QVector<QString> mQueue;
	unsigned int nowPlaying = 0;

public slots:
    void setVolume(int value);
	void update();
	void skipNext();
	void skipBack();

signals:
    void beginPlayback(unsigned int length);
	void updateUI(unsigned int position);
	void reportError(std::string msg);

public:
	void play(QString filepath, QVector<QString> queue);
    void pause();
    void stop();
    void changePosition(unsigned int position);
	void errorReceiver(std::string msg);
};

#endif // PLAYER_H