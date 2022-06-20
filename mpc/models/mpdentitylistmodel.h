#ifndef MPDENTITYLISTMODEL_H
#define MPDENTITYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "mpc/types.h"

class MpdEntityListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MpdEntityListModel(QObject *parent);

    virtual void setEntityList(MpdEntityList list);

    virtual QHash<int, QByteArray> roleNames() const { return p_roles; }
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

protected:
    MpdEntityList m_list;
    QHash<int, QByteArray> p_roles;
};



#endif // MPDENTITYLISTMODEL_H
