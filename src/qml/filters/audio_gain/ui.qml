/*
 * Copyright (c) 2013-2018 Meltytech, LLC
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
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import Shotcut.Controls 1.0

Item {
    width: 200
    height: 50
    property bool blockUpdate: true
    property double startValue: -54
    property double middleValue: 0.0
    property double endValue: -54

    Component.onCompleted: {
        if (filter.isNew) {
            // Set default parameter values
            filter.set('level', 0)
        } else {
            // Convert old version of filter.
            if (filter.getDouble('gain') !== 0.0) {
                filter.set('level', toDb(filter.getDouble('gain')))
                filter.resetProperty('gain')
            }

            middleValue = filter.getDouble('level', filter.animateIn)
            if (filter.animateIn > 0)
                startValue = filter.getDouble('level', 0)
            if (filter.animateOut > 0)
                endValue = filter.getDouble('level', filter.duration - 1)
        }
        setControls()
    }

    Connections {
        target: filter
        onInChanged: updateFilter(null)
        onOutChanged: updateFilter(null)
        onAnimateInChanged: updateFilter(null)
        onAnimateOutChanged: updateFilter(null)
    }

    Connections {
        target: producer
        onPositionChanged: {
            if (filter.animateIn > 0 || filter.animateOut > 0) {
                setControls()
            } else {
                blockUpdate = true
                gainSlider.value = filter.getDouble('level', getPosition())
                blockUpdate = false
                gainSlider.enabled = true
            }
        }
    }

    function toDb(value) {
        return 20 * Math.log(value) / Math.LN10
    }

    function getPosition() {
        return Math.max(producer.position - (filter.in - producer.in), 0)
    }

    function setControls() {
        var position = getPosition()
        blockUpdate = true
        gainSlider.value = filter.getDouble('level', position)
        blockUpdate = false
        gainSlider.enabled = position <= 0 || (position >= (filter.animateIn - 1) && position <= (filter.duration - filter.animateOut)) || position >= (filter.duration - 1)
    }

    function updateFilter(position) {
        if (blockUpdate) return

        if (position !== null) {
            if (position <= 0)
                startValue = gainSlider.value
            else if (position >= filter.duration - 1)
                endValue = gainSlider.value
            else
                middleValue = gainSlider.value
        }

        if (filter.animateIn > 0 || filter.animateOut > 0) {
            filter.resetProperty('level')
            gainKeyframesButton.checked = false
            if (filter.animateIn > 0) {
                filter.set('level', startValue, 0)
                filter.set('level', middleValue, filter.animateIn - 1)
            }
            if (filter.animateOut > 0) {
                filter.set('level', middleValue, filter.duration - filter.animateOut)
                filter.set('level', endValue, filter.duration - 1)
            }
        } else if (!gainKeyframesButton.checked) {
            filter.resetProperty('level')
            filter.set('level', middleValue)
        } else if (position !== null) {
            filter.set('level', gainSlider.value, position)
        }
    }

    GridLayout {
        columns: 4
        anchors.fill: parent
        anchors.margins: 8

        Label {
            text: qsTr('Level')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: gainSlider
            minimumValue: -50
            maximumValue: 24
            suffix: ' dB'
            decimals: 1
            onValueChanged: updateFilter(getPosition())
        }
        UndoButton {
            onClicked: gainSlider.value = 0.0
        }
        KeyframesButton {
            id: gainKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount('level') > 0
            onToggled: {
                if (checked) {
                    blockUpdate = true
                    filter.clearSimpleAnimation('level')
                    blockUpdate = false
                    filter.set('level', gainSlider.value, getPosition())
                } else {
                    filter.resetProperty('level')
                    filter.set('level', gainSlider.value)
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
