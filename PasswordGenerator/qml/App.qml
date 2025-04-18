import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

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

    AddWorkerDialog {
        id : addWorkerDialog
    }

    HorizontalHeaderView {
        x: 20
        id: tableHeader
        syncView: tableView
        acceptedButtons: Qt.NoButton

        delegate: Rectangle {
            border.color: "black"
            color: "light gray"
            implicitHeight: 30
            implicitWidth: 100

            MouseArea {
                anchors.fill: parent
                enabled: index < 2
                cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor

                onClicked: {
                    if (proxyWorkers.sortCol === index)
                    {
                        let order = proxyWorkers.sortOrdr === 1 ? 0 : 1;
                        proxyWorkers.sort(index, order)
                    }
                    else
                    {
                        proxyWorkers.sort(index, 0)
                    }
                }
            }

            Text {
                id: leftSortIndicator
                text: proxyWorkers.sortCol === index ? ">" : ""
                rotation: proxyWorkers.sortOrdr === 0 ? 90 : -90
                verticalAlignment: Text.AlignVCenter

                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                    leftMargin: 10
                }
            }

            Text {
                text: display
                anchors {
                    top: parent.top
                    left: leftSortIndicator.right
                    right: rightSortIndicator.left
                    bottom: parent.bottom
                }

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id: rightSortIndicator
                text: proxyWorkers.sortCol === index ? ">" : ""
                rotation: proxyWorkers.sortOrdr === 0 ? 90 : -90
                verticalAlignment: Text.AlignVCenter

                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: 10
                }
            }
        }
    }

    ScrollView {
        id: tableScrollView
        anchors {
            top: tableHeader.bottom
            left: parent.left
            right: parent.right
            bottom: buttons_row.top
            bottomMargin: 20
            leftMargin: 20
            rightMargin: 20
        }
        ScrollBar.horizontal.visible: false

        clip: true

        TableView {
            id: tableView
            width: root_window.width
            model: proxyWorkers
            columnWidthProvider: function(column) {
                switch (column) {
                case 1: return 120
                case 3: return 120
                default: return (root_window.width - 280) / 2
                }
            }

            delegate: Rectangle {
                border.color: "black"
                implicitHeight: 32

                Loader {
                    anchors.fill: parent
                    id: loader_text
                    sourceComponent: column != 3 ? component_text : component_actions

                    Component {
                        id: component_text
                        Text {
                            id: text
                            text: " " + display + " "
                            leftPadding: horizontalAlignment == Text.AlignLeft ? 20 : 0
                            horizontalAlignment: column == 1 ? Text.AlignHCenter : Text.AlignLeft
                            verticalAlignment:  Text.AlignVCenter

                            MouseArea {
                                enabled: column == 2 && text.text !== ""
                                anchors.fill: parent
                                cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor

                                onClicked: (mouse) => {
                                    workersModel.copyStringToClipboard(text.text)
                                    tooltip.x = mouse.x
                                    tooltip.y = mouse.y
                                    tooltip.open()
                                }
                            }

                            AnimatedToolTip {
                                id: tooltip
                                timeout: 1000

                                Text {
                                    text: "Текст скопирован в буфер обмена"
                                }
                            }
                        }
                    }

                    Component {
                        id: component_actions

                        Item {
                            Row {
                                id: actionsRow
                                anchors.centerIn: parent
                                height: 24
                                width: 24 * 3 + spacing * 2
                                spacing: 6
                                property int idx: row
                                property int realIdx: 1

                                Repeater {
                                    id: actionsRepeater
                                    model: ["qrc:/img/reload.png", "qrc:/img/edit.png", "qrc:/img/bin.png"]

                                    delegate: Image {
                                        width: 24
                                        height: 24
                                        source: actionsRepeater.model[index]

                                        MouseArea {
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            hoverEnabled: true

                                            onClicked: {
                                                switch (index) {
                                                case 0: proxyWorkers.resetPassword(actionsRow.idx); break;
                                                case 1: addWorkerDialog.open(actionsRow.idx); break;
                                                case 2: proxyWorkers.removeRows(actionsRow.idx, 1); break;
                                                }
                                            }

                                            ToolTip {
                                                visible: parent.containsMouse
                                                delay: 256

                                                Component.onCompleted: {
                                                    switch (index) {
                                                    case 0: text = "Обновить пароль"; break
                                                    case 1: text = "Изменить данные"; break
                                                    case 2: text = "Удалить сотрудника"; break
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        color: "transparent"
        border.color: "black"
        anchors.fill: tableScrollView
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

            onClicked: workersModel.resetAllPasswords()
        }

        Button {
            text: "Экспортировать в Excel"
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: "yellow"
            enabled: tableView.rows > 0

            onClicked: exportDialog.open()

            FileDialog {
                id: exportDialog
                fileMode: FileDialog.SaveFile
                nameFilters: ["Файлы таблиц (*.xlsx)", "Все файлы (*)"]
                defaultSuffix: "xlsx"

                onAccepted: {
                    workersModel.exportWorkers(selectedFile)
                }
            }
        }

        Button {
            text: "Добавить сотрудника"
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: "light green"

            onClicked: {
                addWorkerDialog.open(-1)
            }
        }
    }
}
