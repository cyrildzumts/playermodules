#ifndef TRACK_H
#define TRACK_H
#include <QString>
#include "utils.h"
#include <QQuickItem>

using namespace Utils; // to access TrackTag
class Track
{

public:
    Track (QString &title,QString &path,QString &artist,QString&alb,QString& gen,int ID, int pos, int len, int liked,  int bitRate,int y,int count = 0);
    Track (const Track &track);
    Track (const Utils::TrackTag &track);
    Track();


    QString artist()const;
    QString title()const;
    QString path()const;
    QString genre()const;
    QString album()const;
    int trackID()const;
    int position()const;
    int duration()const;
    int liked()const;
    int playCount()const;
    int bitrate()const;
    int year()const;
    QString durationToString()const;


    void setArtist(QString singer);
    void setAlbum(QString alb);
    void setGenre(QString gen);
    void setTitle(QString trackTitle);
    void setPath(QString trackpath);
    void setPosition(int pos);
    void setDuration(int len);
    void setID(int ID);
    void setLiked(const int liked);
    void setPlayCount(const int count);
    void setBitrate(int kbps);
    void setYear(int y);

private:
    QString t_title;
    QString t_artist;
    QString t_album;
    QString t_genre;
    QString t_path;
    int t_ID;
    int t_position;
    int t_duration;
    int t_liked; // 0 = unliked, 1 = liked
               // I'm using an int cause sqlite doesn't implement a boolean type.
    int t_playCount;
    int t_bitRate;
    int t_year;

};

#endif // TRACK_H
