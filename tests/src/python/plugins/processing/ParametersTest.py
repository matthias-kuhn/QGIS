# -*- coding: utf-8 -*-

"""
***************************************************************************
    ParametersTest.py
    ---------------------
    Date                 : March 2013
    Copyright            : (C) 2013 by Victor Olaya
    Email                : volayaf at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Victor Olaya'
__date__ = 'March 2013'
__copyright__ = '(C) 2013, Victor Olaya'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from utilities import unittest

from plugins.processing.core.parameters import ParameterNumber
from plugins.processing.core.parameters import ParameterCrs
from plugins.processing.core.parameters import ParameterExtent


class ParametersTest(unittest.TestCase):

    def testParameterNumber(self):
        param = ParameterNumber('name', 'desc', 0, 10, optional=True)
        assert not param.setValue('wrongvalue')
        assert param.value is None
        assert not param.setValue(25)
        assert param.value is None
        assert param.setValue(5)
        assert param.value == 5
        assert param.setValue(None)
        assert param.value == param.default

    def testParameterCRS(self):
        param = ParameterCrs('name', 'desc', optional=True)
        assert param.setValue('EPSG:12003')
        assert param.value == 'EPSG:12003'
        assert param.setValue(None)
        assert param.value == param.default

    def testParameterExtent(self):
        param = ParameterExtent('name', 'desc', optional=True)
        assert not param.setValue('0,2,0')
        assert not param.setValue('0,2,0,a')
        assert param.setValue('0,2,2,4')
        self.assertEqual(param.value, '0,2,2,4')
        assert param.setValue(None)
        self.assertEqual(param.value, None)

        required = ParameterExtent('name', 'desc', optional=False)
        first_value = '0,2,2,4'
        assert required.setValue(first_value)
        assert not required.setValue(None)
        self.assertEqual(required.value, first_value)

def suite():
    suite = unittest.makeSuite(ParametersTest, 'test')
    return suite


def runtests():
    result = unittest.TestResult()
    testsuite = suite()
    testsuite.run(result)
    return result

if __name__ == '__main__':
    unittest.main()
