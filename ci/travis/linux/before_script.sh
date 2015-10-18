if [ ${QT} == 5 ]; then
  ./ci/travis/linux/qt5/before_script.sh
else
  ./ci/travis/linux/qt4/before_script.sh
fi
