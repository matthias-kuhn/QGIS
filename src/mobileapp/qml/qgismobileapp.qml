import QtQuick 1.0
import org.qgis 1.0

Window {
  id: mainWindow
  anchors.fill: parent; color: "black"

  MapPage {
    id: mapPage
    objectName: 'theMapPage'
    // Visible by default
    visible: true
    anchors.fill: parent
  }
  Rectangle {
    anchors.centerIn: parent
    width: 100; height: 40; radius: 5; color: "lightgray"

    Text { anchors.centerIn: parent; text: "Quit"; color: "black" }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }
  }

  Image { source: "/images/icon" }

 Item {
id: mainmenu
anchors.fill: parent
visible: false
OpaqueBackground {}
Menu {
anchors.centerIn: parent
model: ["Settings", "About", "Quit"]
onValueChanged: {
switch( index ) {
case 0:
console.log("settings")
break;
case 1:
console.log("about");
break;
case 2:
Qt.quit()
break;
}
}
}
} // mainmenu
}
