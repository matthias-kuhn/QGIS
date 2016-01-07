# -*- coding: utf-8 -*-

"""
***************************************************************************
    test_algorithms.py
    ---------------------
    Date                 : January 2016
    Copyright            : (C) 2016 by Matthias Kuhn
    Email                : matthias@opengis.ch
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Matthias Kuhn'
__date__ = 'January 2016'
__copyright__ = '(C) 2016, Matthias Kuhn'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = ':%H$'

import qgis
import os
import yaml
import nose2

import tempfile

import processing

from processing.gui import AlgorithmExecutor

from qgis.core import (
    QgsVectorLayer,
    QgsRasterLayer,
    QgsMapLayerRegistry
)

from qgis.testing import (
    start_app,
    unittest
)

from utilities import (
    unitTestDataPath
)


def processingTestDataPath():
    return os.path.join(os.path.dirname(__file__), 'testdata')


class TestAlgorithms(unittest.TestCase):

    @classmethod
    def setUp(cls):
        start_app()
        from processing.core.Processing import Processing
        Processing.initialize()

    def test_algorithms(self):
        """
        This is the main test function. All others will be executed based on the definitions in testdata/algorithm_tests.yaml
        """
        with open(os.path.join(processingTestDataPath(), 'algorithm_tests.yaml'), 'r') as stream:
            algorithm_tests = yaml.load(stream)

        for algtest in algorithm_tests['tests']:
            yield self.check_algorithm, algtest['name'], algtest

    def check_algorithm(self, name, defs):
        """
        Will run an algorithm definition and check if it generates the expected result
        :param name: The identifier name used in the test output heading
        :param defs: A python dict containing a test algorithm definition
        """
        params = self.load_params(defs['params'])

        alg = processing.Processing.getAlgorithm(defs['algorithm']).getCopy()
        for param in zip(alg.parameters, params):
            param[0].setValue(param[1])

        for r, p in defs['results'].iteritems():
            alg.setOutputValue(r, self.load_result_param(p))

        self.assertTrue(AlgorithmExecutor.runalg(alg))
        print(alg.getAsCommand())
        self.check_results(alg.getOutputValuesAsDictionary(), defs['results'])

    def load_params(self, params):
        """
        Loads an array of parameters
        """
        return [self.load_param(p) for p in params]

    def load_param(self, param):
        """
        Loads a parameter. If it's not a map, the parameter will be returned as-is. If it is a map, it will process the
        parameter based on its key `type` and return the appropriate parameter to pass to the algorithm.
        """
        try:
            if param['type'] == 'vector' or param['type'] == 'raster':
                return self.load_layer(param)
        except TypeError:
            # No type specified, use whatever is there
            return param

        raise KeyError("Unknown type '{}' specified for parameter '{}'".format(param['type'], param['name']))

    def load_result_param(self, param):
        """
        Lodas a result parameter. Creates a temporary destination where the result should go to and returns this location
        so it can be sent to the algorithm as parameter.
        """
        if param['type'] == 'vector':
            outdir = tempfile.mkdtemp()
            basename = os.path.basename(param['name'])
            filepath = os.path.join(outdir, basename)
            return filepath

        raise KeyError("Unknown type '{}' specified for parameter '{}'".format(param['type'], param['name']))

    def load_layer(self, param):
        """
        Loads a layer which was specified as parameter.
        """
        if param['location'] == 'qgs':
            prefix = unitTestDataPath()
        elif param['location'] == 'proc':
            prefix = processingTestDataPath()
        else:
            prefix = ''

        filepath = os.path.join(prefix, param['name'])

        if param['type'] == 'vector':
            lyr = QgsVectorLayer(filepath, param['name'], 'ogr')
        elif param['type'] == 'raster':
            lyr = QgsRasterLayer(filepath, param['name'], 'ogr')

        self.assertTrue(lyr.isValid(), 'Could not load layer "{}"'.format(filepath))
        QgsMapLayerRegistry.instance().addMapLayer(lyr)
        return lyr

    def check_results(self, results, expected):
        """
        Checks if result produced by an algorithm matches with the expected specification.
        """
        for id, expected_result in expected.iteritems():
            if expected_result['type'] == 'vector':
                expected_lyr = self.load_layer(expected_result)
                try:
                    results[id]
                except KeyError as e:
                    raise KeyError('Expected result {} does not exist in {}', e.message, results.keys())

                result_lyr = QgsVectorLayer(results[id], id, 'ogr')

                self.assertLayersEqual(expected_lyr, result_lyr)

if __name__ == '__main__':
    nose2.main()
