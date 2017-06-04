#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#ifndef _function_HPP_INCLUDED__   
#define _function_HPP_INCLUDED__  


#define N 450

int squarConverterX(double xx)
{
	xx = (xx/N)*10;
	int xi = floor(xx);

	return xi;
}


int squarConverterY(double yy)
{
	yy = (yy/N)*10;
	int yi = floor(yy);

	return yi;
}


void winnerComm(std::string s, sf::RenderWindow &D)
{
	//D.clear();
	sf::Font font;
	sf::Text text;
	font.loadFromFile("Kingthings Petrock light.ttf");
	text.setFont(font);
	std::ostringstream ss;
	ss << s;
	text.setString(ss.str());
	text.setCharacterSize(42);
	text.setColor(sf::Color(15, 255, 255,185));
	text.setPosition(100,200);
	D.draw(text);
	ss.str("");
	D.display();

}

int confLoader(std::fstream &f)
{
	std::string data;
	int side;
	getline( f, data );
	std::stringstream ss;
	ss << data[0];
	ss >> side;
	f.seekg( 0, std::ios_base::beg);
	return side;
}


int menu()
{
	char s;
	std::cout <<" Press button N if u wanna play New Game"<< std::endl;
	std::cout <<" Press button L if u wanna load recent Game"<< std::endl;
	while(s != 'n' && s != 'l')
	std::cin >> s;
	if(s == 'n')
	return 0;
	else if(s == 'l')
	return 1;
	
}

#endif
