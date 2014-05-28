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

    #include <QtGui>
    #include <QtOpenGL>
    #include <math.h>
    #include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	xRot = 0;
}

GLWidget::~GLWidget()
{
	makeCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(10, 10);
}

QSize GLWidget::sizeHint() const
{
	return QSize(500, 500);
}

void GLWidget::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != xRot) {
	xRot = angle;
	emit xRotationChanged(angle);
	updateGL();
	}
}

void GLWidget::fakeFunction()
{
		std::cout << "fake!" << std::endl; 
}

void GLWidget::initializeGL()
{
	glClearColor(0.0, 0.0, 0.8, 0.5);	// sets background color to blue
	glClearDepth(1.0);			// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); 	//enables hidden-surface removal
	//object = makeObject();
	//glShadeModel(GL_FLAT);
	//glCullFace(GL_BACK);  //to display back face. We can put also GL_FRONT for front faces
	//glEnable(GL_CULL_FACE);	//enables displaying of both faces of polygons. (enables previous instruction)

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1, 1, 2);
}

void GLWidget::resizeGL(int width, int height)
{
	//int side = qMin(width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, width/height, 1, 10);
	glViewport(0, 0, width, height);
}

void GLWidget::paintGL()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear Screen And Depth Buffer with previous values
	glMatrixMode(GL_MODELVIEW);	//necessary to perform viewing and model transformations
	glLoadIdentity();		//resets the model/view matrix
	/*glTranslated(-10.0,-0.5,-5.0);	//move the camera 10 units far away (or put the object 10 units far away)
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);//rotate the camera along x axis
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);//rotate the camera along y axis
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);//rotate the camera along z axis*/
	gluLookAt(4,4,1,0,0,1,0,0,1);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);//rotate the camera along x axis
	drawTensegrity();
	glFinish();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
	setXRotation(xRot + 8 * dy);
	//setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
	setXRotation(xRot + 8 * dy);
	//setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
}

void GLWidget::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}

void GLWidget::drawTensegrity()
{
	//draws the floor
	glColor3f(0.0,1.0,0.7);
	glBegin(GL_QUADS);//Draws the floor
		//glNormal3f( 0.0, 0.0, 1.0);
		glVertex3f(-1.0,2.0, 0.0);
		glVertex3f(-1.0,-1.0, 0.0);
		glVertex3f( 2.0,-1.0, 0.0);
		glVertex3f( 2.0, 2.0, 0.0);
	glEnd();

	//draws an orange cylinder
	glColor3f(0.8,0.6,0.0);
	//glTranslatef(3.0,0.0,1.0);// moves model origin
	//glRotatef(-90.0,1.0,0.0,0.0);
	gluCylinder(gluNewQuadric(),0.5, 0.5, 3.0, 10,25);
}
