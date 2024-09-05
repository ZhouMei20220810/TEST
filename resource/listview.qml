import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModelEx 1.0
import Qt5Compat.GraphicalEffects
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
                signal qmlSendSignals(string strPhoneName, string strInstanceNo, bool bIsShowMenu,S_PHONE_INFO info);
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

                Image {
                    id: backgroundImage
                    source:imagePath
                    width: QMLSizeManager.cellWidth
                    height: QMLSizeManager.cellHeight
                    visible:false
                    //加上这个缩放之后背景边框会变厚
                    //fillMode: Image.PreserveAspectFit //Image.PreserveAspectFit //保持纵横比
                }

                Rectangle{
                    id:bgImgRect
                    width: backgroundImage.width
                    height: backgroundImage.height
                    radius: 2
                    border.width: 2
                    visible:false
                }

                Rectangle{
                    id:opacityRect
                    width: backgroundImage.width+4
                    height:backgroundImage.height+4
                    color:"#FF6B737E"
                    radius: 2
                    OpacityMask {
                        x:2
                        y:2
                            width:backgroundImage.width
                            height: backgroundImage.height
                             //anchors.fill: grid
                             source: backgroundImage//grid
                             maskSource:bgImgRect //rectBg

                         }

                         //背景点击
                         MouseArea {
                            id: itemClickArea
                            anchors.fill: parent
                            onClicked: {
                                //直接调用C++中的函数
                                //console.log("Item was clicked. index="+windowItem.index+"width="+listView.cellWidth +"height="+ listView.cellHeight); //console.log("Item was clicked: " + modelData.name);
                                //QMLSizeManager.itemClicked();
                                //QML发送信号调用 C++槽函数,三步：第三步
                                qmlSendSignals(phoneName, phoneInstanceNo,true,phoneInfo)
                                //itemClickedSignals(windowItem.index, modelData);
                                
                                // 在这里可以添加更多的逻辑
                                //发送显示PhoneInstanceWidget窗口的信号
                            }
                        }
                } 

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

                 CheckBox {
                    id: checkBox
                    checked:bChecked //isChecked
                    anchors.top: opacityRect.top
                    anchors.right: opacityRect.right

                    onCheckedChanged:
                    {
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
                        top: opacityRect.bottom
                        left: parent.left
                        right: parent.right
                        bottom: opacityRect.bottom+28
                        bottomMargin:parent.bottom //距离底部距离
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
