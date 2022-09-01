#ifndef MPDENTITYLISTMODEL_H
#define MPDENTITYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "mpc/types.h"
#include "mpc/mpdconnector.h"

class MpdEntityListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( MpdConnector* mpdConnector WRITE setMpdConnector READ getMpdConnector NOTIFY mpdConnectorChanged )

public:
    MpdEntityListModel(QObject *parent);

    void setMpdConnector(MpdConnector* mpdConnector) { m_mpdConnector=mpdConnector;}
    MpdConnector* getMpdConnector() { return m_mpdConnector;}

    virtual QHash<int, QByteArray> roleNames() const { return p_roles; }
    virtual int rowCount(const QModelIndex &parent) const {};
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual void queryContent() {}; // = 0;

signals:
    void mpdConnectorChanged();
    void selectionChanged();

protected slots:
    virtual void contentReady() {}; //) = 0;

protected:
    QList<bool> m_selectedIndices;
    MpdConnector* m_mpdConnector;
    QHash<int, QByteArray> p_roles;
};



#endif // MPDENTITYLISTMODEL_H
