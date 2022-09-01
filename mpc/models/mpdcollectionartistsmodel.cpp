
#include "mpdcollectionartistsmodel.h"

MpdCollectionArtistsModel::MpdCollectionArtistsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "path";
}

void MpdCollectionArtistsModel::setArtistList(QList<QSharedPointer<MpdArtist> > list)
{
    beginResetModel();
    m_list = QList<QSharedPointer<MpdArtist> >();
    m_selectedIndices.clear();
    for (int i=0; i<list.length(); i++) {
        m_list.append(list.at(i));
        m_selectedIndices.append(false);
    }
    endResetModel();
    emit selectionChanged();

}

bool MpdCollectionArtistsModel::isSelected(int index) const
{
    if (index >= 0 && m_selectedIndices.size() > index) {
        return m_selectedIndices.at(index);
    } else {
        return false;
    }
}

int MpdCollectionArtistsModel::getNumSelectedSongs() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Song)
            num ++;
    return num;
}

int MpdCollectionArtistsModel::getNumSelectedDirectories() const
{
    int num = 0;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Directory)
            num ++;
    return num;
}

QStringList MpdCollectionArtistsModel::getSelectedPaths() const
{
    QStringList paths;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i])
            paths.append(m_list.at(i)->getPath());
    return paths;
}

QVariant MpdCollectionArtistsModel::data(const QModelIndex &index, int role) const
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

void MpdCollectionArtistsModel::toggleSelection(int index)
{
    m_selectedIndices[index] = !m_selectedIndices[index];
    emit selectionChanged();
}

void MpdCollectionArtistsModel::selectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = true;
    emit selectionChanged();
}

void MpdCollectionArtistsModel::deselectAll()
{
    for (int i=0; i<m_selectedIndices.length(); i++)
        m_selectedIndices[i] = false;
    emit selectionChanged();
}

int MpdCollectionArtistsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:m_list.length();
}

void MpdCollectionArtistsModel::queryContent()
{
    this->setArtistList(QList<QSharedPointer<MpdArtist> >());
    MpdRequest *request = m_mpdConnector->getConnection2()->listArtists();
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionArtistsModel::contentReady);
}

void MpdCollectionArtistsModel::contentReady()
{
       MpdRequest *request = qobject_cast<MpdRequest*>(sender());
       if (request->succesfull()) {
           QList<QSharedPointer<MpdObject>> response = request->getResponse();
           QList<QSharedPointer<MpdArtist> > artistList;
           for (auto& obj : response) {
               artistList.append(obj.dynamicCast<MpdArtist>());
           }
           this->setArtistList(artistList);
       } else {
           qDebug("listArtists got an ACK: '%s'", qPrintable(request->getAck()));
       }
       request->deleteLater();
   }
