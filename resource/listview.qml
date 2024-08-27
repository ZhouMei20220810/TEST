import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: parent.width
    height: parent.height

    ListView {
        id: listView
        anchors.fill: parent
        model: ListModel {
            ListElement {
                name: "item1"
                imagePath: "path/to/image1.png"
                label: "Label 1"
                checked: false
            }
            ListElement {
                name: "item2"
                imagePath: "path/to/image2.png"
                label: "Label 2"
                checked: true
            }
            // 添加更多 ListElement 项
        }

        delegate: Component {
            Rectangle {
                id: windowItem
                width: 207; height: 396
                color: "white"
                border.color: "#FF6B737E"
                border.width: 2

                Image {
                    id: backgroundImage
                    source: modelData.imagePath
                    fillMode: Image.PreserveAspectFit
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom - 20
                        margins: 10
                    }
                    onStatusChanged: {
                        console.log("Image status changed:", status);
                        if (status === Image.Error) {
                            console.log("Image error:", errorString);
                        }
                    }
                }

                CheckBox {
                    id: checkBox
                    checked: modelData.checked
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 10
                }

                Text {
                    id: labelText
                    text: "testtest" //modelData.label
                    anchors {
                        fill: parent // 使用 fill 锚点确保文本占据整个空间
                        verticalCenter: parent.verticalCenter // 保持文本垂直居中
                    }
                    horizontalAlignment: Text.AlignHCenter // 水平居中
                    verticalAlignment: Text.AlignVCenter  // 垂直居中
                }
            }
        }
    }
}
