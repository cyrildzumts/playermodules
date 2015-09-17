#ifndef UTILS_H
#define UTILS_H


#include <QString>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QDir>

namespace Utils
{
    struct Playlist
    {
        QString title;
        int ID;
    };

    struct TrackTag
    {
        QString title;
        QString albumTitle;
        QString artist;
        QString albumArtist;
        QString path;
        QString genre;
        QString lengthString;
        QString cover;
        int ID;
        int year ;
        int length ; // if length = -1 then it indicate that the track is not valide.
        int position ;
        int liked ; // By default a track is unlike
        int playCount ;
        int bitRate;
        int addedDate;
        int modifiedDate;
    };
    //Q_DECLARE_METATYPE(Utils::TrackTag)


    struct Album
    {
        int albumID;
        int genreID;
        int artistID;
        int coverID;
        int year;
        int tracks;
        int duration;
        QString albumTitle;
        QString genre;
        QString coverpath;
        QString albumArtist;

    };
    struct Artist
    {
        int artistID;
        QString name;
        QString description;
        QString cover;
    };

    struct Genre
    {
        int genreID;
        QString genre;

    };

    class Folder
    {
    public:
        Folder();
        Folder(QString dirPath);
        int count()const;
        QString path()const;
        int folderID()const;
        void setCount(int size);
        void setFolderID(int id);
        void setPath(QString dirPath);



    private:
        int itemcount ;
        int ID;
        //QString name;
        QString folderpath;
    };

    class FileSystemInfo
    {
    public:
        FileSystemInfo();
        FileSystemInfo(const QString &path);
        void showInfo();
        void setPath(QString path);
        int count()const;
        uint created()const;
        uint lastModified()const;
        uint lastRead()const;
        //static QStringList folderEarlierThan(QString sourceFolder, QString dateTime);
        static QStringList folderEarlierThan(QString sourceFolder, QDateTime dateTime);
        static QStringList folderEarlierThan(QString sourceFolder, const uint dateTime);
        //static QStringList folderLaterThan(QString sourceFolder, QString dateTime);
        static QStringList folderLaterThan(QString sourceFolder, QDateTime dateTime);
        static QStringList folderLaterThan(QString sourceFolder, const uint dateTime);
        //static QStringList dirList(QStringList filter, QString parent);
        bool isValide()const;
        bool isDir()const;
        //void showContentList();
        void setCount(int size);

     private:

        void setLastModified(uint dateTime);
        void setLastRead(uint dateTime);
        void setCreated(uint dateTime);



    private:
    uint foldercreatedDate;
    uint lastModifiedDate;
    uint folderlastReadDate;
    QString absolutepath ;
    QStringList contents;
    int itemsCount;
    bool folderIsValide;
    bool folderIsDir;

    };



}
#endif // UTILS_H
