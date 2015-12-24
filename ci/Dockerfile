FROM ubuntu:12.04

RUN apt-get update
RUN apt-get install -y libqscintilla2-dev libqt4-dev libqt4-opengl-dev libqt4-sql-sqlite  libqtwebkit-dev   libqwt-dev       pyqt4-dev-tools python-qt4-dev     python-qt4-sql       python-sip-dev python-dev bzip2 wget clang bison cmake-data flex graphviz  libexpat1-dev libfcgi-dev libgeos-dev libgsl0-dev libpq-dev libproj-dev libspatialindex-dev libsqlite3-dev pkg-config poppler-utils txt2tags xauth xvfb flip fftw-dev git
RUN mkdir -p /home/travis/deps
ENV HOME=/home/travis
RUN apt-get install make
ENV CC=clang
ENV CXX=clang++
ENV LD_LIBRARY_PATH=/home/travis/deps/lib
COPY travis/linux /linux
RUN bash /linux/before_install.sh
