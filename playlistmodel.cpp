#include "playlistmodel.h"


PlaylistModel::PlaylistModel(QObject *parent): QAbstractListModel(parent)
{

    databaseName = QDir::homePath() + "/.Player/config/player.db";
    //filter << "*.m3u";
    connectionName  = "Logger";
    queryCmd = QString("SELECT * FROM Playlist;");
    driver = QString("QSQLITE");
    QLocale::setDefault(QLocale(QLocale::AnyLanguage));
}

QHash<int,QByteArray> PlaylistModel::roleNames() const
{
      QHash<int, QByteArray> roles;
      roles[TitleRole] = "title";
      roles[IDRole] = "playlistID";
      return roles;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent)
    return _playlists.size();
}



QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{

    // Version 3.

    if (index.row() < 0 || index.row() >= _playlists.size())
            return QVariant();


    if(!isEmpty())
    {
        Utils::Playlist pls = _playlists.at(index.row());
        switch (role)
        {


        case TitleRole:
            return pls.title;


        case IDRole:
        {
            return pls.ID;
        }
        default:
            return QVariant();

        }
    }
    else
    {
        return QVariant();
    }


}


void PlaylistModel::refresh()
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    resetInternalData();
    if(db.isOpen())
    {
        QSqlQuery query = QSqlQuery(queryCmd,db);
        Utils::Playlist pls;
        query.exec();
        while (query.next())
        {
            pls.title = query.value("title").toString();
            pls.ID = query.value("playlistID").toInt();
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            _playlists.push_back(pls);
            endInsertRows();
        }
    }
    else
    {
        qDebug()<< "DEBUG PlaylistModel::refresh(): Database is not opened" ;
        qDebug() << db.lastError();
    }
}


void PlaylistModel::resetInternalData()
{
    _playlists.clear();
    beginResetModel();
    endResetModel();
}


bool PlaylistModel::isEmpty()const
{
    return _playlists.empty();
}
