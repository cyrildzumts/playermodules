#include "album.h"


Album::Album()
{

}

Album::Album(const Album &album)
{
    _tracks = album.tracks();
    _title = album.title();
    _artist = album.artist();
    _id = album.id();
    _releasedYear = album.releasedYear();
    _cover = album.cover();
    fixAlbumArtist();
}

Album::Album(Utils::Queue<Utils::TrackTag> &tracklist)
{
    int i = 0;
    Utils::TrackTag track;
    while(!tracklist.empty())
    {
        tracklist.try_pop(track);
        if(i == 0)
        {
            _artist = track.artist;
            _cover = track.cover;
            _title = track.title;
            _releasedYear = track.year;
            i = -1;
        }
        _tracks.push(track);
    }
    fixAlbumArtist();
}

Album& Album::operator=(const Album& album)
{

    _tracks = album.tracks();
    _title = album.title();
    _artist = album.artist();
    _id = album.id();
    _releasedYear = album.releasedYear();
    _cover = album.cover();
    fixAlbumArtist();
    return *this;
}

Album::~Album()
{
    clear();
}

 Utils::Queue<Utils::TrackTag> Album::tracks()const
 {
     return Utils::Queue<Utils::TrackTag>(_tracks);
 }
int Album::id()const
{
    return _id;
}

QString Album::cover()const
{
    return _cover;
}
QString Album::genre()const
{
    return _genre;
}

QString Album::artist()const
{
    return _artist;
}

QString Album::title()const
{
    return _title;
}

int Album::count()const
{
    return _tracks.size();
}

int Album::releasedYear()const
{
    return _releasedYear;
}

bool Album::empty()const
{
    return _tracks.empty();
}

void Album::pushTrack(Utils::TrackTag track)
{
    _tracks.push(track);
}

Utils::TrackTag Album::popTrack()
{
    Utils::TrackTag track;
    _tracks.try_pop(track);
    return track;
}


void Album::setArtist(const QString &albumArtist)
{
    _artist = QString(albumArtist);
}

void Album::setTitle(const QString &albumTitle)
{
    _title = QString(albumTitle);
}

void Album::setId(int ID)
{
    _id = ID;
}

void Album::setReleasedYear(int year)
{
    _releasedYear = year;
}

void Album::setCover(const QString &c)
{
    _cover = QString(c);
}

void Album::setGenre(QString g)
{
    _genre = g;
}

void Album::fixAlbumArtist()
{

  Utils::TrackTag track;
  QString previousArtist = QString("");
  QString currentArtist;
  Utils::Queue<Utils::TrackTag> tmpList(_tracks);
  int i = 0;

  while(!tmpList.empty())
  {
    tmpList.try_pop(track);
    currentArtist = track.artist;
    if(currentArtist != previousArtist)
    {
        if(i == 0)
        {
            i = -1;
            previousArtist = currentArtist;
            _genre = track.genre;
            _title = track.albumTitle;
            _releasedYear = track.year;

        }
        else
        {
            _artist = QString("VARIOUS ARTISTS");
            return;
        }
    }
    else
    {
        if(i == 0)
        {
            previousArtist = currentArtist;
            _genre = track.genre;
            _title = track.albumTitle;
            _releasedYear = track.year;
            i = -1;
        }

        else
        {
            if(currentArtist == QString(""))
            {
                _artist = QString("UNKNOWN ARTISTS");
                return;
            }
            if (currentArtist.count() >30) // Some people take the pleasure to set AlbumArtist to all the available
                // present in compilation , in which case endup with a name made up to 30+ chars

            {
                _artist = QString("VARIOUS ARTISTS");
                return;
            }
            _artist = currentArtist;
            return;
        }
    }

 }

  tmpList.clear();
}


void Album::addToDatabase()
{
    /** To fill the database , I have to write the tags in a fixed order:
     *  First :
     * Genre,Cover and Artist Tables should be added first, in any order between them.
     * That is because Album has a foreign keys referencing those tables:
     * coverID, genreID and artistID.
     *
     * Second :
     * Album
     *
     * Third :
     * Track
     */

    QString connectionName = QString("AlbumAdd");
    QString databasePath = QString("/home/cyrildz/MyPlayer/TestDB/logger.db");
    if(_tracks.empty())
    {
        qDebug()<< "Database::addToDatabase(Utils::Queue<TrackTag> &tracks ) :  tracks is empty ";

    }
    else
    {
        QSqlDatabase loggerDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        //QSqlDatabase loggerDB = QSqlDatabase::database(connectionName);
        loggerDB.setDatabaseName(databasePath);
        if(loggerDB.open())
        {
            qDebug() << "Database successfuly opened" ;
            qDebug() << "There are in total : " << _tracks.size() << " file Tracks to add.";
            QSqlQuery loggerQuery = QSqlQuery(loggerDB);
            loggerQuery.exec("BEGIN TRANSACTION");
            loggerQuery.exec("PRAGMA SYNCHRONOUS = OFF");
            loggerQuery.exec("PRAGMA journal_mode = MEMORY");
            addAlbumToDatabase(loggerQuery);
            addTracksToDatabase(loggerQuery);
            loggerQuery.exec("END TRANSACTION");
            loggerQuery.finish();
            loggerDB.removeDatabase(connectionName);
            loggerDB.close();

        }

        else
        {
            qDebug() << "Error : Couldn't open the database with connectionName : " << connectionName;
        }

    }
}

