import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("QML Window")

    Column {
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            model: ListModel {
                id: windowListModel
                ListElement { url: "image1.jpg" }
                ListElement { url: "image2.jpg" }
                // ... 添加更多的 ListElement
            }

            delegate: Rectangle {
                width: parent.width
                height: 100
                color: "transparent"
                property string imageUrl: item.url

                Image {
                    id: backgroundImage
                    source: imageUrl
                    fillMode: Image.PreserveAspectFit
                    async: true
                    width: parent.width
                    height: parent.height
                    visible: backgroundImage.status === Image.Ready
                }
            }
        }
    }
}
