
#include "mpdcollectionplaylistsmodel.h"

MpdCollectionPlaylistsModel::MpdCollectionPlaylistsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "path";
}

void MpdCollectionPlaylistsModel::setPlaylistList(QList<QSharedPointer<MpdPlaylist> > list)
{
    beginResetModel();
    m_list = QList<QSharedPointer<MpdPlaylist> >();
    m_selectedIndices.clear();
    for (int i=0; i<list.length(); i++) {
        m_list.append(list.at(i));
        m_selectedIndices.append(false);
    }
    endResetModel();
    emit selectionChanged();

}

bool MpdCollectionPlaylistsModel::isSelected(int index) const
{
    if (index >= 0 && m_selectedIndices.size() > index) {
        return m_selectedIndices.at(index);
    } else {
        return false;
    }
}

int MpdCollectionPlaylistsModel::getNumSelectedSongs() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Song)
            num ++;
    return num;
}

int MpdCollectionPlaylistsModel::getNumSelectedDirectories() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Directory)
            num ++;
    return num;
}

QStringList MpdCollectionPlaylistsModel::getSelectedPaths() const
{
    QStringList paths;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i])
            paths.append(m_list.at(i)->getPath());
    return paths;
}

QVariant MpdCollectionPlaylistsModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_list.length())
         return QVariant();

    switch(role) {
    case Qt::UserRole:
        return m_list.at(index.row())->getDescription();
    case Qt::UserRole+1: {
        MpdEntity::Type type = m_list.at(index.row())->getType();
        switch(type) {
        case MpdEntity::Artist:
            return "Artist";
        case MpdEntity::Directory:
            return "Directory";
        case MpdEntity::Song:
            return "Song";
        case MpdEntity::Playlist:
            return "Playlist";
        case MpdEntity::Album:
            return "Album";
        case MpdEntity::Status:
            return "Status";
        }
        break;
    }
    case Qt::UserRole+2:
        return QVariant(m_list.at(index.row())->getName());
    default:
        return MpdEntityListModel::data(index, role);
    }
}

void MpdCollectionPlaylistsModel::toggleSelection(int index)
{
    m_selectedIndices[index] = !m_selectedIndices[index];
    emit selectionChanged();
}

void MpdCollectionPlaylistsModel::selectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = true;
    emit selectionChanged();
}

void MpdCollectionPlaylistsModel::deselectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = false;
    emit selectionChanged();
}

int MpdCollectionPlaylistsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:m_list.length();
}

void MpdCollectionPlaylistsModel::queryContent()
{
    this->setPlaylistList(QList<QSharedPointer<MpdPlaylist> >());
    MpdRequest *request = m_mpdConnector->getConnection2()->listPlaylists();
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionPlaylistsModel::contentReady);
}

void MpdCollectionPlaylistsModel::contentReady()
{
       MpdRequest *request = qobject_cast<MpdRequest*>(sender());
       if (request->succesfull()) {
           QList<QSharedPointer<MpdObject>> response = request->getResponse();
           QList<QSharedPointer<MpdPlaylist> > playlistList;
           for (auto& obj : response) {
               playlistList.append(obj.dynamicCast<MpdPlaylist>());
           }
           this->setPlaylistList(playlistList);
       } else {
           qDebug("listPlaylists got an ACK: '%s'", qPrintable(request->getAck()));
       }
       request->deleteLater();
   }
