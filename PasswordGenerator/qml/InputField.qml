import QtQuick
import QtQuick.Controls

import CustomStyle

TextField {
    anchors {
        left: parent.left
        right: parent.right
        margins: 10
    }
    font: GlobalStyle.appFont
    color: "black"
    validator: RegularExpressionValidator {regularExpression: /[А-Я][а-я]*/}
    
    background: Item {
        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: 1
            color: "gray"
        }
    }
}
