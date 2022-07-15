
#include "mpdcollectionartistsmodel.h"

MpdCollectionArtistsModel::MpdCollectionArtistsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "path";
}

void MpdCollectionArtistsModel::setEntityList(MpdEntityList list)
{
    beginResetModel();
    m_list = MpdEntityList();
    m_selectedIndices.clear();
    for (int i=0; i<list.length(); i++) {
        MpdEntity::Type t = list.at(i)->getType();
        if (t == MpdEntity::Directory ||
            t == MpdEntity::Song ||
            t == MpdEntity::Artist ||
            t == MpdEntity::Album ||
            t == MpdEntity::Playlist) {
            m_list.append(list.at(i));
            m_selectedIndices.append(false);
        }
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
    if (role == Qt::UserRole+2) {
        return m_list.at(index.row())->getPath();
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


void MpdCollectionArtistsModel::queryContent()
{
    this->setEntityList(MpdEntityList());
    MpdRequest *request = m_mpdConnector->getConnection2()->listArtists();
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionArtistsModel::contentReady);
}

void MpdCollectionArtistsModel::contentReady()
{
       MpdRequest *request = qobject_cast<MpdRequest*>(sender());
       if (request->succesfull()) {
           QList<QSharedPointer<MpdObject>> response = request->getResponse();
           MpdEntityList entityList;
           for (auto obj : response) {
               entityList.append(obj.dynamicCast<MpdEntity>());
           }
           this->setEntityList(entityList);
       } else {
           qDebug("listArtists got an ACK: '%s'", qPrintable(request->getAck()));
       }
       request->deleteLater();
   }
