#include "TrackItem.h"
#include <QtCore/QString>

TrackItem::TrackItem(QListWidget *parent, QString filePath) : QListWidgetItem(parent, QListWidgetItem::UserType),
                                                              mPath(filePath)
{
    mPath = filePath;
    mName = filePath.mid(filePath.lastIndexOf('/') + 1);
    mMulti = mName.lastIndexOf(".multi") != -1;
    setText(mName);
}

TrackInfo TrackItem::getInfo()
{
	TrackInfo info;

	info.name = mName.toStdString();
	info.path = mPath.toStdString();

	return info;
}

QString TrackItem::getPath()
{
    return mPath;
}

QString TrackItem::getName()
{
    return mName;
}

bool TrackItem::isMulti()
{
    return mMulti;
}

TrackItem::~TrackItem()
{
}