if [ ${QT} == 5 ]; then
  ./ci/travis/linux/qt5/install.sh
else
  ./ci/travis/linux/qt4/install.sh
fi
