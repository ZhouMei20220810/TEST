import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModelEx 1.0
import MyStructNameSpace 1.0
Canvas{
    id: canvas
    width:QMLSizeManager.windowWidth
    height:QMLSizeManager.windowHeight  
    objectName:"rootRect"
    property int borderWidth: 2
    // 定义一个属性来控制图片更新
    ScrollView{
        anchors.fill: parent
        contentWidth:listView.width
        contentHeight: listView.height
        GridView{
            id: listView
            objectName:"listView"
            anchors.fill: parent
            cellWidth:QMLSizeManager.cellWidth+30 // 237 //设置每个item 的宽高，否则会重叠
            cellHeight:QMLSizeManager.cellHeight+20+30 //426
            //displayMarginBeginning:15
            //displayMarginEnd:15
            anchors.margins: 15 //GridView距离间距
            //spacing:15
            model:MyListModelEx//.items //listModel.items
            delegate: Component{
                Rectangle{
                    id: windowItem
                    objectName:"btnBg"
                    /*x:15
                    y:15*/ //无效
                    width: QMLSizeManager.cellWidth //207;
                    height:QMLSizeManager.cellHeight+20+30 //396 //更加单元格与实际的差值，形成间隔
                    //color:"lightblue"
                    //radius: 10

                    property int index:itemIndex  // 设置索引属性
                    //QML发送信号调用 C++槽函数,三步：第一步
                    signal qmlSendSignals(string strPhoneName, string strInstanceNo,bool bIsShowMenu,S_PHONE_INFO info);
                    signal qmlCheckBoxSignals(bool bChecked);
                    //QML发送信号调用 C++槽函数,三步：第二步
                    /*Connections{
                        target:windowItem
                        function onQmlSendSignals(i,str,str2){
                            QMLSizeManager.receiveSignalFromQMLFile(i,str,str2)
                        }
                    }*/
                    //或
                    Component.onCompleted:
                    {
                        //QML信号qmlSendSignals，连接C++ 槽函数qmlSizeManager.receiveSignalFromQMLFile
                        qmlSendSignals.connect(MyListModelEx.ShowInstanceSignalFromQMLFile)
                        qmlCheckBoxSignals.connect(MyListModelEx.QmlCheckBoxSignals)
                    }

                    Rectangle
                    {
                        id:bgImgRect
                        width:QMLSizeManager.cellWidth+4
                        height:QMLSizeManager.cellHeight+4
                        anchors.horizontalCenter: parent.horizontalCenter
                        color:"#FF6B737E"
                        //背景图
                        Image {
                            id: backgroundImage
                            //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/background1.png"  //modelData.imagePath
                            source:imagePath === "qrc:/main/resource/main/defaultSceenShot.png"?"qrc:/main/resource/main/defaultSceenShot.png":"file:///"+imagePath //item.ImagePath
                            //x:2
                            //y:2
                            //width:bgImgRect.width-4
                            //height:bgImgRect.height-4
                            sourceSize.width:QMLSizeManager.itemVerticalScreen?QMLSizeManager.cellWidth:QMLSizeManager.cellHeight //bgImgRect.width-4
                            sourceSize.height:QMLSizeManager.itemVerticalScreen?QMLSizeManager.cellHeight:QMLSizeManager.cellWidth //bgImgRect.height-4

                            //smooth: false //关闭平滑
                            //transformOrigin: Image.Center
                            //fillMode: Image.PreserveAspectFit //Image.PreserveAspectFit //保持纵横比
                            //rotation:QMLSizeManager.itemVerticalScreen?0:270
                            /*transform: Rotation {
                                            origin.x: backgroundImage.height / 2
                                            origin.y: backgroundImage.width / 2
                                            angle: QMLSizeManager.itemVerticalScreen?0:270
                                            axis: Qt.zAxis
                                        }*/
                            anchors.centerIn: parent
                            transform: Rotation {
                                angle: QMLSizeManager.itemVerticalScreen ? 0 : 270
                                origin.x: backgroundImage.width / 2
                                origin.y: backgroundImage.height / 2
                            }
                            anchors.margins: 2

                            //授权状态图
                            Rectangle{
                                id:authorRect
                                x:backgroundImage.left
                                y:backgroundImage.top+8
                                width:52
                                height:19
                                color:"transparent"
                                visible:isShowAuthImg
                                Image {
                                    id: authorStatusImg
                                    anchors.fill:parent
                                    sourceSize.width: authorRect.width
                                    sourceSize.height:authorRect.height
                                    //source:"qrc:/main/resource/main/Authorized.png" //可以显示已授权
                                    source:authorStatus==1?"qrc:/main/resource/main/Authorized.png":"qrc:/main/resource/main/BeAuthorized.png"
                                }
                            }
                            onSourceChanged: {
                                console.log("Image source chagne");

                            }

                            onStatusChanged: {
                                console.log("Image status changed:", status);
                                if (status === Image.Error) {
                                    console.log("Image error:", errorString);
                                }
                            }
                            //
                            //背景点击
                            MouseArea {
                                id: itemClickArea
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton|Qt.RightButton
                                //点击背景图弹实例窗口
                                onPressed: {
                                    // 发送鼠标事件到C++
                                    var event = {
                                    buttons: mouse.button,
                                    x: mouse.x,
                                    y: mouse.y,
                                    index:itemIndex
                                    }
                                    console.log("qml phoneName"+phoneName+" phoneInstanceNo="+phoneInstanceNo+" mouse.button="+mouse.button);
                                    MyListModelEx.mousePressEvent(event);
                                }
                            }
                        }
                    }
                    CheckBox {
                        id: checkBox
                        checked:bChecked //isChecked
                        anchors.top: parent.top
                        anchors.right: parent.right
                        onCheckedChanged:
                        {
                            // 更新模型中的checked状态
                            console.log("index="+itemIndex+" checked="+checked);
                            MyListModelEx.onCheckBoxChanged(index,checked);                            
                        }
                        onClicked: {
                            //发送信号
                            qmlCheckBoxSignals(checked);
                        }
                    }

                    Text {
                        id: labelText
                        text:phoneName //modelData.phoneName
                        elide: Text.ElideMiddle
                        anchors {
                            //fill: parent // 使用 fill 锚点确保文本占据整个空间
                            top: bgImgRect.bottom
                            left: parent.left
                            right: parent.right
                            bottom: bgImgRect.bottom+20
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

            onContentXChanged: {
                console.log("onContentXChanged  XXX")
                MyListModelEx.do_onContentXYChanged();
            }
            onContentYChanged: {
                console.log("onContentXChanged YYY")
                MyListModelEx.do_onContentXYChanged();
            }
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
