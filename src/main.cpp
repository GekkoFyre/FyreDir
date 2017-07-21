#include "mainwindow.hpp"
#include "config.hpp"
#include <QApplication>
#include <memory>

#ifdef Q_OS_LINUX
extern "C" {
#include <X11/Xlib.h>
};

#endif

#define BOOST_FILESYSTEM_NO_DEPRECATED

int main(int argc, char *argv[])
{
    // https://github.com/notepadqq/notepadqq/issues/323
    #ifdef __linux__
    Display *d = XOpenDisplay(nullptr);
    std::unique_ptr<Screen> s = std::make_unique<Screen>();
    s.reset(DefaultScreenOfDisplay(d));
    int width = s->width;
    double ratio = ((double)width / FYREDIR_DEFAULT_RESOLUTION_WIDTH);
    if (ratio > FYREDIR_DEFAULT_RATIO_TO_COMPARE) {
        qputenv("QT_DEVICE_PIXEL_RATIO", QString::number(ratio).toLatin1());
    }

    #else
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    #endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
