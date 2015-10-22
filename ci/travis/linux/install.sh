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
                -DQSCINTILLA_INCLUDE_DIR=/usr/include/qt5
                -DWITH_BINDINGS=OFF"
else
  CMAKE_OPTIONS="${CMAKE_OPTIONS}
               -DWITH_APIDOC=ON"
fi

cmake ${CMAKE_OPTIONS} ..
