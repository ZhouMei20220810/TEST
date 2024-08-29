import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0

Rectangle {
    id: root
    //width: parent.width
    //height: parent.height
    width:800
    height:500    
    objectName:"rootRect"
    GridView {
        id: listView
        objectName:"listView"
        anchors.fill: parent
        //width:237
        //height:426
        cellWidth:QMLSizeManager.cellWidth // 237 //设置每个item 的宽高，否则会重叠
        cellHeight:QMLSizeManager.cellHeight //426
        //displayMarginBeginning:15
        //displayMarginEnd:15
        anchors.margins: 15 //GridView距离间距
        model: 5
        //model:["1","2","3","4"]
        /*ListModel {
            ListElement {
                name: "item1"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/12.png"
                label: "Label 1"
                checked: false
            }
            ListElement {
                name: "item2"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png"
                label: "Label 2"
                checked: true
            }
            ListElement {
                name: "item3"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png"
                label: "Label 3"
                checked: true
            }
            ListElement {
                name: "item4"
                url: "file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/Instance/1.png"
                label: "Label 4"
                checked: true
            }
            // 添加更多 ListElement 项
        }*/    

        /*QMLSizeManager{
            id:qmlSizeManager
            cellWidth:237 //207
            cellHeight:426 //396
            //可以指定值，外面修改值不会改变
            Component.onCompleted: {
                console.log("QMLSizeManager w="+cellWidth+"h="+cellHeight)
            }

            onCellWidthChanged:{
                console.log("QMLSizeManager onCellWidthChanged w="+cellWidth+"h="+cellHeight);
            }
            onCellHeightChanged:{
                console.log("QMLSizeManager OnCellHeightChanged w="+cellWidth+"h="+cellHeight);
            }
        }*/

        delegate: Component {
            Button {
                id: windowItem
                objectName:"btnBg"
                /*x:15
                y:15*/ //无效
                width: QMLSizeManager.cellWidth-30 //207; 
                height:QMLSizeManager.cellHeight-30 //396 //更加单元格与实际的差值，形成间隔
                /*color: "transparent"
                border.color: "#FF6B737E"
                border.width: 2*/
                //QML发送信号调用 C++槽函数,三步：第一步
                signal qmlSendSignals(int i,string str,string str2);
                //QML发送信号调用 C++槽函数,三步：第二步
                /*Connections{
                    target:windowItem
                    function onQmlSendSignals(i,str,str2){
                        QMLSizeManager.receiveSignalFromQMLFile(i,str,str2)
                    }
                }*/
                //或
                Component.onCompleted: {
                    //QML信号qmlSendSignals，连接C++ 槽函数qmlSizeManager.receiveSignalFromQMLFile
                    qmlSendSignals.connect(QMLSizeManager.receiveSignalFromQMLFile)
                }
                
                MouseArea {
                    id: itemClickArea
                    anchors.fill: parent
                    onClicked: {
                        //直接调用C++中的函数
                        QMLSizeManager.itemClicked();
                        //QML发送信号调用 C++槽函数,三步：第三步
                        qmlSendSignals(100,"你好","世界world")
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
                    /*MouseArea {
                        id: checkBoxClickArea
                        anchors.fill: parent
                        onClicked: {
                            console.log("checkBoxClickArea was clicked."+modelData.name); //console.log("Item was clicked: " + modelData.name);
                            // 在这里可以添加更多的逻辑
                            //发送显示PhoneInstanceWidget窗口的信号
                            checked = !checked;
                        }
                    }*/
                }

                Text {
                    id: labelText
                    text: "testtest" //modelData.label
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
            console.log("onWidthChanged listView.columns = "+listView.columns);
        }
    }
}
