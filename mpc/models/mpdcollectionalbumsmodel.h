#ifndef MPDCOLLECTIONALBUMSMODEL_H
#define MPDCOLLECTIONALBUMSMODEL_H

#include "mpdentitylistmodel.h"
#include <QtQml/qqmlregistration.h>

#include "mpc/mpdconnector.h"
#include "mpc/mpdrequest.h"
#include "mpc/musicplayerconnection.h"

class MpdCollectionAlbumsModel : public MpdEntityListModel
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( int numSelectedSongs READ getNumSelectedSongs NOTIFY selectionChanged )
    Q_PROPERTY( int numSelectedDirectories READ getNumSelectedDirectories NOTIFY selectionChanged )
    Q_PROPERTY( MpdConnector* mpdConnector WRITE setMpdConnector READ getMpdConnector NOTIFY mpdConnectorChanged )
    Q_PROPERTY( QString artist WRITE setArtist READ getArtist NOTIFY artistChanged )
    QML_ELEMENT
public:
    MpdCollectionAlbumsModel(QObject *parent=nullptr);

    virtual void setEntityList(MpdEntityList list);

    Q_INVOKABLE bool isSelected(int index) const;
    int getNumSelectedSongs() const;
    int getNumSelectedDirectories() const;
    Q_INVOKABLE QStringList getSelectedPaths() const;
    void setArtist(QString &artist) { m_artistName = artist;}
    QString getArtist() const { return m_artistName;}

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual void queryContent();

signals:
    void artistChanged();

public slots:
    void toggleSelection(int index);
    void selectAll();
    void deselectAll();
    virtual void contentReady();

private:
    QString m_artistName;
};

#endif // MPDCOLLECTIONALBUMSMODEL_H
