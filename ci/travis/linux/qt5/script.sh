docker run \
  -v ${TRAVIS_BUILD_DIR}:/usr/src/QGIS \
  -e "TRAVIS_BUILD_DIR=/usr/src/QGIS" \
  -e "TRAVIS_BRANCH=${TRAVIS_BRANCH}" \
  -e "TRAVIS_PULL_REQUEST=${TRAVIS_PULL_REQUEST}" \
  -e "TRAVIS_COMMIT=${TRAVIS_COMMIT}" \
  qgis-configured /usr/src/build/do-test.sh
