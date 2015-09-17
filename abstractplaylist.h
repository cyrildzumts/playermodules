#ifndef ABSTRACTPLAYLIST_H
#define ABSTRACTPLAYLIST_H
#include <string>
#include <vector>
#include <QString>
#include "utils.h"

class AbstractPlaylist
{
public:
    virtual ~AbstractPlaylist();

    // Playlist Managment
    /**
     * @brief addMedia adds a new track into the playlist
     * @param path is the path of the track we ant to add
     */
    virtual void addMedia(QString path) = 0;
    virtual void addMedia(Utils::TrackTag track) = 0;
    virtual void addMedia(std::vector<QString> paths) = 0;
    virtual void addMedia(std::vector<Utils::TrackTag> paths) = 0;
    /**
     * @brief remove removes the track with the given path from the Playlist
     * @param path is the path of the track to be removed
     */
    virtual void remove(QString path) = 0;
    virtual void remove(int pos) = 0;
    /**
     * @brief clear removes all tracks from the Playlist
     */
    virtual void clear() = 0;
    virtual std::vector<Utils::TrackTag> tracklist() = 0;
    virtual std::vector<QString> pathlist() = 0;
    virtual bool isEmpty() = 0;

    // ACCESSORS Methods
    /**
     * @brief media returns the current Track's path
     * @return the track path
     */
    virtual QString media() = 0;
    /**
     * @brief currentIndex returns the current Index of the Track beeing played
     * @return
     */
    virtual int currentIndex() = 0;
    virtual void setCurrentIndex(int index) = 0;

    /**
     * @brief currentTitle returns the current Track's title
     * @return
     */
    virtual QString currentTitle() = 0;

    /**
     * @brief currentAlbumTitle returns the current album title of the
     * playing track
     * @return
     */
    virtual QString currentAlbumTitle() = 0;
    /**
     * @brief currentArtist
     * @return the artist of the track being played.
     */
    virtual QString currentArtist() = 0;
    /**
     * @brief currentCover
     * @return the cover of the playing track
     */
    virtual QString currentCover() = 0;
    /**
     * @brief currentYear
     * @return the year the track was produced
     */
    virtual int currentYear() = 0;
    /**
     * @brief currentTrackId
     * @return  the current Track's ID
     */
    virtual int currentTrackId() = 0;

    // Playlist  Controls:
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual void shuffle() = 0;

    /***
     * Signals emitted by all Playlist
     */
Q_SIGNALS:
    virtual void durationChanged()= 0;
    virtual void coverChanged(QString currentCover) = 0;
    virtual void nomedia() = 0;
    virtual void tracklistChanged() = 0;
    virtual void titleChanged(QString newTitle) = 0;
    virtual void dataChanged() = 0;
    virtual void currentMediaChanged(QString mediaPath) = 0;
    //virtual void loadFailed() = 0;
    //virtual void	loaded() = 0;
    virtual void playbackModeChanged(int mode) = 0;
    virtual void currentIndexChanged(int index) = 0;
    virtual void currentTitleChanged() = 0;
    virtual void currentAlbumChanged() = 0;
    virtual void currentArtistChanged() = 0;
    virtual void currentYearChanged() = 0;
    virtual void metadataChanged() = 0;


};

#endif // ABSTRACTPLAYLIST_H
