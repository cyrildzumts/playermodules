#include "logger.h"
#include <functional>

using namespace Utils;

Utils::Database::Database(QQuickItem *parent):QQuickItem(parent)
{

    isScanned = 0;
    isConfigured = 0;
    appName = QString("Player");
    configPath =  QDir::homePath() + "/.Player/config";
    databasePath = configPath + "/player.db";
    playlistDirPath = configPath + "/Playlists";
    connectionName = QString("Logger");
    directoryWatcher = new QFileSystemWatcher;
    addedDate = 0;
    modifiedDate = 0;
    audioFormatFilter << QString ("*.m4a") << QString("*.mp3") << QString("*.flac") << QString("*.ogg") << QString("*.wma");
    sourceDirPath = QStandardPaths::locate(QStandardPaths::MusicLocation,"",QStandardPaths::LocateDirectory);
    directoryWatcher->addPath(sourceDirPath);
    //connect(this,SIGNAL(scanSourceDirectoryFinished()),this,SLOT(setup()));
    //connect(this,SIGNAL(updateJobFinished()),this,SLOT(setup()));
    connect(directoryWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(launchUpdateThread()));
    setConfig();
    databaseConfig();

}

bool Utils::Database::isDatabaseConfigured()const
{
    return isConfigured == 0 ? false : true ;
}
void Utils::Database::databaseConfig()
{
    query.exec("SELECT * FROM Settings ;");
    if(query.isActive())
    {
        while (query.next())
        {
            isScanned = query.value("isScanned").toInt();
            isConfigured = query.value("isConfigured").toInt();
        }
    }

    if(isConfigured == 0)
    {
        if ( database.isOpen())
        {

            query.exec(
                        QString( "CREATE TABLE IF NOT EXISTS Cover"
                                 "("
                                 "coverID INTEGER NOT NULL,"
                                 "coverpath TEXT UNIQUE COLLATE NOCASE,"
                                 "PRIMARY KEY (coverID));")
                        );
            query.exec(QString("CREATE TABLE IF NOT EXISTS Genre"
                               "("
                               "genreID INTEGER ,"
                               "genre TEXT UNIQUE COLLATE NOCASE,"
                               "PRIMARY KEY(genreID)"
                               ");"
                               ));

            query.exec(QString("CREATE TABLE IF NOT EXISTS Artist (artistID INTEGER,"
                               "artistname TEXT UNIQUE COLLATE NOCASE,"
                               "description TEXT,"
                               "PRIMARY KEY(artistID));"
                               ));

            query.exec(QString("CREATE TABLE IF NOT EXISTS Album"
                               "("
                               "albumID INTEGER ,"
                               "genreID INTEGER NOT NULL,"
                               "artistID INTEGER NOT NULL,"
                               "coverID INTEGER ,"
                               "albumTitle TEXT UNIQUE NOT NULL COLLATE NOCASE,"
                               //"subDirID INTEGER REFERENCES SubDirectories(subDirectoryID),"
                               "year INTEGER DEFAULT 0,"
                               "tracks INTEGER DEFAULT 0,"
                               "duration INTEGER DEFAULT 0,"
                               "UNIQUE(albumTitle,artistID),"
                               "FOREIGN KEY (artistID) REFERENCES Artist(artistID) ON DELETE CASCADE,"
                               "FOREIGN KEY (genreID) REFERENCES Genre(genreID),"
                               "FOREIGN KEY (coverID) REFERENCES Cover(coverID) ON DELETE CASCADE,"
                               "PRIMARY KEY (albumID) );"
                               ));

            query.exec(QString("CREATE TABLE IF NOT EXISTS BaseTableTracks"
                               "(trackID INTEGER ,"
                               "albumTitle TEXT NOT NULL COLLATE NOCASE,"
                               "artist TEXT NOT NULL COLLATE NOCASE,"
                               "albumArtist TEXT NOT NULL COLLATE NOCASE,"
                               "title TEXT NOT NULL COLLATE NOCASE,"
                               "genre TEXT NOT NULL COLLATE NOCASE,"
                               "trackUrl  TEXT  UNIQUE NOT NULL  COLLATE NOCASE,"
                               "cover  TEXT  NOT NULL  COLLATE NOCASE,"
                               "trackNumber INTEGER ,"
                               "length INTEGER NOT NULL,"
                               "playCount INTEGER DEFAULT 0,"
                               "favorite INTEGER DEFAULT 0,"
                               "bitrate INTEGER,"
                               "year INTEGER,"
                               "addedDate INTERGER NOT NULL,"
                               "modifiedDate INTEGER ,"
                               "UNIQUE(albumTitle,trackUrl),"
                               "PRIMARY KEY (trackID) );"
                               ));
            query.exec(QString("CREATE TABLE IF NOT EXISTS Playlist"
                               "("
                               "playlistID INTEGER,"

                               "title TEXT UNIQUE COLLATE NOCASE,"
                               "favorite INTEGER DEFAULT 0,"
                               "coverID INTEGER,"
                               "FOREIGN KEY (coverID) REFERENCES Cover(coverID),"
                               "PRIMARY KEY (playlistID) );"
                               ));
            query.exec(QString("CREATE TABLE IF NOT EXISTS PlaylistTrack"
                               "(plsTrackID INTEGER ,"
                               "playlistID INTEGER ,"
                               "trackID INTEGER,"
                               "FOREIGN KEY(playlistID) REFERENCES Playlist(playlistID) ON DELETE CASCADE,"
                               "FOREIGN KEY (trackID) REFERENCES BaseTableTracks(trackID)ON DELETE CASCADE,"
                               "PRIMARY KEY (plsTrackID) );"
                               ));


            //                // Directory is the Collection source: for example Music, /media/$USER/Music ...

            //                query.exec(QString("CREATE TABLE IF NOT EXISTS Directories"
            //                                        "(directoryID INTEGER,"
            //                                        "path TEXT NOT NULL UNIQUE COLLATE NOCASE,"
            //                                        "lastScanedDate INTEGER,"
            //                                        "PRIMARY KEY (directoryID));"));
            //                //Subdirectory is the content of the collection
            //                query.exec(QString("CREATE TABLE IF NOT EXISTS SubDirectories"
            //                                        "(subDirectoryID INTEGER,"
            //                                        /*"parentID INTEGER REFERENCES Directories(directoryID)," */
            //                                        "path TEXT NOT NULL UNIQUE COLLATE NOCASE,"
            //                                        "addedDate INTEGER,"
            //                                        "modifiedDate INTEGER,"
            //                                        "PRIMARY KEY (subDirectoryID));"));
            query.exec("UPDATE Settings SET isConfigured = 1 WHERE ID = 1;");
            database.commit();
            query.clear();
        }
        else qDebug() << "Database::config(): database is not opened" ;

    }
}

