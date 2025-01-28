#include <QRasterWindow>
#include <QPainter>
#include <QImage>
#include <vector>
#include <cstdint>

class RenderWindow : public QRasterWindow {
public:
    RenderWindow() {
        setTitle("3D Renderer with QRasterWindow");
        resize(800, 600);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Render your scene to a framebuffer (simulated here as a simple gradient)
        QImage image = RenderScene(width(), height());

        // Draw the rendered image onto the window
        painter.drawImage(0, 0, image);
    }

private:
    QImage RenderScene(int width, int height) {
        // Simulate rendering to a framebuffer (e.g., gradient effect)
        std::vector<uint32_t> framebuffer(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint8_t red = static_cast<uint8_t>((255 * x) / width);
                uint8_t green = static_cast<uint8_t>((255 * y) / height);
                uint8_t blue = 128;
                framebuffer[y * width + x] = (0xFF << 24) | (red << 16) | (green << 8) | blue;
            }
        }

        // Create QImage from raw framebuffer
        return QImage(reinterpret_cast<uchar*>(framebuffer.data()), width, height, QImage::Format_ARGB32);
    }
};

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RenderWindow window;
    window.show();

    return app.exec();
}
