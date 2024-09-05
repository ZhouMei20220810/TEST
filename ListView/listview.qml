import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModel 1.0
import Qt5Compat.GraphicalEffects

Window {
    id: root
    width:800
    height:600
    visible:true
    title:"test"
    objectName:"rootRect"

    /*Grid{
            id:grid
            x:200
            width:15
            height:150
            columns:3
            visible: true
            Repeater{
                model:grid.width/5*grid.height/5
                Rectangle{
                    width:5
                    height:5
                    color:index%2==0?"red":"black"
                }
            }
    }*/
    Image {
        id: backgroundImage
        source: "qrc:/VM010210085185.png"
        width:207
        height:368
    }

    Rectangle{
        id:bgImgRect
        x:100
        width: backgroundImage.width
        height: backgroundImage.height
        radius: 10
        border.width: 2

    }

    Rectangle{
        width: backgroundImage.width+4
        height:backgroundImage.height+4
        color:"black"
        radius: 10
        OpacityMask {
            x:2
            y:2
                width:backgroundImage.width
                height: backgroundImage.height
                 //anchors.fill: grid
                 source: backgroundImage//grid
                 maskSource:bgImgRect //rectBg
             }
    }

    Button{
        id:subBtn
        y:root.height-50
        width:100
        height: 50
        text:"缩小"
        onClicked: {
            backgroundImage.width=backgroundImage.width*0.9;
            backgroundImage.height=backgroundImage.height*0.9;
        }
    }

    Button{
        x:subBtn.width+10
        y:root.height-50
        width:100
        height: 50
        text:"放大"
        onClicked: {
            backgroundImage.width=backgroundImage.width*1.1;
            backgroundImage.height=backgroundImage.height*1.1;
        }
    }

}
