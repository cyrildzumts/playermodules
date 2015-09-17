#include "playlist.h"
#include "tagreader.h"
/**
 * RandomGenerator is used when the user want
 * to activate the Random Playback or Shuffle
 */

#include "randomgenerator.h"
#include <QtSql>


Playlist::Playlist():QAbstractListModel(nullptr)
{
    init();
}


Playlist::Playlist(const QString name)
{
    init();
    _title= name;
}

Playlist::Playlist(const Playlist &pls, QAbstractListModel *parent ):QAbstractListModel(parent)
{
    init();
    _title = pls.title();
    _tracks = pls.tracklist();
    _currentIndex = pls.currentIndex();
    _playbackMode = pls.playbackMode();
}

void Playlist::init(){
    clear();
    connect (this,SIGNAL(currentIndexChanged(int)),this,SLOT(emitAllMetadata()));
//    connect (this,SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentAlbumChanged()));
//    connect (this,SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentArtistChanged()));
//    connect (this,SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentYearChanged()));
    connect (this,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentMedia(int)));
    connect(this, SIGNAL(dataChanged()),this,SIGNAL(durationChanged()));
}

void Playlist::emitAllMetadata()
{
    Q_EMIT currentTitleChanged();
    Q_EMIT currentAlbumChanged();
    Q_EMIT currentArtistChanged();
    Q_EMIT currentYearChanged();
    Q_EMIT coverChanged(cover());
}

void Playlist::clear()
{


    _title.clear();
    _currentIndex = -1;
    _randomCurrentIndex = -1;
    _playbackMode = 2;
    int start = 0;
    int end = isEmpty()? 0: mediaCount() - 1 ;
    beginRemoveRows(QModelIndex(),start, end);
      _tracks.clear();
     endRemoveRows();


}
QString Playlist::durationStr()const
{
    const int hour = 3600;
    QTime tmpTime (0,0);
    QTime t =  tmpTime.addSecs(duration());

    if(duration() < hour)
        return t.toString("mm:ss");
    else
        return t.toString("hh:mm:ss");
}
int Playlist::duration()const
{
    if(!isEmpty())
    {
        int d = 0;
        for(Utils::TrackTag track : _tracks)
        {
            d+= track.length;
        }

        return d;
    }
    return 0;
}
void Playlist::setTitle(QString t)
{
    _title = t;
    Q_EMIT titleChanged(_title);
}


QString Playlist::title()const
{
    return _title;
}

