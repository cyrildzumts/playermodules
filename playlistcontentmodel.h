#ifndef PLAYLISTCONTENTMODEL_H
#define PLAYLISTCONTENTMODEL_H

#include <QAbstractListModel>
#include "tracklistmodel.h"
#include <QtSql>
#include "tagreader.h"
#include "utils.h"

// TO DO : add a remove function
class PlaylistContentModel : public TrackListModel
{
    Q_OBJECT
  public:
    PlaylistContentModel(QObject *parent = 0);
  public Q_SLOTS:
    void refresh(int playlistID);
    //bool removeRows(int row, int count, const QModelIndex &parent);
    void save();

};

#endif // PLAYLISTCONTENTMODEL_H
