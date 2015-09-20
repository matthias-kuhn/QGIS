if [ ${QT} == 5 ]; then
  ./ci/travis/linux/qt5/before_install.sh
else
  ./ci/travis/linux/qt4/before_install.sh
fi
