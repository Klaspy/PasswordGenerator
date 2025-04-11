import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts

import CustomStyle

Window {
    modality: Qt.WindowModal
    flags: Qt.Dialog

    property int editingRow: -1
    title: (editingRow < 0 ? "Добавить нового" : "Изменить данные") + " сотрудника"

    width: 400
    height: 600

    function open(row) {
        editingRow = row
        console.log(editingRow)

        if (editingRow > -1)
        {
            var initData = proxyWorkers.data(proxyWorkers.index(editingRow, 0), workersModel.roleKey("allEditableRoles_"))
            secondName_field.text = initData[0]
            name_field.text       = initData[1]
            surname_field.text    = initData[2]
            cabinet_field.text    = initData[3]
        }

        show()
    }

    Column {
        id: column
        width: parent.width
        spacing: 10
        readonly property int textFieldHeight: 40

        Text {
            leftPadding: 10
            text: "Фамилия"
            height: column.textFieldHeight
            verticalAlignment: Text.AlignBottom
        }

        InputField {id: secondName_field}

        Text {
            leftPadding: 10
            text: "Имя"
            height: column.textFieldHeight
            verticalAlignment: Text.AlignBottom
        }

        InputField {id: name_field}

        Text {
            leftPadding: 10
            text: "Отчество"
            height: column.textFieldHeight
            verticalAlignment: Text.AlignBottom
        }

        InputField {id: surname_field}

        Text {
            leftPadding: 10
            text: "Кабинет"
            height: column.textFieldHeight
            verticalAlignment: Text.AlignBottom
        }

        InputField {
            id: cabinet_field
            validator: IntValidator {bottom: 0}
        }
    }

    RowLayout {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
        height: 50
        spacing: 10

        Button {
            text: editingRow < 0 ? "Добавить" : "Сохранить"
            bgColor: "light green"
            enabled: secondName_field.text !== "" && name_field.text !== "" && cabinet_field.text !== ""
            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked: {
                close()

                if (editingRow > -1)
                {
                    proxyWorkers.setData(proxyWorkers.index(editingRow, 0),
                                         [secondName_field.text, name_field.text, surname_field.text,
                                          Number(cabinet_field.text)], workersModel.roleKey("allEditableRoles_"))
                }

                secondName_field.text = ""
                name_field.text       = ""
                surname_field.text    = ""
                cabinet_field.text    = ""
            }
        }

        Button {
            text: "Отмена"
            bgColor: "red"
            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked: {
                secondName_field.text = ""
                name_field.text       = ""
                surname_field.text    = ""
                cabinet_field.text    = ""
                close()
            }
        }
    }
}
