#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QtSql>
#include <QDir>

class PlaylistManager : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistManager(QObject *parent = 0);

Q_SIGNALS:
    void playlistsModelChanged();

public Q_SLOTS:
    int createPlaylist(QString name); //  return the playlistID
    bool removePlaylist(int playlistID);
    bool addToPlaylist(int playlistID, int trackID);
    bool removeFromPlaylist(int playlistID, int trackID);


private:
    QString connectionName;
    QString databaseName;

};

#endif // PLAYLISTMANAGER_H