void Utils::Database::setConfig()
{
    QDir home = QDir::home();
    bool isOpen = true;
    if (!home.exists(home.absolutePath() + "/.Player"))
    {
        home.mkdir(home.absolutePath() + "/.Player");
        home.mkdir(configPath);
        home.mkdir(playlistDirPath);
        //home.mkpath(playlistDirPath);
    }
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    database = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    database.setDatabaseName(databasePath);
    isOpen = database.open();
    if (isOpen)
    {
        query = QSqlQuery(database);
        query.exec("PRAGMA foreign_keys = ON ;");
        query.exec("PRAGMA SYNCHRONOUS = OFF");
        query.exec("PRAGMA journal_mode = MEMORY");
        query.exec(QString( "CREATE TABLE IF NOT EXISTS Settings"
                            "("
                            "ID INTEGER NOT NULL,"
                            "databasepath TEXT UNIQUE COLLATE NOCASE,"
                            "applicationName TEXT UNIQUE COLLATE NOCASE,"
                            "isConfigured INTEGER DEFAULT 0,"
                            "isScanned INTEGER  DEFAULT 0,"
                            "PRIMARY KEY (ID));")
                   );
        query.exec(
                    QString("INSERT OR IGNORE INTO Settings(databasePath,applicationName) VALUES('%1','%2');")
                    .arg(databasePath,appName));
        query.clear();
    }
    else
    {
        qDebug() << "Database::setConfig(): Database couldn't be opened" ;
        qDebug() << "Database::setConfig(): " << database.lastError() ;
    }

}

