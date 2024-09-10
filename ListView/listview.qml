/*import QtQuick
import QtQuick.Window
import QtQuick.Controls

Item {
    visible: true
    width: 400
    height: 400
    //title: "Image Rotation Example"
    property bool isRotated: false  // 这个属性定义在Window中，可以直接通过isRotated访问
    property int borderRcWidth: 207
    property int borderRcHeight: 368
    Rectangle {
        id: rotateButton
        //anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: borderRcWidth
        height: borderRcHeight
        color: "lightblue"
        radius: 10

        Image {
            id: image
            anchors.centerIn: parent
            source: "qrc:/VM010210085185.png"
            width: 207
            height: 368
            sourceSize.width:207
            sourceSize.height:368

            transform: Rotation {
                angle: isRotated ? 270 : 0
                origin.x: image.width / 2
                origin.y: image.height / 2
            }
        }

        Text {
            anchors.centerIn: parent
            text: "Rotate"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // 直接使用isRotated来访问和修改属性
                isRotated = !isRotated
                borderRcHeight =(borderRcHeight===207?368:207)
                borderRcWidth =(borderRcWidth===207?368:207)

            }
        }
    }
}*/

import QtQuick 2.5
import QtQuick.Controls 2.5
import QMLSizeManager 1.0

Item {
    id: root
    width: 800
    height: 600
    // 旋转角度属性
    property real rotationAngle: 0
    property int itemWidth:rotationAngle===0?207:368
    property int itemHeight:rotationAngle===0?368:207

    Rectangle
    {
        id:imageRect
        width:QMLSizeManager.cellWidth
        height: QMLSizeManager.cellHeight
        color: "lightblue"
        radius: 10
        anchors.horizontalCenter: parent.horizontalCenter

        onWidthChanged: {
            console.log("width="+width)
        }
        onHeightChanged: {
            console.log("height="+height)
        }

        // 图像
        Image {
            id: myImage
            source: "qrc:/VM010210085185.png"
            //anchors.fill: parent // 使用锚点使图像填充父容器
            //preserveRatio: true // 保持图像比例
            //smooth: true // 平滑处理
            anchors.centerIn: parent
            //anchors.left: imageRect.left
            //anchors.top: imageRect.top
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.top: parent.top
            //width: QMLSizeManager.cellWidth
            //height: QMLSizeManager.cellHeight
            sourceSize.width: 207
            sourceSize.height: 368
            //transformOrigin: Image.Center
            //rotation:QMLSizeManager.verticalScreen?0:270
            transform: Rotation {
                angle: QMLSizeManager.verticalScreen ? 0 : 270
                origin.x: myImage.width / 2
                origin.y: myImage.height / 2
            }
        }
    }
}

