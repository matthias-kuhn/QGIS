docker run qgis-configured xvfb-run ctest -V -E 'qgis_openstreetmaptest|qgis_wcsprovidertest' -S ../QGIS/qgis-test-travis.ctest --output-on-failure
