

#include "notify.h"
#include "player.h"
#include "track.h"
#include "albummodel.h"
#include "tracklistmodel.h"
#include "datab.h"
#include "playlistcontentmodel.h"
#include "playlistmodel.h"
#include "playlist.h"
#include "nowplayingplaylist.h"

#include <qqml.h>

void TestModelsPlugin::registerTypes(const char *uri)
{
    // @uri Models
    //qmlRegisterType<ModelsPlugin>(uri, 1, 0, "ModelsPlugin");
    qmlRegisterType<AlbumModel>(uri, 1, 0, "AlbumModel");
    qmlRegisterType<TrackListModel>(uri, 1, 0, "TrackListModel");
    qmlRegisterType<Player>(uri, 1, 0, "UTouchPlayer");
    qmlRegisterType<DataB>(uri, 1, 0, "MusicDatabase");
    qmlRegisterType<Notify>(uri, 1, 0, "Notify");
    qmlRegisterType<PlaylistContentModel>(uri, 1, 0, "PlaylistContentModel");
    qmlRegisterType<PlaylistModel>(uri, 1, 0, "PlaylistModel");
    qmlRegisterType<Playlist>(uri, 1, 0, "Playlist");
    qmlRegisterType<NowPlayingPlaylist>(uri, 1, 0, "NowPlayList");

}


