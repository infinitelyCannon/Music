#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class Player;

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

private:
    Ui::MainWindow *ui;
    Player *player;
    void openMusic();

private slots:
    void updatePosition(double position, double total);
    void endOfPlayback();
    void volumeChanged(int value);
};

#endif // MAINWINDOW_H