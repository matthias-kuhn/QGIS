mkdir build
cd build

export INCLUDE="${HOME}/deps/include"
export LIB_DIR="${HOME}/deps"
export PATH="${HOME}/deps/bin:${PATH}"

# Include this line for debug reasons
#      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
cmake \
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
      -DDOXYGEN_EXECUTABLE=${HOME}/doxygen-1.8.10/bin/doxygen \
      ..
