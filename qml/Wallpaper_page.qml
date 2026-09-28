import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import Backend

Page {
    id: wallpaper_page

    Text {
        id: topText
        text: "Выбор обоев"
        color: Theme.color_text_white
        font.pixelSize: 17
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    Rectangle {
        id: currentWallpaper
        anchors.top: topText.bottom
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 30

        width: 480
        height: 270
        color: "Red"
        radius: 20
    }

    FileDialog {
        id: fileDialog

        title: "Выберите картинку обоев"

        nameFilters: ["png(*.png),jpg(*.jpg)"]

        onAccepted: {
            Backend.setWallpalaer(fileDialog.selectedFile);
        }
    }

    RowLayout {
        anchors.top: currentWallpaper.bottom
        anchors.left: parent.left
        anchors.topMargin: 15
        anchors.leftMargin: 20

        CustomButton {
            text: "Выбать файл"
            isHighlighted: true
            onClicked: {
                fileDialog.open();
            }
        }
        CustomButton {
            text: "Текущие обои Windows"
        }
    }
}
