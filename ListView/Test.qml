import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    visible: true
    width: 640
    height: 480

    // 假设有 1000 个 Item 需要改变大小
    Repeater {
        model: 1000
        delegate: Rectangle {
            id: item
            width: sizeManager.currentSize
            height: sizeManager.currentSize
            color: "red"
        }
    }

    Connections {
        target: sizeManager
        onCurrentSizeChanged: {
            console.log("Size changed to: " + sizeManager.currentSize)
            // 可以在这里更新每个 Item 的大小
            for (var i = 0; i < 1000; i++) {
                var item = root.itemAt(i)
                if (item) {
                    item.width = sizeManager.currentSize
                    item.height = sizeManager.currentSize
                }
            }
        }
    }
}
