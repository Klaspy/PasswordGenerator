import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15

import CustomStyle 1.0

Window {
    id: root_window

    width: 1280
    height: 720

    title: "Генератор паролей"
    visible: true

    readonly property font appFont: Qt.font({
                                                pixelSize: 20,
                                                family: "Roboto Mono"
                                            })

    Controls.ScrollView {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: buttons_row.top
            margins: 10
        }

        contentWidth: width

        TableView {
            width: parent.width
        }
    }

    RowLayout {
        id: buttons_row
        height: 50

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }

        Button {
            text: "Обновить все пароли"
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "qrc:/img/reload.png"
        }

        Button {
            text: "Экспортировать в Excel"
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: "yellow"
        }

        Button {
            text: "Добавить сотрудника"
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: "light green"
        }
    }
}
