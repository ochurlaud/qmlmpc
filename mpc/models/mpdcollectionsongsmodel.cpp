
#include "mpdcollectionsongsmodel.h"

MpdCollectionSongsModel::MpdCollectionSongsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "path";
    m_artistName = "";
    m_albumName = "";
}

void MpdCollectionSongsModel::setSongList(QList<QSharedPointer<MpdSong> > list)
{
    beginResetModel();
    m_list = QList<QSharedPointer<MpdSong> >();
    m_selectedIndices.clear();
    for (int i=0; i<list.length(); i++) {
        m_list.append(list.at(i));
        m_selectedIndices.append(false);
    }
    endResetModel();
    emit selectionChanged();

}

bool MpdCollectionSongsModel::isSelected(int index) const
{
    if (index >= 0 && m_selectedIndices.size() > index) {
        return m_selectedIndices.at(index);
    } else {
        return false;
    }
}

int MpdCollectionSongsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:m_list.length();
}

int MpdCollectionSongsModel::getNumSelectedSongs() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Song)
            num ++;
    return num;
}

int MpdCollectionSongsModel::getNumSelectedDirectories() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Directory)
            num ++;
    return num;
}

QStringList MpdCollectionSongsModel::getSelectedPaths() const
{
    QStringList paths;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i])
            paths.append(m_list.at(i)->getPath());
    return paths;
}

QVariant MpdCollectionSongsModel::data(const QModelIndex &index, int role) const
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
        return QVariant(m_list.at(index.row())->getPath());
    }
    return MpdEntityListModel::data(index, role);
}

void MpdCollectionSongsModel::toggleSelection(int index)
{
    m_selectedIndices[index] = !m_selectedIndices[index];
    emit selectionChanged();
}

void MpdCollectionSongsModel::selectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = true;
    emit selectionChanged();
}

void MpdCollectionSongsModel::deselectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = false;
    emit selectionChanged();
}

void MpdCollectionSongsModel::queryContent()
{
    this->setSongList(QList<QSharedPointer<MpdSong> >());
    MpdRequest* request;
    if (m_playlistName != "") {
        request = m_mpdConnector->getConnection2()->getPlaylistSongs(m_playlistName);
    } else {
        if (m_artistName != "" && m_albumName != "") {
            request = m_mpdConnector->getConnection2()->listSongsByArtistAndAlbum(m_artistName, m_albumName);
        } else if (m_artistName == "" && m_albumName != "" ) {
            request = m_mpdConnector->getConnection2()->listSongsByAlbum(m_albumName);
        }
        else {
            return;
        }
    }
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionSongsModel::contentReady);
}
void MpdCollectionSongsModel::contentReady()
{
    MpdRequest* request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        QList<QSharedPointer<MpdSong> > songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>());
        }
        this->setSongList(songList);
    } else {
        qDebug("listSong got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}
