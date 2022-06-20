#ifndef MPDSONGLISTMODEL_H
#define MPDSONGLISTMODEL_H

#include <QAbstractListModel>
#include "mpc/types.h"

class MpdSongListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MpdSongListModel(QObject *parent);

    void setSongList(MpdSongList list);
    Q_INVOKABLE bool containsId(int songId) { return m_list.getSongById(songId) != nullptr; }
    Q_INVOKABLE int getSongIndex(int songId) { return m_list.getSongIndex(songId); }
    Q_INVOKABLE QVariant getData(int index, QString roleName) const;

    virtual QHash<int, QByteArray> roleNames() const { return p_roles; }
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

private:
    MpdSongList m_list;
    QHash<int, QByteArray> p_roles;
    QString m_filter;

};

#endif // MPDSONGLISTMODEL_H
