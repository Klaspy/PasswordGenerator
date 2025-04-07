import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {
    id: control

    property int radius: 0
    property color bgColor: GlobalStyle.controlBgColor
    property color textColor: GlobalStyle.controlTextColor
    property font  textFont: GlobalStyle.controlFont

    onCheckableChanged: {
        checked = false
    }

    HoverHandler {
        cursorShape: control.hoverEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor
    }

    contentItem: Item {
        Grid {
            id: grid
            anchors.centerIn: parent
            columns: control.display == AbstractButton.TextBesideIcon ? 2 : 1
            rows: control.display == AbstractButton.TextUnderIcon ? 2 : 1
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter
            rowSpacing: control.spacing
            columnSpacing: control.spacing

            Component.onCompleted: {
                width = Qt.binding(function() {
                    if (control.display == AbstractButton.IconOnly)
                    {
                        return Math.min(icon.width, parent.width)
                    }
                    else if (control.display == AbstractButton.TextOnly)
                    {
                        return Math.min(text.width, parent.width)
                    }
                    else
                    {
                        return Math.min((control.display == AbstractButton.TextUnderIcon ?
                                             Math.max(icon.width, text.width) :
                                             icon.width + text.width), parent.width)
                    }
                })
                height = Qt.binding(function() {
                    if (control.display == AbstractButton.IconOnly)
                    {
                        return Math.min(icon.height, parent.height)
                    }
                    else if (control.display == AbstractButton.TextOnly)
                    {
                        return Math.min(text.height, parent.height)
                    }
                    else
                    {
                        return Math.min((control.display == AbstractButton.TextBesideIcon ?
                                             Math.max(icon.height, text.height) :
                                             icon.height + text.height), parent.height)
                    }
                })
            }

            Image {
                id: icon
                visible: control.display != AbstractButton.TextOnly
                source: control.icon.source
                width: source != "" ? control.icon.width : 0
                height: source != "" ?  control.icon.height : 0
                cache: control.icon.cache
            }

            Text {
                id: text
                visible: control.display != AbstractButton.IconOnly
                color: control.textColor
                text: control.text
                // anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font: control.textFont
            }
        }
    }

    background: Rectangle {
        id: background
        color: control.bgColor
        anchors.fill: control
        radius: control.radius

        Behavior on color {ColorAnimation {duration: 100}}
        Behavior on opacity {PropertyAnimation {duration: 50}}

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            samples: 17
            radius: 8
            color: "#50000000"
        }

        Rectangle {
            id: blackout
            color: "black"
            anchors.fill: parent
            radius: control.radius
            opacity: 0

            Behavior on opacity {PropertyAnimation {duration: 50}}
        }
    }

    states: [
        State {
            name: "default"
            when: control.enabled && !control.pressed && !control.checked && !control.flat
            PropertyChanges {target: blackout; opacity: 0}
            PropertyChanges {target: background; opacity: 1}
        },

        State {
            name: "clicked"
            when: control.pressed || control.checked
            PropertyChanges {target: blackout; opacity: 0.2}
        },

        State {
            name: "hovered"
            when: control.hoverEnabled && control.hovered
            PropertyChanges {target: blackout; opacity: 0.05}
        },

        State {
            name: "highlited"
            when: control.highlighted
        },

        State {
            name: "flat"
            when: control.flat
            PropertyChanges {target: background; opacity: 0}
        },

        State {
            name: "disabled"
            when: !control.enabled
            PropertyChanges {target: background; color: "light gray"}
        }
    ]
}