void Album::addTracksToDatabase(QSqlQuery &query)
{
    QFile queryError ("/home/cyrildz/MyPlayer/TestDB/queryError.txt");
    if (!queryError.open(QIODevice::Append | QIODevice::Text))
            return;
    QTextStream outQuery(&queryError);
    //outQuery<< "Adding Track : " + track.title  + " - " + track.albumTitle + " - " +track.artist +  "\n";
    outQuery.setFieldAlignment(QTextStream::AlignLeft);
    while(!_tracks.empty())
    {
        Utils::TrackTag track;
        _tracks.try_pop(track);
        if(!query.exec(QString("INSERT OR IGNORE INTO Track(albumID,artistID, title,path,number,length,bitrate) SELECT Album.albumID, Artist.artistID,'%1','%2','%3','%4','%5' FROM Album,Artist WHERE Album.title='%6' AND Artist.artistName='%7';")
                    .arg(track.title.replace(QLatin1Char('\''), QLatin1String("''")),track.path.replace(QLatin1Char('\''), QLatin1String("''")),QString::number(track.position),QString::number(track.length), QString::number(track.bitRate),_title.replace(QLatin1Char('\''), QLatin1String("''")),_artist.replace(QLatin1Char('\''), QLatin1String("''")))))

        {

            outQuery<< "Error : couldn't add the track  : " << track.title  + " - " + track.artist + " - " + _title << "\n";
           return;
        }
    }

}

void Album::addAlbumToDatabase(QSqlQuery &query)
{
    auto tmpfile = QFileInfo(_cover);
    QFileInfo file = tmpfile.absolutePath();
    QFile queryError ("/home/cyrildz/MyPlayer/TestDB/queryError.txt");
    if (!queryError.open(QIODevice::Append | QIODevice::Text))
            return;
    QTextStream outQuery(&queryError);
    //outQuery<< "Adding Track : " + track.title  + " - " + track.albumTitle + " - " +track.artist +  "\n";
    outQuery.setFieldAlignment(QTextStream::AlignLeft);
    auto modifiedDate = file.created().toTime_t();
    auto addeDate = QDateTime::currentDateTime().toTime_t();
    if(!query.exec(QString("INSERT OR IGNORE INTO Genre (genre)"
                            "VALUES('%1');").arg(_genre.replace(QLatin1Char('\''), QLatin1String("''")))))
    {
        outQuery<< "Error : couldn't add the genre : " << _genre << "\n";
        return;
    }

    if(!query.exec(QString("INSERT OR IGNORE INTO Artist (artistName)"
                            "VALUES('%1');").arg(_artist.replace(QLatin1Char('\''), QLatin1String("''")))))
    {

        outQuery<< "Error : couldn't add the artist : " << _artist << "\n";
        return;
    }

    if(!query.exec(QString("INSERT OR IGNORE INTO Cover (path)"
                            "VALUES('%1');").arg(_cover.replace(QLatin1Char('\''), QLatin1String("''")))))
    {

        outQuery<< "Error : couldn't add the cover : " << _cover << "\n";
        return;
    }

    if(!query.exec(QString("INSERT OR IGNORE INTO Album(genreID,artistID, coverID, title,addedDate, modifiedDate,year) SELECT Genre.genreID, Artist.artistID,Cover.coverID,'%1','%2','%3','%4' FROM Genre,Artist,Cover WHERE Genre.genre='%5' AND Artist.artistName='%6' AND Cover.path='%7';")
                .arg(_title.replace(QLatin1Char('\''), QLatin1String("''")),QString::number(addeDate), QString::number(modifiedDate),QString::number(_releasedYear),_genre.replace(QLatin1Char('\''), QLatin1String("''")),_artist.replace(QLatin1Char('\''), QLatin1String("''")),_cover.replace(QLatin1Char('\''), QLatin1String("''")))))
    {

        outQuery<< "Error : couldn't add the album : " << _title << "\n";
       return;
    }
    //_id = query.lastInsertId().toInt();

}
