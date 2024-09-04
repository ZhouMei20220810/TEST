import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLSizeManager 1.0
import MyListModelEx 1.0

Window{
    id: root
    width:800
    height:600
    visible:true
    title:"MyListModelEx 1.0"
    objectName:"rootRect"
    property int borderWidth: 2
    // 定义一个属性来控制图片更新
    ListView
    {
        width:800
        height:600
        id:listview
        model:MyListModelEx
        /*model:ListModel{
        ListElement{
        name:"zhangsan"
        value:"111"
        }
        ListElement{
        name:"lisi"
        value:"222"
        }
        ListElement{
        name:"wangwu"
        value:"333"
        }
        }*/

        delegate:Text
        {
            id:txt
            width:100
            height:20
            text: name //+ " "+ value
        }
    }
}
