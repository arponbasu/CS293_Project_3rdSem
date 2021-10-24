#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>
#include <cmath>
#include <iostream>


static constexpr int IMAGE_WIDTH = 1000;
static constexpr int IMAGE_HEIGHT = 600;
static constexpr long double ZOOM_FACTOR = 0.9;
static constexpr int OFFSET_FACTOR = 40;


class Mandelbrot {
public:
    Mandelbrot();
    void updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, std::string mode) const; 
private:
    static const int MAX = 127; // maximum number of iterations for mandelbrot()
                         // don't increase MAX or the colouring will look strange
    std::array<sf::Color, MAX+1> colors;

    int mandelbrot(double startReal, double startImag) const;
    sf::Color getColor(int iterations) const;
    sf::Color getSmoothColor(double startReal, double startImag) const;
    std::vector<double> getVectorColor(int iterations) const;
    void updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY, std::string mode) const;
};

Mandelbrot::Mandelbrot() {
    for (int i=0; i <= MAX; ++i) {
        colors[i] = getColor(i);
    }
}

int Mandelbrot::mandelbrot(double startReal, double startImag) const {
    double zReal = startReal;
    double zImag = startImag;

    for (int counter = 0; counter < MAX; ++counter) {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;
        if (r2 + i2 > 4.0) {
            return counter;
        }
        zImag = 2.0 * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
    }
    return MAX;
}

sf::Color Mandelbrot::getColor(int iterations) const {
    int r, g, b;

    // colour gradient:      Red -> Blue -> Green -> Red -> Black
    // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
    if (iterations < 16) {
        r = 16 * (16 - iterations);
        g = 0;
        b = 16 * iterations - 1;
    } else if (iterations < 32) {
        r = 0;
        g = 16 * (iterations - 16);
        b = 16 * (32 - iterations) - 1;
    } else if (iterations < 64) {
        r = 8 * (iterations - 32);
        g = 8 * (64 - iterations) - 1;
        b = 0;
    } else { // range is 64 - 127
        r = 255 - (iterations - 64) * 4;
        g = 0;
        b = 0;
    }
    return sf::Color(r, g, b);
}

std::vector<double> Mandelbrot::getVectorColor(int iterations) const {
    double r, g, b;
    // colour gradient:      Red -> Blue -> Green -> Red -> Black
    // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
    if(iterations <= 0){
        r = 0.0;
        g = 0.0;
        b = 0.0;

    } else if (0 < iterations && iterations < 16) {
        r = 16.0 * (16 - iterations);
        g = 0.0;
        b = 16.0 * iterations - 1;
    } else if (iterations < 32) {
        r = 0.0;
        g = 16.0 * (iterations - 16);
        b = 16.0 * (32 - iterations) - 1;
    } else if (iterations < 64) {
        r = 8.0 * (iterations - 32);
        g = 8.0 * (64 - iterations) - 1;
        b = 0.0;
    } else { // range is 64 - 127
        r = 255.0 - (iterations - 64) * 4;
        g = 0.0;
        b = 0.0;
    }
    return {r, g, b};
}

double absoluteValue (double startReal, double startImag){
    return sqrt(startReal*startReal + startImag*startImag);

}

void multScalar (std::vector<double>& v, double k){
    for(int i = 0; i < v.size(); ++i) v[i] *= k;
}

std::vector<double> addVector (std::vector<double>& v1, std::vector<double>& v2){
    std::vector<double> retval;
    for(int i = 0; i < v1.size(); ++i) retval.push_back(v1[i] + v2[i]);
    return retval;
}

sf::Color Mandelbrot::getSmoothColor(double startReal, double startImag) const {

    double zReal = startReal;
    double zImag = startImag;
    double expiter = 0;
    int iter = MAX;
    for (int counter = 0; counter < MAX; ++counter) {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;
        if (r2 + i2 > 4.0) {
            //return counter;
            iter = counter;
            break;
        }
        auto zImag_old = zImag;
        auto zReal_old = zReal;
        zImag = 2.0 * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
        expiter += exp(-absoluteValue(zReal, zImag)-0.5/(absoluteValue(zReal_old - zReal, zImag_old - zImag)));
    }
    auto toValue = getVectorColor(iter);
    auto fromValue = getVectorColor(std::min(iter + 1, MAX));
    multScalar(toValue, expiter);
    multScalar(fromValue, 1 - expiter);
    auto x = addVector(toValue, fromValue);
    return sf::Color(int(x[0]),int(x[1]),int(x[2]));
}


