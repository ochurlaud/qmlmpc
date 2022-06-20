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

#include "mpdentitylistmodel.h"

MpdEntityListModel::MpdEntityListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    p_roles[Qt::UserRole] = "description";
    p_roles[Qt::UserRole+1] = "type";
}

void MpdEntityListModel::setEntityList(MpdEntityList list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
}

int MpdEntityListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:m_list.length();
}

QVariant MpdEntityListModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_list.length())
        return QVariant();
    switch(role) {
    case Qt::UserRole:
        return m_list.at(index.row())->getDescription();
    case Qt::UserRole+1:
        MpdEntity::Type type = m_list.at(index.row())->getType();
        switch(type) {        
        case MpdEntity::Artist:
            return "Artist";
        case MpdEntity::Directory:
            return "Directory";
        case MpdEntity::Song:
            return "Song";
        case MpdEntity::Playlist:
            return "Playlist";
        case MpdEntity::Album:
            return "Album";
        case MpdEntity::Status:
            return "Status";
        }
    }
    return QVariant();
}
