#ifndef MPDCOLLECTIONPLAYLISTSMODEL_H
#define MPDCOLLECTIONPLAYLISTSMODEL_H

#include "mpdentitylistmodel.h"
#include <QtQml/qqmlregistration.h>
#include "mpc/mpdrequest.h"
#include "mpc/musicplayerconnection.h"

class MpdCollectionPlaylistsModel : public MpdEntityListModel
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( int numSelectedSongs READ getNumSelectedSongs NOTIFY selectionChanged )
    Q_PROPERTY( int numSelectedDirectories READ getNumSelectedDirectories NOTIFY selectionChanged )
    QML_ELEMENT

public:
    MpdCollectionPlaylistsModel(QObject *parent=nullptr);

    void setPlaylistList(QList<QSharedPointer<MpdPlaylist> > list);
    virtual int rowCount(const QModelIndex &parent) const;

    Q_INVOKABLE bool isSelected(int index) const;
    int getNumSelectedSongs() const;
    int getNumSelectedDirectories() const;
    Q_INVOKABLE QStringList getSelectedPaths() const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual void queryContent();

public slots:
    void toggleSelection(int index);
    void selectAll();
    void deselectAll();
    virtual void contentReady();

private:
    QList<QSharedPointer<MpdPlaylist> > m_list;

};

#endif // MPDCOLLECTIONPLAYLISTSMODEL_H
