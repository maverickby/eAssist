import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 500
    height: 500

    Button {
        id: button_left
        x: 95
        y: 157
        width: 30
        height: 30
        text: "<-"
        checked: false

        objectName: "PU.PUP.Rx.key_Vlevo"
    }

    Button {
        id: button_right
        x: 210
        y: 157
        width: 30
        height: 30
        text: "->"
        checked: false

        objectName: "PU.PUP.Rx.key_Vpravo"
    }

    Button {
        id: button3
        x: 140
        y: 157
        width: 59
        height: 30
        text: "Menu"
        checkable: false
        objectName: "PU.PUP.Rx.key_Menu_Enter"
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
        objectName: "PU.PUP.Rx.key_Vniz"
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
        objectName: "PU.PUP.Rx.key_Vverh"
        checked: false
    }

    Button {
        id: button6
        x: 89
        y: 256
        width: 59
        height: 30
        text: "Obogrev"
        objectName: "PU.PUP.Rx.key_Obogrev"
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
        objectName: "PU.PUP.Rx.key_Usilenie"
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
        objectName: "PU.PUP.Rx.key_SHPZ"
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
        objectName: "PU.PUP.Rx.key_UPZ_UPZ2"
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
        objectName: "PU.PUP.Rx.key_TV_TPV"
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
        objectName: "PU.PUP.Rx.key_Focus"
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
        objectName: "PU.PUP.Rx.key_Polyarnost"
        checkable: false
        checked: false
    }

    CheckBox {
        id: checkBox1
        x: 119
        y: 73
        text: qsTr("SSZ")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_SSZ"
    }

    CheckBox {
        id: checkBox2
        x: 184
        y: 73
        text: qsTr("LD")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_LD"
    }

    CheckBox {
        id: checkBox3
        x: 301
        y: 73
        text: qsTr("TP")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_TP"
    }

    CheckBox {
        id: checkBox4
        x: 245
        y: 73
        text: qsTr("TV")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_TV"
    }

    CheckBox {
        id: checkBox5
        x: 89
        y: 233
        text: qsTr("Obogrev")
        enabled: false
        objectName: "PU.PUP.Tx.Obogrev"
    }

    CheckBox {
        id: checkBox6
        x: 27
        y: 303
        text: qsTr("Podsvetka")
        enabled: false
        objectName: "PU.PUP.Tx.Vkl_Light"
    }

    SpinBox {
        id: spinBox1
        x: 33
        y: 73
        width: 47
        height: 20
        minimumValue: 0
        value: 1
        prefix: ""
        maximumValue: 3

        signal changed()
        objectName: "PU.PUP.Rx.tpv_control"
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
        text: "Engineering menu"
        objectName: "PU.force_engineering_menu"
        checkable: false
        checked: false
    }

    Button {
        id: button16
        x: 141
        y: 344
        width: 103
        height: 30
        text: "Main menu"
        objectName: "PU.force_main_menu"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox_up
        x: 192
        y: 120
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vverh"
    }

    CheckBox {
        id: checkBox_right
        x: 251
        y: 164
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vpravo"
    }

    CheckBox {
        id: checkBox_left
        x: 72
        y: 164
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vlevo"
    }

    CheckBox {
        id: checkBox_down
        x: 190
        y: 209
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vniz"
    }

    Button {
        id: button17
        x: 345
        y: 164
        width: 59
        height: 30
        text: "LD"
        objectName: "PU.PUP.Rx.key_LD"
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
        objectName: "PU.PUP.Rx.key_DR_TPV"
        checked: false
        checkable: false
    }

    Button {
        id: button20
        x: 351
        y: 271
        width: 59
        height: 30
        text: "Kalibrovka"
        objectName: "PU.PUP.Rx.key_Kalibrovka"
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
        objectName: "PU.PUP.Rx.key_Marka"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox7
        x: 113
        y: 303
        text: qsTr("VKL TPV")
        objectName: "PU.PUP.Tx.Vkl_TPV"
        enabled: false
    }

    Slider {
        id: sliderHorizontal1
        x: 63
        y: 429
        width: 422
        height: 22
        z: 1
        minimumValue: -4000
        maximumValue: 4000

        objectName: "PU.PUUP.Rx.PU_VN"
        signal changed()
        onValueChanged: changed()
    }

    Slider {
        id: sliderHorizontal2
        x: 63
        y: 462
        width: 422
        height: 22
        z: 2
        minimumValue: -4000
        maximumValue: 4000

        objectName: "PU.PUUP.Rx.PU_GN"
        signal changed()
        onValueChanged: changed()
    }

    Rectangle {
        id: rectangle1
        x: 8
        y: 380
        width: 484
        height: 112
        color: "#ffffff"
        border.color: "#110000"
    }

    Button {
        id: button23
        x: 416
        y: 314
        width: 59
        height: 30
        text: "GPO"
        objectName: "PU.PUP.Rx.key_GPO"
        checked: false
        checkable: false
    }

    Button {
        id: button24
        x: 15
        y: 390
        width: 59
        height: 30
        text: "PIL"
        objectName: "PU.PUUP.Rx.REGIM_PIL"
        checked: false
        checkable: true
    }

    Button {
        id: button25
        x: 81
        y: 390
        width: 87
        height: 30
        text: "PRIVOD OTKL"
        objectName: "PU.PUUP.Rx.PRIVOD_OTKL"
        checked: false
        checkable: true
    }

    Button {
        id: button26
        x: 174
        y: 390
        width: 59
        height: 30
        text: "IZM D"
        objectName: "PU.PUUP.Rx.IZM_D"
        checked: false
        checkable: false
    }

    Button {
        id: button27
        x: 239
        y: 390
        width: 59
        height: 30
        text: "Sbros L"
        objectName: "PU.PUUP.Rx.SBROS_L"
        checked: false
        checkable: false
    }

    Label {
        id: label2
        x: 15
        y: 438
        text: qsTr("Pult VN")
    }

    Label {
        id: label3
        x: 15
        y: 466
        text: qsTr("Pult GN")
    }

}
