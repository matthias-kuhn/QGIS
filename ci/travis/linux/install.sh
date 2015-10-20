mkdir build
cd build

CMAKE_OPTIONS="-DWITH_SERVER=ON
               -DWITH_STAGED_PLUGINS=OFF
               -DWITH_GRASS=ON
               -DSUPPRESS_QT_WARNINGS=ON
               -DENABLE_MODELTEST=ON
               -DENABLE_PGTEST=ON
               -DWITH_QWTPOLAR=OFF
               -DWITH_PYSPATIALITE=ON
               -DGRASS_PREFIX7=/usr/lib/grass70
               -DGRASS_INCLUDE_DIR7=/usr/lib/grass70/include"

if [ ${QT} == 5 ]; then
  CMAKE_OPTIONS="${CMAKE_OPTIONS}
                -DENABLE_QT5=ON
                -DWITH_GRASS=OFF
                -DWITH_GRASS7=ON
                -DQSCINTILLA_LIBRARY=/usr/lib/libqt5scintilla2.so
                -DQSCINTILLA_INCLUDE_DIR=/usr/include/x86_64-linux-gnu/qt5
                -DQCA_LIBRARY:FILEPATH=/usr/lib/x86_64-linux-gnu/libqca-qt5.so.2
                -DQCA_INCLUDE_DIR:PATH=/usr/include/Qca-qt5/QtCrypto/
                -DWITH_BINDINGS=OFF"
else
  CMAKE_OPTIONS="${CMAKE_OPTIONS}
               -DWITH_APIDOC=ON"
fi

cmake ${CMAKE_OPTIONS} ..
