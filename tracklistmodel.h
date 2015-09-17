#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include <QtSql>
#include "utils.h"
#include <QAbstractListModel>
#include <vector>

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int sourceID READ sourceID WRITE setSourceID NOTIFY sourceIDChanged)
    Q_PROPERTY(QString sourceAlbum READ sourceAlbum WRITE setSourceAlbum NOTIFY sourceAlbumChanged)
    //Q_PROPERTY(std::vector<int>  tracklist READ tracklist)

public:
     explicit TrackListModel (QObject *parent = 0);
    virtual ~TrackListModel();
public Q_SLOTS:
    void reset ()
    {
        resetInternalData();
    }

    virtual QVariant data(const QModelIndex &index, int role) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual void refresh(QString album);
    virtual void refresh(int id);
    virtual void refresh();
    void setSourceID(int ID);
    void setSourceAlbum(QString albumSource);
    int sourceID();
    virtual void queryAllTracks();
    QString sourceAlbum();
    void setQuery(QString query);
    std::vector<Utils::TrackTag> tracklist()const;
    TrackListModel *getModel()
    {
        return this;
    }

Q_SIGNALS:
    void sourceAlbumChanged(QString);
    void sourceIDChanged(int);
    void dataChanged();
protected:
    QHash<int, QByteArray> roleNames()const;
    void resetInternalData();
protected:
    enum TrackRole
    {
        IDRole = Qt::UserRole + 1,
        ArtistRole,
        AlbumArtistRole,
        AlbumRole,
        TitleRole ,
        PathRole ,
        PositionRole,
        LengthRole,
        LengthStringRole,
        PlaycountRole,
        BitrateRole,
        YearRole,
        FavoriteRole,
        CoverRole,
        AddedDateRole,
        GenreRole

    };
    std::vector<Utils::TrackTag> _tracklist;
    QString driver;
    QString queryCmd;
    QString connectionName;
    QString databaseName;
    int _sourceID;
    QString _albumSource;

};
#endif // TRACKLISTMODEL_H
