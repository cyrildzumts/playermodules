#ifndef TAGREADER_H
#define TAGREADER_H

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <taglib.h>
#include <tag.h>
#include <id3v2tag.h>
#include <id3v1tag.h>
#include <taglib/fileref.h>
#include <tfile.h>
#include <mpegfile.h> // for MPEG::File
#include <asffile.h> // WMA::File
#include <flacfile.h> // FLAC::File
#include <mp4file.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <oggflacfile.h>
#include <vorbisproperties.h>
#include <tpropertymap.h>
#include <tstringlist.h>

#include <QStringList>
#include <QTextStream>
#include "utils.h" // for TrackTag
#include <QDebug>

//ID3 LIB headers
//#include <id3/tag.h>
//#include <id3/misc_support.h>
#include <QDir>

namespace Utils
{

    //void tagreader(TagList *sameTag, TagList *diffTag, QDir parent);
    TrackTag tagreader( QString file);
    TrackTag tagreaderTAGLIB( QString file);
    TrackTag tagreaderFLAC(QString file);
    TrackTag tagreaderMPEG(QString file);
    TrackTag tagreaderOGG(QString file);
    TrackTag tagreaderMP4(QString file);
    TrackTag tagreaderWMA(QString file);

    // Testing ID3
    //TrackTag tagreaderID3LIB(QString file);
    QString cover(QString audioFile);
}

namespace TestTagReader
{
    Utils::TrackTag tagreader( const QString &file);
    //Utils::TrackTag readTag(TagLib::Tag& tag);
    //Utils::TrackTag readTag(TagLib::PropertyMap& propertymap);
    Utils::TrackTag readTag(TagLib::File& file);
    QString cover(QString audioFile);

}

#endif // TAGREADER_H