void Utils::Database::addToDatabase(TrackTag &track, QSqlQuery &query)
{

    query.prepare(
                "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist,title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                "VALUES (?, ?, ?,?, ?, ?,?, ?, ?,?, ?, ?,?,?,?)");
    query.bindValue(0, track.albumTitle);
    query.bindValue(1, track.artist);
    query.bindValue(2, track.albumArtist);
    query.bindValue(3, track.title);
    query.bindValue(4, track.genre);
    query.bindValue(5, track.path);
    query.bindValue(6, track.cover);
    query.bindValue(7, QString::number(track.position));
    query.bindValue(8, QString::number(track.length));
    query.bindValue(9, QString::number(track.playCount));
    query.bindValue(10,QString::number(track.liked));
    query.bindValue(11,QString::number(track.bitRate));
    query.bindValue(12,QString::number(track.year));
    query.bindValue(13,QString::number(addedDate));
    query.bindValue(14,QString::number(modifiedDate));
    query.exec();

}

void Utils::Database::addToDatabase(TrackTag &track)
{

    query.prepare(
                "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist,title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                "VALUES (?, ?, ?,?, ?, ?,?, ?, ?,?, ?, ?,?,?,?)");
    query.bindValue(0, track.albumTitle);
    query.bindValue(1, track.artist);
    query.bindValue(2, track.albumArtist);
    query.bindValue(3, track.title);
    query.bindValue(4, track.genre);
    query.bindValue(5, track.path);
    query.bindValue(6, track.cover/*.replace(QLatin1Char('\''), QLatin1String("''"))*/);
    query.bindValue(7, QString::number(track.position));
    query.bindValue(8, QString::number(track.length));
    query.bindValue(9, QString::number(track.playCount));
    query.bindValue(10,QString::number(track.liked));
    query.bindValue(11,QString::number(track.bitRate));
    query.bindValue(12,QString::number(track.year));
    query.bindValue(13,QString::number(addedDate));
    query.bindValue(14,QString::number(modifiedDate));
    query.exec();
}


void Utils::Database::scanSourceDirectory()
{
    /******************************************
     * If the source directory was not yet scanned,
     * we scann it, if not we simply look for changes
     * through launchUpdateThread()
     */

    if(isScanned == 0)
    {

        qDebug() << "Database::scanSourceDirectory(): source Directory was not scanned yet" ;
        if(database.isOpen())
        {
            std::lock_guard<std::mutex>dbLock(databaseMutex);
            //int i = 0;
            qDebug() << "Database::scanSourceDirectory(): Database successfuly opened" ;
            QDir::Filters filter = QDir::Dirs|QDir::Files | QDir::NoDotAndDotDot;
            TrackTag  track;
            QString tmppath;
            QFileInfo file;
            QDirIterator dirIter (sourceDirPath,audioFormatFilter, filter,QDirIterator::Subdirectories);
            QTime t;
            addedDate = QDateTime::currentDateTime().toTime_t();

            t.start();
            query.exec("BEGIN TRANSACTION");
            while(dirIter.hasNext())
            {
                tmppath = dirIter.next();
                track = TestTagReader::tagreader(tmppath);
                track.playCount = 0;
                track.liked = 0;
                file = QFileInfo(track.path);
                modifiedDate = file.lastModified().toTime_t();
                addToDatabase(track,query);
                //i++;
            }
            loggerTime = t.elapsed();
            query.exec("UPDATE Settings SET isScanned = 1 WHERE ID = 1;");
            query.exec("END TRANSACTION");
            query.clear();
            database.commit();

            //                qDebug("Time taken to write into the database : %d ms", loggerTime);
            //                qDebug() << "number of Tracks read : " << i;
            Q_EMIT databaseChanged();

        }

        else
        {
            qDebug() << "Error ScanDirectory: Couldn't open the database with connectionName : " << connectionName;
        }
    }
    else
    {
        launchUpdateThread();
    }
}

void Utils::Database::addToDatabase(Utils::Queue<TrackTag> &tracks)
{

    /** To fill the database , I have to write the tags in a fixed order:
     *  First :
     * Genre,Cover and Artist Tables should be added first, in any order between them.
     * That is because Album has a foreign keys referencing those tables:
     * coverID, genreID and artistID.
     *
     * Second :
     * Album
     *
     * Third :
     * Track
     */

    if(!tracks.empty())
    {
        int i = 0;
        if(database.open())
        {
            qDebug() << "Database successfuly opened" ;
            qDebug() << "There are in total : " << tracks.size() << " file Tracks to add.";
            query.exec("BEGIN TRANSACTION");
            QTime t;
            addedDate = QDateTime::currentDateTime().toTime_t();
            //modifiedDate = addedDate;
            t.start();
            while(!tracks.empty())
            {

                TrackTag track;
                tracks.try_pop(track);
                QFileInfo file(track.path);
                modifiedDate = file.lastModified().toTime_t();
                addToDatabase(track,query);
                i++;
            }
            loggerTime = t.elapsed();
            query.exec("END TRANSACTION");
            query.clear();
            database.commit();

            qDebug("Time taken to write into the database : %d ms", loggerTime);
            qDebug() << "number of Tracks read : " << i;
            Q_EMIT databaseChanged();
        }

        else
        {
            qDebug() << "Error  addToDatabase(Utils::Queue<TrackTag> &tracks): Couldn't open the database with connectionName : " << connectionName;
        }

    }
}


int Utils::Database::createPlaylist(QString playlistName)
{
    if(playlistName.isEmpty() || playlistName.compare( QString(" ")) == 0)
    {
        return -1;
    }
    //bool openSuccess = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    int id = 0;
    if(database.isOpen())
    {

        query.exec(QString("INSERT OR IGNORE INTO Playlist(title) VALUES ('%1');").arg(playlistName.replace(QLatin1Char('\''), QLatin1String("''"))));
        id = query.lastInsertId().toInt();
        query.clear();
        database.commit();
        Q_EMIT playlistsModelChanged();

    }
    else
    {
        qDebug() << "Error on Database::CreatePlaylist(): the database couldn't be opened.";
        id = -1;
    }
    return id;
}


bool Utils::Database::addToPlaylist(int trackID, int playlistID)
{
    if(trackID < 1 || playlistID < 1)
    {
        return false;
    }
    bool addSuccess = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);

    if(!database.isOpen())
    {
        database.open();
    }

    if(database.isOpen())
    {
         addSuccess = query.exec(QString("INSERT OR IGNORE INTO PlaylistTrack(playlistID,trackID) VALUES (%1, %2);").arg(QString::number(playlistID), QString::number(trackID)));
        Q_EMIT playlistsModelChanged();
        query.clear();
        database.commit();
    }
    else
    {
        qDebug() << "Error on Database::addToPlaylist(): the database couldn't be opened.";
    }
    return addSuccess;
}



bool Utils::Database::addToPlaylist(int trackID, QString playlist)
{
    if(trackID < 1 || playlist.isEmpty() || playlist == QString(" "))
    {
        return false;
    }
    int playlistID = -1;
    bool addSuccess = false;
    if(!database.isOpen())
    {
        database.open();
    }

    if(database.isOpen())
    {
        query.exec(QString("SELECT playlistID FROM Playlist WHERE title ='%1';").arg(playlist));
        if (query.isActive() && query.isValid())
        {
            while(query.next())
            {
                playlistID = query.value("playlistID").toInt();
            }
            query.clear();
        }
         addSuccess = addToPlaylist(trackID,playlistID);

    }

    else
    {
        qDebug() << "Error on Database::addToPlaylist(): the database couldn't be opened.";
    }
    return addSuccess;
}


bool Utils::Database::removePlaylist(int playlistID)
{
    if( playlistID < 1)
    {
        return false;
    }
    bool removeSuccess= false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
       database.open();
    }

    if(database.isOpen())
    {
        removeSuccess = query.exec(QString("DELETE FROM Playlist WHERE playlistID = %1;").arg(QString::number(playlistID)));
        query.clear();
        Q_EMIT playlistsModelChanged();
    }
    else
    {
        qDebug() << "Error on Database::removePlaylist(playlistID): the database couldn't be opened.";
    }
    return removeSuccess;
}

