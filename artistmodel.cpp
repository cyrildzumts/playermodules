#include "artistmodel.h"

ArtistModel::ArtistModel(QObject *parent) : QAbstractListModel(parent)
{

    configPath = QDir::homePath() + "/.Player/config";
    databaseName = configPath + "/player.db";
    connectionName  = "Logger";
    //queryCmd = QString("SELECT * FROM Artist ORDER BY artistName  COLLATE NOCASE;");
    queryCmd = QString("SELECT albumArtist as artistname,cover FROM BaseTableTracks GROUP BY albumArtist ORDER BY albumArtist;");
    QLocale::setDefault(QLocale(QLocale::AnyLanguage));
}

ArtistModel::~ArtistModel()
{
    artistList.clear();
}

void ArtistModel::refresh()
{
    if(QFile::exists(databaseName))
    {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        Utils::Artist artist;
        artistList.clear();

        if(db.isOpen())
        {
            QSqlQuery artistQuery = QSqlQuery(db);
            artistQuery.exec(queryCmd);
            if(artistQuery.isActive())
            {
                while(artistQuery.next())
                {
                    artist.name = artistQuery.value("artistname").toString();
                    artist.cover = artistQuery.value("cover").toString();
                    beginInsertRows(QModelIndex(), rowCount(), rowCount());
                    artistList.push_back(artist);
                    endInsertRows();

                }
            }
            else
            {
                qDebug() << "debugQuery is not active";
            }
            artistQuery.finish();
        }
    }

    else
    {
        qDebug() << databaseName << " doesn't exists : ArtistModel::refresh()";
    }
}


QVariant ArtistModel::data(const QModelIndex &index, int role) const
{

    // Version 3.

    if (index.row() < 0 || index.row() >= rowCount()  )
            return QVariant();

     else
    {
        Utils::Artist artist = artistList.at(index.row());
        switch (role)
        {
            case ArtistNameRole:
                return artist.name;
            case ArtistCoverRole:
                return artist.cover;

//            case ArtistIDRole:
//                return artist.artistID;

//            case DescriptionRole:
//                return artist.description;

            default:
                return QVariant();
        }
    }

}

QHash<int,QByteArray> ArtistModel::roleNames() const
{
      QHash<int, QByteArray> roles;
      roles[ArtistIDRole] = "artistID";
      roles[ArtistNameRole] = "artist";
      roles[DescriptionRole] = "description";
      roles[ArtistCoverRole] = "cover";
      return roles;
}

int ArtistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return artistList.size();
}


void ArtistModel::resetInternalData()
{
    beginResetModel();
    artistList.clear();
    endResetModel();
    Q_EMIT dataChanged();
}

