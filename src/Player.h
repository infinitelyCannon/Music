#ifndef PLAYER_H
#define PLAYER_H

#include "Mx3.hpp"
#include "TrackItem.h"

#include <vector>
#include <QtCore/QTimer>
#include <QtCore/QVector>

struct NowPlayingInfo
{
	QString name;
	unsigned int length;
};

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    ~Player();

private:
    QTimer *tick;
    Mx3 *mAudio;
	unsigned int mLength;
	SA::delegate<void(std::string)> errorDelegate;
	const static int PLAYER_UPDATE_RATE_MS = 20;
	std::vector<TrackInfo> mQueue;
	unsigned int nowPlaying = 0;
	NowPlayingInfo playingInfo;

public slots:
    void setVolume(int value);
	void update();
	void skipNext();
	void skipBack();

signals:
    void beginPlayback(unsigned int length, QString name);
	void updateUI(unsigned int position);
	void reportError(std::string msg);

public:
	void play(int index, std::vector<TrackInfo> queue);
    void pause();
	bool isPaused();
	bool isPlaying();
    void stop();
    void changePosition(unsigned int position);
	void errorReceiver(std::string msg);
};

#endif // PLAYER_H