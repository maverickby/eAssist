import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 400
    height: 220

    CheckBox {
        id: check_box1
        x: 29
        y: 22
        text: "test_bool"

        objectName: "sTest.test_bool"
    }

    Label {
        id: label1
        x: 13
        y: 74
        width: 83
        height: 19
        color: "#e80d0d"
        text: "Label"
        styleColor: "#e62121"
        style: Text.Normal
        font.pointSize: 12

        objectName: "sTest.test_int16"
    }

    Button {
        id: button1
        x: 119
        y: 22
        text: "test_bool"

        objectName: "sTest.test_bool"
    }

    ProgressBar {
        id: progress_bar1
        x: 13
        y: 102
        width: 368
        height: 23
        minimumValue: -1000
        maximumValue: 1000

        objectName: "sTest.test_int16"
    }

    Slider {
        id: slider__horizontal_1
        x: 29
        y: 150
        width: 328
        height: 22
        minimumValue: -1000
        maximumValue: 1000

        signal changed()
        objectName: "sTest.test_int16"
        onValueChanged: changed()
    }

    SpinBox {
        id: spinbox1
        x: 215
        y: 74
        width: 87
        height: 22
        stepSize: 50
        minimumValue: -1000
        maximumValue: 1000

        signal changed()
        objectName: "sTest.test_int16"
        onValueChanged: changed()
    }

    ToolButton {
        id: tool_button1
        x: 200
        y: 22
        text: "test_bool"
        iconName: ""
        checkable: true

        objectName: "sTest.test_bool"
    }

    TextField {
        id: text_field1
        x: 29
        y: 189
        validator: DoubleValidator {bottom: -1000; top: 1000; notation: "StandardNotation";}
        font.pointSize: 12
        placeholderText: "Text Field"

        signal changed()
        objectName: "sTest.test_float"
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
        id: label2
        x: 303
        y: 47
        text: "Label"

        objectName: "sTest.test_int32"
    }
}
