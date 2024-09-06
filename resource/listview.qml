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
        GridView {
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
        delegate: Component {
            Rectangle {
                id: windowItem
                objectName:"btnBg"
                /*x:15
                y:15*/ //无效
                width: QMLSizeManager.cellWidth //207;
                height:QMLSizeManager.cellHeight+20+30 //396 //更加单元格与实际的差值，形成间隔
                property int index:itemIndex  // 设置索引属性
                /*color: "transparent"
                border.color: "#FF6B737E"
                border.width: 2*/
                //QML发送信号调用 C++槽函数,三步：第一步
                //signal qmlSendSignals(bool bIsShowMenu);
                //发送左键点击item背景
                signal qmlSendSignals(string strPhoneName, string strInstanceNo,bool bIsShowMenu,S_PHONE_INFO info);
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
                    //qmlSendSignals.connect(itemSignal.ShowInstanceSignalFromQMLFile)
                    qmlSendSignals.connect(MyListModelEx.ShowInstanceSignalFromQMLFile)
                    //itemClickedSignals.connect(MyListModel.do_ItemClickSignals)
                    //qmlSendSignals.connect(QMLSizeManager.receiveSignalFromQMLFile)
                }

                Rectangle
                {
                    id:bgImgRect
                    width:QMLSizeManager.cellWidth+4
                    height:QMLSizeManager.cellHeight+4
                    color:"#FF6B737E"
                //indicator:
                    //背景图
                    Image {
                        id: backgroundImage
                        //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/background1.png"  //modelData.imagePath
                        source:imagePath //item.ImagePath
                        x:2
                        y:2
                        width:bgImgRect.width-4
                        height:bgImgRect.height-4
                        //smooth: false //关闭平滑
                        //transformOrigin: Image.Center
                        //fillMode: Image.PreserveAspectFit //Image.PreserveAspectFit //保持纵横比
                        rotation:QMLSizeManager.itemVerticalScreen?0:270
                        /*transform: Rotation {
                                        origin.x: backgroundImage.height / 2
                                        origin.y: backgroundImage.width / 2
                                        angle: QMLSizeManager.itemVerticalScreen?0:270
                                        axis: Qt.zAxis
                                    }*/
                        anchors.centerIn: parent
                        anchors.margins: 2
                        /*width:368
                        height:207
                        anchors.centerIn: parent*/
                        //加上这个缩放之后背景边框会变厚,去掉OpacityMask又不变了
                        //fillMode: Image.PreserveAspectFit //Image.PreserveAspectFit //保持纵横比
                        //anchors.centerIn: parent
                        //anchors.margins:2
                        /*anchors {
                            top: parent.top+2
                            left: parent.left+2
                            right: parent.right-2
                            bottom: parent.bottom-2
                            //margins: 2
                        }*/
                        //授权状态图
                        Rectangle{
	                        id:authorRect
	                        x:2
	                        y:5
	                        width:52
	                        height:19
	                        color:"transparent"
	                        visible:isShowAuthImg
	                        /*indicator:*/Image {
	                            id: authorStatusImg
	                            anchors.fill:parent
	                            //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/0.png" 
	                            //source:"qrc:/main/resource/main/Authorized.png" //可以显示已授权
	                            source:authorStatus==1?"qrc:/main/resource/main/Authorized.png":"qrc:/main/resource/main/BeAuthorized.png"
	                        }
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
                            /*onClicked: {
                                //直接调用C++中的函数
                                //QML发送信号调用 C++槽函数,三步：第三步
                                qmlSendSignals(phoneName, phoneInstanceNo,true,phoneInfo)
                                // 在这里可以添加更多的逻辑
                                //发送显示PhoneInstanceWidget窗口的信号
                            }*/
                            onPressed: {
                                // 发送鼠标事件到C++
                                var event = {
                                buttons: mouse.button,
                                x: mouse.x,
                                y: mouse.y,
                                index:itemIndex
                                }
                                console.log(".qml phoneName"+phoneName+" phoneInstanceNo="+phoneInstanceNo+" mouse.button="+mouse.button);
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
                }

                Text {
                    id: labelText
                    text:phoneName //modelData.phoneName //"testtest" //modelData.label
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
