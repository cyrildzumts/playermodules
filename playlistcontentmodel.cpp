#include "playlistcontentmodel.h"
#include <QDir>

PlaylistContentModel::PlaylistContentModel(QObject *parent) :
    TrackListModel(parent)
{
    //playlistDir = QDir::homePath() + "/MyPlayer/Playlist";
    //filter << ".m3u";
    connectionName = "Logger";
    _sourceID = 0;
    connect(this,&PlaylistContentModel::sourceIDChanged,this,&PlaylistContentModel::refresh);
}

   void PlaylistContentModel::refresh(int playlistID)
   {
       queryCmd = QString("SELECT * FROM PlaylistTrack  P   JOIN BaseTableTracks  B ON P.playlistID = %1 AND P.trackID = B.trackID  ORDER BY P.plsTrackID;").arg(playlistID);
       QSqlDatabase db = QSqlDatabase::database(connectionName);
       Utils::TrackTag track;
       _tracklist.clear();

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
                   //track.position = trackQuery.value("trackNumber").toInt();
                   //track.albumArtist = trackQuery.value("albumArtist").toString();
                   //track.addedDate = trackQuery.value("addedDate").toInt();
                   //track.modifiedDate = trackQuery.value("modifiedDate").toInt();
                   //track.liked = trackQuery.value("favorite").toInt();
                   //track.playCount = trackQuery.value("playCount").toInt();
                   beginInsertRows(QModelIndex(), rowCount(), rowCount());
                   _tracklist.push_back(track);
                   endInsertRows();
               }
               //Q_EMIT dataChanged();

           }
           else
           {
               qDebug() << "DEBUG PlaylistContentModel::refresh(int ) debugQuery is not active";
           }
           trackQuery.finish();
           trackQuery.clear();
       }
   }

 void PlaylistContentModel::save()
 {
     //M3UTools::writeM3U(playlistName, contents);
 }

