import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModel 1.0

Rectangle {
    id: root
    width:800
    height:500    
    objectName:"rootRect"
    GridView {
        id: listView
        objectName:"listView"
        anchors.fill: parent
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

                Text {
                    id: labelText
                    text:modelData.phoneName //"testtest" //modelData.label
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
