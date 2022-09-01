#ifndef MPDCOLLECTIONSONGSMODEL_H
#define MPDCOLLECTIONSONGSMODEL_H

#include "mpdentitylistmodel.h"
#include <QtQml/qqmlregistration.h>

#include "mpc/mpdconnector.h"
#include "mpc/mpdrequest.h"
#include "mpc/musicplayerconnection.h"

class MpdCollectionSongsModel : public MpdEntityListModel
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( int numSelectedSongs READ getNumSelectedSongs NOTIFY selectionChanged )
    Q_PROPERTY( int numSelectedDirectories READ getNumSelectedDirectories NOTIFY selectionChanged )
    Q_PROPERTY( QString artist WRITE setArtist READ getArtist NOTIFY artistChanged )
    Q_PROPERTY( QString album WRITE setAlbum READ getAlbum NOTIFY albumChanged )
    Q_PROPERTY( QString playlist WRITE setPlaylist READ getPlaylist NOTIFY playlistChanged )

    QML_ELEMENT

public:
    MpdCollectionSongsModel(QObject *parent=nullptr);

    void setSongList(QList<QSharedPointer<MpdSong> > list);
    virtual int rowCount(const QModelIndex &parent) const;

    Q_INVOKABLE bool isSelected(int index) const;
    int getNumSelectedSongs() const;
    int getNumSelectedDirectories() const;
    Q_INVOKABLE QStringList getSelectedPaths() const;
    void setArtist(QString& artist) { m_artistName = artist;}
    QString getArtist() const { return m_artistName;}
    void setAlbum(QString& album) { m_albumName = album;}
    QString getAlbum() const { return m_albumName;}
    void setPlaylist(QString& playlist) { m_playlistName = playlist;}
    QString getPlaylist() const { return m_playlistName;}

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE virtual void queryContent();

signals:
    void artistChanged();
    void albumChanged();
    void playlistChanged();

public slots:
    void toggleSelection(int index);
    void selectAll();
    void deselectAll();
    virtual void contentReady();

private:
    QList<QSharedPointer<MpdSong> > m_list;
    QString m_artistName;
    QString m_albumName;
    QString m_playlistName;
};

#endif // MPDCOLLECTIONSONGSMODEL_H
