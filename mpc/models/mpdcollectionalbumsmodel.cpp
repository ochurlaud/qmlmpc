
#include "mpdcollectionalbumsmodel.h"

MpdCollectionAlbumsModel::MpdCollectionAlbumsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "albumName";
    p_roles[Qt::UserRole+3] = "artist";
    m_artistName = "";
}

void MpdCollectionAlbumsModel::setAlbumList(QList<QSharedPointer<MpdAlbum> > list)
{
    beginResetModel();
    m_list.clear();
    m_selectedIndices.clear();
    for (int i=0; i<list.length(); i++) {
        m_list.append(list.at(i));
        m_selectedIndices.append(false);
    }
    endResetModel();
    emit selectionChanged();

}

bool MpdCollectionAlbumsModel::isSelected(int index) const
{
    if (index >= 0 && m_selectedIndices.size() > index) {
        return m_selectedIndices.at(index);
    } else {
        return false;
    }
}

int MpdCollectionAlbumsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_list.length();
}

int MpdCollectionAlbumsModel::getNumSelectedSongs() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Song)
            num ++;
    return num;
}

int MpdCollectionAlbumsModel::getNumSelectedDirectories() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++) {
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Directory) {
            num ++;
        }
    }
    return num;
}

QStringList MpdCollectionAlbumsModel::getSelectedPaths() const
{
    QStringList paths;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i])
            paths.append(m_list.at(i)->getName());
    return paths;
}

QVariant MpdCollectionAlbumsModel::data(const QModelIndex &index, int role) const
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
    case Qt::UserRole+3:
        return QVariant(m_list.at(index.row())->getArtist());
    }
    return MpdEntityListModel::data(index, role);


}

void MpdCollectionAlbumsModel::toggleSelection(int index)
{
    m_selectedIndices[index] = !m_selectedIndices[index];
    emit selectionChanged();
}

void MpdCollectionAlbumsModel::selectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = true;
    emit selectionChanged();
}

void MpdCollectionAlbumsModel::deselectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = false;
    emit selectionChanged();
}

void MpdCollectionAlbumsModel::queryContent()
{
    this->setAlbumList(QList<QSharedPointer<MpdAlbum> >());
    MpdRequest *request = m_mpdConnector->getConnection2()->listAlbums(m_artistName);
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionAlbumsModel::contentReady);
}

void MpdCollectionAlbumsModel::contentReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        QList<QSharedPointer<MpdAlbum> > albumList;
        for (auto& obj : response) {
            albumList.append(obj.dynamicCast<MpdAlbum>());
        }
        this->setAlbumList(albumList);
    } else {
        qDebug("listAlbum got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}
