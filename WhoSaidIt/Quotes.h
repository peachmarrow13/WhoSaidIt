#pragma once

#include <string>
#include <vector>
#include <random>

class Quotes {
	std::string Character;
	std::string Quote;
	std::vector<std::string> QuotesList;
	void LoadQuotes();
};