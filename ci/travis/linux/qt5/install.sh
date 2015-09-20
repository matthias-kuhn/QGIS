docker run \
  --name qgis-configured \
  -v ${TRAVIS_BUILD_DIR}:/usr/src/QGIS \
  --net=host \
  qgis-build \
  cmake -DWITH_BINDINGS=OFF \
        -DQSCINTILLA_LIBRARY=/usr/lib/libqt5scintilla2.so \
        -DQSCINTILLA_INCLUDE_DIR=/usr/include/x86_64-linux-gnu/qt5 \
        -DWITH_QWTPOLAR=OFF \
        -DENABLE_QT5=ON \
        -DWITH_QTWEBKIT=OFF \
        ../QGIS
docker commit qgis-configured qgis-configured
