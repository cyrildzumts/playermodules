#include "tracklistmodel.h"
//#include "track.h"
TrackListModel::TrackListModel(QObject *parent):QAbstractListModel(parent)
{

    _sourceID = 0;
    _albumSource = "";
    queryCmd = QString("SELECT * FROM BaseTableTracks WHERE  albumTitle = (SELECT albumTitle FROM Album WHERE albumID = %1) ORDER BY trackNumber;").arg(_sourceID);
    databaseName = QDir::homePath() + "/.Player/config/player.db";
    connectionName = QString("Logger");
    driver = QString("QSQLITE");
    connect(this,SIGNAL(sourceAlbumChanged(QString)),this,SLOT(refresh(QString)));
    QLocale::setDefault(QLocale(QLocale::AnyLanguage));

}

TrackListModel::~TrackListModel()
{
  //qDebug()<< "TrackListModel Destructor: This model is out of Scope" ;
  _tracklist.clear();
}

void TrackListModel::refresh(QString album)
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    Utils::TrackTag track;
    _tracklist.clear();

    if(db.isOpen())
    {
        QSqlQuery albumQuery = QSqlQuery(db);
        queryCmd = QString("SELECT * FROM BaseTableTracks WHERE  albumTitle = '%1' ORDER BY trackNumber;").arg(album.replace(QLatin1Char('\''), QLatin1String("''")));
        if(albumQuery.exec(queryCmd))
        {
            while(albumQuery.next())
            {
                //qDebug()<< "TrackListModel::refresh() query has next ";
                track.title = albumQuery.value("title").toString();
                track.albumTitle = albumQuery.value("albumTitle").toString();
                track.ID = albumQuery.value("trackID").toInt();
                track.cover = albumQuery.value("cover").toString();
                track.length = albumQuery.value("length").toInt();
                track.genre = albumQuery.value("genre").toString();
                track.year = albumQuery.value("year").toInt();
                track.artist = albumQuery.value("artist").toString();
                track.bitRate = albumQuery.value("bitrate").toInt();
                track.path = albumQuery.value("trackUrl").toString();
                track.position = albumQuery.value("trackNumber").toInt();
                track.albumArtist = albumQuery.value("albumArtist").toString();
                track.addedDate = albumQuery.value("addedDate").toInt();
                track.modifiedDate = albumQuery.value("modifiedDate").toInt();
                track.liked = albumQuery.value("favorite").toInt();
                track.playCount = albumQuery.value("playCount").toInt();
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                _tracklist.push_back(track);
                endInsertRows();
            }
        }
        else
        {
            qDebug() << "DEBUG TracklistModel::refresh(Qstring album): debugQuery is not active";
        }
        albumQuery.finish();
        albumQuery.clear();
    }
    //Q_EMIT dataChanged();
}

void TrackListModel::refresh()
{

        QSqlDatabase db = QSqlDatabase::database(connectionName);
        Utils::TrackTag track;

        _tracklist.clear();

        if(db.isOpen())
        {
            QSqlQuery albumQuery = QSqlQuery(db);
            queryCmd = QString("SELECT * FROM BaseTableTracks WHERE playCount > 0 ORDER BY playCount DESC  LIMIT 10;");
            albumQuery.exec(queryCmd);
            if(albumQuery.isActive())
            {

                while(albumQuery.next())
                {
                    track.title = albumQuery.value("title").toString();
                    track.albumTitle = albumQuery.value("albumTitle").toString();
                    track.ID = albumQuery.value("trackID").toInt();
                    track.cover = albumQuery.value("cover").toString();
                    track.length = albumQuery.value("length").toInt();
                    track.genre = albumQuery.value("genre").toString();
                    track.year = albumQuery.value("year").toInt();
                    track.artist = albumQuery.value("artist").toString();
                    track.bitRate = albumQuery.value("bitrate").toInt();
                    track.path = albumQuery.value("trackUrl").toString();
                    track.position = albumQuery.value("trackNumber").toInt();
                    track.albumArtist = albumQuery.value("albumArtist").toString();
                    track.addedDate = albumQuery.value("addedDate").toInt();
                    track.modifiedDate = albumQuery.value("modifiedDate").toInt();
                    track.liked = albumQuery.value("favorite").toInt();
                    track.playCount = albumQuery.value("playCount").toInt();
                    beginInsertRows(QModelIndex(), rowCount(), rowCount());
                    _tracklist.push_back(track);
                    endInsertRows();
                }
                //Q_EMIT dataChanged();

            }
            else
            {
                qDebug() << "DEBUG TracklistModel::refresh():  debugQuery is not active";
            }
            albumQuery.finish();
            albumQuery.clear();
        }
}

