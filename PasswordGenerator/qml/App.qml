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
        // model: workersModel
        acceptedButtons: Qt.NoButton

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

                MouseArea {
                    anchors.fill: parent
                    preventStealing: true
                }
            }
        }
    }

    ScrollView {
        anchors {
            top: tableHeader.bottom
            left: parent.left
            right: parent.right
            bottom: buttons_row.top
        }

        clip: true

        TableView {
            id: tableView
            width: root_window.width
            model: proxyWorkers
            columnWidthProvider: function(column) {
                return (root_window.width) / 4
            }

            delegate: Text {
                id: text
                text: " " + display + " "
                topPadding: 5
                bottomPadding: 5
                width: 200
                visible: column != 3
                horizontalAlignment: column == 1 ? Text.AlignRight : Text.AlignLeft
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
            icon.width: 24
            icon.height: 24
            bgColor: "violet"
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
