import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 372
    height: 312

    Button {
        id: button_left
        x: 168
        y: 73
        width: 30
        height: 30
        text: "<-"
        checked: false

        objectName: "PU.Rx.key_Rotate_minus"
    }

    Button {
        id: button_right
        x: 221
        y: 73
        width: 30
        height: 30
        text: "->"
        checked: false

        objectName: "PU.Rx.key_Rotate_plus"
    }

    Button {
        id: button_menu
        x: 18
        y: 194
        width: 59
        height: 30
        text: "Menu"
        checkable: false
        objectName: "PU.Rx.key_Menu"
        checked: false
    }

    Button {
        id: button_usilenie
        x: 289
        y: 18
        width: 66
        height: 30
        text: "Usilenie"
        objectName: "PU.Rx.key_Gain"
        checked: false
        checkable: false
    }

    Button {
        id: button_minus_pz
        x: 186
        y: 152
        width: 65
        height: 30
        text: "- PZ"
        objectName: "PU.Rx.key_DecreaseFOV"
        checked: false
        checkable: false
    }

    Button {
        id: button_plus_pz
        x: 186
        y: 194
        width: 65
        height: 30
        text: "+ PZ"
        objectName: "PU.Rx.key_IncreaseFOV"
        checkable: false
        checked: false
    }

    Button {
        id: button_focus
        x: 110
        y: 18
        width: 59
        height: 30
        text: "Focus"
        objectName: "PU.Rx.key_Focus"
        checked: false
        checkable: false
    }

    Button {
        id: button_polyarnost
        x: 289
        y: 131
        width: 66
        height: 30
        text: "Polyarnost"
        objectName: "PU.Rx.key_Polarity"
        checkable: false
        checked: false
    }

    Button {
        id: button_menu_vyverka
        x: 18
        y: 265
        width: 103
        height: 30
        text: "Menu Vyverka"
        objectName: "PU.force_menu_vyverka"
        checkable: false
        checked: false
    }

    Button {
        id: button_menu_nastroika
        x: 132
        y: 265
        width: 103
        height: 30
        text: "Menu Nastroika"
        objectName: "PU.force_menu_nastroika"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox_right
        x: 262
        y: 80
        text: qsTr("")
        objectName: "PU.Rx.key_Rotate_plus"
    }

    CheckBox {
        id: checkBox_left
        x: 145
        y: 80
        text: qsTr("")
        objectName: "PU.Rx.key_Rotate_minus"
    }

    CheckBox {
        id: checkBox_cp_on
        x: 8
        y: 18
        text: qsTr("CP_ON")
         objectName: "PU.Rx.key_CP_ON"
    }

}