void TrackListModel::queryAllTracks()
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    Utils::TrackTag track;

    _tracklist.clear();

    if(db.isOpen())
    {
        QSqlQuery albumQuery = QSqlQuery(db);
        albumQuery.exec(QString("SELECT * FROM BaseTableTracks  ORDER BY title;"));
        if(albumQuery.isActive())
        {

            while(albumQuery.next())
            {
                track.title = albumQuery.value("title").toString();
                track.albumTitle = albumQuery.value("albumTitle").toString();
                track.ID = albumQuery.value("trackID").toInt();
                track.cover = albumQuery.value("cover").toString();
                track.length = albumQuery.value("length").toInt();
                track.genre = albumQuery.value("genre").toString();
                track.year = albumQuery.value("year").toInt();
                track.artist = albumQuery.value("artist").toString();
                track.bitRate = albumQuery.value("bitrate").toInt();
                track.path = albumQuery.value("trackUrl").toString();
                track.position = albumQuery.value("trackNumber").toInt();
                track.albumArtist = albumQuery.value("albumArtist").toString();
                track.addedDate = albumQuery.value("addedDate").toInt();
                track.modifiedDate = albumQuery.value("modifiedDate").toInt();
                track.liked = albumQuery.value("favorite").toInt();
                track.playCount = albumQuery.value("playCount").toInt();
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                _tracklist.push_back(track);
                endInsertRows();
            }
            //Q_EMIT dataChanged();

        }
        else
        {
            qDebug() << "DEBUG TracklistModel::refresh():  debugQuery is not active";
        }
        albumQuery.finish();
        albumQuery.clear();
    }
}

void TrackListModel::refresh(int id)
{

}

void TrackListModel::setQuery(QString query)
{
    queryCmd = query;
    refresh();
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
            return QVariant();
    Utils::TrackTag track = _tracklist.at(index.row());

        switch (role)
        {

        case TitleRole:
            return track.title;

        case PathRole:
            return track.path;
        case ArtistRole:
            return track.artist;
        case PositionRole:
            return track.position;
        case LengthRole:
            return track.length;
        case LengthStringRole:
        {
            int duration = track.length;
            const int hour = 3600;
            QTime tmpTime (0,0);
            QTime t =  tmpTime.addSecs(duration);

            if(duration < hour)
                return t.toString("mm:ss");
            else
                return t.toString("hh:mm:ss");
        }
        case IDRole:
            return track.ID;
        case BitrateRole:
            return track.bitRate;
        case AlbumRole:
            return track.albumTitle;
        case YearRole:
            return track.year;
        case FavoriteRole:
            return track.liked;
        case AddedDateRole:
            return track.addedDate;
        case PlaycountRole:
            return track.playCount;
        case CoverRole:
            return track.cover;
        case AlbumArtistRole:
            return track.albumArtist;
        case GenreRole:
            return track.genre;
        default:
            return QVariant();
            break;
        }

}



int TrackListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _tracklist.size();
}

 QHash<int, QByteArray> TrackListModel::roleNames()const
 {
     QHash<int, QByteArray> roles;
     roles[TitleRole] = "title";
     roles[PathRole]  = "trackUrl" ;
     roles[AlbumArtistRole] = "albumArtist";
     roles[ ArtistRole] = "artist";
     roles[PositionRole] ="trackNumber";
     roles[LengthRole] = "length";
     roles [LengthStringRole] = "lengthStr";
     roles[IDRole] = "trackID";
     roles[AlbumRole] = "albumTitle";
     roles[PlaycountRole] = "playcount";
     roles[BitrateRole] = "bitrate";
     roles[YearRole] = "year";
     roles[FavoriteRole] = "favorite";
     roles[CoverRole] = "cover";
     roles[GenreRole] = "genre";

     return roles;
 }



void TrackListModel::setSourceID(int ID)
{
    _sourceID = ID;
    Q_EMIT sourceIDChanged(_sourceID);
}

void TrackListModel::setSourceAlbum(QString albumSource)
{
    _albumSource = albumSource;
    Q_EMIT sourceAlbumChanged(_albumSource);
}

QString TrackListModel::sourceAlbum()
{
    return _albumSource;
}

int  TrackListModel::sourceID()
{
    return _sourceID;
}

void TrackListModel::resetInternalData()
{
    beginResetModel();
    _sourceID = 0;
    _tracklist.clear();
    endResetModel();
    //Q_EMIT dataChanged();
}

std::vector<Utils::TrackTag> TrackListModel::tracklist()const
{
    return _tracklist;
}
