export DEBIAN_FRONTEND=noninteractive

cmake --version
clang --version

if [ -d $HOME/deps ]
then
  mkdir -p $HOME/deps
  mkdir -p $HOME/src

  pushd $HOME/src
  wget http://download.osgeo.org/gdal/2.0.1/gdal-2.0.1.tar.gz
  tar xvf gdal-2.0.1.tar.gz
  cd gdal-2.0.1
  ./configure --prefix=$HOME/deps --with-python
  make -j2
  make install
  popd
fi
