#include "tagreader.h"

using namespace Utils;

TrackTag Utils::tagreaderTAGLIB(QString file)
{
    TagLib::FileRef f(file.toStdString().c_str());
    TagLib::Tag *mytag  = f.file()->tag();
    TrackTag track;

    // DEBUG ID3V2

    {
        qDebug() << "####### DEBUG ID3V2 START ######";
        qDebug() << mytag->album().toCString();
        qDebug() << mytag->artist().toCString();
        qDebug() << mytag->title().toCString();
        qDebug() << mytag->track();
        qDebug() << mytag->year();
        qDebug() << "####### DEBUG ID3V2 END ######";

    }
    if(!f.isNull() && f.tag())
    {
      TagLib::Tag  *tag = f.tag();
      track.title = QString( tag->title().toCString(true));
      track.path = file;
      track.artist = QString(tag->artist().toCString(true)) ;
      track.position = tag->track();
      track.albumTitle = tag->album().toCString(true);
      track.genre = tag->genre().toCString(true);
      track.year = tag->year();
      if(!f.audioProperties())
      {

        TagLib::AudioProperties *properties = f.audioProperties();
        track.length = properties->length() ;
      }
    }

    return track;
}

//TrackTag Utils::tagreaderID3LIB(QString file)
//{
//   ID3_Tag tag ;
//   TrackTag track;
//   ID3_Frame *frame ;

//   tag.Link((file.toStdString().c_str()));



//       track.path = file;
//       track.albumTitle = QString (ID3_GetAlbum(&tag));
//       track.artist = QString( ID3_GetArtist(&tag));
//       track.title = QString (ID3_GetTitle(&tag));
//       track.genre = QString(ID3_GetGenre(&tag));
//       // Length Frame
//       frame = tag.Find(ID3FID_SONGLEN);
//       if (NULL != frame)
//         {
//           char buffer[10];
//           track.length = frame->GetField(ID3FN_TEXT)->Get(buffer,30);
//         }
//       track.position = ID3_GetTrackNum(&tag);
//       track.year = QString(ID3_GetYear(&tag)).toInt();


//    // DEBUG ID3V2

//    {
//        qDebug() << "####### DEBUG ID3V2 START ######";
//        qDebug() << track.albumTitle;
//        qDebug() << track.artist;
//        qDebug() << track.title;
//        qDebug() << track.position;
//        qDebug() << track.year;
//        qDebug() << "####### DEBUG ID3V2 END ######";

//    }
//    return track;
//}



TrackTag Utils::tagreader(QString file)
{
    QFileInfo tmpfile (file);
    if(tmpfile.isFile())
    {
        QFileInfo audiofile(file);
        QString ext = audiofile.suffix().toLower();


        if (ext == "mp3")
        {
            auto track = tagreaderMPEG(file);
            track.cover = cover(file);
            return track;
        }

        if (ext == "flac")
        {
            auto track = tagreaderFLAC(file);
            track.cover = cover(file);
            return track;
        }


        if (ext == "wma")
        {
            auto track = tagreaderWMA(file);
            track.cover = cover(file);
            return track;
        }

        if (ext == "ogg")
        {
            auto track = tagreaderOGG(file);
            track.cover = cover(file);
            return track;
        }

        if ((ext == "m4a" )|| (ext == "mp4"))
        {
            auto track = tagreaderMP4(file);
            track.cover = cover(file);
            return track;
        }


        else
        {
             TrackTag error;
             qDebug()<< "ERROR: File type ."+ ext + " is not supported" ;
             return error;
        }
    }

    else
    {
        TrackTag error;
        error.length = -1;
        qDebug()<< "ERROR: " << file  << " is not a file";
        return error;
    }

}

// MP3 TAGS READER
TrackTag Utils::tagreaderMPEG(QString file)
{
    TagLib::MPEG::File audioFile (file.toStdString().c_str());
    TrackTag track;

    TagLib::MPEG::Properties *audioproperties = audioFile.audioProperties();
    qDebug("reading Tags from file");


    if (audioFile.hasID3v2Tag())
    {
        //qDebug ()  << "valide ID3V2 Tag for that file : " +  file ;
        auto framelistMap = audioFile.ID3v2Tag()->frameListMap();
        TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag();
//        track.albumTitle = tag->album().toCString();
//        track.artist = tag->artist().toCString();
//        track.genre = tag->genre().toCString();
//        track.path = file;
//        track.title = tag->title().toCString();
//        track.year = tag->year();
//        track.position = tag->track();
//        track.length = audioproperties->length();
        track.bitRate = audioproperties->bitrate();

        // read the tag

        auto list = framelistMap["TALB"];
        if(!list.isEmpty())
        {
            track.albumTitle = QString(list.front()->toString().toCString(true));
        }
        else
        {
            track.albumTitle = QString("UNKNOWN ALBUM TITLE");
        }
        list = framelistMap["TPE1"]; // track artist
        if(!list.isEmpty())
        {
            track.artist = QString(list.front()->toString().toCString(true));
            if(track.artist.count() > 30)
            {
                track.artist = QString("VARIOUS ARTISTS");
            }
        }
        else
        {
            track.artist = QString("UNKNOWN ARTIST");
        }
        list = framelistMap["TPE2"]; // album artist
        if(!list.isEmpty())
        {
            track.albumArtist = QString(list.front()->toString().toCString(true));
        }
        else
        {
            track.albumArtist = track.artist;
        }
        list = framelistMap["TCON"]; // genre music
        if(!list.isEmpty())
        {
            track.genre = QString(list.front()->toString().toCString(true));
        }
        else
        {
            track.genre = QString(tag->genre().toCString(true));
        }
        list = framelistMap["TIT2"]; // track title
        if(!list.isEmpty())
        {
            track.title = QString(list.front()->toString().toCString(true));
        }

        else
        {
            track.title = QString(tag->title().toCString(true));
        }


        track.year = tag->year();
        track.position = tag->track();
        track.length = audioproperties->length();
        track.bitRate = audioproperties->bitrate();
        tag = NULL;
        audioproperties = NULL;


    }

    else if(audioFile.hasID3v1Tag())
    {
        //qDebug ()  << "No valide ID3V2 Tag for that file : " +  file ;
        TagLib::ID3v1::Tag *tag = audioFile.ID3v1Tag();
        track.albumTitle = QString(tag->album().toCString());
        track.artist = QString(tag->artist().toCString());
        track.genre = QString (tag->genre().toCString());
        track.path = file;
        track.title = QString(tag->title().toCString());
        track.year = tag->year();
        track.position = tag->track();
        track.length = audioproperties->length();
        track.bitRate = audioproperties->bitrate();
        tag = NULL;
        audioproperties = NULL;

    }
    return track;
}


