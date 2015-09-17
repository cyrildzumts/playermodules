#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H
/**
 ****************************************************
 * Author   : Cyrille Ngassam Nkwenga
 * Email    : cyrildz@gmail.com
 * Date     : 01.05.2015
 * class : AlbumModel
 *
 * @brief
 * Description:
 * AlbumModel represents a list of albums present in the Database.
 * By default it represents all the albums present in the Database
 * It is based on QAbstractListModel so that it can be passed to any
 * viewer as model.
 * It implements the data() method from QAbstractListModel to present its
 * contents.
 *
 */

#include <QtSql>
#include <QAbstractListModel>
#include <vector>
#include "utils.h"


class AlbumModel: public QAbstractListModel
{
    Q_OBJECT

    public:
    /**
     * @brief The AlbumRole enum allows the viewer to
     * query the right data based on the provided role.
     */
    enum AlbumRole
    {

       AlbumTitleRole = Qt::UserRole,
       CoverpathRole,
       ArtistRole,
       YearRole,
       IDRole,
       TracksRole,
       DurationRole,
       GenreRole
    };
    explicit AlbumModel(QObject *parent = 0);

public Q_SLOTS:
    /**
      * @brief data() queries the corresponding information situated at index
      * based on the supplied role.
      * @return a variant object which represent the desired information
      */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    /**
     * @brief rowCount implement the rowCount() method from QAbstractListModel.
     * @param parent is unused.
     * @return the number of the albums contained in this model
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief getQueryText returns the current query command.
     * @return queryCmd the current query command.
     */
    QString getQueryText()const;

    /**
     * @brief refresh updates the current model
     */
    void refresh();

    /**
     * @brief queryArtistAlbum queries all albums from a given Artist
     * @param artist name of the artist we want to query the albums
     */
    void queryArtistAlbum(QString artist);

    /**
     * @brief getModel returns a pointer to this model
     * @return this, a pointer to this model
     */
    AlbumModel *getModel()
    {
        return this;
    }

    void debug();
Q_SIGNALS:

    void dataChanged();
protected:
    /**
      * @brief roleNames maps the role to a name so that the viewer can fetch
      * the needed information
      * @return an integer value that represent a role.
      */
     QHash<int,QByteArray> roleNames()const;
     void resetInternalData();

private:

     /**
      * @brief albumList is the list of albums
      */

     std::vector<Utils::Album> albumList;

     /**
      * @brief queryString is the command used to query the albums from the Database
      */

     QString queryString;

     QString queryArtistAlbumString;

     /**
      * @brief driver represents the driver used by the Database
      */

     QString driver;

     /**
      * @brief databaseName represents the Database path
      */

     QString databaseName;

     /**
      * @brief connectionName represents the connection used to connect to the
      * Database
      */

     QString connectionName;

     QString configPath;
};


#endif // ALBUMMODEL_H
