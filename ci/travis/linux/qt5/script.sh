export PYTHONPATH=${HOME}/osgeo4travis/lib/python3/dist-packages/

xvfb-run ctest -V -E "$(cat blacklist.txt | paste -sd '|' -)" -S ./qgis-test-travis.ctest --output-on-failure
