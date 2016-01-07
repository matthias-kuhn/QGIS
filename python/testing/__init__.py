# -*- coding: utf-8 -*-

"""
***************************************************************************
    __init__.py
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

import os
import sys

from qgis.core import QgsApplication, QgsFeatureRequest, QgsVectorLayer
from nose2.compat import unittest

# Get a backup, we will patch this one later
_TestCase = unittest.TestCase


class TestCase(_TestCase):

    def assertLayersEqual(self, layer1, layer2, request=QgsFeatureRequest()):
        """
        :param layer1: The first layer to compare
        :param layer2: The second layer to compare
        :param request: Optional, A feature request. This can be used to specify
                        an order by clause to make sure features are compared in
                        a given sequence if they don't match by default.
        """

        # Compare fields
        _TestCase.assertEqual(self, layer1.fields().count(), layer2.fields().count())
        for fieldnum in range(layer1.fields().count()):
            field1 = layer1.fields().at(fieldnum)
            field2 = layer2.fields().at(fieldnum)
            _TestCase.assertEqual(self, field1.name(), field2.name())
            _TestCase.assertEqual(self, field1.type(), field2.type(), 'Fields are not equal: {}({}) != {}({})'.format(field1.typeName(), field1.type(), field2.typeName(), field2.type()))

        # Compare CRS
        _TestCase.assertEqual(self, layer1.dataProvider().crs().authid(), layer2.dataProvider().crs().authid())

        # Compare features
        _TestCase.assertEqual(self, layer1.featureCount(), layer2.featureCount())

        for feats in zip(layer1.getFeatures(request), layer2.getFeatures(request)):
            if feats[0].geometry() is not None:
                geom0 = feats[0].geometry().exportToWkt()
            else:
                geom0 = None
            if feats[1].geometry() is not None:
                geom1 = feats[1].geometry().exportToWkt()
            else:
                geom1 = None
            _TestCase.assertEqual(
                self,
                geom0,
                geom1,
                'Features {}/{} differ in geometry: \n\n {}\n\n vs \n\n {}'.format(
                    feats[0].id(),
                    feats[1].id(),
                    geom0,
                    geom1
                )
            )

            for attrs in zip(feats[0].attributes(), feats[1].attributes(), range(layer1.fields().count())):
                _TestCase.assertEqual(
                    self,
                    attrs[0],
                    attrs[1],
                    'Features {}/{} differ in attribute "{}": "{}" != "{}"'.format(feats[0].id(),
                                                                                   feats[1].id(),
                                                                                   layer1.fields().at(attrs[2]).name(),
                                                                                   attrs[0],
                                                                                   attrs[1]
                                                                                   )
                )

# Patch unittest
unittest.TestCase = TestCase


def start_app():
    """
    Will start a QgsApplication and call all initialization code like
    registering the providers and other infrastructure. It will not load
    any plugins.

    You can always get the reference to a running app by calling `QgsApplication.instance()`.

    The initialization will only happen once, so it is safe to call this method repeatedly.

        Returns
        -------
        QgsApplication

        A QgsApplication singleton
    """
    global QGISAPP

    try:
        QGISAPP
    except NameError:
        myGuiFlag = True  # All test will run qgis in gui mode

        # In python3 we need to convert to a bytes object (or should
        # QgsApplication accept a QString instead of const char* ?)
        try:
            argvb = list(map(os.fsencode, sys.argv))
        except AttributeError:
            argvb = sys.argv

        # Note: QGIS_PREFIX_PATH is evaluated in QgsApplication -
        # no need to mess with it here.
        QGISAPP = QgsApplication(argvb, myGuiFlag)

        QGISAPP.initQgis()
        s = QGISAPP.showSettings()
        print(s)

    return QGISAPP


def stop_app():
    """
    Cleans up and exits QGIS
    """
    global QGISAPP

    QGISAPP.exitQgis()
    del QGISAPP
