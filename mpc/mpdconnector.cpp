/*
 * Copyright 2015, Maarten Sebregts <maartensebregts AT gmail DOT com>
 *
 * This file is part of qmlmpc.
 *
 * qmlmpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qmlmpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qmlmpc. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mpdconnector.h"

#include "musicplayerconnection.h"
#include "models/mpdentitylistmodel.h"
#include "models/mpdsonglistmodel.h"
#include "models/mpdcollectionartistsmodel.h"
#include "mpdrequest.h"

#define SIMPLE_REQUEST(func) MpdRequest *req = p_connection->func; if (req) connect(req, &MpdRequest::resultReady, p_connection, &MusicPlayerConnection::debugAndDelete);
#define MOVE_REQUEST(songId, direction) MpdSong *song = getSong(songId); if (song) { SIMPLE_REQUEST(move(song, direction)) } else { qDebug("Move: invalid songId"); }

MpdConnector::MpdConnector(QObject *parent) :
    QObject(parent)
{
    p_connection = new MusicPlayerConnection(this);
    connect(p_connection, &MusicPlayerConnection::statusChanged, this, &MpdConnector::statusUpdated);
    p_status = QSharedPointer<MpdStatus>(new MpdStatus()); //construct default status
    m_lastPlaylistVersion = -1; // force update when next status is ready
    p_queueModel = new MpdSongListModel(this);
    p_playlistSongsModel = new MpdSongListModel(this);
    p_searchResultModel = new MpdSongListModel(this);
}

QString MpdConnector::getNowPlaying()
{
    MpdSong *song = getSong(p_status->currentSongId());
    if (song)
        return song->getDescription();
    return "";
}

void MpdConnector::play() { SIMPLE_REQUEST(play()) }
void MpdConnector::pause() { SIMPLE_REQUEST(pause()) }
void MpdConnector::stop() { SIMPLE_REQUEST(stop()) }
void MpdConnector::next() { SIMPLE_REQUEST(next()) }
void MpdConnector::previous() { SIMPLE_REQUEST(previous()) }
void MpdConnector::repeat(bool repeat) { SIMPLE_REQUEST(repeat(repeat)) }
void MpdConnector::random(bool random) { SIMPLE_REQUEST(random(random)) }
void MpdConnector::consume(bool consume) { SIMPLE_REQUEST(consume(consume)) }
void MpdConnector::playSong(int songId) { SIMPLE_REQUEST(playSong(songId)) }
void MpdConnector::removeSong(int songId) { SIMPLE_REQUEST(removeSong(songId)) }
void MpdConnector::clearQueue() { SIMPLE_REQUEST(clearQueue()) }
void MpdConnector::savePlaylist(const QString& name) { SIMPLE_REQUEST(savePlaylist(name)) }
void MpdConnector::renamePlaylist(const QString& playlist, const QString& newName) { SIMPLE_REQUEST(renamePlaylist(playlist, newName)) }
void MpdConnector::removePlaylist(const QString& playlist) { SIMPLE_REQUEST(removePlaylist(playlist)) }
void MpdConnector::appendPlaylist(const QString& playlist) { SIMPLE_REQUEST(appendPlaylist(playlist)) }
void MpdConnector::playPlaylist(const QString& playlist) { SIMPLE_REQUEST(playPlaylist(playlist)) }
void MpdConnector::insertSong(const QString& path) { if (getCurrentSongId()==-1) { return qDebug("Cannot insert song: no song is playing."); } SIMPLE_REQUEST(insertSong(path)) }
void MpdConnector::appendSong(const QString& path) { SIMPLE_REQUEST(appendSong(path)) }
void MpdConnector::prependSong(const QString& path) { SIMPLE_REQUEST(prependSong(path)) }
void MpdConnector::insertSongs(const QStringList& paths) { SIMPLE_REQUEST(insertSongs(paths)) }
void MpdConnector::addSongs(const QStringList& paths) { SIMPLE_REQUEST(addSongs(paths)) }
void MpdConnector::moveSongUp(int songId) { MOVE_REQUEST(songId, MusicPlayerConnection::MoveOneUp) }
void MpdConnector::moveSongDown(int songId) { MOVE_REQUEST(songId, MusicPlayerConnection::MoveOneDown) }
void MpdConnector::moveSongFirst(int songId) { MOVE_REQUEST(songId, MusicPlayerConnection::MoveToFirst) }
void MpdConnector::moveSongLast(int songId) { MOVE_REQUEST(songId, MusicPlayerConnection::MoveToLast) }
void MpdConnector::moveSongAfterCurrent(int songId) { MOVE_REQUEST(songId, MusicPlayerConnection::MoveAfterCurrent) }

void MpdConnector::seek(int time)
{
    SIMPLE_REQUEST(seek(p_status->currentSongId(), time));
}

void MpdConnector::appendAlbum(const QString& artist, const QString& album, const bool playNow)
{
    MpdRequest *request = p_connection->listSongsByArtistAndAlbum(artist, album);
    if (playNow) {
        connect(request, &MpdRequest::resultReady, this, &MpdConnector::songsToAppendReadyAndPlay);
    } else {
        connect(request, &MpdRequest::resultReady, this, &MpdConnector::songsToAppendReady);
    }
}

void MpdConnector::songsToAppendReadyAndPlay()
{
    this->songsToAppendReady();
    this->play();
}

void MpdConnector::songsToAppendReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        QStringList songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>()->getPath());
        }
        this->addSongs(songList);
    } else {
        qDebug("listSongsByArtistAndAlbum got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

void MpdConnector::insertAlbum(const QString& artist, const QString& album)
{
    MpdRequest *request = p_connection->listSongsByArtistAndAlbum(artist, album);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::songsToInsertReady);
}

void MpdConnector::songsToInsertReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        QStringList songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>()->getPath());
        }
        this->insertSongs(songList);
    } else {
        qDebug("listSongsByArtistAndAlbum got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

/*
void MpdConnector::listDirectory(const QString& path)
{
    m_currentCollectionPath = path;
    emit currentCollectionPathChanged();
    p_collectionModel->setEntityList(MpdEntityList());
    MpdRequest *request = p_connection->listDirectory(path);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::directoryListingReady()));
}

void MpdConnector::listAlbums()
{
    this->listAlbums("");
}

void MpdConnector::listAlbums(const QString &artist)
{
    p_collectionModel->setEntityList(MpdEntityList());
    MpdRequest *request = p_connection->listAlbums(artist);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::albumListingReady()));
}

void MpdConnector::listSongsByArtistAndAlbum(const QString& artist, const QString& album)
{
    p_collectionModel->setEntityList(MpdEntityList());
    MpdRequest *request = p_connection->listSongsByArtistAndAlbum(artist, album);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::songListingReady()));
}

void MpdConnector::directoryListingReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        MpdEntityList entityList;
        for (auto obj : response) {
            entityList.append(obj.dynamicCast<MpdEntity>());
        }
        p_collectionModel->setEntityList(entityList);
    } else {
        qDebug("listDirectory got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}




void MpdConnector::songListingReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        MpdEntityList entityList;
        for (auto obj : response) {
            entityList.append(obj.dynamicCast<MpdEntity>());
        }
        p_collectionModel->setEntityList(entityList);
    } else {
        qDebug("listSong got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

void MpdConnector::listPlaylists()
{
    MpdRequest *request = p_connection->listPlaylists();
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::playlistsReady()));
}

void MpdConnector::playlistsReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        MpdEntityList entityList;
        for (auto obj : response) {
            entityList.append(obj.dynamicCast<MpdEntity>());
        }
        p_collectionModel->setEntityList(entityList);
    } else {
        qDebug("listPlaylists got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

void MpdConnector::listSongsByPlaylist(const QString& playlist)
{
    p_collectionModel->setEntityList(MpdEntityList());
    MpdRequest *request = p_connection->getPlaylistSongs(playlist);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::songListingReady()));
}*/

