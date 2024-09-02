import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import ListItem 1.0
import MyListModel 1.0

Rectangle {
    id: root
    width:QMLSizeManager.windowWidth
    height:QMLSizeManager.windowHeight  
    objectName:"rootRect"
    property int iBorderWidth: 2
    ListItem{id:itemSignal}
    /*Component.onCompleted:
    {
        console.log("组件加载完成后执行强制刷新");
        root.forceActiveFocus();
    }*/
    ScrollView{
        anchors.fill: parent
        contentWidth:listView.width
        contentHeight: listView.height
        background:null
        GridView {
        id: listView
        objectName:"listView"
        //anchors.fill: parent
        width: QMLSizeManager.windowWidth
        height: QMLSizeManager.windowHeight
        cellWidth:QMLSizeManager.cellWidth // 237 //设置每个item 的宽高，否则会重叠
        cellHeight:QMLSizeManager.cellHeight //426        
        //displayMarginBeginning:15
        //displayMarginEnd:15
        anchors.margins: 15 //GridView距离间距
        model:MyListModel.items //listModel.items
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
                //signal qmlSendSignals(bool bIsShowMenu);
                signal qmlSendSignals(int iId,string strPhoneName, string strInstanceNo, string strExpireTime, bool bIsShowMenu);
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
                    qmlSendSignals.connect(itemSignal.ShowInstanceSignalFromQMLFile)
                    //qmlSendSignals.connect(QMLSizeManager.receiveSignalFromQMLFile)
                }
                
                MouseArea {
                    id: itemClickArea
                    anchors.fill: parent
                    onClicked: {
                        //直接调用C++中的函数
                        QMLSizeManager.itemClicked();
                        //QML发送信号调用 C++槽函数,三步：第三步
                        qmlSendSignals(modelData.phoneId,modelData.phoneName, modelData.phoneInstanceNo,modelData.ExpireTime, true)
                        console.log("Item was clicked. width="+listView.cellWidth +"height="+ listView.cellHeight); //console.log("Item was clicked: " + modelData.name);
                        // 在这里可以添加更多的逻辑
                        //发送显示PhoneInstanceWidget窗口的信号
                    }
                }

                Rectangle{
                id:authorRect
                x:iBorderWidth
                y:5
                width:52
                height:19
                color:"transparent"
                visible:modelData.bShowAuthorImg
                /*indicator:*/Image {
                    id: authorStatusImg
                    anchors.fill:parent
                    //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/0.png" 
                    //source:"qrc:/main/resource/main/Authorized.png" //可以显示已授权
                    source:modelData.authorStatus==1?"qrc:/main/resource/main/Authorized.png":"qrc:/main/resource/main/BeAuthorized.png"
                }
                }

                indicator:Image {
                    id: backgroundImage
                    //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/0.png"  //modelData.imagePath
                    source:modelData.ImagePath //item.ImagePath
                    fillMode: Image.PreserveAspectFit
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom-20
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
                    checked: modelData.checkBox
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 10
                    background:null

                    onCheckedChanged: {
                        //QML发送信号调用 C++槽函数,三步：第三步
                        qmlSendSignals(100,"你好","世界world")
                    }
                    /*MouseArea {
                        id: checkBoxClickArea
                        anchors.fill: parent
                        onClicked: {
                            //console.log("checkBoxClickArea was clicked.name="+modelData.phoneName+" No=" +modelData.phoneInstanceNo); //console.log("Item was clicked: " + modelData.name);
                            // 在这里可以添加更多的逻辑
                            //发送显示PhoneInstanceWidget窗口的信号
                            //checked = !checked;
                            console.log("checkBoxClickArea was clicked. width="+listView.cellWidth +"height="+ listView.cellHeight+"name="+modelData.phoneName+" No=" +modelData.phoneInstanceNo+" checked="+checked); //console.log("Item was clicked: " + modelData.name);
                       
                        }
                    }*/
                }

                Text {
                    id: labelText
                    text:modelData.phoneName //"testtest" //modelData.label
                    anchors {
                        //fill: parent // 使用 fill 锚点确保文本占据整个空间
                        top: parent.bottom-20
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
}
