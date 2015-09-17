#ifndef ALBUM_H
#define ALBUM_H

#include <QQuickItem>

class Album : public QQuickItem
{
    Q_OBJECT

public:
    explicit Album( QString &title,
                    QString &coverpath,
                    QString &artist,
                    int year,
                    int ID,
                    QQuickItem *parent =0);

    Album (const Album& alb, QQuickItem *parent =0);

    QString artist() const;
    QString title()const;
    QString cover()const;
    int year()const;
    int albumID() const;

private:
    QString a_artist;
    QString a_title;
    QString a_cover;
    int a_year;
    int a_ID;
};

#endif // ALBUM_H
