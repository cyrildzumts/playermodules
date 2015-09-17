#ifndef ALBUM_H
#define ALBUM_H
#include "utils.h"
#include "queue.h"
#include <QtSql>

class Album
{
public:
    Album();
    ~Album();
    Album(const Album &album);
   // Album(Album&& album);
    Album& operator=(const Album& album);
    //Album& operator =(Album&& album);
    Album(Utils::Queue<Utils::TrackTag> &tracklist);

    QString title()const;
    int releasedYear()const;
    QString artist()const;
    Utils::Queue<Utils::TrackTag> tracks()const;
    Utils::TrackTag popTrack();
    void pushTrack(Utils::TrackTag track);
    bool empty()const;
    int count()const;
    int id()const;
    QString genre()const;
    QString cover()const;
    void clear()
    {
        _title.clear();
        _artist.clear();
        _genre.clear();
        _id = -1;
        _releasedYear = 0;
        _tracks.clear();
    }

    void fixAlbumArtist();
    void setTitle(const QString &albumTitle);
    void setArtist(const QString &albumArtist);
    void setReleasedYear(int year);
    void setId(int ID);
    void setGenre(const QString g);
    void setCover(const QString &cover);
    void addToDatabase();


private:
    void addAlbumToDatabase(QSqlQuery &query);
    void addTracksToDatabase(QSqlQuery &query);

protected:

    Utils::Queue<Utils::TrackTag> _tracks;
    QString _title;
    QString _artist;
    QString _genre;
    QString _cover;

    int _releasedYear;
    int _id;

};

#endif // ALBUM_H
