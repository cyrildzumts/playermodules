#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QtSql>
#include <QAbstractListModel>
#include "utils.h"

class ArtistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ArtistModel(QObject *parent = 0);
    ~ArtistModel();
    enum ArtistRole
    {
        ArtistNameRole = Qt::UserRole + 1,
        ArtistIDRole,
        ArtistCoverRole,
        DescriptionRole
    };

public Q_SLOTS:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    ArtistModel *getModel()
    {
        return this;
    }
    void refresh();

Q_SIGNALS:
    void dataChanged();
protected:
     QHash<int,QByteArray> roleNames()const;
     void resetInternalData();
private:
    std::vector<Utils::Artist> artistList;
    QString  queryCmd;
    QString databaseName;
    QString configPath;
    QString connectionName;

};

#endif // ARTISTMODEL_H
