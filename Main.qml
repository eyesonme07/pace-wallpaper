import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 800
    height: 480
    minimumWidth: 200
    minimumHeight: 250
    visible: true

    property string color_background: "#141414"
    property string color_background_hover: "#1E1E1E"
    property string color_text_white: "#F0F0F0"
    property string color_border: "#2A2A2A"
    property string color_red: "#E8002D"
    property string color_red_hover: "#FF1744"
    property string color_background_selected: "#3A1620"
    property string color_text_gray: "#C0C0C0"

    Material.background: color_background
    Material.accent: color_red

    Rectangle {
        id: leftMenu

        width: window.width * 0.23
        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height

        color: color_background_hover
        ListModel {
            id: menuModel

            ListElement {
                name: "Обои"
            }
            ListElement {
                name: "Виджеты"
            }
            ListElement {
                name: "Расположение"
            }
            ListElement {
                name: "Обновление\nданных"
            }
            ListElement {
                name: "О программе"
            }
        }

        ListView {
            id: menuListView
            model: menuModel
            anchors.fill: parent
            currentIndex: 0
            delegate: Item {
                id: delegateRoot
                width: ListView.view.width
                height: 40
                readonly property bool isSelected: index === ListView.view.currentIndex

                Rectangle {
                    anchors.fill: parent
                    color: delegateRoot.isSelected ? color_background_selected : "transparent"

                    Rectangle {
                        width: 3
                        height: parent.height - 6
                        color: color_red
                        visible: delegateRoot.isSelected
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                    }
                }

                Text {
                    text: name
                    anchors.leftMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    color: delegateRoot.isSelected ? color_red : color_text_gray
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: menuListView.currentIndex = index
                }
            }
        }
    }
    Rectangle {
        color: color_border
        height: leftMenu.height
        width: 1
        anchors.left: leftMenu.right
        anchors.top: leftMenu.top
    }

    footer: Rectangle {
        id: rec_Footer
        height: 50
        color: color_background_hover

        Rectangle {
            height: 1
            color: color_border
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
        }

        RowLayout {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10

            CustomButton {
                id: customButton
                implicitHeight: 45
                text: "Отмена"
            }

            CustomButton {
                id: button_Apply
                implicitHeight: 45
                isHighlighted: true
                text: "Применить"
            }
        }
    }
}
