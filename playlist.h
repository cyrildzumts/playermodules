#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QAbstractListModel>
#include "utils.h"
#include "tagreader.h"
#include "tracklistmodel.h"


class Playlist : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PlaybackMode
    {
        CurrentItemOnce = 0,
        CurrentItemInLoop,
        Sequential,
        Loop,
        Random,
        Preview
    };
    Q_ENUMS(PlaybackMode)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString durationStr READ durationStr NOTIFY durationChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
    Q_PROPERTY(int mediaCount READ mediaCount)
    Q_PROPERTY(QString  media READ media)
    Q_PROPERTY(QString currentMedia READ currentMedia  NOTIFY currentMediaChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY currentArtistChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle  NOTIFY currentTitleChanged)
    Q_PROPERTY(QString currentAlbum READ currentAlbum NOTIFY currentAlbumChanged)
    Q_PROPERTY(int currentYear READ currentYear  NOTIFY currentYearChanged)


public:
    enum Role
    {
        IDRole = Qt::UserRole + 1,
        ArtistRole,
        AlbumArtistRole,
        PlaylistRole,
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
    //explicit Playlist();
    Playlist(/*QAbstractListModel *parent = 0*/);
    Playlist(const QString name);
    Playlist(const Playlist &pls, QAbstractListModel *parent = 0);
    Playlist &operator=(const Playlist & pls);
    ~Playlist()
    {
        clear();
    }

public Q_SLOTS:
    int duration()const;
    QString durationStr() const;
    void setCover(QString path);
    QString cover()const;
    void setTracklist(std::vector<Utils::TrackTag> tracks);
    void setTracklist(TrackListModel *model);

    QString currentAlbum()const;
    QString currentTitle()const;
    QString currentArtist()const;
    int currentYear()const;

    std::vector<Utils::TrackTag> tracklist()const;

    int currentIndex()const;
    void setCurrentIndex(int index);
    void goToNextIndex();
    void goToPreviousIndex();
    int playbackMode()const;
    void setPlaybackMode(int mode);
    void clear();
    QString title()const;
    void setTitle(QString t);
    //void addMedia(QString url, int trackID);
    void addMedia(Utils::TrackTag track);
    void addMedia(std::vector<Utils::TrackTag> tracks);
    void addMedia(int trackID);
    Utils::TrackTag currentTrack()const;
    bool isEmpty()const;
    void insertMedia(int pos,Utils::TrackTag track);
    void insertMedia(int pos, std::vector<Utils::TrackTag> tracks);
    QString media(int index)const;
    QString media()const;
    QString currentMedia()const;

    void nextMedia();
    int mediaCount()const;
    int	nextIndex(int steps = 1) const;
    int	previousIndex(int steps = 1) const;
    bool removeMedia(int pos);
    bool removeMedia(int start, int end);
    int currentTrackID()const;
    void save(const QUrl & location, const char * format = 0);
    void save(QIODevice * device, const char * format);
    std::vector<Utils::TrackTag> tracks()const;
    //PlayerControls:
    void next();
    void previous();
    void shuffle();
    void repeat();


    // QAbstractlistModel Overwriting :
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void resetInternalData();
    QVariant data(const QModelIndex &index, int role) const;



Q_SIGNALS:
    void durationChanged();
    void coverChanged(QString currentCover);
    void nomedia();
    void tracklistChanged();
    void titleChanged(QString newTitle);
    void dataChanged();
    void currentMediaChanged(QString mediaPath);
    //void loadFailed();
    //void	loaded();
    void playbackModeChanged(int mode);
   // void currentIndexChanged();
    void currentIndexChanged(int index);
    void playbackModeChanged();
    void currentTitleChanged();
    void currentAlbumChanged();
    void currentArtistChanged();
    void currentYearChanged();
    void metadataChanged();

protected:
    QHash<int, QByteArray> roleNames()const;
    void showContents();
protected Q_SLOTS:
    void setCurrentMedia(int index);
    void init();
    void emitAllMetadata();

private:

    int _duration;
    /**
     * @brief _currentIndex is current index of the track being played
     */
    int _currentIndex;
    /**
     * @brief _randomCurrentIndex is used to access the random index from
     * randomIndices
     */
    int _randomCurrentIndex;

    /**
     * @brief _title is the Playlist's title
     */
    QString _title;
    /**
     * @brief _cover is the cover of the playlist
     */
    QString _cover;
    /**
     * @brief _playbackMode defines how the tracks contained in the playlist
     * are being played back.
     */
    int _playbackMode;

    //PlaybackEnum::PlaybackModeObject mode;
    /**
     * @brief _tracks represents the list of the tracks
     */
    std::vector<Utils::TrackTag> _tracks;
    /**
     * @brief randomIndices contains the indices of the tracks when
     * the Random Playback is activated. _randomCurrentIndex is used
     * to access these indices.
     */
    std::vector<int> randomIndices;

    QString _currentMedia;

    /**
     * @brief _currentTitle holds the currently playing track's title
     */
    //QString _currentTitle;
    /**
     * @brief _currentArtist holds the currently playing track's artist
     */
    //QString _currentArtist;
    /**
     * @brief _currentAlbum holds the currently playing track's album
     */
    //QString _currentAlbum;
    /**
     * @brief _currentYear holds the currently playing track's year
     */
    //int _currentYear;
};

#endif // PLAYLIST_H
