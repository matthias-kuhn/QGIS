echo $PATH

export PATH=/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin

ctest -V -E 'qgis_openstreetmaptest|qgis_wcsprovidertest|PyQgsServer' -S ./qgis-test-travis.ctest --output-on-failure

