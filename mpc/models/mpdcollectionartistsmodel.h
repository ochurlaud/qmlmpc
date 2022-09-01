#ifndef MPDCOLLECTIONARTISTSMODEL_H
#define MPDCOLLECTIONARTISTSMODEL_H

#include "mpdentitylistmodel.h"
#include <QtQml/qqmlregistration.h>
#include "mpc/mpdrequest.h"
#include "mpc/musicplayerconnection.h"

class MpdCollectionArtistsModel : public MpdEntityListModel
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( int numSelectedSongs READ getNumSelectedSongs NOTIFY selectionChanged )
    Q_PROPERTY( int numSelectedDirectories READ getNumSelectedDirectories NOTIFY selectionChanged )
    QML_ELEMENT

public:
    MpdCollectionArtistsModel(QObject *parent=nullptr);

    void setArtistList(QList<QSharedPointer<MpdArtist> > list);
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
    QList<QSharedPointer<MpdArtist> > m_list;

};

#endif // MPDCOLLECTIONARTISTSMODEL_H
