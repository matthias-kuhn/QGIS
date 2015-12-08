export DEBIAN_FRONTEND=noninteractive

cmake --version
clang --version

if [ -d $HOME/deps ]
then
  mkdir -p $HOME/deps
  mkdir -p $HOME/src

#  pushd $HOME/src
#  wget http://download.osgeo.org/gdal/2.0.1/gdal-2.0.1.tar.gz
#  tar xvf gdal-2.0.1.tar.gz
#  pushd gdal-2.0.1
#  ./configure --prefix=$HOME/deps --with-python
#  make -j2
#  make install
#  popd

  # Build QCA
  wget http://delta.affinix.com/download/qca/2.0/qca-2.1.0.tar.gz
  tar xvf qca-2.1.0.tar.gz
  mkdir build-qca
  pushd build-qca
  cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=$HOME/deps \
    ../qca-2.1.0
  make -j2
  make install
  popd

  # Build grass
  wget https://grass.osgeo.org/grass70/source/grass-7.0.2.tar.gz
  tar xvf grass-7.0.2.tar.gz
  mkdir grass-build
  pushd grass-build
  cmake \
    -DCMAKE_INSTALL_PREFIX:PATH=$HOME/deps \
    ../grass-7.0.2
  make -j2
  make install
  popd

  popd
fi
