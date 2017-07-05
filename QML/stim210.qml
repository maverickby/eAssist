import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 513
    height: 312

    Slider {
        id: slider__horizontal_1
        x: 70
        y: 13
        minimumValue: -1000
        maximumValue: 1000

        objectName: "App.navedenie"
        signal changed()
        onValueChanged: changed()
    }

    Label {
        id: label1
        x: 8
        y: 22
        text: "Наведение"
    }

    Button {
        id: button1
        x: 159
        y: 41
        width: 22
        height: 23
        text: "^"

        onClicked: slider__horizontal_1.value = 0
    }

    Label {
        id: label2
        x: 304
        y: 22
        text: "K mems:"
    }

    TextField {
        id: text_field_kmems
        x: 355
        y: 15
        placeholderText: ""

        signal changed()
        objectName: "Coefs.Kmems"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label3
        x: 304
        y: 51
        text: "Увод:"
    }

    TextField {
        id: text_field_uvod
        x: 355
        y: 44
        placeholderText: ""

        signal changed()
        objectName: "Coefs.uvod"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    GroupBox {
        id: group_box1
        x: 20
        y: 86
        width: 192
        height: 207
        title: "ПИД регулятор"
    }

    GroupBox {
        id: group_box2
        x: 226
        y: 85
        width: 271
        height: 103
        title: "Закачка"
    }

    Label {
        id: label4
        x: 262
        y: 107
        text: "Форма:"
    }

    SpinBox {
        id: spinbox_testing_shape
        x: 304
        y: 100
        maximumValue: 2

        signal changed()
        objectName: "App.Testing.1-meandr,2-sin"
        onValueChanged: changed()
    }

    Label {
        id: label5
        x: 341
        y: 107
        text: "0-выкл, 1-меандр, 2-синус"

        Label {
            id: label7
            x: 122
            y: 25
            text: "ms"
        }
    }

    Label {
        id: label6
        x: 256
        y: 133
        text: "Период:"
    }

    TextField {
        id: text_field_period
        x: 304
        y: 126
        width: 151
        height: 20
        placeholderText: ""

        signal changed()
        objectName: "App.Testing.period"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label8
        x: 238
        y: 159
        text: "Амплитуда:"
    }

    TextField {
        id: text_field_amplituda
        x: 304
        y: 152
        width: 151
        height: 20
        placeholderText: ""

        signal changed()
        objectName: "App.Testing.amplitude"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label9
        x: 33
        y: 114
        text: "K"
        font.pointSize: 10
    }

    TextField {
        id: text_field_K
        x: 64
        y: 107
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.K"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label10
        x: 33
        y: 140
        text: "Td"
        font.pointSize: 10
    }

    TextField {
        id: text_field_Td
        x: 64
        y: 133
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.Td"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label11
        x: 33
        y: 166
        text: "N"
        font.pointSize: 10
    }

    TextField {
        id: text_field_N
        x: 64
        y: 159
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.N"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label12
        x: 33
        y: 192
        text: "Ti"
        font.pointSize: 10
    }

    TextField {
        id: text_field_Ti
        x: 64
        y: 185
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.Ti"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label13
        x: 33
        y: 218
        text: "b"
        font.pointSize: 10
    }

    TextField {
        id: text_field_b
        x: 64
        y: 211
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.b"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label14
        x: 33
        y: 244
        text: "c"
        font.pointSize: 10
    }

    TextField {
        id: text_field_c
        x: 64
        y: 237
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.c"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    Label {
        id: label15
        x: 33
        y: 270
        text: "Ta"
        font.pointSize: 10
    }

    TextField {
        id: text_field_Ta
        x: 64
        y: 263
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PID.Ta"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    CheckBox {
        id: check_box_use_pid
        x: 20
        y: 51
        text: "ПИД"

        objectName: "App.use_pid"
    }

    GroupBox {
        id: group_box3
        x: 226
        y: 194
        width: 271
        height: 110
        title: "ПИ регулятор"
    }

    Label {
        id: label16
        x: 238
        y: 221
        text: "K"
        font.pointSize: 10
    }

    Label {
        id: label17
        x: 238
        y: 247
        text: "Ti"
        font.pointSize: 10
    }

    Label {
        id: label18
        x: 238
        y: 272
        text: "Ta"
        font.pointSize: 10
    }

    TextField {
        id: text_field_PI_K
        x: 262
        y: 214
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PI.K"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    TextField {
        id: text_field_PI_Ti
        x: 262
        y: 242
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PI.Ti"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }

    TextField {
        id: text_field_PI_Ta
        x: 262
        y: 270
        placeholderText: ""

        signal changed()
        objectName: "Coefs.PI.Ta"
        property bool editing: false
        onAccepted: {
            editing = false
            changed()
        }
        onTextChanged: editing = true
        onEditingChanged: {
            if(editing == true) textColor = "blue"
            else textColor = "black"
        }
    }
}
