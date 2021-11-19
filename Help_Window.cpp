#include <SFML/Graphics.hpp>

#include <fstream>

#include <string>


int main() {

    sf::RenderWindow window(sf::VideoMode(600, 575), "Help Window");
    
    sf::Font font;
    
    std::ifstream ifs("README.txt");
    
    std::string content;

    content.assign( (std::istreambuf_iterator<char>(ifs) ),
                  (std::istreambuf_iterator<char>()    ) );
    
    if (!font.loadFromFile("times new roman.ttf")) 

        return -1;
    
    sf::Text message(content, font);

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                window.close();
                break;
            }
        }

        window.clear();
        window.draw(message);
        window.display();
    }
    return 0;
}