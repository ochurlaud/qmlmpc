#ifndef MPDCOLLECTIONMODEL_H
#define MPDCOLLECTIONMODEL_H

#include "mpdentitylistmodel.h"
#include <QtQml/qqmlregistration.h>
#include "mpc/mpdconnector.h"
#include "mpc/mpdrequest.h"
#include "mpc/musicplayerconnection.h"

class MpdCollectionModel : public MpdEntityListModel
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( int numSelectedSongs READ getNumSelectedSongs NOTIFY selectionChanged )
    Q_PROPERTY( int numSelectedDirectories READ getNumSelectedDirectories NOTIFY selectionChanged )
    Q_PROPERTY( MpdConnector* mpdConnector WRITE setMpdConnector READ getMpdConnector NOTIFY mpdConnectorChanged )


    QML_ELEMENT
public:
    MpdCollectionModel(QObject *parent=nullptr);

    virtual void setEntityList(MpdEntityList list);

    Q_INVOKABLE bool isSelected(int index) const;
    int getNumSelectedSongs() const;
    int getNumSelectedDirectories() const;
    Q_INVOKABLE QStringList getSelectedPaths() const;
    Q_INVOKABLE void listArtists() {
        this->setEntityList(MpdEntityList());
        MpdRequest *request = m_mpdConnector->getConnection2()->listArtists();
        connect(request, SIGNAL(resultReady()), SLOT(artistListingReady()));
    }
    void setMpdConnector(MpdConnector* mpdConnector) { m_mpdConnector=mpdConnector;}
    MpdConnector* getMpdConnector() { return m_mpdConnector;}

    virtual QVariant data(const QModelIndex &index, int role) const;

signals:
    void selectionChanged();
    void mpdConnectorChanged();
public slots:
    void toggleSelection(int index);
    void selectAll();
    void deselectAll();
    void artistListingReady() {
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

private:
    QList<bool> m_selectedIndices;
    MpdConnector* m_mpdConnector;
};

#endif // MPDCOLLECTIONMODEL_H
