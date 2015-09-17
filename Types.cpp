#include "Types.h"

FileSystemInfo::FileSystemInfo()
{
    itemsCount = 0;
    absolutepath = "";
    lastModifiedDate =0;
    foldercreatedDate = 0;
    folderlastReadDate = 0;
    folderIsDir = false;
    folderIsValide = false;

}

FileSystemInfo::FileSystemInfo(const QString &path)
{
    setPath(path);
}

void FileSystemInfo::setPath(QString path)
{
    if(!path.isEmpty())
    {
        absolutepath = path;

        QFileInfo fileInfo =QFileInfo(absolutepath);
        QStringList filter;
        filter << "*.mp3" <<"*.flac" << "*.ogg" <<"*.wav" << "*.wma" << "*.oga" << "*.aac" << "*.mp4" << "*.m4a" ;
        if(fileInfo.isDir())
        {
            QDir dir(fileInfo.dir());
            dir.setFilter(QDir::NoDotAndDotDot| QDir::Files);
            dir.setNameFilters(filter);
            setCount(dir.count());
            contents = dir.entryList();
            folderIsDir = true;
        }

        setCreated( fileInfo.created().toTime_t());
        setLastModified(fileInfo.lastModified().toTime_t());
        setLastRead (fileInfo.lastRead().toTime_t());
        folderIsValide = true;
    }


}


void FileSystemInfo::showInfo()
{
    if(folderIsValide)
    {
        if(folderIsDir)
        {
            qDebug() << "#########  Infos about " << absolutepath << " Folder #########";
            qDebug()<< "It contains a total of " << itemsCount << " folder(s)";
            qDebug()<< "The folder was created on : " << QDateTime::fromTime_t( foldercreatedDate) ;
            qDebug()<< "and it was lastly modified on : " << QDateTime::fromTime_t( lastModifiedDate);
            qDebug()<< "and it was lastly read on : " << QDateTime::fromTime_t( folderlastReadDate);
            //qDebug() << "Contents : " << contents;

            qDebug()<< "We have " << itemsCount <<" folder(s)" ;

        }

        else
        {
            qDebug() << "#########  Infos about " << absolutepath << " File #########";

            qDebug()<< "The file was created on : " << QDateTime::fromTime_t( foldercreatedDate );
            qDebug()<< "and it was lastly modified on : " << QDateTime::fromTime_t( lastModifiedDate);
            qDebug()<< "and it was lastly read on : " << QDateTime::fromTime_t( folderlastReadDate);
        }
    }
    else
    {
        qDebug()<< "No Infos available, Please check the file Path";
    }

}

int FileSystemInfo::count()const
{
    return itemsCount;
}

uint FileSystemInfo::created()const
{
    return foldercreatedDate;
}

uint FileSystemInfo::lastModified()const
{
    return lastModifiedDate;
}

uint FileSystemInfo::lastRead()const
{
    return folderlastReadDate;
}

bool FileSystemInfo::isDir()const
{
    return folderIsDir;
}

bool FileSystemInfo::isValide()const
{
    return folderIsValide;
}


void FileSystemInfo::setCount(int size)
{
    itemsCount = size;
}

void FileSystemInfo::setCreated(uint dateTime)
{
    foldercreatedDate = dateTime;
}

void FileSystemInfo::setLastModified(uint dateTime)
{
    lastModifiedDate = dateTime;
}

void FileSystemInfo::setLastRead(uint dateTime)
{
    folderlastReadDate = dateTime;
}

QStringList FileSystemInfo::folderEarlierThan(QString sourceFolder, QDateTime dateTime)
{
    QDir baseDir (sourceFolder);
    QStringList dirList ;
    baseDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    baseDir.setSorting(QDir::Time);
    QFileInfoList dirInfo = baseDir.entryInfoList();
    QFileInfoList::Iterator dirIter = dirInfo.begin();
    while(dirIter != dirInfo.end())
    {
        if((*dirIter).created() < dateTime)
        {
            dirList << (*dirIter).canonicalFilePath();

        }
        dirIter++;

    }

    return dirList;

}

QStringList FileSystemInfo::folderEarlierThan(QString sourceFolder, uint dateTime)
{

    QDateTime tmpDate = QDateTime::fromTime_t(dateTime);
    return folderEarlierThan(sourceFolder,tmpDate);
}

QStringList FileSystemInfo::folderLaterThan(QString sourceFolder, QDateTime dateTime)
{
    QDir baseDir (sourceFolder);
    QStringList dirList = QStringList("");
    baseDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    baseDir.setSorting(QDir::Time | QDir::Reversed);
    QFileInfoList dirInfo = baseDir.entryInfoList();
    QFileInfoList::Iterator dirIter = dirInfo.begin();
    while(dirIter != dirInfo.end())
    {
        if((*dirIter).created() > dateTime)
        {
            dirList << (*dirIter).canonicalFilePath();

        }
        dirIter++;
    }
    return dirList;
}


QStringList FileSystemInfo::folderLaterThan(QString sourceFolder, uint dateTime)
{
    QDateTime tmpDate = QDateTime::fromTime_t(dateTime);
    return folderLaterThan(sourceFolder,tmpDate);
}

//QStringList FileSystemInfo::dirList(QStringList filter, QString parent)
//{
//     QDir sourceDir(parent);
//     sourceDir.setNameFilters(filter);
//     sourceDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
//}
