#include "playermodules_plugin.h"

#include "player.h"
#include "utils.h"
#include "logger.h"
#include "albummodel.h"
#include "artistmodel.h"
#include "genremodel.h"
#include "tracklistmodel.h"
#include "playlist.h"

#include "playlistmodel.h"
#include "playlistcontentmodel.h"
#include <qqml.h>



void PlayerModulesPlugin::registerTypes(const char *uri)
{
    //@uri PlayerModules
    qmlRegisterType<AlbumModel>(uri, 1, 0, "AlbumModel");
    qmlRegisterType<TrackListModel>(uri, 1, 0, "TrackListModel");
    qmlRegisterType<Player>(uri, 1, 0, "MusicPlayer");
    qmlRegisterType<Utils::Database>(uri, 1, 0, "MusicDatabase");
    qmlRegisterType<ArtistModel>(uri, 1, 0, "ArtistModel");
    qmlRegisterType<GenreModel>(uri, 1, 0, "GenreModel");
    qmlRegisterType<Playlist>(uri, 1,0,"Playlist");
    qmlRegisterType<PlaylistModel>(uri, 1,0,"PlaylistModel");
    qmlRegisterType<PlaylistContentModel>(uri, 1,0,"PlaylistContentModel");
}


