/***************************************************************************
    ToolButton.qml  -  Tool button based on Android design guidelines
     --------------------------------------
    Date                 : 20.10.2014
    Copyright            : (C) 2014 by Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 1.1

Item {
  id: container

  property alias iconHeight: actionButton.iconHeight
  property alias iconWidth: actionButton.iconWidth

  property variant model: []

  signal clicked()
  signal pressAndHold()
  signal currentToolChanged( string id )

  anchors.margins: 5*dp

  onModelChanged: {
    model.forEach(
          function( element ) {

          }
    )
  }

  ActionButton
  {
    id: actionButton
    onClicked: {
      container.currentToolChanged( 'test' )
    }
  }

  Menu {
    id: menu
    visible: false
  }
}
