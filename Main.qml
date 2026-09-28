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

    Material.background: Theme.color_background
    Material.accent: Theme.color_red

    Rectangle {
        id: leftMenu

        width: window.width * 0.23
        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height

        color: Theme.color_background_hover
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
                    color: delegateRoot.isSelected ? Theme.color_background_selected : "transparent"

                    Rectangle {
                        width: 3
                        height: parent.height - 6
                        color: Theme.color_red
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
                    color: delegateRoot.isSelected ? Theme.color_red : Theme.color_text_gray
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: menuListView.currentIndex = index
                }
            }
        }
    }
    Rectangle {
        color: Theme.color_border
        height: leftMenu.height
        width: 1
        anchors.left: leftMenu.right
        anchors.top: leftMenu.top
    }

    StackLayout {
        currentIndex: menuListView.currentIndex
        anchors.left: leftMenu.right
        anchors.top: parent.top
        width: parent.width - leftMenu.width
        height: parent.height

        Wallpaper_page {
            id: wallpaper_page
        }
        Widgets_page {
            id: widgets_page
        }
        Location_page {
            id: location_page
        }
        Update_data_page {
            id: update_data_page
        }
        About_page {
            id: about_page
        }
    }

    footer: Rectangle {
        id: rec_Footer
        height: 50
        color: Theme.color_background_hover

        Rectangle {
            height: 1
            color: Theme.color_border
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
