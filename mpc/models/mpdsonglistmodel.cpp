#include "mpdsonglistmodel.h"

MpdSongListModel::MpdSongListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "path";
    p_roles[Qt::UserRole+2] = "title";
    p_roles[Qt::UserRole+3] = "artist";
    p_roles[Qt::UserRole+4] = "album";
    p_roles[Qt::UserRole+5] = "duration";
    p_roles[Qt::UserRole+6] = "songId";
    p_roles[Qt::UserRole+7] = "songPos";
}

void MpdSongListModel::setSongList(MpdSongList list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
}

QVariant MpdSongListModel::getData(int index, QString roleName) const
{
    int role = roleNames().key(roleName.toUtf8(), -1);
    if (role == -1)
        return QVariant();
    return data(createIndex(index, 0), role);
}

int MpdSongListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:m_list.length();
}

QVariant MpdSongListModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_list.length())
        return QVariant();
    switch(role) {
    case Qt::UserRole:
        return m_list.at(index.row())->getDescription();
    case Qt::UserRole+1:
        return m_list.at(index.row())->getPath();
    case Qt::UserRole+2:
        return m_list.at(index.row())->getTitle();
    case Qt::UserRole+3:
        return m_list.at(index.row())->getArtist();
    case Qt::UserRole+4:
        return m_list.at(index.row())->getAlbum();
    case Qt::UserRole+5:
        return m_list.at(index.row())->getDuration();
    case Qt::UserRole+6:
        return m_list.at(index.row())->getPlaylistId();
    case Qt::UserRole+7:
        return m_list.at(index.row())->getPlaylistPosition();
    }
    return QVariant();
}
