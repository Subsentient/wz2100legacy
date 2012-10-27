/*This code copyrighted (2012) for the Warzone 2100 Legacy Project under the GPLv2.
Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/

#include <QtGui/QMessageBox>

#include "lib/framework/frame.h"
#include "lib/ivis_opengl/pieclip.h"
#include "src/warzoneconfig.h"
#include "lib/framework/frameint.h"
#include "wzapp_backend.h"

// used in crash reports & version info

int realmain(int argc, char *argv[]);
int main(int argc, char *argv[])
{
	return realmain(argc, argv);
}

QApplication *appPtr;
WzMainWindow *mainWindowPtr;

void wzMain(int &argc, char **argv)
{
	appPtr = new QApplication(argc, argv);
}

bool wzMain2()
{
	debug(LOG_MAIN, "Qt initialization");
	QGL::setPreferredPaintEngine(QPaintEngine::OpenGL); // Workaround for incorrect text rendering on nany platforms.

	// Setting up OpenGL
	QGLFormat format;
	format.setDoubleBuffer(true);
	format.setAlpha(true);
	int w = pie_GetVideoBufferWidth();
	int h = pie_GetVideoBufferHeight();

	if (war_getFSAA())
	{
		format.setSampleBuffers(true);
		format.setSamples(war_getFSAA());
	}
	mainWindowPtr = new WzMainWindow(QSize(w, h), format);
	WzMainWindow &mainwindow = *mainWindowPtr;
	mainwindow.setMinimumResolution(QSize(640, 480));
	//We had this set to 800x600 for some retarded reason, effectively making 640x480 impossible. How stupid. -Subsentient
	if (!mainwindow.context()->isValid())
	{
		QMessageBox::critical(NULL, "Oops!", "Warzone2100 failed to create an OpenGL context. This probably means that your graphics drivers are out of date. Try updating them!");
		return false;
	}

	screenWidth = w;
	screenHeight = h;
	if (war_getFullscreen())
	{
		mainwindow.resize(w,h);
		mainwindow.showFullScreen();
		if(w>mainwindow.width()) {
			w = mainwindow.width();
		}
		if(h>mainwindow.height()) {
			h = mainwindow.height();
		}
		pie_SetVideoBufferWidth(w);
		pie_SetVideoBufferHeight(h);
	}
	else
	{
		mainwindow.show();
		mainwindow.setMinimumSize(w, h);
		mainwindow.setMaximumSize(w, h);
	}

	mainwindow.setSwapInterval(war_GetVsync());
	war_SetVsync(mainwindow.swapInterval() > 0);

	mainwindow.setReadyToPaint();

	return true;
}

void wzMain3()
{
	QApplication &app = *appPtr;
	WzMainWindow &mainwindow = *mainWindowPtr;
	mainwindow.update(); // kick off painting, needed on macosx
	app.exec();
}

void wzShutdown()
{
	delete mainWindowPtr;
	mainWindowPtr = NULL;
	delete appPtr;
	appPtr = NULL;
}

void wzToggleFullscreen()
{
}

QList<QSize> wzAvailableResolutions()
{
	return WzMainWindow::instance()->availableResolutions();
}

void wzSetSwapInterval(int swap)
{
	WzMainWindow::instance()->setSwapInterval(swap);
}

int wzGetSwapInterval()
{
	return WzMainWindow::instance()->swapInterval();
}
