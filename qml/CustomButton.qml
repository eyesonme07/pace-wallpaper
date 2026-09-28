import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Controls.Material
import QtQuick.Layouts

Button {
    id: customButton
    property bool isHighlighted: false

    hoverEnabled: true

    contentItem: Text {
        text: customButton.text
        color: Theme.color_text_white
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: customButton.isHighlighted
    }

    background: Rectangle {
        color: customButton.hovered ? customButton.isHighlighted ? Theme.color_red_hover : Theme.color_background_hover : (customButton.isHighlighted ? Theme.color_red : Theme.color_background)
        radius: 10
        border.color: Theme.color_border
        border.width: 1

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }
    }
}