bool Utils::Database::removePlaylist(QString &playlistName)
{
    if(playlistName.isEmpty() || playlistName == QString(" "))
    {
        return false;
    }
    bool removeSuccess = false;
    if(!database.isOpen())
    {
         database.open();
    }

    if(database.isOpen())
    {
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        removeSuccess = query.exec(QString("DELETE FROM Playlist WHERE title = '%1';").arg(playlistName));
        query.clear();
        Q_EMIT playlistsModelChanged();
    }
    else
    {
        qDebug() << "Error on Database::removePlaylist(playlistName): the database couldn't be opened.";
    }
    return removeSuccess;
}

bool Utils::Database::removeFromPlaylist(int trackID, int playlistID)
{
    if(trackID < 1 || playlistID < 1)
    {
        return false;
    }
    bool removeSuccess = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
        database.open();
    }

    if(database.isOpen())
    {
        removeSuccess =  query.exec(QString("DELETE FROM PlaylistTrack WHERE playlistID = '%1' AND trackID = %2;").arg(QString::number(playlistID),QString::number(trackID)));
        query.clear();
    }
    else
    {
        qDebug() << "Error on Database::removePlaylist(playlistName): the database couldn't be opened.";
    }
    return removeSuccess;
}


bool Utils::Database::removeFromPlaylist(int trackID, QString playlist)
{
    if(trackID < 1 || playlist.isEmpty() || playlist == QString(" "))
    {
        return false;
    }
    bool removeSuccess = false;
    if(!database.isOpen())
    {
         database.open();
    }

    if(database.isOpen())
    {
        int playlistID = -1;
        query.exec(QString("SELECT playlistID FROM Playlist WHERE title = '%1';").arg(playlist));
        if(query.isActive())
        {
            while(query.next())
            {
                playlistID = query.value(0).toInt();
            }

        }

        removeSuccess = removeFromPlaylist(trackID,playlistID);
        query.clear();
    }
    else
    {
        qDebug() << "Error on Database::removeFromPlaylist(trackID,playlistID): the database couldn't be opened.";
    }

    return removeSuccess;
}

