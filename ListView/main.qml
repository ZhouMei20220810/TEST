import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("ImageWindow")

    Image {
        id: testImg
        width:50
        height:50
        source: "/BVIP.png" //不能含有特殊字符
    }
    Image {
        id: testImg2
        x:60
        width:50
        height:50
        //source: "C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png" //不能含有特殊字符
        //source: "C:\\Users\\Administrator\\AppData\\Local\\Temp\\YiShunYun\\Instance\\1.png" //不能含有特殊字符
        //source: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/12.png" //可以正常显示本地路径
        source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/VM010210172002.png"
    }
    Image {
        id: testImg3
        x:110
        width:50
        height:50
        //source: "C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png" //不能含有特殊字符
        //source: "C:\\Users\\Administrator\\AppData\\Local\\Temp\\YiShunYun\\Instance\\1.png" //不能含有特殊字符
        source: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png"
    }
    Button{
        id:btn
        x:500
        width:50
        height:50
        //border.color:"black"
        //icon.source: "D:/zm/QtProject/ListView/QVIP%402x.png"
        indicator: Image{
        id:btnBkImg
        anchors.centerIn: btn
        source: "/QVIP.png"

        }

    }
}
/*ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480

    ListView {
        id: listView
        anchors.fill: parent
        model: ListModel {
            ListElement {
                imagePath: "path/to/image1.jpg"
                label: "Image 1"
                checked: false
            }
            // 添加更多 ListElement 项
        }

        delegate: Component {
            Button {
                id: windowItem
                width: parent.width
                height: 100
                //color: "transparent"


                indicator:Image {
                    id: backgroundImage
                    anchors.centerIn: parent
                    source:"/BVIP%402x.png" //modelData.imagePath
                    //fillMode: Image.PreserveAspectFit
                    //async: true
                    //width: parent.width
                    //height: parent.height
                    //visible: backgroundImage.status === Image.Ready
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
                    text: modelData.label
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // 定时更新图片
        Timer {
            id: updateTimer
            interval: 5000 // 每5秒
            running: true
            repeat: true
            onTriggered: {
                var index = Math.floor(Math.random() * listView.model.count)
                backgroundImage.source = listView.model.get(index).imagePath
            }
        }
    }
}*/
