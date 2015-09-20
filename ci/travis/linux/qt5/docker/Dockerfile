FROM      docker.io/jemc/qt5
MAINTAINER Matthias Kuhn <matthias@opengis.ch>

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y --force-yes --no-install-recommends --no-install-suggests \
                       libqwt-qt5-dev libqt5scintilla2-dev clang \
                       bison cmake cmake-data doxygen flex git graphviz \
                       grass-dev grass7-dev libexpat1-dev libfcgi-dev \
                       libgdal1-dev libgeos-dev libgsl0-dev libpq-dev \
                       libproj-dev libqscintilla2-dev \
                       libqtwebkit-dev \
                       libqwt-dev libspatialindex-dev libspatialite-dev \
                       libsqlite3-dev lighttpd pkg-config poppler-utils \
                       python python-dev \
                       python-sip python-sip-dev spawn-fcgi \
                       txt2tags xauth xfonts-100dpi xfonts-75dpi xfonts-base \
                       xfonts-scalable xvfb

mkdir /usr/src/QGIS/build
WORKDIR /usr/src/QGIS/build
RUN cmake .. -DWITH_BINDINGS=OFF -DQSCINTILLA_LIBRARY=/usr/lib/libqt5scintilla2.so -DQSCINTILLA_INCLUDE_DIR=/usr/include/x86_64-linux-gnu/qt5 -DWITH_QWTPOLAR=OFF
