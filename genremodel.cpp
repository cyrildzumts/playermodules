#include "genremodel.h"



GenreModel::GenreModel(QObject *parent):QAbstractListModel(parent)
{

    configPath = QDir::homePath() + "/.Player/config";
    databaseName = configPath + "/player.db";
    connectionName  = "Logger";
    queryCmd = QString("SELECT genre FROM BaseTableTracks GROUP BY genre ORDER BY genre;");
    QLocale::setDefault(QLocale(QLocale::AnyLanguage));
    refresh();
}

GenreModel::~GenreModel()
{
    genreList.clear();
}

void GenreModel::refresh()
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    Utils::Genre genre;
    genreList.clear();

    if(db.isOpen())
    {
        QSqlQuery genreQuery = QSqlQuery(db);

        genreQuery.exec(queryCmd);
        if(genreQuery.isActive())
        {

            while(genreQuery.next())
            {
                genre.genre = genreQuery.value("genre").toString();
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                genreList.push_back(genre);
                endInsertRows();

            }
            //Q_EMIT dataChanged();

        }
        else
        {
            qDebug() << "debugQuery is not active";
        }
        genreQuery.finish();
    }
}

QVariant GenreModel::data(const QModelIndex &index, int role) const
{

    // Version 3.
    if (index.row() < 0 || index.row() >= rowCount()  )
            return QVariant();

     else
    {
        Utils::Genre genre = genreList.at(index.row());
        switch (role)
        {
            case GenreNameRole:
                return genre.genre;

//            case GenreIDRole:
//                return genre.genreID;

            default:
                return QVariant();
        }
    }

}

QHash<int,QByteArray> GenreModel::roleNames() const
{
      QHash<int, QByteArray> roles;
      roles[GenreIDRole] = "genreID";
      roles[GenreNameRole] = "genre";
      return roles;
}

int GenreModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return genreList.size();
}

void GenreModel::resetInternalData()
{
    beginResetModel();
    genreList.clear();
    endResetModel();

}

