#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QtSql>
#include "utils.h"
#include <QDirIterator>


class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT

    public:
    enum PlaylistRole
    {

       PlaylistTitleRole = Qt::UserRole + 1,
       TitleRole,
       IDRole
    };

    explicit PlaylistModel(QObject *parent = 0);
 public Q_SLOTS:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool isEmpty()const;
    void refresh();
//    void addPlaylist(const Playlist &pls);
//    bool removeRows(int row, int count, const QModelIndex &parent);
//    bool insertRow(int row, const QModelIndex &parent); // actually it the same as addPlaylist.
//    int createPlaylist(const QString name); // return the PlaylistID or -1 on error
//    bool removePlaylist(int playlistID);
//    bool addToPlaylist(int playlistID, int trackID);
//    bool removeFromPlaylist(int playlistID, int trackID);
//    void queryPlaylists(bool DB = true);
    PlaylistModel *getModel()
    {
        return this;
    }

Q_SIGNALS:
    void dataChanged();
protected:
    void resetInternalData();
     QHash<int,QByteArray> roleNames()const;

private:
     std::vector<Utils::Playlist> _playlists;
     QString databaseName;
     QString connectionName;
     QString queryCmd;
     QString driver;
};

#endif // PLAYLISTMODEL_H
