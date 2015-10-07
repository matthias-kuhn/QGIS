docker run \
  --name qgis-configured \
  -v ${TRAVIS_BUILD_DIR}:/usr/src/QGIS \
  --net=host \
  qgis-build \
  cmake -DWITH_BINDINGS=OFF \
        -DWITH_SERVER=OFF \
        -DWITH_STAGED_PLUGINS=OFF \
        -DWITH_GRASS=OFF \
        -DQSCINTILLA_LIBRARY=/usr/lib/libqt5scintilla2.so \
        -DQSCINTILLA_INCLUDE_DIR=/usr/include/x86_64-linux-gnu/qt5 \
        -DQCA_LIBRARY:FILEPATH=/usr/lib/x86_64-linux-gnu/libqca-qt5.so.2 \
        -DQCA_INCLUDE_DIR:PATH=/usr/include/Qca-qt5/QtCrypto/ \
        -DWITH_QWTPOLAR=OFF \
        -DWITH_APIDOC=OFF \
        -DENABLE_QT5=ON \
        -DWITH_DESTKOP=OFF \
        -DWITH_QTWEBKIT=OFF \
        ../QGIS
docker commit qgis-configured qgis-configured
