if [ ${QT} == 5 ]; then
  ./ci/travis/linux/qt5/script.sh
else
  ./ci/travis/linux/qt4/script.sh
fi
