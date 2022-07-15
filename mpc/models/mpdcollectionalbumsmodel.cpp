
#include "mpdcollectionalbumsmodel.h"

MpdCollectionAlbumsModel::MpdCollectionAlbumsModel(QObject *parent) :
    MpdEntityListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
    p_roles[Qt::UserRole+2] = "path";
    m_artistName = "";
}

void MpdCollectionAlbumsModel::setEntityList(MpdEntityList list)
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

bool MpdCollectionAlbumsModel::isSelected(int index) const
{
    if (index >= 0 && m_selectedIndices.size() > index) {
        return m_selectedIndices.at(index);
    } else {
        return false;
    }
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
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i] && m_list.at(i)->getType()==MpdEntity::Directory)
            num ++;
    return num;
}

QStringList MpdCollectionAlbumsModel::getSelectedPaths() const
{
    QStringList paths;
    for (int i=0; i<m_selectedIndices.length(); i++)
        if (m_selectedIndices[i])
            paths.append(m_list.at(i)->getPath());
    return paths;
}

QVariant MpdCollectionAlbumsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole+2) {
        return m_list.at(index.row())->getPath();
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
    this->setEntityList(MpdEntityList());
    MpdRequest *request = m_mpdConnector->getConnection2()->listAlbums(m_artistName);
    connect(request, &MpdRequest::resultReady, this, &MpdCollectionAlbumsModel::contentReady);
}

void MpdCollectionAlbumsModel::contentReady()
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
        qDebug("listAlbum got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}
