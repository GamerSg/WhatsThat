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

        states: [
            State {
                name: "loaded"

                PropertyChanges {
                    target: introTimeline
                    enabled: true
                }

                PropertyChanges {
                    target: introAnimation
                    running: false
                }

                PropertyChanges {
                    target: menuAnimation
                    running: true
                }
            }
        ]

        Component.onCompleted: {
            //Game.listenFor("start");
        }

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent

            Image {
                id: titleLbl
                source: "img/title.png"
                fillMode: Image.PreserveAspectFit
                scale: 5
                Layout.topMargin: 0
                Layout.fillHeight: false
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Rectangle {
                id: spacer2
                width: 200
                height: 200
                opacity: 0
                visible: true
                color: "#ffffff"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }


            Text {
                id: micBtn
                width: 100
                height: 100
                color: "#ffffff"
                text: "Say \"Start\" to begin"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                font.pointSize: 15
                opacity: 0
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                //fillMode: Image.PreserveAspectFit

            }

            Rectangle {
                id: spacer
                width: 200
                height: 100
                color: "#00ffffff"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }



        Timeline {
            id: introTimeline
            animations: [
                TimelineAnimation {
                    id: introAnimation
                    onFinished: menuContainer.state = "loaded"
                    running: true
                    duration: 4000
                    loops: 1
                    to: 200
                    from: 0
                },
                TimelineAnimation {
                    id: menuAnimation
                    running: false
                    duration: 3000
                    loops: -1
                    to: 230
                    from: 200
                }
            ]

            endFrame: 230
            enabled: true
            startFrame: 0
            property bool playedIntro: false
            property bool playedPrompt: false
            onCurrentFrameChanged: {
                var frame = Math.floor(currentFrame);
                //console.log(frame);
                if (frame == 100 && playedIntro==false)
                {
                    Game.openingMenu();
                    playedIntro = true;
                }
                else if(frame == 199 && playedPrompt==false)
                {
                    Game.speak("Say start to begin!");
                    playedPrompt = true;
                }
            }

            KeyframeGroup {
                target: titleLbl
                property: "scale"
                Keyframe {
                    frame: 100
                    value: 5
                }

                Keyframe {
                    frame: 150
                    value: 1
                }
            }

            KeyframeGroup {
                target: titleLbl
                property: "y"
                Keyframe {
                    easing.bezierCurve: [0.00311,0.396,0.646,1,1,1]
                    frame: 200
                    value: 100
                }

                Keyframe {
                    frame: 150
                    value: 240
                }

                Keyframe {
                    frame: 100
                    value: 0
                }
            }

            KeyframeGroup {
                target: micBtn
                property: "opacity"
                Keyframe {
                    frame: 200
                    value: 0
                }

                Keyframe {
                    frame: 206
                    value: 1
                }

                Keyframe {
                    value: 0
                    frame: 212
                }

                Keyframe {
                    value: 1
                    frame: 218
                }

                Keyframe {
                    value: 0
                    frame: 224
                }
            }

            KeyframeGroup {
                target: overlay
                property: "color"
                Keyframe {
                    value: "#000000"
                    frame: 0
                }

                Keyframe {
                    value: "#000000"
                    frame: 100
                }

                Keyframe {
                    value: "#00000000"
                    frame: 200
                }
            }

            KeyframeGroup {
                target: presentTxt
                property: "visible"
                Keyframe {
                    value: false
                    frame: 100
                }
            }
        }


    }

    Rectangle {
        id: overlay
        anchors.fill: parent


        ColumnLayout {
            id: presentLayout
            anchors.fill: parent

            Text {
                id: presentTxt
                height: 100
                color: "#ffffff"
                text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif','Comic Sans MS'; font-size:26pt;\">Luv Khemani</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif','Comic Sans MS'; font-size:26pt;\">2071541c </span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif','Comic Sans MS'; font-size:26pt;\">presents</span></p></body></html>"
                font.pixelSize: 32
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                textFormat: Text.RichText
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                maximumLineCount: 3
                transformOrigin: Item.Center
            }
        }
    }

}
