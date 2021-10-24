#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

int main() {
    sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Help Window");
    sf::Font font;
    //You need to pass the font file location
    if (!font.loadFromFile("times new roman.ttf")) {
        return -1;
    }
    sf::Text message("Hello, World !", font);

    while (sfmlWin.isOpen()) {

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        sfmlWin.clear();
        sfmlWin.draw(message);
        sfmlWin.display();
    }
    return 0;
}
