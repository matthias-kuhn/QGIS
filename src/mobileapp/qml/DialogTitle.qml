/***************************************************************************
    DialogTitle.qml  -  Title for dialogs
     --------------------------------------
    Date                 : 30-Jul-2012
    Copyright            : (C) 2012 by Ramon Carrillo
    Email                : racarrillo91 at gmail.com
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
    property alias text: title.text

    height: 48*dp
    width: parent.width
    anchors.top: parent.top

    Text {
        id: title
        color: visual.dialogHeaderForeground
        font.pixelSize: visual.mediumFontSize
        anchors.verticalCenter: parent.verticalCenter
        x: 16*dp
    }

    Rectangle {
        width: parent.width
        height: 2*dp
        anchors.bottom: parent.bottom
        color: visual.dialogHeaderBorder
    }
}
