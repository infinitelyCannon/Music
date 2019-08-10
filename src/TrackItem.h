#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <QtWidgets/QListWidgetItem>

struct TrackInfo
{
	std::string name;
	std::string path;
};

class TrackItem : public QListWidgetItem
{
public:
    explicit TrackItem(QListWidget *parent = nullptr, QString filePath = "");
    virtual ~TrackItem();
    QString getPath();
    QString getName();
    bool isMulti();
	TrackInfo getInfo();

private:
    QString mPath;
    QString mName;
    bool mMulti;
};

#endif // TRACKITEM_H