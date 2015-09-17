#include "albummodel.h"

AlbumModel::AlbumModel(QObject *parent):QAbstractListModel(parent)
{
    configPath = QDir::homePath() + "/.Player/config";
    databaseName = configPath + "/player.db";
    //queryString = QString("SELECT * FROM Album JOIN Artist USING (artistID) JOIN Genre USING(genreID) JOIN Cover USING(coverID) ORDER BY albumTitle;");
    queryString = QString("SELECT albumTitle,albumArtist as artistname,genre,cover as coverpath,COUNT(albumTitle) as tracks ,SUM(length) as duration , year, genre FROM BaseTableTracks GROUP BY albumTitle ORDER BY albumTitle;");
    connectionName  = "Logger";
    driver = QString("QSQLITE");

    QLocale::setDefault(QLocale(QLocale::AnyLanguage));
}
void AlbumModel::queryArtistAlbum(QString artist)
{
    QString tmp = QString("SELECT albumTitle,albumArtist as artistname,genre,cover as coverpath,COUNT(albumTitle) as tracks ,SUM(length) as duration , year, genre FROM BaseTableTracks GROUP BY albumTitle ORDER BY albumTitle;");
    queryString = QString("SELECT albumTitle,albumArtist as artistname,genre,cover as coverpath,COUNT(albumTitle) as tracks ,SUM(length) as duration , year, genre FROM BaseTableTracks  WHERE albumArtist = '%1' GROUP BY albumTitle ORDER BY albumTitle;").arg(artist.replace(QLatin1Char('\''), QLatin1String("''")));
    refresh();
    queryString = tmp;
}

void AlbumModel::refresh()
{
        QSqlDatabase db;
        db = QSqlDatabase::database(connectionName);
        Utils::Album album;
        resetInternalData();
        if(db.isOpen())
        {
            QSqlQuery albumQuery = QSqlQuery(db);
            albumQuery.exec(queryString);
            if(albumQuery.isActive())
            {
                while(albumQuery.next())
                {
                    //album.albumID = albumQuery.value("albumID").toInt();
                    //album.artistID = albumQuery.value("artistID").toInt();
                    //album.coverID = albumQuery.value("coverID").toInt();
                    //album.genreID = albumQuery.value("genreID").toInt();
                    album.duration = albumQuery.value("duration").toInt();
                    album.albumTitle = albumQuery.value("albumTitle").toString();
                    album.tracks = albumQuery.value("tracks").toInt();
                    album.year = albumQuery.value("year").toInt();
                    album.albumArtist = albumQuery.value("artistname").toString();
                    album.genre = albumQuery.value("genre").toString();
                    album.coverpath = albumQuery.value("coverpath").toString();
                    beginInsertRows(QModelIndex(), rowCount(), rowCount());
                    albumList.push_back(album);
                    endInsertRows();
                }

            }

            albumQuery.finish();
        }
}

void AlbumModel::debug()
{
    qDebug() << "AlbumModel::debug()";
    for(Utils::Album album : albumList)
    {
        qDebug() << " Title : " << album.albumTitle + " - Tracks : " << album.tracks;
    }

    qDebug()<<"AlbumModel::debug() - Number of Albums: " << rowCount();

}



QVariant AlbumModel::data(const QModelIndex &index, int role) const
{

    // Version 3.
        Utils::Album album = albumList.at(index.row());
        if(index.row() < 0 || index.row() >= rowCount())
            return QVariant();

        else
        {
            switch (role)
            {

            case AlbumTitleRole:
            {
                 return album.albumTitle;
            }
            case CoverpathRole:
                return album.coverpath;
            case ArtistRole:
                return album.albumArtist;
            case YearRole:
                return album.year;
            case IDRole:
               return album.albumID;
            case DurationRole:
               return album.duration;
            case TracksRole:
                return album.tracks;
            case GenreRole:
                return album.genre;
            default:
                return QVariant();
                break;
            }
        }
}

 QHash<int,QByteArray> AlbumModel::roleNames() const
{
       QHash<int, QByteArray> roles;
       roles[AlbumTitleRole] = "title";
       roles[CoverpathRole] = "coverpath";
       roles[ArtistRole]  = "artist";
       roles[YearRole]  = "year";
       roles[IDRole] = "albumID";
       roles[TracksRole] ="tracks";
       roles[DurationRole] = "duration";
       roles[GenreRole] = "genre";
       return roles;
}

 int AlbumModel::rowCount(const QModelIndex &parent) const
 {   
     return albumList.size();
 }


 QString AlbumModel::getQueryText()const
 {
     return queryString;
 }



 void AlbumModel::resetInternalData()
 {
     beginResetModel();
     albumList.clear();
     endResetModel();
     //Q_EMIT dataChanged();
 }
