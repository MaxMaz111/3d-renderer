#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLabel label("Hello, Qt!");
    label.resize(400, 300);
    label.show();

    return app.exec();
}
