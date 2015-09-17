#ifndef LOGGER_H
#define LOGGER_H

#include <QtSql>
#include <QQuickItem>
#include <QFileSystemWatcher>
#include <thread>
#include <mutex>
#include "queue.h"
#include "utils.h"
#include "tagreader.h"

namespace Utils
{

/**
     * @brief
     * defines a database to
     * control the Music collection. It provides an
     * Interface that provides some defined allowed actions.
     * Please see the METHOD documentations for more infos.
     */
    class Database: public QQuickItem
    {

        Q_OBJECT
    private:

        /**
         * @brief This variable is used only to mesure how long it takes to scan the whole
         * music directory
         */
        int loggerTime;

        /**
         * @brief  The date at which an Item is added into the Database.
         */
        int addedDate;

        /**
         * @brief The datae at which an Item is modified  into the Database.
         */
        int modifiedDate;

        /**
         * @brief The path to the database
         */
        QString databasePath;

        /**
         * @brief The Music directory's path.
         */
        QString sourceDirPath;

        /**
         * @brief The name of our connection to the Database.
         */
        QString connectionName;

        /**
         * @brief The list of the supperted audio Format. This is limited to
         * files from which I can read the meta tag.
         */
        QStringList audioFormatFilter;

        /**
         * @brief  The Database which stores the meta data of the audio files
         */
        QSqlDatabase database;

        /**
         * @brief The query's object to be used to execute a query instead of creating
         * everytime a new query object when  I want to add a new track into the Database
         */
        QSqlQuery query;

        /**
         * @brief updateThread runs update() in the background. I mainly use it to
         * update the database when the audio files's source Directory has changed.
         * Maybe I should use a future for that task.
         */
        std::thread updateThread;

        /**
         * @brief directoryScannThread runs scanSourceDirectory() in the background.
         */
        std::thread directoryScannThread;

        /**
         * @brief databaseMutex is used to lock the database when the database is being accessed
         * from one Thread.
         */
        std::mutex databaseMutex;

        /**
         * @brief directoryWatcher watches for any changes in the directory source.
         */
        QFileSystemWatcher *directoryWatcher;

        /**
         * @brief appName is the Application's Name
         */
        QString appName;

        /**
         * @brief configPath is the database configuration's path on the
         */
        QString configPath;

        /**
         * @brief playlistDirPath is the path where the Playlists are stored
         */
        QString playlistDirPath;

        /**
         * @brief isConfigured defined whether the Database is already configured.
         */
        int isConfigured;

        /**
         * @brief isScanned defined whether a  source directory is already scanned
         */
        int isScanned;

    public:
        /**
         * @brief
         * this constructor builds a database with a defined
         * structure if a database file doesn't already exists.
         * @param parent An used parameter. It is needed since Database
         * inherits from a QQuickItem.
         */
        Database(QQuickItem* parent=0);



        ~Database()
        {
            delete directoryWatcher;
            query.finish();
            database.close();
            QSqlDatabase::removeDatabase(connectionName);

        }

    public Q_SLOTS:

        /**
         * @brief setConfig set up the default configurations:
         * database path,
         * m3u playlist directory,
         * podcast directory.
         * the application home path.
         */
        void setConfig();

        /**
         * @brief databaseConfig set up the database tables structure.
         */
        void databaseConfig();

        /**
         * @brief  scans the source directory and
         * look for any audio file presents in this directory.
         * If it find audio files, it will read the tag(ID3 Tag) present
         * in the files and store them in the database
         */
        void scanSourceDirectory();

        /**
         * @brief  This method add the Track with the ID trackID into
         * the Playlist with the ID playlistID.
         * Only use this method if you are sure that
         * the ID of the playlist if valide since this method doesn't check whether the playlist's
         * ID is valid or not. Use instead the 2nd version of this method.
         * @param trackID The ID of the Track to be added.
         * @param playlistID The ID of the Playlist into which the Track
         * should be added.

         */

        bool addToPlaylist(int trackID, int playlistID);

        /**
         * @brief This method add the Track with the ID trackID into
         * the Playlist with the name playlist.
         * @param trackID The ID of the Track to be added.
         * @param playlist The name of the Playlist into which the Track
         * should be added.
         */
        bool addToPlaylist(int trackID, QString playlist);

        /**
         * @brief This Method create a Playlist with the name playlistName in
         * the Database and return the ID of the created Playlist.
         * @param playlistName
         * @return playlistID This methode return the ID of the created
         * Playlist.
         */
        int createPlaylist(QString playlistName);

        /**
         * @brief This method remove the playlist with ID playlistID from the
         * database. Only use this method if you are sure that the ID is a valid one. If you are
         * unsure ,please then use the 2nd variant of this method.
         * @param playlistID The ID of the Playlist to be removed from the Database.
         */
        bool removePlaylist(int playlistID);

