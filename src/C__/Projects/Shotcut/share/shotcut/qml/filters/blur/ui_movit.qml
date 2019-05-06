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
    property double startValue: 0.0
    property double middleValue: 3.0
    property double endValue: 0.0

    Component.onCompleted: {
        if (filter.isNew) {
            // Set default parameter values
            filter.set('radius', 3.0)
        } else {
            middleValue = filter.getDouble('radius', filter.animateIn)
            if (filter.animateIn > 0)
                startValue = filter.getDouble('radius', 0)
            if (filter.animateOut > 0)
                endValue = filter.getDouble('radius', filter.duration - 1)
        }
        setControls()
    }

    function getPosition() {
        return Math.max(producer.position - (filter.in - producer.in), 0)
    }

    function setControls() {
        var position = getPosition()
        blockUpdate = true
        slider.value = filter.getDouble('radius', position)
        blockUpdate = false
        slider.enabled = position <= 0 || (position >= (filter.animateIn - 1) && position <= (filter.duration - filter.animateOut)) || position >= (filter.duration - 1)
    }

    function updateFilter(position) {
        if (blockUpdate) return
        var value = slider.value

        if (position !== null) {
            if (position <= 0 && filter.animateIn > 0)
                startValue = value
            else if (position >= filter.duration - 1 && filter.animateOut > 0)
                endValue = value
            else
                middleValue = value
        }

        if (filter.animateIn > 0 || filter.animateOut > 0) {
            filter.resetProperty('radius')
            keyframesButton.checked = false
            if (filter.animateIn > 0) {
                filter.set('radius', startValue, 0)
                filter.set('radius', middleValue, filter.animateIn - 1)
            }
            if (filter.animateOut > 0) {
                filter.set('radius', middleValue, filter.duration - filter.animateOut)
                filter.set('radius', endValue, filter.duration - 1)
            }
        } else if (!keyframesButton.checked) {
            filter.resetProperty('radius')
            filter.set('radius', middleValue)
        } else if (position !== null) {
            filter.set('radius', value, position)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8

        RowLayout {
            Label { text: qsTr('Radius') }
            SliderSpinner {
                id: slider
                minimumValue: 0
                maximumValue: 99.99
                decimals: 2
                onValueChanged: updateFilter(getPosition())
            }
            UndoButton {
                onClicked: slider.value = 3.0
            }
            KeyframesButton {
                id: keyframesButton
                checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount('radius') > 0
                onToggled: {
                    if (checked) {
                        blockUpdate = true
                        filter.clearSimpleAnimation('radius')
                        blockUpdate = false
                        filter.set('radius', slider.value, getPosition())
                    } else {
                        filter.resetProperty('radius')
                        filter.set('radius', slider.value)
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
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
                slider.value = filter.getDouble('radius', getPosition())
                blockUpdate = false
                slider.enabled = true
            }
        }
    }
}
