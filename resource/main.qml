import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480

    Column {
        anchors.fill: parent

        QQuickListView {
            id: listView
            anchors.fill: parent
            model: ListModel {
                id: itemsModel
                ListElement {
                    name: "Window 1"
                }
                ListElement {
                    name: "Window 2"
                }
                // 添加更多 ListElement 项
            }

            delegate: Rectangle {
                id: windowItem
                width: 200; height: 100
                color: "lightblue"
                Text {
                    anchors.centerIn: parent
                    text: modelData.name
                }
            }
        }
    }
}
