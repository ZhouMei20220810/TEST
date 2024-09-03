import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModel 1.0

Canvas{
    id: root
    width:QMLSizeManager.windowWidth
    height:QMLSizeManager.windowHeight  
    objectName:"rootRect"
    property int borderWidth: 2
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
        model:MyListModel.items //listModel.items
        delegate: Component {
            Rectangle {
                id: windowItem
                objectName:"btnBg"
                /*x:15
                y:15*/ //无效
                width: QMLSizeManager.cellWidth //207;
                height:QMLSizeManager.cellHeight+20+30 //396 //更加单元格与实际的差值，形成间隔
                /*color: "transparent"
                border.color: "#FF6B737E"
                border.width: 2*/
                //QML发送信号调用 C++槽函数,三步：第一步
                //signal qmlSendSignals(bool bIsShowMenu);
                signal qmlSendSignals(int iId,string strPhoneName, string strInstanceNo, string strExpireTime, bool bIsShowMenu);
                //signal itemClickedSignals(int index, QVariant data);
                signal notifyRefreshWindow();
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
                    qmlSendSignals.connect(MyListModel.ShowInstanceSignalFromQMLFile)
                    //itemClickedSignals.connect(MyListModel.do_ItemClickSignals)
                    notifyRefreshWindow.connect(MyListModel.do_notifyRefreshWindow)
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
                        source:modelData.ImagePath //item.ImagePath
                        x:2
                        y:2
                        width: QMLSizeManager.cellWidth
                        height: QMLSizeManager.cellHeight
                        //smooth: false //关闭平滑
                        fillMode: Image.PreserveAspectFit //Image.PreserveAspectFit //保持纵横比
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
	                        visible:modelData.bShowAuthorImg
	                        /*indicator:*/Image {
	                            id: authorStatusImg
	                            anchors.fill:parent
	                            //source:"file:///C:/Users/Administrator/AppData/Local/Temp/YiShunYun/0.png" 
	                            //source:"qrc:/main/resource/main/Authorized.png" //可以显示已授权
	                            source:modelData.authorStatus==1?"qrc:/main/resource/main/Authorized.png":"qrc:/main/resource/main/BeAuthorized.png"
	                        }
                        }

                            CheckBox {
                            id: checkBox
                            checked: modelData.checkBox
                            anchors.top: parent.top
                            anchors.right: parent.right
                            checkState: allChildrenChecked ? Qt.Checked :
                                                anyChildChecked ? Qt.PartiallyChecked : Qt.Unchecked
                            onCheckedChanged: {
                                // 更新模型中的checked状态
                                                modelData.checked = checked;
                                console.log("onCheckedChanged "+modelData.checked)

                                                // 强制刷新当前项
                                                parent.updateCurrentItem();
                            }
                            /*onCheckStateChanged: {
                                console.log("do_notifyRefreshWindow");
                                notifyRefreshWindow();
                            }
                            MouseArea {
                                id: checkBoxClickArea
                                anchors.fill: parent
                                onClicked: {
                                    console.log("checkBoxClickArea was clicked."+modelData.phoneName); //console.log("Item was clicked: " + modelData.name);
                                    // 在这里可以添加更多的逻辑
                                    //发送显示PhoneInstanceWidget窗口的信号
                                    checked = !checked;
                                    //modelData.setCheckBox(checked);
                                    modelData.setCheckBox(checked);
                            
                                    //modelData.setCheckBox(checkState == Qt.Checked);
                                }
                            }*/
                            //anchors.margins: 10
                            /*MouseArea {
                                id: checkBoxClickArea
                                anchors.fill: parent
                                onClicked: {
                                    console.log("checkBoxClickArea was clicked."+modelData.name); //console.log("Item was clicked: " + modelData.name);
                                    // 在这里可以添加更多的逻辑
                                    //发送显示PhoneInstanceWidget窗口的信号
                                    notifyRefreshWindow();
                                }
                            }*/
                        }

                        onStatusChanged: {
                            console.log("Image status changed:", status);
                            if (status === Image.Error) {
                                console.log("Image error:", errorString);
                            }
                        }
                        MouseArea {
                            id: itemClickArea
                            anchors.fill: parent
                            onClicked: {
                                //直接调用C++中的函数
                                QMLSizeManager.itemClicked();
                                //QML发送信号调用 C++槽函数,三步：第三步
                                qmlSendSignals(modelData.phoneId,modelData.phoneName, modelData.phoneInstanceNo,modelData.ExpireTime, true)
                                //itemClickedSignals(windowItem.index, modelData);
                                console.log("Item was clicked. width="+listView.cellWidth +"height="+ listView.cellHeight); //console.log("Item was clicked: " + modelData.name);
                                // 在这里可以添加更多的逻辑
                                //发送显示PhoneInstanceWidget窗口的信号
                            }
                        }
                    }
                }

                

                Text {
                    id: labelText
                    text:modelData.phoneName //"testtest" //modelData.label
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
