import QtQuick 2.9
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3




ApplicationWindow {

    visible: true
    width: 640
    height: 640
    title: qsTr("IAQ sensoring")












    ScrollView {
        id: scrollview
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        anchors.top: parent.width>parent.height*1.5?parent.top:statusbar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentWidth: availableWidth
        width:parent.width>parent.height*1.5?parent.width/2:parent.width







        ListView {

            anchors.fill: parent
            id: listview

            model: ListModel{



                ListElement { key: "iaq"; name : "IAQ"; unit: " "; sensor:"zmod"}
                ListElement { key: "eco2"; name : "eCO2"; unit: "ppm"; sensor:"zmod" }
                ListElement { key: "tvoc"; name : "TVOC"; unit: "mg/m^3"; sensor:"zmod" }


                ListElement { key: "stabilization"; name: "Stabilization";unit:"%";sensor:"zmod"}
                ListElement { key: "temperature"; name: "Temperature";unit:"C";sensor:"hs3003"}
                ListElement { key: "humidity"; name: "Humidity";unit:"%";sensor:"hs3003"}
                ListElement { key: "bme680_temperature"; name: "Temperature";unit:"C";sensor:"bme680"}
                ListElement { key: "bme680_humidity"; name: "Humidity";unit:"%";sensor:"bme680"}
                ListElement { key: "bme680_pressure"; name: "Pressure";unit:"hPa";sensor:"bme680"}
                ListElement { key: "bme680_iaq"; name: "IAQ";unit:"";sensor:"bme680"}
                ListElement { key: "bme680_eco2"; name: "eCO2";unit:"ppm";sensor:"bme680"}
                ListElement { key: "bme680_evoc"; name: "eVOC";unit:"ppm";sensor:"bme680"}
                ListElement { key: "bme680_accuracy"; name: "Accuracy";unit:"";sensor:"bme680"}
                ListElement { key: "uart04l_pm1"; name: "PM1";unit:"";sensor:"uart04l"}
                ListElement { key: "uart04l_pm25"; name: "PM2.5";unit:"";sensor:"uart04l"}
                ListElement { key: "uart04l_pm10"; name: "PM10";unit:"";sensor:"uart04l"}


                ListElement { key: "cda"; name : "CDA"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "tracking"; name : "Tracking"; unit: ""; sensor:"zmod"}

                ListElement { key: "rmox0"; name : "RMOX0"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox1"; name : "RMOX1"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox2"; name : "RMOX2"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox3"; name : "RMOX3"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox4"; name : "RMOX4"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox5"; name : "RMOX5"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox6"; name : "RMOX6"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox7"; name : "RMOX7"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox8"; name : "RMOX8"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox9"; name : "RMOX9"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox10"; name : "RMOX10"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox11"; name : "RMOX11"; unit: "ohm"; sensor:"zmod"}
                ListElement { key: "rmox12"; name : "RMOX12"; unit: "ohm"; sensor:"zmod"}



            }

            delegate: ItemDelegate {


                font.pointSize: 24
                font.family: "Tahoma"

                width: listview.width


                visible:  (zmodQtData.sensor_map[key] !== undefined)


                Label {

                    id: sensorlabel
                    text: qsTr(sensor)
                    rightPadding: 5
                    leftPadding: 5
                    anchors.top: parent.top
                    anchors.left: parent.left
                     font.bold: true
                     font.pointSize: 6
                     font.family: "Arial"


                }


                Label {

                    text: qsTr(name)
                    rightPadding: 10
                    leftPadding: 10
                    bottomPadding: 10
                    anchors.left: sensorlabel.right
                }

                Label {
                    id:unitlabel
                    width:100

                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    rightPadding: 10
                    leftPadding: 10
                    bottomPadding: 10

                    font.italic: true

                    font.pointSize: 12

                    text: qsTr(unit)
                }


                Label {

                    anchors.right: unitlabel.left
                    rightPadding: 10
                    leftPadding: 10
                    bottomPadding: 10
                    font.bold: true



                    text: (zmodQtData.sensor_map[key] === undefined)?'':zmodQtData.sensor_map[key]
                }


                 MenuSeparator {
                    anchors.bottom: parent.bottom
                    width: listview.width
                 }






            }





        }
    }



    Rectangle{
        id:statusbar
        anchors.top: meter.bottom
        anchors.left: meter.left
      //  anchors.right: parent.right
        anchors.right: meter.right
       // anchors.bottom: scrollview.top
        color: "#EEEEEE"

        height: 20


        Rectangle {
            id:readingindicator

            anchors.right: parent.right
            width: parent.height
            height: parent.height

            color: zmodQtData.sensor_progress>=99?"#B30000":"#EEEEEE"
            border.color: "#EEEEEE"
            border.width: 4

            radius: 4





        }

        Rectangle{

             anchors.left:   parent.left
             anchors.right:  readingindicator.left
             anchors.top: parent.top
             anchors.bottom: readingindicator.bottom
             anchors.leftMargin: 10
             anchors.rightMargin: 2
             anchors.topMargin: 8
             anchors.bottomMargin: 2

             Button {
                    id:resetbutton
                    height:25
                    width: height*4
                    text: qsTr("Reset")
                    font.pointSize: 10

                    contentItem: Text {
                            text: resetbutton.text
                            font: resetbutton.font
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom

                        }


                     onClicked: zmodQtData.sensorReset()
                     anchors.left: parent.left

                     anchors.bottom: parent.bottom

                 }

             Button {
                    id:logbutton
                    height:25
                    width: height*4

                    text: zmodQtData.sensor_logging?"Log Off":"Log On"
                    font.pointSize: 10

                    contentItem: Text {
                            text: logbutton.text
                            font: logbutton.font
                            anchors.fill: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom

                        }


                     onClicked: zmodQtData.sensorLoggingSwtich()
                     anchors.left: resetbutton.right
                     anchors.leftMargin: 5


                     anchors.bottom: parent.bottom

                 }




            Rectangle {
                id:progressbar


                anchors.left: logbutton.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom


            radius: 10
            color:"#EEEEEE"
            border.color: "#CCCCCC"
            border.width: 2


            }

            Rectangle {
                id:progressbar_blue
                anchors.left: logbutton.right
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                  width:progressbar.width/100*zmodQtData.sensor_progress

                Behavior on width {

                    enabled: (!(zmodQtData.sensor_progress >= 99))
                    NumberAnimation {



                        duration: 500

                        easing.type: Easing.Linear
                        }
                    }



            //    width: parent.width/100*(zmodQtData.sensor_progress>100?100:zmodQtData.sensor_progress)






                radius: 10


                color: "blue"


            }



        }
    }




    Rectangle{
        id:meter
//height: width*2/3;
        height: parent.width>parent.height*1.5?parent.height-statusbar.height:width/2;
        color: "#EEEEEE"


        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.width>parent.height*1.5?scrollview.left:parent.right




        Item {
            id: semicircle
            width: parent.width * 0.9
            height: width/2
            anchors.centerIn: parent
            clip:true


            Rectangle{
                id: circle
                width: parent.width
                height: parent.width
                color: "#EEEEEE"

                //radius: parent.width


                layer.enabled: true
                layer.smooth: true
                layer.effect: ShaderEffect {
                    fragmentShader: "
                        #define PI 3.14159265358979323844
                        uniform lowp sampler2D source; // this item
                        uniform lowp float qt_Opacity; // inherited opacity of this item
                        varying highp vec2 qt_TexCoord0;
                        void main() {
                            lowp vec4 p = texture2D(source, qt_TexCoord0);
                            highp vec2 offset = qt_TexCoord0-vec2(0.5);
                            highp vec2 polar = vec2(atan(offset.y,offset.x) * 0.5 / PI + 0.5,length(offset));


                            if (polar.t>0.5){
                                     gl_FragColor = p * qt_Opacity;
                                     return;
                            }

                            if (polar.t>0.49){

                                    gl_FragColor = vec4(0.6,0.6,0.6,0.85) * qt_Opacity;
                                    return;
                            }


                            highp vec4 color = vec4(polar.s>0.25?p.r:polar.s*4.0*p.r,
                                                              polar.s<0.25?p.g:(0.5-polar.s)*4.0*p.g,0.0,p.a);

                            color.rg *= polar.t>0.25?1.0:polar.t*2.0+0.5;


                            color.rg *= mod(polar.s/0.1,2.0)>=1.0?0.95:1.0;

                            gl_FragColor = color * qt_Opacity;
                        }"
                    }

               // gradient: Gradient {
               //     GradientStop { position: 0; color: "white" }
               //     GradientStop { position: 1; color: "steelblue"}
               // }


            }
        }


        Item{
            id: indicator
            width: semicircle.width/20
            height: semicircle.height * 1.75
            anchors.bottom: semicircle.bottom
            anchors.left: semicircle.left
           anchors.leftMargin:  (semicircle.width-width)/2
            anchors.bottomMargin: -semicircle.width/2.6
            clip: true
            rotation: (zmodQtData.sensor_map["iaq"]-1)*36-90
           // rotation: zmodQtData.sensor_map["iaq"]*180

            Behavior on rotation {


                                NumberAnimation {



                                    duration: 2000

                                    easing.type: Easing.InOutQuad
                                    }
                                }


            Rectangle{
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: (parent.height + parent.width )/ 2

                radius: width/5



                gradient: Gradient {
                    GradientStop { position: 0; color: "black" }
                    GradientStop { position: 1; color: "steelblue"}
                }
            }


        }

        Rectangle{
            anchors.bottom: semicircle.bottom
            anchors.left: semicircle.left
            anchors.leftMargin:  (semicircle.width-width)/2
            anchors.bottomMargin: -semicircle.height/10
            width:semicircle.width/5
            height: width
            radius: width
            color: "black"
        }


      //  anchors.bottom: statusbar.top

    }


}