        /**
         * @brief  This method remove the playlist with the name playlistName from the
         * database.
         * @param playlistName The name of the Playlist to be removed from the Database.
         */
        bool removePlaylist(QString& playlistName);

        /**
         * @brief This Method remove the Track with the ID trackID
         * from the Playlist with the ID playlistID.
         * @param trackID The ID of the Track to be removed from the Playlist.
         * @param playlistID The ID of the Playlist from which the Track should be removed
         * from.
         */
        bool removeFromPlaylist(int trackID, int playlistID);

        /**
         * @brief This Method remove the Track with the ID trackID
         * from the Playlist with the name playlist.
         * @param trackID The ID of the Track to be removed from the Playlist.
         * @param playlist The name of the Playlist from which the Track should be removed
         */
        bool removeFromPlaylist(int trackID, QString playlist);

        /**
         * @brief This Method update the Database content
         * based on what has changed in the  audio files source directory
         */
        void update();

        /**
         * @brief launchUpdateThread execute update() in the thread updateThread
         */
        void launchUpdateThread();

        /**
         * @brief launchScanThread execute scanSourceDirectory() in the thread directoryScannThread
         */
        void launchScanThread();
        /**
         * @brief This method increment the Track's table field playCount.
         * playCount represents how many times a Track has been played by the User
         * @param trackID The ID of the Track for which the field playCount should be
         * incremented
         */

        bool playCountUp(int trackID);

        /**
         * @brief This method marked the Track with ID trackID as favorite
         * @param trackID The ID of the Track to be marked as favorite
         */
        bool addToFavorite(int trackID);

        /**
         * @brief This Method unmarked the Track with ID trackID as favorite.
         * @param trackID The ID of the Track to be unmarked as favorite
         */
        bool removeFromFavorite(int trackID);

        /**
         * @brief This method fetch the date at which an Item was
         * added in the database. This value is used by Update() to determine which audio files
         * are new and which one are old.
         * @return addedDate The last date a track was added into the database in milliseconds
         */
        int getLastAddedDate();

        /**
         * @brief This method fetch the date at which an Item was
         * modified  in the database. This value is used by Update() to determine which audio files
         * are new and which one are old.
         * @return modifiedDate The last date a track was added into the database in milliseconds
         */
        int getLastModifiedDate();

        /**
         * @brief This Method a url list containing all the audio Track contained in the
         * database.
         * @return std::vector<QString>  The list of Urls of all the Tracks present in the database.
         */
        std::vector<QString> getTrackUrls();

        /**
         * @brief This Method set the database Tables and their contents : Album, Artist, Cover and Genre.
         * This Method is call right after scanSourceDirectory() has been called. It sis juste called once.
         */
        void setup();

        //void removeFromDatabase(Utils::TrackTag &track);
        //void removeFromDatabase(int id);
        bool isDatabaseConfigured()const;

    protected:
        /**
         * @brief This Method adds the Tracks' list tracks into
         * the Database.
         * @param tracks The list of tracks to be added into the Database.
         */
        void addToDatabase(Utils::Queue<Utils::TrackTag> &tracks);

        /**
         * @brief This Method adds a Track  into
         * the Database.
         * @param track The Track to be added into the Database
         */
        void addToDatabase(Utils::TrackTag &track);

        /**
         * @brief This Method adds a Track  into
         * the Database using a provided query instead of creating
         * a new Query object anytime it wants to add a Track into
         * the Database.
         * @param track The Track to be added into the Database
         * @param query The Query object to be used to proceed
         */
        void addToDatabase(TrackTag &track, QSqlQuery &query);

    Q_SIGNALS:
        /**
         * @brief This signal is emitted
         * when scanSourceDirectory() has finished scanning the music
         * Directory
         */
        void scanSourceDirectoryFinished();

        /**
         * @brief  This signal is emitted when the setup()
         * has finished setting up the Tables on the Database.
         */
        void setupFinished();

        /**
         * @brief This signal is emitted when
         * an update has finished.
         */
        void updateJobFinished();

        /**
         * @brief This signal is emitted when
         * a playlist has changed.
         */
        void playlistsModelChanged();

        /**
         * @brief This signal is emitted when the database has changed.
         * This signal is particularly used to update a View.
         */
        void databaseChanged();

        /**
         * @brief This signal is emitted when the field favorite of a Track
         * has changed.
         */
        void favoriteSongsChanged();

        /**
         * @brief This signal is emitted when the playCount field of a Track
         * has changed
         */
        void playCountChanged();

    };

}

#endif // LOGGER_H
