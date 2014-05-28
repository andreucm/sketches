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

#include "window.h"

qtWindow::qtWindow()
{
	glWidget = new GLWidget;
	xSlider = createSlider();
	button = new QPushButton("&Press Me!");
	click = new QTimer(this);

	connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
	connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
	connect(button, SIGNAL(clicked()), glWidget, SLOT(fakeFunction()));
	connect(click, SIGNAL(timeout()), glWidget, SLOT(updateGL()));

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(glWidget);
	mainLayout->addWidget(xSlider);
	mainLayout->addWidget(button);
	setLayout(mainLayout);

	xSlider->setValue(0 * 16);
	setWindowTitle(tr("openGL and Qt"));

	click->start(1000);
}

QSlider *qtWindow::createSlider()
{
	QSlider *slider = new QSlider(Qt::Vertical);
	slider->setRange(0, 360 * 16);
	slider->setSingleStep(16);
	slider->setPageStep(15 * 16);
	slider->setTickInterval(15 * 16);
	slider->setTickPosition(QSlider::TicksRight);
	return slider;
}
