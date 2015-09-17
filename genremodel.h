#ifndef GENREMODEL_H
#define GENREMODEL_H

#include <QtSql>
#include <QAbstractListModel>
#include "utils.h"

class GenreModel : public QAbstractListModel
{
public:
    GenreModel(QObject *parent = 0);
    ~GenreModel();
    enum GenreRole
    {
        GenreNameRole = Qt::UserRole + 1,
        GenreIDRole
    };

public Q_SLOTS:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void refresh();
    GenreModel *getModel()
    {
        return this;
    }

Q_SIGNALS:
    void dataChanged();
protected:
     QHash<int,QByteArray> roleNames()const;
     void resetInternalData();
private:
    std::vector<Utils::Genre > genreList;
    QString queryCmd;
    QString configPath;
    QString databaseName;
    QString connectionName;

};

#endif // GENREMODEL_H
