mkdir build
cd build

export INCLUDE="${HOME}/deps/include"
export LIB_DIR="${HOME}/deps/lib"
export PATH="${HOME}/deps/bin:${PATH}"

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
      -DGDAL_CONFIG=/home/travis/deps/bin/gdal-config \
      ..
