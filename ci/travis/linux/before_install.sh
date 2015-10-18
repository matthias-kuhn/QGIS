export DEBIAN_FRONTEND=noninteractive

wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository 'deb http://llvm.org/apt/precise/ llvm-toolchain-precise-3.7 main' -y

sudo add-apt-repository ppa:ubuntugis/ppa -y
sudo add-apt-repository ppa:ubuntugis/ubuntugis-unstable -y # For postgresql-9.1-postgis-2.1
sudo add-apt-repository ppa:smspillaz/cmake-3.0.2 -y
sudo add-apt-repository ppa:kedazo/doxygen-updates-precise -y # For doxygen 1.8.8
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install --force-yes -y --no-install-recommends --no-install-suggests \
        bison \
        cmake \
        cmake-data \
        doxygen \
        flex \
        git \
        graphviz \
        grass-dev \
        libexpat1-dev \
        libfcgi-dev \
        libgdal1-dev \
        libgeos-dev \
        libgsl0-dev \
        libpq-dev \
        libproj-dev \
        libqscintilla2-dev \
        libqtwebkit-dev \
        libqwt-dev \
        libspatialindex-dev \
        libspatialite-dev \
        libsqlite3-dev \
        lighttpd \
        pkg-config \
        poppler-utils \
        python \
        python-dev \
        python-sip \
        python-sip-dev \
        python-gdal \
        spawn-fcgi \
        txt2tags \
        xauth \
        xfonts-100dpi \
        xfonts-75dpi \
        xfonts-base \
        xfonts-scalable \
        xvfb
#        postgresql-9.1-postgis-2.1/trusty # from ubuntugis-unstable, not pgdg

if [ ${QT} == 5 ]; then
  sudo apt-get install --force-yes -y --no-install-recommends --no-install-suggests \
    qtbase5-dev \
    qttools5-dev \
    qt5-default \
    libqt5gui5 \
    qttools5-dev-tools \
    qtscript5-dev \
    qtpositioning5-dev \
    libqt5xmlpatterns5-dev \
    libqt5svg5-dev
    libqwt-qt5-dev \
    libqt5scintilla2-dev \
    libqca-qt5-2-dev \
    libqca-qt5-2-plugins \
else
  sudo apt-get install --force-yes -y --no-install-recommends --no-install-suggests \
    libqca2-dev \
    libqca2-plugin-ossl \
    libqt4-dev \
    libqt4-opengl-dev \
    libqt4-sql-sqlite \
    python-qt4 \
    python-qt4-dev \
    python-qt4-sql \
    pyqt4-dev-tools
fi

cmake --version
clang --version
