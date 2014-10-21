import QtQuick 1.0
import org.qgis 1.0

Window {
    id: mainWindow
    anchors.fill: parent
    color: "black"

    MapPage {
        id: mapPage
        objectName: 'mapPage'
        // Visible by default
        visible: true
        anchors.fill: parent
    }

    Item {
      id: mainmenu
      anchors.fill: parent
      visible: false

      Menu {
        z: 1500
        anchors.centerIn: parent
        model: [ 'Load Project'
               , 'Settings'
               , 'Quit'
        ]

        onValueChanged: {
          switch ( id ) {
            case 0:
              console.log( "loadProject" )
              break
            case 1:
              console.log( "about" )
              break
            case 2:
              Qt.quit()
              break
          }
        }
      }

      OpaqueBackground {
      }
    } // mainmenu

    // The style
   HoloDark {
       id: visual
   }
}