QString Playlist::currentAlbum()const
{
    if (!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
        return _tracks.at(_currentIndex).albumTitle;
    return QString();
}

int Playlist::currentYear()const
{
    if (!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
        return _tracks.at(_currentIndex).year;
    return 0;
}

QString Playlist::currentTitle()const
{
    if (!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
        return _tracks.at(_currentIndex).title;
    return QString();
}

QString Playlist::currentArtist()const
{
    if (!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
        return _tracks.at(_currentIndex).artist;
    return QString();
}

Playlist &Playlist::operator=(const Playlist &pls)
{

    _tracks.clear();
    _title = pls.title();
    addMedia(pls.tracks());
    _currentIndex = pls.currentIndex();
    _playbackMode = pls.playbackMode();
    return *(this);
}


int Playlist::currentIndex()const
{
    return _currentIndex;
}

void Playlist::setCurrentIndex(int index)
{
    if (!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
    {

        setTitle(_tracks.at(index).albumTitle);

        _currentIndex = index;

        Q_EMIT currentIndexChanged(_currentIndex);

    }

}
void Playlist::goToNextIndex()
{
     if (!isEmpty())
     {
         switch (_playbackMode)
         {
         case PlaybackMode::CurrentItemOnce :
         case PlaybackMode::CurrentItemInLoop :
         case PlaybackMode::Sequential :
         {
                 if(_currentIndex  < _tracks.size() - 1)
                 {
                     _currentIndex++;
                     Q_EMIT currentIndexChanged(_currentIndex);
                 }
         }

             break;


         case PlaybackMode::Loop :
         {

                 if(_currentIndex  < _tracks.size() - 1)
                 {
                     _currentIndex++;
                 }
                 else
                 {
                     _currentIndex = 0;
                 }
                 Q_EMIT currentIndexChanged(_currentIndex);

         }
             break;
         case PlaybackMode::Random:
         {
                 if( _randomCurrentIndex  < _tracks.size() -1)
                 {
                     _randomCurrentIndex++;
                     _currentIndex =  randomIndices.at(_randomCurrentIndex);
                     Q_EMIT currentIndexChanged(_currentIndex);
                 }
                 else
                 {
                     _randomCurrentIndex = 0;
                 }

             break;
         }
         }
     }
}

void Playlist::goToPreviousIndex()
{
     if (!isEmpty())
     {

         switch (_playbackMode)
         {
         case PlaybackMode::CurrentItemOnce :
         case PlaybackMode::CurrentItemInLoop :
         case PlaybackMode::Sequential :
         {
                 if(_currentIndex  > 1)
                 {
                     _currentIndex--;
                     Q_EMIT currentIndexChanged(_currentIndex);
                 }
         }

             break;


         case PlaybackMode::Loop :
         {

                 if(_currentIndex  > 1)
                 {
                     _currentIndex--;
                 }
                 else
                 {
                     _currentIndex = mediaCount() -1;
                 }
                 Q_EMIT currentIndexChanged(_currentIndex);

         }
             break;
         case PlaybackMode::Random:
         {
                 if( _randomCurrentIndex  > 1)
                 {
                     _randomCurrentIndex--;
                     _currentIndex =  randomIndices.at(_randomCurrentIndex);
                     Q_EMIT currentIndexChanged(_currentIndex);
                 }

             break;
         }
         }
     }
}

int Playlist::playbackMode()const
{
    return _playbackMode;
}

void Playlist::setPlaybackMode(int  mode)
{
    _playbackMode = mode;
    Q_EMIT playbackModeChanged();
}

void Playlist::addMedia(Utils::TrackTag track)
{
    if(isEmpty())
        _currentIndex = 0;
    beginInsertRows(QModelIndex(), rowCount(),rowCount());
    _tracks.push_back(track);
    endInsertRows();
}

void Playlist::addMedia(int trackID)
{
    QString queryCmd = QString("SELECT * FROM BaseTableTracks WHERE  trackID = %1;").arg(trackID);
    QString connectionName = QString("Logger");

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    Utils::TrackTag track;

    if(db.isOpen())
    {
        QSqlQuery trackQuery = QSqlQuery(db);
        if(trackQuery.exec(queryCmd))
        {
            while(trackQuery.next())
            {
                track.title = trackQuery.value("title").toString();
                track.albumTitle = trackQuery.value("albumTitle").toString();
                track.ID = trackQuery.value("trackID").toInt();
                track.cover = trackQuery.value("cover").toString();
                track.length = trackQuery.value("length").toInt();
                track.genre = trackQuery.value("genre").toString();
                track.year = trackQuery.value("year").toInt();
                track.artist = trackQuery.value("artist").toString();
                track.bitRate = trackQuery.value("bitrate").toInt();
                track.path = trackQuery.value("trackUrl").toString();
                track.position = trackQuery.value("trackNumber").toInt();
                track.albumArtist = trackQuery.value("albumArtist").toString();
                track.addedDate = trackQuery.value("addedDate").toInt();
                track.modifiedDate = trackQuery.value("modifiedDate").toInt();
                track.liked = trackQuery.value("favorite").toInt();
                track.playCount = trackQuery.value("playCount").toInt();
                addMedia(track);
            }
            //Q_EMIT dataChanged();

        }
        else
        {
            qDebug() << "debugQuery is not active";
        }
        trackQuery.finish();
        trackQuery.clear();
    }

    else
    {
        qDebug() << " Database doesn't exists : AlbumModel::refresh()";
    }
    //Q_EMIT dataChanged();
}

void Playlist::addMedia(std::vector<Utils::TrackTag> tracks)
{
    if(isEmpty()  )
        _currentIndex = 0;
    beginInsertRows(QModelIndex(),rowCount(),tracks.size() -1 );
    _tracks.insert(_tracks.end(),tracks.begin(),tracks.end());
    endInsertRows();
}

Utils::TrackTag Playlist::currentTrack()const
{
    if(!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
    {
        return _tracks.at(_currentIndex);
    }
    return Utils::TrackTag() ;
}

bool Playlist::isEmpty()const
{
    return _tracks.empty();
}

void Playlist::insertMedia(int pos, Utils::TrackTag track)
{
    if(isEmpty())
    {
        pos = 0;
    }
    beginInsertRows(QModelIndex(),pos,pos);
    _tracks.insert(_tracks.begin()+ pos, track);
    endInsertRows();
}

void Playlist::insertMedia(int pos, std::vector<Utils::TrackTag> tracks)
{
    if(isEmpty())
    {
        pos = 0;
    }
    beginInsertRows(QModelIndex(),pos,tracks.size() - 1);
    _tracks.insert(_tracks.begin()+ pos, tracks.begin(),tracks.end());
    endInsertRows();
}

QString Playlist::media(int index) const
{
    if(!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
    {
        return _tracks.at(index).path;
    }
    return QString() ;
}

QString Playlist::media() const
{
    if(!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
        return _tracks.at(_currentIndex).path;
    return QString();
}

QString Playlist::currentMedia()const
{
    return media();
}

void Playlist::setCurrentMedia(int index)
{
    if(!isEmpty() && (_currentIndex >= 0 ) &&( _currentIndex < mediaCount()) )
    {
        _currentMedia = _tracks.at(index).path;
        Q_EMIT currentMediaChanged(_currentMedia);
    }
}

int Playlist::mediaCount()const
{
    return (int)_tracks.size();
}

int Playlist::nextIndex(int steps) const
{
    if(!isEmpty())
    {
        return _currentIndex +steps ;
    }
    return -1;
}

int Playlist::previousIndex(int steps) const
{
    if(!isEmpty())
    {
        return _currentIndex - steps;
    }
    return -1;
}

bool Playlist::removeMedia(int pos)
{
    if(!isEmpty())
    {
        if( (pos < (int) _tracks.size() )|| (pos >= 0))
        {
            beginRemoveRows(QModelIndex(),pos,pos);
            _tracks.erase(_tracks.begin() + pos);
            endRemoveRows();
            _currentIndex--;

            return true;
        }
    }
    return false;
}


bool Playlist::removeMedia(int start, int end)
{
    if(!isEmpty())
    {
        if( (start < _tracks.size() )&& (start >= 0))
        {
            if( (end < _tracks.size() )&& (end >= 0))
            {
                if ( start < end)
                {
                    _tracks.erase(_tracks.begin()+start, _tracks.begin()+end);
                    _currentIndex-= end - start;
                    return true;
                }

                else
                {
                    _tracks.erase(_tracks.begin()+end, _tracks.begin()+start);
                    _currentIndex-= start -end;
                    return true;
                }
            }

            else
                return false;
        }
        return false;
    }
    return false;
}


void Playlist::save(const QUrl &location, const char *format)
{

}

void Playlist::save(QIODevice *device, const char *format)
{

}

std::vector<Utils::TrackTag> Playlist::tracks()const
{
    return _tracks;
}

void Playlist::nextMedia()
{
    if(!isEmpty())
    {
        switch (_playbackMode)
        {
        case PlaybackMode::CurrentItemOnce :
        {
            Q_EMIT nomedia();
        }
            break;

        case PlaybackMode::CurrentItemInLoop :
        {
            Q_EMIT currentIndexChanged(_currentIndex);
        }
            break;
        case PlaybackMode::Sequential :
        {
            if (_currentIndex < _tracks.size() -1)
            {
                _currentIndex++;
                Q_EMIT currentIndexChanged(_currentIndex);
            }

            else
            {
                Q_EMIT nomedia();
            }
        }
            break;

        case PlaybackMode::Loop :
        {
            if(_currentIndex <_tracks.size() -1)
            {
                _currentIndex++;
            }
            else
            {
                _currentIndex = 0;
            }
            Q_EMIT currentIndexChanged(_currentIndex);
        }
            break;
        case PlaybackMode::Random:
        {
            if( ( _randomCurrentIndex  ) < _tracks.size() - 1)
            {
                _randomCurrentIndex++;
                _currentIndex =  randomIndices.at(_randomCurrentIndex);
                Q_EMIT currentIndexChanged(_currentIndex);
            }
            else
            {
                Q_EMIT nomedia();
            }
        }
            break;
        }
    }
}

/**
 * @brief Playlist::next advances the index up to the next track on the list.
 * next pays attention to the playbackmode.
 * This methode is mainly used to receive the signal when the user has
 * clicked on the next button.
 * @see Playlist::nextMedia() if you want to get the next track.
 */
void Playlist::next()
{
    /**
     * Before I go to the next song, I have to check the playbackMode:
     * On CurrentItemOnce : I set currentIndex at the end and stop the player.
     * On CurrentItemLoop : I loop back to the same currentIndex
     * On Sequential : I navigate sequencialy the playlist until the end.
     * On Loop :  At the end of the playlist I set currentIndex to the first position
     * an continue playing the file from there.
     * On Rondom : the next random track index is set as currentIndex
     *
    */

    if(!isEmpty())
    {
        switch (_playbackMode)
        {
        case PlaybackMode::CurrentItemOnce :
        {
            Q_EMIT nomedia();
        }
            break;

        case PlaybackMode::CurrentItemInLoop :
        {
            Q_EMIT currentIndexChanged(_currentIndex);
        }
            break;
        case PlaybackMode::Sequential :
        {
            if (_currentIndex < _tracks.size() -1)
            {
                _currentIndex++;
                Q_EMIT currentIndexChanged(_currentIndex);
            }

            else
            {
                Q_EMIT nomedia();
            }
        }
            break;

        case PlaybackMode::Loop :
        {
            if(_currentIndex <_tracks.size() -1)
            {
                _currentIndex++;
            }
            else
            {
                _currentIndex = 0;
            }
            Q_EMIT currentIndexChanged(_currentIndex);
        }
            break;
        case PlaybackMode::Random:
        {
            if( ( _randomCurrentIndex  ) < _tracks.size() - 1)
            {
                _randomCurrentIndex++;
                _currentIndex =  randomIndices.at(_randomCurrentIndex);
                Q_EMIT currentIndexChanged(_currentIndex);
            }
            else
            {
                Q_EMIT nomedia();
            }
        }
            break;
        }
    }
}

/**
 * @brief Playlist::previous set the current index  to the previous track on the list.
 * previous pays attention to the playbackmode.
 * This methode is mainly used to receive the signal when the user has
 * clicked on the previous button.
 */
void Playlist::previous()
{
    if(!isEmpty())
    {
        if(_currentIndex >= 0 && _currentIndex < mediaCount())
        {
            switch (_playbackMode)
            {
            case PlaybackMode::CurrentItemOnce :
            case PlaybackMode::CurrentItemInLoop :
            case PlaybackMode::Sequential :
                if(_currentIndex - 1 >= 0 )
                {
                    _currentIndex--;
                    Q_EMIT currentIndexChanged(_currentIndex);
                }
                break;
            case PlaybackMode::Loop :
            {
                if(_currentIndex - 1 < 0)
                {
                    _currentIndex = _tracks.size() - 1;

                }

                else
                {
                    _currentIndex--;

                }
                Q_EMIT currentIndexChanged(_currentIndex);
            }
                break;
            case PlaybackMode::Random:
            {
                if( (_randomCurrentIndex  - 1 ) >= 0 )
                {
                    _randomCurrentIndex--;
                    _currentIndex = randomIndices.at(_randomCurrentIndex);
                    Q_EMIT currentIndexChanged(_currentIndex);
                }
                break;
            }
            }
        }

    }
}

void Playlist::shuffle()
{
    switch(_playbackMode)
    {
    case PlaybackMode::Random:
        _currentIndex = _randomCurrentIndex;
        setPlaybackMode( PlaybackMode::Sequential);
        break;

    default:
    {
        randomIndices = RandomGenerator::Instance()->getShuffleList(_tracks.size());
        _randomCurrentIndex = 0;
        setPlaybackMode( PlaybackMode::Random);
        break;
    }

    }
    Q_EMIT playbackModeChanged();
}

void Playlist::repeat()
{

}

QHash<int, QByteArray> Playlist::roleNames()const
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
    roles[PlaylistRole] = "playlistTitle";
    roles[PlaycountRole] = "playcount";
    roles[BitrateRole] = "bitrate";
    roles[YearRole] = "year";
    roles[FavoriteRole] = "favorite";
    roles[CoverRole] = "cover";
    roles[GenreRole] = "genre";
    return roles;
}

void Playlist::resetInternalData()
{
    _tracks.clear();
    beginResetModel();

    _currentIndex = 0;
    endResetModel();
    //Q_EMIT dataChanged();
}

int Playlist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _tracks.size();
}


QVariant Playlist::data(const QModelIndex &index, int role) const
{

    if(isEmpty())
    {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    Utils::TrackTag track = _tracks.at(index.row());


    switch (role)
    {
    case TitleRole:
        return track.title;
    case PathRole:
        return track.path;
    case ArtistRole:
        return track.artist;
    case PositionRole:
        return index.row();
    case LengthRole:
        return track.length;
    case LengthStringRole:
    {
        const int hour = 3600;
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addSecs(track.length);

        if(track.length < hour)
            return t.toString("mm:ss");
        else
            return t.toString("hh:mm:ss");
    }
    case IDRole:
        return track.ID;
    case BitrateRole:
        return track.bitRate;
    case PlaylistRole:
        return _title;
    case  YearRole:
        return track.year;
    case FavoriteRole:
        return track.liked;
    case PlaycountRole:
        return track.playCount;
    case CoverRole:
        return track.cover;
    case AlbumArtistRole:
        return track.albumArtist;
    case GenreRole:
        return track.genre;

    }
}

void Playlist::setTracklist(std::vector<Utils::TrackTag> tracks)
{
    clear();

    randomIndices = RandomGenerator::Instance()->getShuffleList(tracks.size());
    addMedia(tracks);

    Q_EMIT tracklistChanged();
}

void Playlist::setTracklist(TrackListModel *model)
{
    setTracklist(model->tracklist());
}

std::vector<Utils::TrackTag> Playlist::tracklist()const
{
    return _tracks;
}

void Playlist::setCover(QString path)
{
    _cover = path;
    Q_EMIT coverChanged(_cover);
}

QString Playlist::cover()const
{
    if((_currentIndex >= 0 && _currentIndex < _tracks.size()) )
    {
        QString trackCover = _tracks.at(_currentIndex).cover;
        if(!trackCover.isEmpty())
            return trackCover;
        else
        {
            return TestTagReader::cover(_tracks.at(_currentIndex).path);
        }

    }

    else
        return QString("");
}

void Playlist::showContents()
{
    qDebug() << " The Playlist contains " << _tracks.size() << " Tracks." ;
    for (auto track : _tracks)
    {
        qDebug() << track.title;
    }
}

int Playlist::currentTrackID()const
{
    if((_currentIndex >= 0 && _currentIndex < _tracks.size()) )
        return _tracks.at(_currentIndex).ID;
    return -1;
}