void MpdConnector::getPlaylistSongs(const QString& playlist)
{
    if (playlist.isEmpty())
        return p_playlistSongsModel->setSongList(MpdSongList());
    MpdRequest *request = p_connection->getPlaylistSongs(playlist);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::playlistSongsReady);
}

void MpdConnector::playlistSongsReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        MpdSongList songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>());
        }
        p_playlistSongsModel->setSongList(songList);
    } else {
        qDebug("listPlaylistSongs got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

void MpdConnector::search(const QString& query, const QString& scope)
{
    MpdRequest *request = p_connection->search(query, scope);
    connect(request, &MpdRequest::resultReady, this, &MpdConnector::searchResultsReady);
}

void MpdConnector::searchResultsReady()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (request->succesfull()) {
        QList<QSharedPointer<MpdObject>> response = request->getResponse();
        MpdSongList songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>());
        }
        p_searchResultModel->setSongList(songList);
    } else {
        qDebug("search got an ACK: '%s'", qPrintable(request->getAck()));
    }
    request->deleteLater();
}

void MpdConnector::statusUpdated(QSharedPointer<MpdStatus> status)
{
    p_status = status;
    emit statusChanged();
    emit currentSongChanged();
    if (status->playlistVersion() != m_lastPlaylistVersion) {
        // request the queue
        MpdRequest *request = p_connection->getQueue();
        connect(request, &MpdRequest::resultReady, this, &MpdConnector::queueReady);
        m_lastPlaylistVersion = status->playlistVersion();
    }
}

void MpdConnector::queueReady()
{
    MpdRequest *req = qobject_cast<MpdRequest*>(sender());
    if (!req || !req->succesfull()) {
        qDebug("getQueue() unsuccesfull");
        m_lastPlaylistVersion = -1;
        statusUpdated(p_status); // force updating the queue
    } else { // success :)
        QList<QSharedPointer<MpdObject>> response = req->getResponse();
        MpdSongList songList;
        for (auto& obj : response) {
            songList.append(obj.dynamicCast<MpdSong>());
        }
        m_queue = songList;
        p_queueModel->setSongList(m_queue);
    }
    if (req)
        req->deleteLater();
}

MpdSong *MpdConnector::getSong(int songId)
{
    if (songId==-1)
        return 0;
    return m_queue.getSongById(songId).data();
}
