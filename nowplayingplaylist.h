#ifndef NOWPLAYINGPLAYLIST_H
#define NOWPLAYINGPLAYLIST_H

#include <QAbstractListModel>
#include "utils.h"
#include "tagreader.h"
#include <vector>
#include <random>

/**
 * @brief The NowPlayingPlaylist class
 * @deprecated
 */
class NowPlayingPlaylist : public QAbstractListModel
{
    Q_OBJECT
   // Q_PROPERTY(QString addTrack WRITE addTrack NOTIFY dataChanged)
    //Q_PROPERTY(QStringList addTracks WRITE addTracks NOTIFY dataChanged)
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
public:
    explicit NowPlayingPlaylist(QObject *parent = 0);
    ~NowPlayingPlaylist();
public Q_SLOTS:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void addTrack(const QString path);
    void addTrack(const Utils::TrackTag track);
    void addTracks(std::vector<Utils::TrackTag> tracks);
    void setTracklist(std::vector<Utils::TrackTag> tracks);
    bool removeRows(int row , int count, const QModelIndex &parent = QModelIndex());
    void save(QString name);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QString getTotalTime();
    NowPlayingPlaylist *getModel()
    {
        return this;
    }
    void setIndex(int _index);
    QString previous();
    QString next();
    QString getTrackAtPos(int pos);
    QString random();
protected:
    QHash<int, QByteArray> roleNames()const;
    void resetInternalData();

Q_SIGNALS:
void dataChanged();
void endReached();

private:
    QString source;
    int repeatMode;
    int currentIndex;
    int totalTime; // time in seconds
    std::vector<Utils::TrackTag> trackList;
};

#endif // NOWPLAYINGPLAYLIST_H