void Utils::Database::launchScanThread()
{
    directoryScannThread = std::thread(&Utils::Database::scanSourceDirectory,this);
    if(directoryScannThread.joinable())
        directoryScannThread.detach();
}

void Utils::Database::launchUpdateThread()
{
    updateThread = std::thread (&Utils::Database::update,this);
    if (updateThread.joinable())
        updateThread.detach();
}

/**
 * @brief Utils::Database::update gets first the last time a file was modified in the database
 * and store it in date.
 * This Date is the same value that QFileInfo.created() or QFileInfo.lastModified() returns.
 * This date will be used after , but first I get the list of the tracks Urls that is in the database
 * When an Url is no more valide I remove that track from the Database. When the url is a valide one
 * I store it in urlsFromDatabase.
 * Next I read this time all the tracks Urls from the soure directory using a QDirIterator.
 * When getting those Urls I compare the last time they were modified ( Remember QFileInfo returns
 * the same value for QFileInfo.lastModified() and QFileInfo.created() )  with date.
 * If the Date is bigger than date then they are modified files and I store them in changedFiles.
 * If not, I store them in urls. From now I know that the difference between the list I read from the
 * Database and the from fetched from the source directory are the new files.
 */
void Utils::Database::update()
{

    //bool openSuccess = false;
    std::vector<QString> changedFile,urls,urlsFromDatabase,fileToadd;
    QDir::Filters filter = QDir::Dirs|QDir::Files | QDir::NoDotAndDotDot;

    QFileInfo audioFile;
    /**
     * @brief databaseUpdated is used to checked whether we should emit
     * a signal telling that the database has changed.
     * This is needed since we check at start up whether or not there are
     * new changes in the source directory.
     * When we detect a change ( there are new files or some files were
     * deleted then this flag will be set to true.
     */
    bool databaseUpdated = false;
    //auto date = getLastModifiedDate();

    //rootPath.setFilter(filter);
    QDirIterator dirIter (sourceDirPath,audioFormatFilter, filter,QDirIterator::Subdirectories);

    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(database.isOpen())
    {
        query = QSqlQuery(database);
        QSqlQuery delQuery = QSqlQuery(database);
        query.exec(QString("SELECT trackUrl,albumTitle,cover FROM BaseTableTracks ORDER BY albumTitle;"));

        if(query.isActive())
        {
            QString path ;
            QString album;
            QString cover;
            delQuery.exec("BEGIN TRANSACTION");
            while(query.next())
            {
                path = query.value(QString("trackUrl")).toString();
                cover = query.value(QString("cover")).toString();
                album = query.value(QString("albumTitle")).toString();
                if(!QFile::exists(path))
                {

                    delQuery.exec(QString("DELETE FROM BaseTableTracks WHERE trackUrl ='%1';").arg(path.replace(QLatin1Char('\''), QLatin1String("''"))));
                    databaseUpdated = true;
                }
                else
                {
                    urlsFromDatabase.push_back(path);
                }
            }
            delQuery.exec("END TRANSACTION");
            delQuery.clear();
            query.clear();
            database.commit();
        }

        // get the new tracks and add them into the database.
        QString path;
        while(dirIter.hasNext())
        {
            path = dirIter.next();
            audioFile.setFile(path);
            urls.push_back(path);
            //            if(audioFile.lastModified().toTime_t() > date)
            //            {
            //                /***
            //                 * Changed Files are those the for which the url
            //                 * are still valide but only the metadata have changed.
            //                 */
            //                qDebug()<< "Utils::Database::update(): Changed File -  " << path ;
            //                changedFile.push_back(path);
            //            }
            //            else
            //            {
            //                urls.push_back(path);
            //            }
        }

        /***
         *  I compute the difference between the urls I obtained from the database
         * with the ones I got from the source directory and store the result in fileToadd
         * and then add all the urls present in fileToadd into the Database.
         *
         * */
        std::sort(urls.begin(), urls.end());
        std::sort(urlsFromDatabase.begin(), urlsFromDatabase.end());
        std::set_difference(urls.begin(), urls.end(),
                            urlsFromDatabase.begin(), urlsFromDatabase.end(),
                            std::inserter(fileToadd, fileToadd.begin()));


        if(!fileToadd.empty())
        {
            Utils::TrackTag track;
            databaseUpdated = true;
            addedDate = QDateTime::currentDateTime().toTime_t();
            query.exec("BEGIN TRANSACTION");
            for(auto file: fileToadd)
            {

                //qDebug()<< "Utils::Database::update(): File To Add-  " << file ;
                modifiedDate = QFileInfo(file).lastModified().toTime_t();
                track = TestTagReader::tagreader(file);
                track.playCount = 0;
                track.liked = 0;
                addToDatabase(track,query);
            }
            query.exec("END TRANSACTION");
            query.clear();
            urls.clear();
            urlsFromDatabase.clear();
            fileToadd.clear();
            database.commit();

        }
        else
        {
            qDebug()<< "Utils::Database::update(): nothing to add " ;
        }

    }
    else
    {
        qDebug() << "Error on Database::update(): the database couldn't be opened.";
    }
    qDebug() << "Utils::Database::update(): update finished";
    if(databaseUpdated)
    {
        Q_EMIT databaseChanged();
    }
}

