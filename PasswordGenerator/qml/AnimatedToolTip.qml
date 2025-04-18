import QtQuick
import QtQuick.Controls

ToolTip {
    id: root
    opacity: 0

    Behavior on opacity {NumberAnimation {duration: 200}}

    onOpened: {
        opacity = 1
        timer.start()
    }

    Timer {
        id: timer
        interval: root.timeout - 200 < 0 ? 0 : root.timeout - 200

        onTriggered: root.opacity = 0
    }
}
