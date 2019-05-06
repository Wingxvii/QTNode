/*
 * Copyright (c) 2014-2017 Meltytech, LLC
 * Author: Brian Matherly <code@brianmatherly.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import 'FilterMenu.js' as Logic

Rectangle {
    id: wrapper
    
    visible: isVisible
    height: visible ? Logic.ITEM_HEIGHT : 0
    color: activePalette.base

    SystemPalette { id: activePalette }

    Row {
        height: Logic.ITEM_HEIGHT

        Rectangle {
            color: activePalette.base
            anchors.top: parent.top
            width: parent.height
            height: parent.height

            ToolButton {
                id: favButton
                implicitWidth: 20
                implicitHeight: 18
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                opacity: favorite ? 1.0 : 0.3
                iconName: 'bookmarks'
                iconSource: 'qrc:///icons/oxygen/32x32/places/bookmarks.png'
                onClicked: favorite = !favorite
            }
        }

        Rectangle {
            id: itemBackground
            color: activePalette.base
            width: wrapper.ListView.view.width - favButton.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            
            ToolButton {
                id: itemIcon
                implicitWidth: 20
                implicitHeight: 18
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                enabled: false
                iconName: needsGpu ? 'cpu' : isAudio ? 'speaker' : 'video-television'
                iconSource: needsGpu ? 'qrc:///icons/oxygen/32x32/devices/cpu.png' : isAudio ? 'qrc:///icons/oxygen/32x32/actions/speaker.png' : 'qrc:///icons/oxygen/32x32/devices/video-television.png'
            }
            
            Label {
                id: itemText
                anchors.left: itemIcon.right
                anchors.right: itemBackground.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: name
                color: activePalette.text
                verticalAlignment: Text.AlignVCenter
            }
            
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: wrapper.height > 0
                onClicked: {
                    wrapper.ListView.view.itemSelected(index)
                }
                onEntered: {
                    itemBackground.color = activePalette.highlight
                    itemText.color = activePalette.highlightedText
                }
                onExited: {
                    itemBackground.color = activePalette.base
                    itemText.color = activePalette.text
                }
            }
        }
    }
}
