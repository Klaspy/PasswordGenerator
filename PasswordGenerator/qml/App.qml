import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
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


    HorizontalHeaderView {
        id: tableHeader
        syncView: tableView
        model: workersModel

        delegate: Rectangle {
            border.color: "black"
            color: "light gray"
            implicitHeight: 30
            implicitWidth: 100

            Text {
                text: display
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
            }
        }
    }

    ScrollView {
        anchors {
            top: tableHeader.bottom
            left: parent.left
            right: parent.right
            bottom: buttons_row.top
            margins: 10
        }

        contentWidth: width - 20

        TableView {
            x: 0
            id: tableView
            width: root_window.width - 20
            model: workersModel
            resizableColumns: false
            columnWidthProvider: function(column) {
                return (root_window.width - 20) / 4
                // switch (column) {
                // case 0:
                // }
            }

            delegate: Text {
                id: text
                text: " " + display + " "
                topPadding: 5
                bottomPadding: 5
                width: 200
            }
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