void Mandelbrot::updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY, std::string mode) const
{
    double real = 0 * zoom - IMAGE_WIDTH / 2.0 * zoom + offsetX;
    double imagstart = minY * zoom - IMAGE_HEIGHT / 2.0 * zoom + offsetY;
    for (int x = 0; x < IMAGE_WIDTH; x++, real += zoom) {
        double imag = imagstart;
        for (int y = minY; y < maxY; y++, imag += zoom) {
	    if(mode == "normal") image.setPixel(x, y, colors[mandelbrot(real, imag)]);
	    else if (mode == "exp-res") image.setPixel(x, y, getSmoothColor(real, imag));
        }
    }
}

void Mandelbrot::updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, std::string mode) const
{
    const int STEP = IMAGE_HEIGHT/std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for (int i = 0; i < IMAGE_HEIGHT; i += STEP) {
        threads.push_back(std::thread(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, offsetY, std::ref(image), i, std::min(i+STEP, IMAGE_HEIGHT), mode));
    }
    for (auto &t : threads) {
        t.join();
    }
}

std::string generateFileName(){
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string str = std::ctime(&end_time);
    for(int i = 0 ; i < 6; ++i) str.pop_back();
    for(int i = 0; i < 11; ++i) str.erase(str.begin());
    str = "Mandelbrot_" + str + ".png";
    return str;
}

int main() {
    double offsetX = -0.7; // and move around
    double offsetY = 0.0;
    double zoom = 0.004; // allow the user to zoom in and out...
    double factor = 1.0;
    Mandelbrot mb;
    std::string mode;
    std::cin >> mode;
    sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Mandelbrot");
    window.setFramerateLimit(0);
    
    sf::Image image;
    image.create(IMAGE_WIDTH, IMAGE_HEIGHT, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    bool stateChanged = true; // track whether the image needs to be regenerated

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        case sf::Keyboard::Equal: //Zooms in
                            zoom *= ZOOM_FACTOR;
                            factor /= ZOOM_FACTOR;
                            break;
                        case sf::Keyboard::Dash: //Zooms out
                            zoom /= ZOOM_FACTOR;
                            factor *= ZOOM_FACTOR;
                            break;
                        case sf::Keyboard::D: //Figure moves down (Orig : W)
                            offsetY -= OFFSET_FACTOR * zoom;
                            break;
                        case sf::Keyboard::U: //Figure moves up (Orig : S)
                            offsetY += OFFSET_FACTOR * zoom;
                            break;
                        case sf::Keyboard::R: //Figure moves right (Orig : A)
                            offsetX -= OFFSET_FACTOR * zoom;
                            break;
                        case sf::Keyboard::L: //Figure moves right (Orig : D)
                            offsetX += OFFSET_FACTOR * zoom;
                            break;
                        case sf::Keyboard::S: //Saves current image
                            window.capture().saveToFile(generateFileName());
                            break;
                        default: 
                            stateChanged = false;
                            break;
                    }
                default:
                    break;
            }
        }

        if (stateChanged) { 
            mb.updateImage(zoom, offsetX, offsetY, image, mode);
            texture.loadFromImage(image);
            texture.setSmooth(true);
            const std::string strg = "Mandelbrot: " + std::__cxx11::to_string(factor) + "x"; 
            window.setTitle	(strg);	
            sprite.setTexture(texture);
            stateChanged = false;
        }
        window.draw(sprite);
        auto sfmg = sf::Mouse::getPosition(window);
        std::cout << "(" << sfmg.x << ", " << sfmg.y << ")";
        window.display();
    }
}

