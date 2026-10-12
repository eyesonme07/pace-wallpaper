import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Controls.Material

Page {
    id: about_page
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            width: 60
            height: 60
            radius:17
            color: Theme.color_red_hover
            border.width: 1
            border.color: Theme.color_red
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
