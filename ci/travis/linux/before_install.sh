###########################################################################
#    before_install.sh
#    ---------------------
#    Date                 : March 2016
#    Copyright            : (C) 2016 by Matthias Kuhn
#    Email                : matthias at opengis dot ch
###########################################################################
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
###########################################################################

export DEBIAN_FRONTEND=noninteractive
export CORES=2

##################################################
#
# Get precompiled dependencies
#
##################################################

pushd ${HOME}

# Setup Qt repo
sudo add-apt-repository -y ppa:beineri/opt-qt57-trusty
# Setup clang repo
echo "deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main" | sudo tee -a /etc/apt/sources.list
sudo wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-get -y update

# Install Qt deps
sudo apt-get -y install qt5-qmake qtbase5-dev libqt5xmlpatterns5-dev
/opt/qt57/bin/qt57-env.sh

# Install clang
sudo apt-get install clang-3.9 lldb-3.9

# Install cmake (requires PATH mangling)
curl -L https://cmake.org/files/v3.7/cmake-3.7.1-Linux-x86_64.tar.gz | tar --strip-components=1 -zxC /home/travis/apps

# Download OTB package for Processing tests
wget https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-5.6.0-Linux64.run -O /home/travis/OTB-5.6.0-Linux64.run && sh /home/travis/OTB-5.6.0-Linux64.run

popd

pip install psycopg2 numpy nose2 pyyaml mock future termcolor