// FLAC TAGS READER
TrackTag Utils::tagreaderFLAC(QString file)
{
    TagLib::FLAC::File audioFile (file.toStdString().c_str());
    TrackTag track;

    TagLib::FLAC::Properties *audioproperties = audioFile.audioProperties();

    if (audioFile.hasID3v1Tag())
    {
        TagLib::ID3v1::Tag *tag = audioFile.ID3v1Tag();
        track.albumTitle = QString(tag->album().toCString(true));
        track.artist = QString(tag->artist().toCString(true));
        track.genre = QString (tag->genre().toCString(true));
        track.path = file;
        track.title = QString(tag->title().toCString(true));
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }

    else if(audioFile.hasID3v2Tag())
    {
        //qDebug ()  << "No valide ID3V2 Tag for that file : " +  file ;
        TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag();
        track.albumTitle = QString(tag->album().toCString(true));
        track.artist = QString(tag->artist().toCString(true));
        track.genre = QString (tag->genre().toCString(true));
        track.path = file;
        track.title = QString(tag->title().toCString(true));
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }
    else
    {
        TagLib::Ogg::XiphComment *tag  = audioFile.xiphComment();
        track.albumTitle = QString(tag->album().toCString(true));
        track.artist = QString(tag->artist().toCString(true));
        track.genre = QString (tag->genre().toCString(true));
        track.path = file;
        track.title = QString(tag->title().toCString(true));
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }
    return track;
}



// OGG TAGS READER
TrackTag Utils::tagreaderOGG(QString file)
{
    TagLib::Ogg::Vorbis::File audioFile (file.toStdString().c_str());
    TrackTag track;
    TagLib::Ogg::XiphComment *tag = audioFile.tag();
    TagLib::Ogg::Vorbis::AudioProperties *audioproperties = audioFile.audioProperties();

    if (tag)
    {
        track.albumTitle = QString(tag->album().toCString());
        track.artist = QString(tag->artist().toCString());
        track.genre = QString (tag->genre().toCString());
        track.path = file;
        track.title = QString(tag->title().toCString());
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }


    return track;
}


// MP4 TAGS READER
TrackTag Utils::tagreaderMP4(QString file)
{
    TagLib::MP4::File audioFile (file.toStdString().c_str());
    TrackTag track;
    TagLib::MP4::Tag * tag = audioFile.tag();
    TagLib::MP4::Properties *audioproperties = audioFile.audioProperties();

    if (tag)
    {
        track.albumTitle = QString(tag->album().toCString());
        track.artist = QString(tag->artist().toCString());
        track.genre = QString (tag->genre().toCString());
        track.path = file;
        track.title = QString(tag->title().toCString());
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }

    return track;
}


// WMA TAGS READER
TrackTag Utils::tagreaderWMA(QString file)
{
    TagLib::ASF::File audioFile (file.toStdString().c_str());
    TrackTag track;
    TagLib::ASF::Tag *tag = audioFile.tag();
    TagLib::ASF::Properties *audioproperties = audioFile.audioProperties();

    if (tag)
    {

        track.albumTitle = QString(tag->album().toCString());
        track.artist = QString(tag->artist().toCString());
        track.genre = QString (tag->genre().toCString());
        track.path = file;
        track.title = QString(tag->title().toCString());
        track.year = tag->year();
        track.position = tag->track();
        if(audioproperties)
        {
            track.length = audioproperties->length();
            track.bitRate = audioproperties->bitrate();
        }
        tag = NULL;
        audioproperties = NULL;
    }


    return track;
}

QString Utils::cover(QString audioFile)
{
    QFileInfo file(audioFile);
    QString path = file.path();
    QDirIterator workingDir (path, QStringList()<< "*.jpeg "<< "*.jpg" << "*.png" << "*.svg",QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);

    if (workingDir.hasNext())
    {

              return QString(workingDir.next());

    }
    else
        return QString("");
}
