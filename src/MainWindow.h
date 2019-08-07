#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>

class Player;
class QListWidgetItem;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	static std::string FormatMilli(unsigned int milli);

private:
    Ui::MainWindow *ui;
    Player *player;
    void openMusic();

private slots:
    void updatePosition(unsigned int position);
    void endOfPlayback();
	void beginPlayback(QListWidgetItem *item);

public slots:
	void reportError(std::string msg);
	void playbackStarted(unsigned int length);
};

#endif // MAINWINDOW_H