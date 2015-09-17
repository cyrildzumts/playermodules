#include "track.h"

Track::Track()
{

}

Track::Track(QString &title, QString &path, QString &artist,QString&alb,QString& gen, int ID, int pos, int len, int liked, int bitRate,int y, int count)
{
    t_title = title;
    t_path = path;
    t_artist = artist;
    t_album = alb;
    t_genre = gen;
    t_position = pos;
    t_duration = len;
    t_ID = ID;
    t_liked = liked;
    t_playCount = count;
    t_bitRate = bitRate;
    t_year = y;
}

Track::Track(const Track &track)
{
    t_title = track.title();
    t_artist = track.artist();
    t_path = track.path();
    t_album = track.album();
    t_genre = track.genre();
    t_position = track.position();
    t_duration = track.duration();
    t_ID = track.trackID();
    t_liked = track.liked();
    t_playCount = track.playCount();
    t_bitRate = track.bitrate();
    t_year = track.year();
}

Track::Track(const TrackTag &track)
{
    t_title = track.title;
    t_artist = track.artist;
    t_album = track.albumTitle;
    t_genre = track.genre;
    t_path = track.path;
    t_position = track.position;
    t_duration = track.length;
    t_ID = track.ID;
    t_liked = track.liked;
    t_playCount = track.playCount;
    t_bitRate = track.bitRate;
    t_year = track.year;
}

QString Track::album()const
{
    return t_album;
}

QString Track::genre()const
{
    return t_genre ;
}
QString Track::artist()const
{
    return t_artist;
}

QString Track::title()const
{
    return t_title;
}


QString Track::path()const
{
    return t_path;
}

int Track::trackID()const
{
    return t_ID;
}

int Track::position()const
{
    return t_position;
}

int Track::duration()const
{
    return t_duration;
}

QString Track::durationToString()const
{
    const int hour = 3600;
    QTime tmpTime (0,0);
    QTime t =  tmpTime.addSecs(t_duration);

    if(t_duration < hour)
        return t.toString("mm:ss");
    else if (t_duration >= hour)
        return t.toString("hh:mm:ss");

}
int Track::liked()const
{
    return t_liked;
}

int Track::playCount()const
{
    return t_playCount;
}

int Track::bitrate()const
{
    return t_bitRate;
}


int Track::year()const
{
    return t_year;
}
void Track::setDuration(int len)
{
    t_duration = len;
}

void Track::setPosition(int pos)
{
    t_position = pos;
}


void Track::setArtist(QString singer)
{
    t_artist = singer;
}

void Track::setPath(QString trackpath)
{
    t_path = trackpath;
}

void Track::setTitle(QString trackTitle)
{
    t_title = trackTitle;
}

void Track::setID(int ID)
{
    t_ID = ID;
}

void Track::setLiked(const int liked)
{
    t_liked = liked;
}

void Track::setPlayCount(const int count)
{
    t_playCount = count;
}


void Track::setBitrate(int kbps)
{
    t_bitRate = kbps;
}

void Track::setYear(int y)
{
    t_year = y;
}

void Track::setAlbum(QString alb)
{
    t_album = alb;
}

void Track::setGenre(QString gen)
{
    t_genre = gen;
}
