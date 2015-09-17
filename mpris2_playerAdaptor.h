/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -a mpris2_playerAdaptor org.mpris.MediaPlayer2.Player.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef MPRIS2_PLAYERADAPTOR_H_1405611734
#define MPRIS2_PLAYERADAPTOR_H_1405611734

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.mpris.MediaPlayer2.Player
 */
class PlayerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.mpris.MediaPlayer2.Player\">\n"
"    <property access=\"readwrite\" type=\"s\" name=\"ApplicationName\"/>\n"
"    <property access=\"read\" type=\"s\" name=\"PlaybackStatus\"/>\n"
"    <property access=\"readwrite\" type=\"s\" name=\"LoopStatus\"/>\n"
"    <property access=\"readwrite\" type=\"d\" name=\"Rate\"/>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Shuffle\"/>\n"
"    <property access=\"read\" type=\"a{sv}\" name=\"Metadata\">\n"
"      <annotation value=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName\"/>\n"
"    </property>\n"
"    <property access=\"readwrite\" type=\"d\" name=\"Volume\"/>\n"
"    <property access=\"read\" type=\"x\" name=\"Position\"/>\n"
"    <property access=\"read\" type=\"d\" name=\"MinimumRate\"/>\n"
"    <property access=\"read\" type=\"d\" name=\"MaximumRate\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanGoNext\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanGoPrevious\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanPlay\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanPause\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanSeek\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanControl\"/>\n"
"    <signal name=\"Seeked\">\n"
"      <arg direction=\"out\" type=\"x\" name=\"position\"/>\n"
"    </signal>\n"
"    <signal name=\"nextClicked\"/>\n"
"    <signal name=\"PreviousClicked\"/>\n"
"    <signal name=\"playClicked\"/>\n"
"    <signal name=\"stopClicked\"/>\n"
"    <signal name=\"positionChanged\">\n"
"      <arg direction=\"out\" type=\"i\"/>\n"
"    </signal>\n"
"    <method name=\"ApplicationName\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"setApplicationName\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"    </method>\n"
"    <method name=\"PlaybackStatus\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"LoopStatus\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"SetLoopStatus\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"Status\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Rate\">\n"
"      <arg direction=\"out\" type=\"d\"/>\n"
"    </method>\n"
"    <method name=\"SetRate\">\n"
"      <arg direction=\"in\" type=\"d\" name=\"rate\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"MaximumRate\">\n"
"      <arg direction=\"out\" type=\"d\"/>\n"
"    </method>\n"
"    <method name=\"MinimumRate\">\n"
"      <arg direction=\"out\" type=\"d\"/>\n"
"    </method>\n"
"    <method name=\"Shuffle\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"SetShuffle\">\n"
"      <arg direction=\"in\" type=\"b\" name=\"shuffle\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"CanGoNext\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"CanGoPrevious\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"CanPlay\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"CanPause\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"CanControl\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"Next\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Previous\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Pause\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"PlayPause\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Stop\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Play\">\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"Seek\">\n"
"      <arg direction=\"in\" type=\"x\" name=\"offset\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"SetPosition\">\n"
"      <arg direction=\"in\" type=\"o\" name=\"trackId\"/>\n"
"      <arg direction=\"in\" type=\"x\" name=\"offset\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"OpenUri\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"uri\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    PlayerAdaptor(QObject *parent);
    virtual ~PlayerAdaptor();

public: // PROPERTIES
    Q_PROPERTY(QString ApplicationName READ applicationName WRITE setApplicationName)
    QString applicationName() const;
    void setApplicationName(const QString &value);

    Q_PROPERTY(bool CanControl READ canControl)
    bool canControl() const;

    Q_PROPERTY(bool CanGoNext READ canGoNext)
    bool canGoNext() const;

    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    bool canGoPrevious() const;

    Q_PROPERTY(bool CanPause READ canPause)
    bool canPause() const;

    Q_PROPERTY(bool CanPlay READ canPlay)
    bool canPlay() const;

    Q_PROPERTY(bool CanSeek READ canSeek)
    bool canSeek() const;

    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    QString loopStatus() const;
    void setLoopStatus(const QString &value);

    Q_PROPERTY(double MaximumRate READ maximumRate)
    double maximumRate() const;

    Q_PROPERTY(QVariantMap Metadata READ metadata)
    QVariantMap metadata() const;

    Q_PROPERTY(double MinimumRate READ minimumRate)
    double minimumRate() const;

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    QString playbackStatus() const;

    Q_PROPERTY(qlonglong Position READ position)
    qlonglong position() const;

    Q_PROPERTY(double Rate READ rate WRITE setRate)
    double rate() const;
    void setRate(double value);

    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    bool shuffle() const;
    void setShuffle(bool value);

    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    double volume() const;
    void setVolume(double value);

public Q_SLOTS: // METHODS
    QString ApplicationName();
    bool CanControl();
    bool CanGoNext();
    bool CanGoPrevious();
    bool CanPause();
    bool CanPlay();
    QString LoopStatus();
    double MaximumRate();
    double MinimumRate();
    Q_NOREPLY void Next();
    Q_NOREPLY void OpenUri(const QString &uri);
    Q_NOREPLY void Pause();
    Q_NOREPLY void Play();
    Q_NOREPLY void PlayPause();
    QString PlaybackStatus();
    Q_NOREPLY void Previous();
    double Rate();
    Q_NOREPLY void Seek(qlonglong offset);
    Q_NOREPLY void SetLoopStatus(const QString &Status);
    Q_NOREPLY void SetPosition(const QDBusObjectPath &trackId, qlonglong offset);
    Q_NOREPLY void SetRate(double rate);
    Q_NOREPLY void SetShuffle(bool shuffle);
    bool Shuffle();
    Q_NOREPLY void Stop();
    //void setApplicationName(const QString &name);
Q_SIGNALS: // SIGNALS
    void PreviousClicked();
    void Seeked(qlonglong position);
    void nextClicked();
    void playClicked();
    void positionChanged(int in0);
    void stopClicked();
};

#endif