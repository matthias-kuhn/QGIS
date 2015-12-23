export DEBIAN_FRONTEND=noninteractive

##################################################
#
# Get precompiled dependencies
#
##################################################

# Get CMake 3.1
wget https://github.com/Viq111/travis-container-packets/releases/download/cmake-3.1.2/cmake.tar.bz2
tar -xjf cmake.tar.bz2 > /dev/null
rm cmake.tar.bz2
export PATH=$(pwd)/cmake/bin:$PATH

# Get doxygen 1.8.10
if [ ! -d ${HOME}/deps/doxygen-1.8.10 ]; then
  wget ftp://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.10.linux.bin.tar.gz
  tar xvf doxygen-1.8.10.linux.bin.tar.gz -C ${HOME}/deps > /dev/null
fi

##################################################
#
# Compile further dependencies in cached directory
#
##################################################

export LD_LIBRARY_PATH=${HOME}/deps/lib

cmake --version
clang --version

mkdir -p ${HOME}/deps/src
pushd ${HOME}/deps/src

# Build geos
if [ ! -f geos-3.4.2.tar.bz2 ]; then
  wget http://download.osgeo.org/geos/geos-3.4.2.tar.bz2
  tar xjf geos-3.4.2.tar.bz2 > /dev/null
  mkdir build-geos
  pushd build-geos
  cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/deps \
    ../geos-3.4.2
  make -j2 > make.log
  make install
  popd
fi

# Build proj
if [ ! -f http://download.osgeo.org/proj/proj-4.9.0b2.tar.gz ]; then
  wget http://download.osgeo.org/proj/proj-4.9.0b2.tar.gz
  tar xvf proj-4.9.0b2.tar.gz > /dev/null
  pushd proj-4.9.0b2
  ./configure --prefix=${HOME}/deps
  make -j2 > make.log
  make install
  popd
fi

# Build spatialite
if [ ! -f libspatialite-4.3.0a.tar.gz ]; then
  wget http://www.gaia-gis.it/gaia-sins/libspatialite-sources/libspatialite-4.3.0a.tar.gz
  tar xvf libspatialite-4.3.0a.tar.gz > /dev/null
  pushd libspatialite-4.3.0a
  ./configure --prefix=${HOME}/deps \
    --enable-freexl=no \
    --with-geosconfig=${HOME}/deps/bin/geos-config \
    CFLAGS="-I$HOME/deps/include" \
    LDFLAGS="-L${HOME}/deps/lib"
  make -j2 > make.log
  make install
  popd
fi

# Build gdal
if [ ! -f gdal-2.0.1.tar.gz ]; then
  wget http://download.osgeo.org/gdal/2.0.1/gdal-2.0.1.tar.gz
  tar xvf gdal-2.0.1.tar.gz > /dev/null
  pushd gdal-2.0.1
  ./configure --prefix=${HOME}/deps \
    --with-python \
    --with-geos=${HOME}/deps/bin/geos-config
  make -j2 > make.log
  make install
  popd
fi

# Build QCA
if [ ! -f qca-2.1.0.tar.gz ]; then
  wget http://delta.affinix.com/download/qca/2.0/qca-2.1.0.tar.gz
  tar xvf qca-2.1.0.tar.gz > /dev/null
  mkdir build-qca
  pushd build-qca
  cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/deps \
    ../qca-2.1.0
  make -j2 > make.log
  make install
  popd
fi

# Build grass 7
# if [ ! -f grass-7.0.2.tar.gz ]; then
#  wget https://grass.osgeo.org/grass70/source/grass-7.0.2.tar.gz
  tar xvf grass-7.0.2.tar.gz
  pushd grass-7.0.2
  ./configure --prefix=${HOME}/deps \
    --with-cxx \
    --with-sqlite \
    --with-gdal=${HOME}/deps/bin/gdal-config \
    --with-geos=${HOME}/deps/bin/geos-config
  make -j2
  make install
  popd
# fi

ls -al ${HOME}/deps

popd
