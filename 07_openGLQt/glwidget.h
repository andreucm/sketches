/****************************************************************************
    **
    ** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
    **
    ** This file is part of the documentation of the Qt Toolkit.
    **
    ** This file may be used under the terms of the GNU General Public
    ** License version 2.0 as published by the Free Software Foundation
    ** and appearing in the file LICENSE.GPL included in the packaging of
    ** this file.  Please review the following information to ensure GNU
    ** General Public Licensing requirements will be met:
    ** http://www.trolltech.com/products/qt/opensource.html
    **
    ** If you are unsure which license is appropriate for your use, please
    ** review the following information:
    ** http://www.trolltech.com/products/qt/licensing.html or contact the
    ** sales department at sales@trolltech.com.
    **
    ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
    ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
    **
    ****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <iostream>
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

class GLWidget : public QGLWidget
{
	Q_OBJECT

	public:
		GLWidget(QWidget *parent = 0);
		~GLWidget();
	
		QSize minimumSizeHint() const;
		QSize sizeHint() const;

	public 
	slots:
		void setXRotation(int angle);
		void fakeFunction();

	signals:
		void xRotationChanged(int angle);

	protected:
		void initializeGL();
		void resizeGL(int width, int height);
		void paintGL();
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		void normalizeAngle(int *angle);
		void drawTensegrity();

/*
		void drawMyModel();
		void drawBoxSet();
		void drawMapStrips();
		void drawMapStripsOld();
		void drawMapQuads();
		void animatedView1(); //sets the eye coordinates
		void animatedView2(); //sets the eye coordinates
*/

		//GLuint objectId;
		int xRot;
		QPoint lastPos;
};
#endif