int Utils::Database::getLastAddedDate()
{

    // bool openSuccess = false;
    int date = -1;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if( database.isOpen())
    {
        query.exec(QString("SELECT MAX(addedDate) AS date FROM BaseTableTracks;"));
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.clear();
        }
        return date;
    }
    else
    {
        qDebug() << "Error on Database::getLastaddedDate: the database couldn't be opened.";
        return date;
    }
}


int Utils::Database::getLastModifiedDate()
{
    int date = -1;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(database.isOpen())
    {
        query.exec(QString("SELECT MAX(modifiedDate) AS date FROM BaseTableTracks;"));
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.clear();
        }
        query.clear();
        return date;
    }
    else
    {
        qDebug() << "Error on Database::getLastModifiedDate: the database couldn't be opened.";
        return date;
    }
}

std::vector<QString> Utils::Database::getTrackUrls()
{
    std::vector<QString> urls;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(database.isOpen())
    {
        query.exec(QString("SELECT trackUrl  FROM BaseTableTracks;"));
        if(query.isActive())
        {
            while(query.next())
            {
                urls.push_back(query.value("trackUrl").toString());
            }
            query.clear();
        }
        std::sort(urls.begin(), urls.end());
        return urls;
    }
    else
    {
        qDebug() << "Error on Database::getLastModifiedDate: the database couldn't be opened.";
        return urls;
    }
}


