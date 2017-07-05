import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 500
    height: 500

    Button {
        id: button_left
        x: 105
        y: 104
        width: 30
        height: 30
        text: "<-"
        checked: false

        objectName: "PU.PUP.Rx.key_Vlevo"
    }

    Button {
        id: button_right
        x: 206
        y: 105
        width: 30
        height: 30
        text: "->"
        checked: false

        objectName: "PU.PUP.Rx.key_Vpravo"
    }

    Button {
        id: button3
        x: 141
        y: 104
        width: 59
        height: 30
        text: "Menu"
        checkable: false
        objectName: "PU.PUP.Rx.key_Menu_Enter"
        checked: false
    }

    Button {
        id: button_down
        x: 156
        y: 140
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
        y: 68
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
        x: 156
        y: 220
        width: 59
        height: 30
        text: "Обогрев"
        objectName: "PU.PUP.Rx.key_Obogrev"
        checked: false
        checkable: false
    }

    Button {
        id: button8
        x: 348
        y: 68
        width: 72
        height: 30
        text: "Усиление"
        objectName: "PU.PUP.Rx.key_Usilenie"
        checked: false
        checkable: false
    }

    Button {
        id: button9
        x: 426
        y: 111
        width: 59
        height: 30
        text: "ШПЗ"
        objectName: "PU.PUP.Rx.key_SHPZ"
        checked: false
        checkable: false
    }

    Button {
        id: button10
        x: 426
        y: 147
        width: 59
        height: 30
        text: "УПЗ/УПЗ2"
        objectName: "PU.PUP.Rx.key_UPZ_UPZ2"
        checkable: false
        checked: false
    }

    Button {
        id: button12
        x: 426
        y: 68
        width: 59
        height: 30
        text: "ТВ/ТПВ"
        objectName: "PU.PUP.Rx.key_TV_TPV"
        checkable: false
        checked: false
    }

    Button {
        id: button13
        x: 283
        y: 68
        width: 59
        height: 30
        text: "Фокус"
        objectName: "PU.PUP.Rx.key_Focus"
        checked: false
        checkable: false
    }

    Button {
        id: button14
        x: 348
        y: 147
        width: 72
        height: 30
        text: "Полярность"
        objectName: "PU.PUP.Rx.key_Polyarnost"
        checkable: false
        checked: false
    }

    CheckBox {
        id: checkBox1
        x: 133
        y: 24
        text: qsTr("ССЗ")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_SSZ"
    }

    CheckBox {
        id: checkBox2
        x: 178
        y: 24
        text: qsTr("ЛД")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_LD"
    }

    CheckBox {
        id: checkBox3
        x: 260
        y: 24
        text: qsTr("ТП")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_TP"
    }

    CheckBox {
        id: checkBox4
        x: 220
        y: 24
        text: qsTr("ТВ")
        enabled: false
        objectName: "PU.PUP.Tx.Gotov_TV"
    }

    CheckBox {
        id: checkBox5
        x: 157
        y: 197
        text: qsTr("обогрев")
        enabled: false
        objectName: "PU.PUP.Tx.Obogrev"
    }

    CheckBox {
        id: checkBox6
        x: 15
        y: 279
        text: qsTr("Подсветка")
        enabled: false
        objectName: "PU.PUP.Tx.Vkl_Light"
    }

    SpinBox {
        id: spinBox1
        x: 27
        y: 47
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
        x: 21
        y: 28
        text: qsTr("Тепловизор")
    }

    Button {
        id: button15
        x: 15
        y: 302
        width: 103
        height: 30
        text: "Инженерное меню"
        objectName: "PU.force_engineering_menu"
        checkable: false
        checked: false
    }

    Button {
        id: button16
        x: 124
        y: 302
        width: 103
        height: 30
        text: "Основное меню"
        objectName: "PU.force_main_menu"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox_up
        x: 191
        y: 75
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vverh"
    }

    CheckBox {
        id: checkBox_right
        x: 242
        y: 111
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vpravo"
    }

    CheckBox {
        id: checkBox_left
        x: 82
        y: 111
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vlevo"
    }

    CheckBox {
        id: checkBox_down
        x: 191
        y: 147
        text: qsTr("")
        objectName: "PU.PUP.Rx.key_Vniz"
    }

    Button {
        id: button17
        x: 221
        y: 220
        width: 69
        height: 30
        text: "ЛД"
        objectName: "PU.PUP.Rx.key_LD"
        checked: false
        checkable: false
    }

    Button {
        id: button19
        x: 296
        y: 220
        width: 59
        height: 30
        text: "Выкл. ТП"
        objectName: "PU.PUP.Rx.key_DR_TPV"
        checked: false
        checkable: false
    }

    Button {
        id: button20
        x: 348
        y: 111
        width: 72
        height: 30
        text: "Калибровка"
        objectName: "PU.PUP.Rx.key_Kalibrovka"
        checked: false
        checkable: false
    }

    Button {
        id: button21
        x: 426
        y: 220
        width: 59
        height: 30
        text: "Марка"
        objectName: "PU.PUP.Rx.key_Marka"
        checked: false
        checkable: false
    }

    CheckBox {
        id: checkBox7
        x: 105
        y: 279
        text: qsTr("ВКЛ ТП")
        objectName: "PU.PUP.Tx.Vkl_TPV"
        enabled: false
    }

    Slider {
        id: sliderHorizontal1
        x: 64
        y: 394
        width: 422
        height: 22
        z: 1
        minimumValue: -5000
        maximumValue: 5000

        objectName: "PU.PUUP.Rx.PU_VN"
        signal changed()
        onValueChanged: changed()
    }

    Slider {
        id: sliderHorizontal2
        x: 64
        y: 443
        width: 422
        height: 22
        z: 2
        minimumValue: -5000
        maximumValue: 5000

        objectName: "PU.PUUP.Rx.PU_GN"
        signal changed()
        onValueChanged: changed()
    }

    Rectangle {
        id: rectangle1
        x: 8
        y: 338
        width: 484
        height: 154
        color: "#ffffff"
        border.color: "#110000"

        Button {
            id: button_zero_vn
            x: 244
            y: 79
            width: 54
            height: 24
            text: "^"
            checked: false
            checkable: false
            onClicked: sliderHorizontal1.value = 0;
        }

        Button {
            id: button_zero_gn
            x: 244
            y: 130
            width: 54
            height: 24
            text: "^"
            checked: false
            checkable: false
            onClicked: sliderHorizontal2.value = 0;
        }
    }

    Button {
        id: button23
        x: 361
        y: 220
        width: 59
        height: 30
        text: "ГПО"
        objectName: "PU.PUP.Rx.key_GPO"
        checked: false
        checkable: false
    }

    Button {
        id: button24
        x: 15
        y: 346
        width: 59
        height: 30
        text: "Пыль"
        objectName: "PU.PUUP.Rx.REGIM_PIL"
        checked: false
        checkable: true
    }

    Button {
        id: button25
        x: 83
        y: 346
        width: 87
        height: 30
        text: "Откл. Привод"
        objectName: "PU.PUUP.Rx.PRIVOD_OTKL"
        checked: false
        checkable: true
    }

    Button {
        id: button26
        x: 178
        y: 346
        width: 59
        height: 30
        text: "Изм. Д"
        objectName: "PU.PUUP.Rx.IZM_D"
        checked: false
        checkable: false
    }

    Button {
        id: button27
        x: 245
        y: 346
        width: 59
        height: 30
        text: "Сброс Л"
        objectName: "PU.PUUP.Rx.SBROS_L"
        checked: false
        checkable: false
    }

    Label {
        id: label2
        x: 15
        y: 399
        text: qsTr("Пульт ВН")
    }

    Label {
        id: label3
        x: 15
        y: 448
        text: qsTr("Пульт ГН")
    }

}
