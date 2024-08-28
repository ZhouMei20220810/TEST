import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    //width: parent.width
    //height: parent.height
    //width:WIDTH
    //height:HEIGHT
    width:700
    height:600
    GridView {
        id: listView		
        anchors.fill: parent
        //width:237
        //height:426
        cellWidth:237 //设置每个item 的宽高，否则会重叠
        cellHeight:426
        //displayMarginBeginning:15
        //displayMarginEnd:15
        anchors.margins: 15 //GridView距离间距 
        model:1000
        /*model: ListModel {
            id:listModel
            ListElement {
                name: "item1"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/12.png"
                label: "Label 1"
                checked: false
                objectName:"vm_01"
            }
            ListElement {
                name: "item2"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png"
                label: "Label 2"
                checked: true
                objectName:"vm_02"
            }
            // 添加更多 ListElement 项
        }*/
        Timer {
        id: timer
        interval: 1000 // 立即触发
        running: true
        repeat: true
        onTriggered: {
            // 如果需要的话，在这里也可以改变大小
            listView.cellWidth -= 50;
            listView.cellHeight -= 50;
            console.log("listView.cellWidth="+listView.cellWidth+"listView.cellHeight="+listView.cellHeight)
        }
    }
        delegate: Component {
            Button {
                id: windowItem
                /*x:15
                y:15*/ //无效                
                /*color: "transparent"
                border.color: "#FF6B737E"
                border.width: 2*/
                //width:listView.cellWidth-30 // 207;
                //height:listView.cellHeight-30 //396 //更加单元格与实际的差值，形成间隔
                width:listView.cellWidth //207 //sizeManager.m_iWidth
                height:listView.cellHeight //396

                MouseArea {
                    id: itemClickArea
                    anchors.fill: parent
                    onClicked: {
                        console.log("Item was clicked. width="+listView.cellWidth +"height="+ listView.cellHeight); //console.log("Item was clicked: " + modelData.name);
                        // 在这里可以添加更多的逻辑
                        //发送显示PhoneInstanceWidget窗口的信号
                    }
                }

                indicator:Image {
                    id: backgroundImage
                    source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/background1.png"  //modelData.imagePath
                    //source:modelData.url
                    fillMode: Image.PreserveAspectFit
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom - 20
                        margins: 2
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
                    text:listModel.label // "testtest" //modelData.label
                    anchors {
                        //fill: parent // 使用 fill 锚点确保文本占据整个空间
			            top: parent.bottom - 20
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        bottomMargin:10 //距离底部距离
                        //verticalCenter: parent.verticalCenter // 保持文本垂直居中
                    }
                    horizontalAlignment: Text.AlignHCenter // 水平居中
                    verticalAlignment: Text.AlignVCenter  // 垂直居中
                }
            }
        }
        //设置间距
        //spacing:10
        // 计算每行的列数
        onWidthChanged: {
            var itemWidth = windowItem.width + windowItem.border.width * 2;
            var columns = Math.floor(parent.width / itemWidth);
            columns = columns <= 0 ? 1 : columns; // 防止除法结果为零
            listView.columns = columns;
        }
    }


    Connections {
        target: sizeManager
        onCurrentSizeChanged: 
        {
            console.log("Size changed to: " + sizeManager.currentSize)
            // 可以在这里更新每个 Item 的大小
            for (var i = 0; i < 5; i++) {
                var item = root.itemAt(i)
                if (item) {
                    item.width = sizeManager.currentSize
                    item.height = sizeManager.currentSize
                }
            }
        }
    }
}
