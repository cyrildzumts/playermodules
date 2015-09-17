#include "album.h"


Album::Album(QString &title, QString &coverpath, QString &artist, int year, int ID, QQuickItem *parent):QQuickItem(parent)


{
    a_artist =artist;
    a_title=title;
    a_cover=coverpath;
    a_year=year;
    a_ID = ID;
}

Album::Album (const Album& alb, QQuickItem *parent):QQuickItem(parent)
{
    a_artist = alb.artist();
    a_title = alb.title();
    a_cover = alb.cover();
    a_year = alb.year();
    a_ID = alb.albumID();
}

QString Album::artist()const
{
    return QString::fromLatin1(a_artist.toStdString().c_str());
}

QString Album::title()const
{
    return QString::fromLatin1(a_title.toStdString().c_str());
}

QString Album::cover()const
{
    return a_cover;
}


int Album::year()const
{
    return a_year;

}

int Album::albumID() const
{
    return a_ID;
}

