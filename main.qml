import QtQuick 2.9
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.0

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Dia Projector 2.0 -- ") +  trigger.mode
    color: "white"


    Connections {
        target: trigger
        onTrigger: {
            imageChangeAnimation.changeImage();
        }
    }

    Item {
        anchors.fill: parent
        focus: true


        Image {
            id: backgroundLogo
            anchors.centerIn: parent
            source: "./logo_40jahre.png"
            opacity: 0.4

        }

        FileDialog {
            id: fileSelector
            fileMode: FileDialog.OpenFiles
            options: FileDialog.ReadOnly
            title: "Bilder für die Diashow auswählen"

            property int index: 0

        }



        Component.onCompleted: {
            fileSelector.open()
        }



        Rectangle {
            id: img
            width: parent.width
            height: parent.height
            property alias source: realImg.source
            color: "black"
        }

        Image {
            id: realImg
            anchors.fill: img
            fillMode: Image.PreserveAspectFit
            autoTransform: true
            //            opacity: 0.9

        }

        SequentialAnimation {
            id: imageChangeAnimation

            property int index: 0
            property var images: fileSelector.files

            function changeImage() {
                if(images.length > 0) {
                    index++;
                    index %= images.length;
                    sourceChangeAction.value = images[index];
                    start()
                }


            }

            NumberAnimation { target: img; property: "x"; to: mainWindow.width; duration: 700 }
            PauseAnimation { duration: 500 }
            PropertyAction { id: sourceChangeAction; target: img; property: "source"; }
            NumberAnimation { target: img; property: "x"; from: -img.width; to: 0; duration: 700 }
        }

        Rectangle {
            id: border
            anchors.fill: parent
            anchors.margins: -50
            radius: 75
            color: "transparent"

            border.width: 55
            border.color: "black"

            //            layer.enabled: true
            //            layer.effect: Glow {
            //                samples: 25
            //                color: background.color
            //                radius: 20
            //                transparentBorder: true
            //            }
        }


        Keys.onPressed: {
            if(event.key === Qt.Key_F) {
                if(mainWindow.visibility == Window.FullScreen) {
                    mainWindow.visibility = Window.Windowed;
                } else {
                    mainWindow.visibility = Window.FullScreen;
                }
            }
        }
    }


}
