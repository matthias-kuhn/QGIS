mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DWITH_SERVER=ON \
      -DWITH_STAGED_PLUGINS=OFF \
      -DWITH_GRASS=ON \
      -DSUPPRESS_QT_WARNINGS=ON \
      -DENABLE_MODELTEST=ON \
      -DENABLE_PGTEST=ON \
      -DWITH_QWTPOLAR=OFF \
      -DWITH_APIDOC=ON \
      -DWITH_ASTYLE=ON \
      -DWITH_PYSPATIALITE=ON \
      -DGRASS_PREFIX7=/usr/lib/grass70 \
      -DGDAL_CONFIG=/home/travis/deps/bin/gdal-config \
      -DGRASS_INCLUDE_DIR7=/usr/lib/grass70/include ..
