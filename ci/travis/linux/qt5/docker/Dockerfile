FROM      ubuntu:15.04
MAINTAINER Matthias Kuhn <matthias@opengis.ch>

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y --force-yes --no-install-recommends --no-install-suggests \
                       libqwt-qt5-dev libqt5scintilla2-dev clang \
                       bison cmake cmake-data doxygen flex git graphviz \
                       grass-dev libexpat1-dev libfcgi-dev \
                       libgdal1-dev libgeos-dev libgsl0-dev libpq-dev \
                       libproj-dev libgeos++-dev \
                       libspatialindex-dev libspatialite-dev \
                       libsqlite3-dev lighttpd pkg-config poppler-utils \
                       python python-dev \
                       libqca-qt5-2-dev libqca-qt5-2-plugins \
                       python-sip python-sip-dev spawn-fcgi \
                       txt2tags xauth xfonts-100dpi xfonts-75dpi xfonts-base \
                       xfonts-scalable xvfb libqt5svg5-dev qtpositioning5-dev \
                       qtscript5-dev libqt5xmlpatterns5-dev qttools5-dev \
                       qttools5-dev-tools ccache

RUN mkdir -p /usr/src/build
COPY /do-test.sh /usr/src/build/do-test.sh
WORKDIR /usr/src/build
