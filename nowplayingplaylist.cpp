#include "nowplayingplaylist.h"
#include <QDirIterator>
#include <QTime>

NowPlayingPlaylist::NowPlayingPlaylist(QObject *parent) :
    QAbstractListModel(parent)
{
    totalTime = 0;
    repeatMode = 0;
    /**
      RepeatMode Values:
      0 --> Normal Playback. Stop at the end.
      1 --> Repeat the Current Track.
      2 --> Repeat All the Tracks in the playlist.
      3 --> Shuffle All the Tracks.
      4 --> Previous the playlist. Play randomly 20 seconds of the track.
      */
    trackList.clear();
}


NowPlayingPlaylist::~NowPlayingPlaylist()
{
  qDebug()<< "NowPlaingModel Destructor: This model is out of Scope" ;
  trackList.clear();

}

QVariant NowPlayingPlaylist::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= trackList.size())
            return QVariant();

        Utils::TrackTag track = trackList.at(index.row());
        switch (role)
        {

            case TitleRole:
                return track.title;

            case PathRole:
                return track.path;
            case ArtistRole:
                return track.artist;
            case PositionRole:
                return track.position;
            case LengthRole:
                return track.length;
            case LengthStringRole:
                {
                    int duration = track.length;
                    const int hour = 3600;
                    QTime tmpTime (0,0);
                    QTime t =  tmpTime.addSecs(duration);

                    if(duration < hour)
                        return t.toString("mm:ss");
                    else
                        return t.toString("hh:mm:ss");
                }
            case IDRole:
                return track.ID;
            case BitrateRole:
                return track.bitRate;
            case AlbumRole:
                return track.albumTitle;
            case YearRole:
                return track.year;
        case AlbumArtistRole:
                return track.albumArtist;
        case GenreRole:
                return track.genre;
        case CoverRole:
                return track.cover;
        case PlaycountRole:
                return track.playCount;
        case FavoriteRole:
                return track.liked;
            default:
                return QVariant();

        }

}



int NowPlayingPlaylist::rowCount(const QModelIndex &parent) const
{
    return trackList.size();
}

 QHash<int, QByteArray> NowPlayingPlaylist::roleNames()const
 {
     QHash<int, QByteArray> roles;
     roles[TitleRole] = "title";
     roles[PathRole]  = "trackUrl" ;
     roles[AlbumArtistRole] = "albumArtist";
     roles[ ArtistRole] = "artist";
     roles[PositionRole] ="trackNumber";
     roles[LengthRole] = "length";
     roles [LengthStringRole] = "lengthStr";
     roles[IDRole] = "trackID";
     roles[AlbumRole] = "albumTitle";
     roles[PlaycountRole] = "playcount";
     roles[BitrateRole] = "bitrate";
     roles[YearRole] = "year";
     roles[FavoriteRole] = "favorite";
     roles[CoverRole] = "cover";
     roles[GenreRole] = "genre";


     return roles;
 }

 void NowPlayingPlaylist::addTrack(const QString path)
 {
     Utils::TrackTag track =  TestTagReader::tagreader(path);
     totalTime+=track.length;
     beginInsertRows(QModelIndex(), rowCount(), rowCount());
     trackList.push_back(track);
     endInsertRows();
     Q_EMIT dataChanged();
 }

 void NowPlayingPlaylist::addTrack(const Utils::TrackTag track)
 {
     beginInsertRows(QModelIndex(), rowCount(), rowCount());
     trackList.push_back(track);
     endInsertRows();
     Q_EMIT dataChanged();
 }

 void NowPlayingPlaylist::addTracks(std::vector<Utils::TrackTag> tracks)
 {
    beginInsertRows(QModelIndex(), rowCount(), tracks.size());
    trackList.insert(trackList.end(),tracks.begin(),tracks.end());
    endInsertRows();
    Q_EMIT dataChanged();
    tracks.clear();
 }

 void NowPlayingPlaylist::resetInternalData()
 {
     beginResetModel();
     totalTime = 0;
     trackList.clear();
     endResetModel();
     Q_EMIT dataChanged();
 }

 bool NowPlayingPlaylist::removeRows(int row, int count, const QModelIndex &parent)
 {
     beginRemoveRows(QModelIndex(), row, count);
     for (int i = row; i < count; ++i )
     {
         removeRow(i);
     }
     endRemoveRows();
     Q_EMIT dataChanged();
     return true;
 }

 void NowPlayingPlaylist::setTracklist(std::vector<Utils::TrackTag> tracks)
 {
    trackList.clear();
    trackList = std::move(tracks);

    Q_EMIT dataChanged();
 }

QString NowPlayingPlaylist::getTotalTime()
{
    QTime myTime(0,0) ;
    totalTime = 0;

    for(auto track : trackList)
    {
        totalTime += track.length;
    }
    QTime t = myTime.addSecs(totalTime);
    return t.toString(Qt::ISODate);
}

bool NowPlayingPlaylist::removeRow(int row, const QModelIndex &parent)
{
    trackList.erase(trackList.begin()+row);

    Q_EMIT dataChanged();
    return true;
}

void NowPlayingPlaylist::save(QString name)
{

}

void NowPlayingPlaylist::setIndex(int _index)
{
    if(_index < 0 || _index >= trackList.size())
    {
        currentIndex = -1;
        source = QString();
    }

    else
    {

        currentIndex = _index;
        source = data(index(currentIndex),TrackRole::PathRole).toString();
    }

}

QString NowPlayingPlaylist::next()
{
    setIndex(currentIndex +1);
    return source;

}

QString NowPlayingPlaylist::previous()
{
    setIndex(currentIndex - 1);
    return source;
}

QString NowPlayingPlaylist::getTrackAtPos(int pos)
{
    //currentTrackIndex = pos;
    //return trackList.at(currentTrackIndex).path;
    return data(index(pos),TrackRole::PathRole).toString();
}

QString NowPlayingPlaylist::random()
{
    return QString();
}
