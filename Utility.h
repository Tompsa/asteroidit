#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Window/Keyboard.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif // UTILITY_H
