###########################################################################
#    after_script.sh
#    ---------------------
#    Date                 : June 2017
#    Copyright            : (C) 2017 by Matthias Kuhn
#    Email                : matthias at opengis dot ch
###########################################################################
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
###########################################################################

make install
wget -c "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt*.AppImage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
./linuxdeployqt*.AppImage /home/travis/osgeo4travis/bin/qgis -bundle-non-qt-libs
./linuxdeployqt*.AppImage /home/travis/osgeo4travis/bin/qgis -appimage
# find ./appdir -executable -type f -exec ldd {} \; | grep " => /usr" | cut -d " " -f 2-3 | sort | uniq
# curl --upload-file ./APPNAME*.AppImage https://transfer.sh/APPNAME-git.$(git rev-parse --short HEAD)-x86_64.AppImage