/**
 * @deprecated
 * @brief Utils::Database::setup
 */
void Utils::Database::setup()
{


    bool openSuccess = true;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
        openSuccess = database.open();
    }

    if(openSuccess)
    {
        qDebug() << "Succes on Database::setup(): the database could be opened.";
        query.exec("BEGIN TRANSACTION");
        query.exec(QString("INSERT OR IGNORE INTO Genre(genre) SELECT genre FROM BaseTableTracks GROUP BY genre;"));
        query.exec(QString("INSERT OR IGNORE INTO Cover(coverpath) SELECT cover FROM BaseTableTracks GROUP BY cover;"));
        query.exec(QString("INSERT OR IGNORE INTO Artist(artistName) SELECT albumArtist FROM BaseTableTracks GROUP BY albumArtist;"));
        query.exec(QString("CREATE TEMP TABLE  BaseAlbum  AS SELECT genre,albumArtist,cover,albumTitle,year,COUNT(albumTitle) AS tracks, SUM(length) AS duration FROM BaseTableTracks GROUP BY albumTitle;"));
        query.exec(QString("INSERT OR IGNORE INTO Album(genreID,artistID,coverID,albumTitle,year,tracks,duration) SELECT  Genre.genreID, Artist.artistID, Cover.coverID , BaseAlbum.albumTitle, BaseAlbum.year,BaseAlbum.tracks, BaseAlbum.duration FROM Genre,Artist,Cover, BaseAlbum WHERE Genre.genre=BaseAlbum.genre AND Artist.artistName=BaseAlbum.albumArtist AND Cover.coverpath=BaseAlbum.cover;"));
        query.exec("END TRANSACTION");
        database.commit();
        query.clear();

        Q_EMIT databaseChanged();
    }
    else
    {
        qDebug() << "Error on Database::setup(): the database couldn't be opened.";
    }
}


bool Utils::Database::addToFavorite(int trackID)
{
    if(trackID < 1)
    {
        return false;
    }
    bool addSuccess = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
        database.open();
    }

    if(database.isOpen())
    {
        addSuccess = query.exec(QString("UPDATE BaseTableTracks SET favorite = 1 WHERE trackID = %1").arg(QString::number(trackID)));
        query.clear();
        database.commit();
        Q_EMIT databaseChanged();
    }
    else
    {
        qDebug() << "Error on Database::AddToFavorite(): the database couldn't be opened.";
    }
    return addSuccess;
}

bool Utils::Database::removeFromFavorite(int trackID)
{
    if(trackID < 1)
    {
        return false;
    }
    bool removeSuccess = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
        database.open();
    }

    if(database.isOpen())
    {
        removeSuccess = query.exec(QString("UPDATE BaseTableTracks SET favorite = 0 WHERE trackID = %1").arg(QString::number(trackID)));
        query.clear();
        database.commit();
        Q_EMIT databaseChanged();
    }
    else
    {
        qDebug() << "Error on Database::removeFromFavorite(): the database couldn't be opened.";
    }

    return removeSuccess;
}
bool Utils::Database::playCountUp(int trackID)
{
    if(trackID < 1)
    {
        return false;
    }
    bool error = false;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(!database.isOpen())
    {
       database.open();
    }

    if(database.isOpen())
    {
        error =  query.exec(QString("UPDATE BaseTableTracks SET playCount = playCount + 1  WHERE trackID = %1;").arg(QString::number(trackID)));
        database.commit();
        query.clear();
        Q_EMIT playCountChanged();
    }
    else
    {
        qDebug() << "Error on Database::playCount(): the database couldn't be opened.";
    }

    return error;
}
