#include "player.h"
#include "tagreader.h"

Player::Player(QMediaPlayer *parent):QMediaPlayer(parent)
{
       HOUR = 3600000; // Hour in Milliseconds
      connect(this,&Player::metaDataAvailableChanged,this, &Player::notify);
      connect(&Busmanager,SIGNAL(nextClicked()),this,SIGNAL(dbusNextClicked()));
      connect(&Busmanager,SIGNAL(previousClicked()),this,SIGNAL(dbusPreviousClicked()));
      connect(&Busmanager,SIGNAL(playClicked()),this,SIGNAL(dbusPlayClicked()));
}


void Player::notify(bool isReady)
{
        if( isReady)
        {
            QString body,summary, icon;
            summary = metaData(QString("Title")).toString();
            body = QString("From ") + metaData(QString("AlbumTitle")).toString()  + " , " + QString("By ")
                    +  metaData(QString("AlbumArtist")).toString();
            icon = _cover ;
            Busmanager.setNotification(summary,body,icon);

            qDebug() << " Notify():: " << summary + " - " + body + " - " + icon;
        }


}


void Player::setMedia(QString path)
{
    if(!path.isEmpty())
    {
        bool wasPlaying = state() == QMediaPlayer::PlayingState ? true : false ;
        QMediaPlayer::setMedia(QUrl::fromLocalFile(path));
        if(wasPlaying)
            play();
    }
}

void Player::setCover(QString path)
{
  _cover = path;
  Q_EMIT coverChanged();
}

QString Player::cover()const
{
  return _cover;
}

QString Player::positionString()const
{
        int d = position();
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addMSecs(d);
        if(d < HOUR)
        {
           return  t.toString("mm:ss");
        }
        else
        {
            return t.toString("hh:mm:ss");
        }
}

QString Player::durationString()const
{
        int d = duration();
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addMSecs(d);
        if(d < HOUR)
        {
            return  t.toString("mm:ss");
        }
        else
        {
            return t.toString("hh:mm:ss");
        }
}
