#ifndef PLAYER_H
#define PLAYER_H

//#include <QMediaPlayer>
#include <QtMultimedia>
#include <memory>
#include "playlist.h"
#include "dbusmanager.h"

//struct PlayerStateBackup
//{
//    int index;
//    int position;
//    int volume;
//    QStringList list;
//    int  playBackMode;
//    bool ShuffleMode;

//};


class Player : public QMediaPlayer
{
    Q_OBJECT
    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString positionString READ positionString )
    Q_PROPERTY(QString durationString READ durationString )
    //Q_PROPERTY(QString media READ media WRITE setMedia NOTIFY mediaChanged)

    public:
        Player(QMediaPlayer *parent = 0);
    public Q_SLOTS:
        void notify(bool isReady);
        void setMedia(QString path);
        void setCover(QString path);
        QString cover()const;
        QString positionString()const;
        QString durationString()const;

Q_SIGNALS:
        void coverChanged();
        void dbusNextClicked();
        void dbusPreviousClicked();
        void dbusPlayClicked();


    private:
        DBusManager Busmanager;
        QString currentPath;
        QString _cover;
        int count;
        int  HOUR;
};

#endif // PLAYER_H
