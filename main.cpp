#include "tts_app.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    TTSApp window;
    window.show();
    return app.exec();
}