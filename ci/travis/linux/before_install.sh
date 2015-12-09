export DEBIAN_FRONTEND=noninteractive

# Get CMake 3.1
wget https://github.com/Viq111/travis-container-packets/releases/download/cmake-3.1.2/cmake.tar.bz2
tar -xjf cmake.tar.bz2
rm cmake.tar.bz2
export PATH=$(pwd)/cmake/bin:$PATH

cmake --version
clang --version

mkdir -p ${HOME}/deps/src
pushd ${HOME}/deps/src

wget http://download.osgeo.org/gdal/2.0.1/gdal-2.0.1.tar.gz
tar xvf gdal-2.0.1.tar.gz
pushd gdal-2.0.1
./configure --prefix=${HOME}/deps --with-python
make -j2 2> make.log
make install
popd

# Build QCA
wget http://delta.affinix.com/download/qca/2.0/qca-2.1.0.tar.gz
tar xvf qca-2.1.0.tar.gz
mkdir build-qca
pushd build-qca
cmake \
  -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/deps \
  ../qca-2.1.0
make -j2 2> make.log
make install
popd

# Build QWT
wget http://downloads.sourceforge.net/qwt/qwt/6.1.2/qwt-6.1.2.tar.bz2
tar xjf qwt-6.1.2.tar.bz2
# Patch install path
sed -i "s|QWT_INSTALL_PREFIX *=.*$|QWT_INSTALL_PREFIX = ${HOME}/deps|" qwt-6.1.2/qwtconfig.pri
mkdir build-qwt
pushd build-qwt
cat qwtconfig.pri
qmake ../qwt-6.1.2
make -j2
make install
popd

  # Build grass
#  wget https://grass.osgeo.org/grass70/source/grass-7.0.2.tar.gz
#  tar xvf grass-7.0.2.tar.gz
#  mkdir grass-build
#  pushd grass-build
#  cmake \
#    -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/deps \
#    ../grass-7.0.2
#  make -j2
#  make install
#  popd

popd
