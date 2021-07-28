import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import whatsthat.game 1.0

Window {
    width: 600
    height: 800
    visible: true
    title: qsTr("Whats That?")

    Connections{
        target: Game
        onGameStart:{
            stackView.replace("Game.qml");
        }
     }
    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Intro{}
    }



}






