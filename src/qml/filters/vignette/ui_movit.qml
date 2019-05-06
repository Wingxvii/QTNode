/*
 * Copyright (c) 2014-2015 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
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

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Shotcut.Controls 1.0

Item {
    width: 350
    height: 100
    Component.onCompleted: {
        if (filter.isNew)
            filter.savePreset(preset.parameters)
    }

    GridLayout {
        columns: 3
        anchors.fill: parent
        anchors.margins: 8

        Label {
            text: qsTr('Preset')
            Layout.alignment: Qt.AlignRight
        }
        Preset {
            id: preset
            Layout.columnSpan: 2
            parameters: ['radius', 'inner_radius']
            onPresetSelected: {
                radiusSlider.value = filter.getDouble('radius') * 100
                innerSlider.value = filter.getDouble('inner_radius') * 100
            }
        }

        Label {
            text: qsTr('Outer radius')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: radiusSlider
            minimumValue: 0
            maximumValue: 100
            suffix: ' %'
            value: filter.getDouble('radius') * 100
            onValueChanged: filter.set('radius', value / 100)
        }
        UndoButton {
            onClicked: radiusSlider.value = 30
        }

        Label {
            text: qsTr('Inner radius')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: innerSlider
            minimumValue: 0
            maximumValue: 100
            suffix: ' %'
            value: filter.getDouble('inner_radius') * 100
            onValueChanged: filter.set('inner_radius', value / 100)
        }
        UndoButton {
            onClicked: innerSlider.value = 30
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
