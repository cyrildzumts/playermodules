#include "playlistmanager.h"

PlaylistManager::PlaylistManager(QObject *parent) :
    QObject(parent)
{
    connectionName = QString("PlaylistMAnager");
    databaseName = QDir::homePath() + QString("/MyPlayer/Database/musicDB.db");

}

int PlaylistManager::createPlaylist(QString name)
{
    QSqlDatabase musicDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    musicDB.setDatabaseName(databaseName);
    bool opened = musicDB.open();
    if(opened)
    {
        int id;
        QSqlQuery musicQuery = QSqlQuery(musicDB);
        musicQuery.exec(QString("REPLACE INTO Playlist (title) VALUES('%1') ;").arg(name));
        id = musicQuery.lastInsertId().toInt();
        Q_EMIT playlistsModelChanged();
        musicQuery.clear();
        musicDB.close();
        return id;
    }
    else
    {
        musicDB.close();
         qDebug() << "PlaylistManager::createPlaylist:  Database is not opened";
        return -1; // please check the return value first to know if
    // the operation was succesfull
    }
}


bool PlaylistManager::addToPlaylist(int playlistID, int trackID)
{
    QSqlDatabase musicDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    musicDB.setDatabaseName(databaseName);
    bool opened = musicDB.open();
    if(opened)
    {
        QSqlQuery musicQuery = QSqlQuery(musicDB);
        musicQuery.exec(QString("INSERT INTO PlaylistTrack (trackID,trackplaylistID) VALUES('%1', '%2') ;").arg(QString::number(trackID),QString::number(playlistID)));
        Q_EMIT playlistsModelChanged();
        musicQuery.finish();
        musicDB.close();
        return true;
    }
    else
    {
        qDebug() << "PlaylistManager::addToPlaylist:  Database is not opened";
        return false;
    }
    musicDB.close();

}


bool PlaylistManager::removeFromPlaylist(int playlistID, int trackID)
{
    QSqlDatabase musicDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    musicDB.setDatabaseName(databaseName);
    bool opened = musicDB.open();
    if(opened)
    {
        QSqlQuery musicQuery = QSqlQuery(musicDB);
        musicQuery.exec(QString("DELETE  * FROM  PlaylistTrack  WHERE trackplaylistID='%1' AND trackID ='%2' ;").arg(QString::number(playlistID),QString::number(trackID)));
        Q_EMIT playlistsModelChanged();
        //musicQuery.clear();
        musicDB.close();
        return true;

     }

    else
    {
        qDebug() << "PlaylistManager::removeFromPlaylist:  Database is not opened";
        return false;
    }
}

bool PlaylistManager::removePlaylist(int playlistID)
{
    QSqlDatabase musicDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    musicDB.setDatabaseName(databaseName);
    bool opened = musicDB.open();
    if(opened)
    {
        QSqlQuery musicQuery = QSqlQuery(musicDB);
        musicQuery.exec("BEGIN TRANSACTION");
        musicQuery.exec(QString("DELETE  * FROM  PlaylistTrack  WHERE trackplaylistID='%1' ;").arg(QString::number(playlistID)));
        musicQuery.exec(QString("DELETE  * FROM  Playlist  WHERE playlistID='%1' ;").arg(QString::number(playlistID)));
        Q_EMIT playlistsModelChanged();
        musicQuery.exec("END TRANSACTION");

        musicQuery.finish();
        musicDB.close();
        Q_EMIT playlistsModelChanged();
        return true;
    }


    else
    {
        qDebug() << "PlaylistManager::removePlaylist: Database is not opened";
        return false;
    }
}

