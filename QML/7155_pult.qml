import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 500
    height: 400

    Button {
        id: button_left
        x: 95
        y: 157
        width: 30
        height: 30
        text: "<-"
        checked: false

        objectName: "PU.Rx.key_Vlevo"
    }

    Button {
        id: button_right
        x: 210
        y: 157
        width: 30
        height: 30
        text: "->"
        checked: false

        objectName: "PU.Rx.key_Vpravo"
    }

    Button {
        id: button3
        x: 140
        y: 157
        width: 59
        height: 30
        text: "Menu"
        checkable: false
        objectName: "PU.Rx.key_Menu_Enter"
        checked: false
    }

    Button {
        id: button_down
        x: 155
        y: 202
        width: 30
        height: 30
        text: "->"
        rotation: 90
        objectName: "PU.Rx.key_Vniz"
        checked: false
    }

    Button {
        id: button_up
        x: 155
        y: 113
        width: 30
        height: 30
        text: "->"
        checkable: false
        rotation: -90
        objectName: "PU.Rx.key_Vverh"
        checked: false
    }

    Button {
        id: button6
        x: 89
        y: 256
        width: 59
        height: 30
        text: "Obogrev"
        objectName: "PU.Rx.key_Obogrev"
        checked: false
        checkable: false
    }

    Button {
        id: button8
        x: 345
        y: 113
        width: 59
        height: 30
        text: "Usilenie"
        objectName: "PU.Rx.key_Usilenie"
        checked: false
        checkable: false
    }

    Button {
        id: button9
        x: 419
        y: 164
        width: 59
        height: 30
        text: "SHPZ"
        objectName: "PU.Rx.key_SHPZ"
        checked: false
        checkable: false
    }

    Button {
        id: button10
        x: 419
        y: 209
        width: 59
        height: 30
        text: "UPZ/UPZ2"
        objectName: "PU.Rx.key_UPZ_UPZ2"
        checkable: false
        checked: false
    }

    Button {
        id: button12
        x: 419
        y: 113
        width: 59
        height: 30
        text: "TV/TPV"
        objectName: "PU.Rx.key_TV_TPV"
        checkable: false
        checked: false
    }

    Button {
        id: button13
        x: 267
        y: 114
        width: 59
        height: 30
        text: "Focus"
        objectName: "PU.Rx.key_Focus"
        checked: false
        checkable: false
    }

    Button {
        id: button14
        x: 345
        y: 209
        width: 59
        height: 30
        text: "Polyarn"
        objectName: "PU.Rx.key_Polyarnost"
        checkable: false
        checked: false
    }

    CheckBox {
        id: checkBox1
        x: 119
        y: 73
        text: qsTr("SSZ")
        enabled: false
        objectName: "PU.Tx.Gotov_SSZ"
    }

    CheckBox {
        id: checkBox2
        x: 184
        y: 73
        text: qsTr("LD")
        enabled: false
        objectName: "PU.Tx.Gotov_LD"
    }

    CheckBox {
        id: checkBox3
        x: 247
        y: 73
        text: qsTr("TP")
        enabled: false
        objectName: "PU.Tx.Gotov_TP"
    }

    CheckBox {
        id: checkBox4
        x: 319
        y: 73
        text: qsTr("TV")
        enabled: false
        objectName: "PU.Tx.Gotov_TV"
    }

    CheckBox {
        id: checkBox5
        x: 89
        y: 233
        text: qsTr("Obogrev")
        enabled: false
        objectName: "PU.Tx.Obogrev"
    }

    CheckBox {
        id: checkBox6
        x: 27
        y: 303
        text: qsTr("Podsvetka")
        enabled: false
        objectName: "PU.Tx.Vkl_Light"
    }

    SpinBox {
        id: spinBox1
        x: 33
        y: 73
        width: 47
        height: 20
        value: 1
        prefix: ""
        maximumValue: 3

        signal changed()
        objectName: "PU.Rx.key_TPV"
        onValueChanged: changed()
    }

    Label {
        id: label1
        x: 33
        y: 54
        text: qsTr("TP Mode")
    }

    Button {
        id: button15
        x: 27
        y: 344
        width: 103
        height: 30
        text: "Menu Vyverka"
        objectName: "PU.force_menu_vyverka"
        checkable: false
        checked: false
    }

    Button {
        id: button16
        x: 141
        y: 344
        width: 103
        height: 30
        text: "Menu Nastroika"
        objectName: "PU.force_menu_nastroika"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox_up
        x: 192
        y: 120
        text: qsTr("")
        objectName: "PU.Rx.key_Vverh"
    }

    CheckBox {
        id: checkBox_right
        x: 251
        y: 164
        text: qsTr("")
        objectName: "PU.Rx.key_Vpravo"
    }

    CheckBox {
        id: checkBox_left
        x: 72
        y: 164
        text: qsTr("")
        objectName: "PU.Rx.key_Vlevo"
    }

    CheckBox {
        id: checkBox_down
        x: 190
        y: 209
        text: qsTr("")
        objectName: "PU.Rx.key_Vniz"
    }

    Button {
        id: button17
        x: 345
        y: 164
        width: 59
        height: 30
        text: "Kalibrovka"
        objectName: "PU.Rx.key_Kalibrovka"
        checked: false
        checkable: false
    }

    Button {
        id: button18
        x: 221
        y: 271
        width: 59
        height: 30
        text: "LD"
        objectName: "PU.Rx.key_LD"
        checked: false
        checkable: false
    }

    Button {
        id: button19
        x: 286
        y: 271
        width: 59
        height: 30
        text: "Standby"
        objectName: "PU.Rx.key_DR_TPV"
        checked: false
        checkable: false
    }

    Button {
        id: button20
        x: 351
        y: 271
        width: 59
        height: 30
        text: "TV2"
        objectName: "PU.Rx.key_TV2"
        checked: false
        checkable: false
    }

    Button {
        id: button21
        x: 416
        y: 271
        width: 59
        height: 30
        text: "Marka"
        objectName: "PU.Rx.key_Marka"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox7
        x: 113
        y: 303
        text: qsTr("VKL TPV")
        objectName: "PU.Tx.Vkl_TPV"
        enabled: false
    }

    Button {
        id: button22
        x: 251
        y: 344
        width: 103
        height: 30
        text: "Menu Regulirovka"
        objectName: "PU.force_menu_regulirovka"
        checkable: false
        checked: false
    }

}
