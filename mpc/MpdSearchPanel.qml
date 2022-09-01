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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    property alias searchText: searchInputField.text
    property string searchScope: "any" // any, artist, title (https://mpd.readthedocs.io/en/latest/protocol.html#filter-syntax)

    function doSearch() {
        if (searchText && searchText.length >= 3)
            mpdConnector.search(searchText, searchScope)
    }
    onSearchTextChanged: doSearch()
    onSearchScopeChanged: doSearch()

    Column {
        anchors.fill: parent
        RowLayout {
            width: parent.width
            TextField {
                id: searchInputField
                Layout.fillWidth: true
                placeholderText: "search here..."
            }
            ComboBox {
                id: searchOptions
                textRole: "text"
                valueRole: "value"

                model: ListModel {
                    id: model
                    ListElement { value: "any"; text: "Any" }
                    ListElement { value: "artist"; text: "Artist" }
                    ListElement { value: "title"; text: "Title" }
                }
                onActivated: {
                    searchScope = currentValue
                }
            }
        }

        MpdCollectionSongsPanel {
            id: songlistView
            width: parent.width
            height: parent.height - searchOptions.height
            model: mpdConnector.searchResultModel
        }
    }
}
