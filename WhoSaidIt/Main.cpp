#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#define NOMINMAX
#include <windows.h>
#include <limits>
#include <cctype>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

bool IsEnglish;
//! Localization
std::string TextCorrect;
std::string TextIncorrect;
std::string TextGuess;
std::string TextHint;
std::string TextCharacters;
std::string TextPlayAgain;
std::string TextGoodbye;
std::string TextWelcome;
std::string TextHint2;
std::string TextThankYou;
std::string TextInputAnswer;
std::string TextSaid;

void LoadTexts() {
	TextCorrect = IsEnglish ? "Correct!" : "正解！";
	TextIncorrect = IsEnglish ? "Incorrect. The correct answer is: " : "不正解。正しい答えは：";
	TextGuess = IsEnglish ? "Guess who said this: " : "このセリフを言ったのは誰？";
	TextHint = IsEnglish ? "Type 'hint' for a hint." : "ヒントが欲しい場合は「ヒント」と入力してください。";
	TextCharacters = IsEnglish ? "Here are the characters:" : "キャラクター一覧：";
	TextPlayAgain = IsEnglish ? "Would you like to play again?" : "もう一度プレイしますか？";
	TextGoodbye = IsEnglish ? "Goodbye!" : "さようなら！";
	TextWelcome = IsEnglish ? "Welcome to Who Said It!" : "「誰が言った？」へようこそ！";
	TextHint2 = IsEnglish ? "Hint: The character's show is " : "ヒント：キャラクターの番組は ";
	TextThankYou = IsEnglish ? "Thank you for playing!" : "プレイしてくれてありがとう！";
	TextInputAnswer = IsEnglish ? "Your answer: " : "あなたの答え：";
	TextSaid = IsEnglish ? " said: " : "が言ったのは：";
}


static inline std::string ToLower(const std::string& str);
void GameLoop(json Quotes);

int main() {
	// Set console to UTF-8 to handle Japanese characters
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8); // for reading input if needed
	std::ifstream inFile("Quotes.json");
	json Quotes;
	inFile >> Quotes;
	inFile.close();


	std::cout << "Choose a language (1 for English, 2 for 日本語): " << std::endl << "言語を選択してください (英語の場合は 1、日本語の場合は 2):";
	int choice;
	std::cin >> choice;
	if (choice == 1) {
		IsEnglish = true;
		std::cout << "You chose English.\n";
	} else if (choice == 2) {
		IsEnglish = false;
		std::cout << "あなたは日本語を選択しました";
	} else {
		std::cout << "Invalid choice. Defaulting to English.\n";
		IsEnglish = true;
	}

	srand((unsigned)time(0)); // Seed the random number generator
	std::cout << TextWelcome << std::endl;
	LoadTexts(); // Load the localized texts based on the chosen language
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer to avoid issues with getline after cin
	GameLoop(Quotes);
	std::cout << TextThankYou << std::endl;
}

void GameLoop(json Quotes) {
	while (true) {
		int RandomIndex = rand() % Quotes.size();
		std::string Quote;
		std::string Character;
		std::string Hint;
		std::string OtherQuote;
		std::string OtherCharacter;
		std::string OtherHint;
		if (IsEnglish) {
			Quote = Quotes[RandomIndex]["quoteEng"];
			Hint = Quotes[RandomIndex]["hintEng"];
			Character = Quotes[RandomIndex]["characterEng"];
			OtherQuote = Quotes[RandomIndex]["quoteJap"];
			OtherHint = Quotes[RandomIndex]["hintJap"];
			OtherCharacter = Quotes[RandomIndex]["characterJap"];
		}



		std::cout << TextCharacters << std::endl;
		for (const auto& c : Quotes) {
			if (IsEnglish) {
				std::cout << c["characterEng"] << std::endl;
			}
			else {
				std::cout << c["characterJap"] << std::endl;
			}
		}

		std::cout << TextGuess << Quote << "\n";
		std::cout << TextHint << std::endl;
		std::string Answer;
		while (true) {

			std::cout << TextInputAnswer << " ";
			std::getline(std::cin, Answer);
			if (ToLower(Answer) == "hint" || Answer == "ヒント") {
				std::cout << TextHint2 << " " << Hint << std::endl;
				continue; // Ask for the answer again
			}
			else if (ToLower(Answer) == ToLower(Character) || ToLower(Answer) == ToLower(OtherCharacter))
				std::cout << TextCorrect << " " << Character << " " << TextSaid << " " << Quote << "\n";
			else
				std::cout << TextIncorrect << " " << Character << "\n";

			std::cout << TextThankYou << std::endl << TextPlayAgain << std::endl;
			std::string PlayAgain;
			std::getline(std::cin, PlayAgain);
			if (ToLower(PlayAgain) == "yes" || ToLower(PlayAgain) == "y" || ToLower(PlayAgain) == "1" || PlayAgain == "はい" || PlayAgain == "ハイ") {
				break; // Restart the game loop
			}
			else {
				std::cout << TextGoodbye << std::endl;
				return;
			}
		}
		continue; // Restart the game loop
	}
}

static inline std::string ToLower(const std::string& str) {

	std::string lowerStr = str;
	if (IsEnglish) { // Convert to lowercase only if the language is English
		for (auto& c : lowerStr) {
			c = static_cast<char>(std::tolower(c));
		}
		return lowerStr;
	}
	return lowerStr; // No conversion for Japanese characters
}
