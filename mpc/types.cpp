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

#include "types.h"

#include <QStringList>
#include <QDebug>

MpdStatus::MpdStatus() : MpdStatus::MpdStatus(QMultiHash<QByteArray,QByteArray>())
{

}

MpdStatus::MpdStatus(const QMultiHash<QByteArray,QByteArray>& hash)
{
    m_repeat = hash.value("repeat") == "1";
    m_random = hash.value("random") == "1";
    m_playlistVersion = hash.value("playlist", "-1").toUInt();
    m_playlistLength = hash.value("playlistlength", "0").toUInt();
    QString state = hash.value("state");
    if (state == "stop")
        m_state = Stop;
    else if (state == "play")
        m_state = Play;
    else if (state == "pause")
        m_state = Pause;
    else {
        qDebug() << "Could not parse state" << qPrintable(state);
        m_state = Unknown;
    }
    m_currentSongPos = hash.value("song", "-1").toInt();
    m_currentSongId = hash.value("songid", "-1").toInt();
    QString time = hash.value("time");
    if (time.contains(":")) {
        m_elapsedTime = time.section(':',0,0).toUInt();
        m_totalTime = time.section(':',1,1).toUInt();
    } else {
        m_elapsedTime = 0;
        m_totalTime = 0;
    }
    // ignore all other values, they aren't used anyway
}

bool MpdStatus::operator ==(MpdStatus &other)
{
    return (other.m_elapsedTime == m_elapsedTime && // put this one up front, because it is most likely to be the property that has changed
            other.m_repeat == m_repeat &&
            other.m_random == m_random &&
            other.m_playlistVersion == m_playlistVersion &&
            other.m_playlistLength == m_playlistLength &&
            other.m_state == m_state &&
            other.m_currentSongPos == m_currentSongPos &&
            other.m_currentSongId == m_currentSongId &&
            other.m_totalTime == m_totalTime);
}

MpdSong::MpdSong(const QMultiHash<QByteArray,QByteArray> &hash)
{
    m_path = hash.value("file");
    m_title = hash.value("title");
    m_artist = hash.value("artist");
    m_album = hash.value("album");
    m_duration = hash.value("time", "-1").toInt();
    m_pos = hash.value("pos", "-1").toInt();
    m_id = hash.value("id", "-1").toInt();
}

QString MpdSong::getDescription()
{
    if (m_title.isEmpty() || m_artist.isEmpty())
        return m_path.section('/', -1);
    return m_title+ " - " + m_artist;
}

MpdDirectory::MpdDirectory(const QMultiHash<QByteArray, QByteArray>& hash)
{
    m_path = hash.value("directory");
}

MpdPlaylist::MpdPlaylist(const QMultiHash<QByteArray,QByteArray>& hash)
{
    m_name = hash.value("playlist");
}

MpdArtist::MpdArtist(const QMultiHash<QByteArray,QByteArray>& hash)
{
    m_name = hash.value("Artist");
    if (m_name.isEmpty()) {
        m_name = "Unknown Artist";
    }
}


MpdAlbum::MpdAlbum(const QMultiHash<QByteArray,QByteArray>& hash)
{
    m_name = hash.value("Album");
}

QSharedPointer<MpdSong> MpdSongList::getSongById(int songId)
{
    QSharedPointer<MpdSong> ptr;
    foreach (ptr, *this)
        if (ptr->getPlaylistId()==songId)
            return ptr;
    return QSharedPointer<MpdSong>();
}

int MpdSongList::getSongIndex(int songId)
{
    for (int i=0; i<length(); i++)
        if (at(i)->getPlaylistId()==songId)
            return i;
    return -1;
}

MpdSongList MpdSongList::fromEntityList(MpdEntityList l)
{
    MpdSongList songList;
    for (int i=0; i<l.length(); i++)
        if (l.at(i)->getType()==MpdEntity::Song)
            songList.append(l.at(i).dynamicCast<MpdSong>());
    return songList;
}

