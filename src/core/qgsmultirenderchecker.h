/***************************************************************************
    qgsmultirenderchecker.h
     --------------------------------------
    Date                 : 6.11.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSMULTIRENDERCHECKER_H
#define QGSMULTIRENDERCHECKER_H

#include "qgsrenderchecker.h"

/**
 *
 * @note added in 2.8
 */

class CORE_EXPORT QgsMultiRenderChecker
{
  public:
    QgsMultiRenderChecker();

    /**
     * Base directory name for the control image (with control image path
     * suffixed) the path to the image will be constructed like this:
     * controlImagePath + '/' + mControlName + '/' + mControlName + '.png'
     */
    void setControlName( const QString& theName );

    void setMapSettings( const QgsMapSettings& mapSettings );

    /** Set tolerance for color components used by runTest() and compareImages().
     * Default value is 0.
     * @param theColorTolerance is maximum difference for each color component
     * including alpha to be considered correct.
     * @note added in 2.1
     */
    void setColorTolerance( unsigned int theColorTolerance ) { mColorTolerance = theColorTolerance; }

    /**
     * Test using renderer to generate the image to be compared.
     * @param theTestName - to be used as the basis for writing a file to
     * e.g. /tmp/theTestName.png
     * @param theMismatchCount - defaults to 0 - the number of pixels that
     * are allowed to be different from the control image. In some cases
     * rendering may be non-deterministic. This parameter allows you to account
     * for that by providing a tolerance.
     * @note make sure to call setExpectedImage and setMapRenderer first
     */
    bool runTest( const QString& theTestName, unsigned int theMismatchCount = 0 );

    const QString& report() { return mReport; }

    const QString controlImagePath() const;

  private:
    QString mReport;
    QString mControlName;
    unsigned int mColorTolerance;
    QgsMapSettings mMapSettings;
};

#endif // QGSMULTIRENDERCHECKER_H
