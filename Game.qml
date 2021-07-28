import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Timeline 1.0

import whatsthat.game 1.0

Page {
    Rectangle {
        id: menuContainer
        color: "#3bb1ff"
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent

            Rectangle {
                id: rectangle
                width: 200
                height: 100
                color: "#00ffffff"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Image {
                id: qsnImg
                width: 100
                height: 100
                source: "qrc:/qtquickplugin/images/template_image.png"
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
            }

            Image {
                id: wtImg
                source: "img/title.png"
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {
                id: spacer
                width: 200
                height: 100
                color: "#00ffffff"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Connections{
                target: Game
                onShowQsn:{
                    qsnImg.source = img;
                }
             }

        }

    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                pingPong: false
                running: true
                duration: 2000
                loops: -1
                to: 100
                from: 0
            }
        ]
        enabled: true
        startFrame: 0
        endFrame: 1000

        KeyframeGroup {
            target: wtImg
            property: "scale"
            Keyframe {
                frame: 0
                value: 1
            }

            Keyframe {
                frame: 50
                value: 1.1
            }

            Keyframe {
                frame: 100
                value: 1
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:800;width:600}D{i:2}D{i:8}
}
##^##*/
