export DEBIAN_FRONTEND=noninteractive

wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty main' -y

sudo add-apt-repository ppa:ubuntugis/ppa -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install --force-yes --no-install-recommends --no-install-suggests \
        bison \
        cmake \
        cmake-data \
        doxygen \
        flex \
        gdal-bin \
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
        libqca2-dev \
        libqca2-plugin-ossl \
        libqscintilla2-dev \
        libqtwebkit-dev \
        libqwt-dev \
        libspatialindex-dev \
        libspatialite-dev \
        libsqlite3-dev \
        lighttpd \
        pkg-config \
        poppler-utils \
        spawn-fcgi \
        txt2tags \
        xauth \
        xfonts-100dpi \
        xfonts-75dpi \
        xfonts-base \
        xfonts-scalable \
        xvfb \
        python-pip \
        flip \
        jq

sudo -H pip install autopep8 # TODO when switching to trusty or above: replace python-pip with python-autopep8
sudo -H pip install nose2 pyyaml mock

#update clang
sudo apt-get install --force-yes llvm-3.8 llvm-3.8-dev clang-3.8 libstdc++-4.9-dev

sudo ln -s /usr/bin/ccache /usr/lib/ccache/clang-3.8
sudo ln -s /usr/bin/ccache /usr/lib/ccache/clang++-3.8
