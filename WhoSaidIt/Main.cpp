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
std::string TextEasterEgg;

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
	TextEasterEgg = IsEnglish ? "All's good in the goblin hood!" : "ゴブリンの里は今日も平和だ！";
}


static inline std::string ToLower(const std::string& str);
static inline bool IsJapanese(const std::string& str);
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
	std::string choice;
	std::cin >> choice;
	if (choice == "1") {
		IsEnglish = true;
		std::cout << "You chose English.\n";
	}
	else if (choice == "2") {
		IsEnglish = false;
		std::cout << "あなたは日本語を選択しました";
	} 
	else {
		std::cout << "Invalid choice. Defaulting to English.\n";
		IsEnglish = true;
	}

	LoadTexts(); // Load the localized texts based on the chosen language
	srand((unsigned)time(0)); // Seed the random number generator
	std::cout << TextWelcome << std::endl;
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
		std::vector<std::string> Aliases;
		std::vector<std::string> OtherAliases;
		if (IsEnglish) {
			Quote = Quotes[RandomIndex]["quoteEng"];
			Hint = Quotes[RandomIndex]["hintEng"];
			Character = Quotes[RandomIndex]["characterEng"];
			OtherQuote = Quotes[RandomIndex]["quoteJap"];
			OtherHint = Quotes[RandomIndex]["hintJap"];
			OtherCharacter = Quotes[RandomIndex]["characterJap"];
			Aliases = Quotes[RandomIndex]["aliasesEng"].get<std::vector<std::string>>();
			OtherAliases = Quotes[RandomIndex]["aliasesJap"].get<std::vector<std::string>>();

		}
		else {
			Quote = Quotes[RandomIndex]["quoteJap"];
			Hint = Quotes[RandomIndex]["hintJap"];
			Character = Quotes[RandomIndex]["characterJap"];
			OtherQuote = Quotes[RandomIndex]["quoteEng"];
			OtherHint = Quotes[RandomIndex]["hintEng"];
			OtherCharacter = Quotes[RandomIndex]["characterEng"];
			Aliases = Quotes[RandomIndex]["aliasesJap"].get<std::vector<std::string>>();
			OtherAliases = Quotes[RandomIndex]["aliasesEng"].get<std::vector<std::string>>();
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
			else if (ToLower(Answer) == ToLower(Character) || ToLower(Answer) == ToLower(OtherCharacter)) {
				if (ToLower(Hint) == "that time i got reincarnated as a slime" || Hint == "転生したらスライムだった件")
					std::cout << TextEasterEgg << std::endl; // Easter egg for Tensura
				std::cout << TextCorrect << " " << Character << " " << TextSaid << " " << Quote << "\n";
			}
			else{
				for (const auto& alias : Aliases) {
					if (ToLower(Answer) == ToLower(alias)) {
						std::cout << TextCorrect << " " << Character << " " << TextSaid << " " << Quote << "\n";
						break;
					}
					else {
						for (const auto& otherAlias : OtherAliases) {
							if (ToLower(Answer) == ToLower(otherAlias)) {
								std::cout << TextCorrect << " " << Character << " " << TextSaid << " " << Quote << "\n";
								break;
							}
							else
								std::cout << TextIncorrect << Character << " " << TextSaid << " " << Quote << "\n";
						}
					}
				}
			}

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
	if (!IsJapanese(str)) { // Convert to lowercase only if the language is English
		for (auto& c : lowerStr) {
			c = static_cast<char>(std::tolower(c));
		}
		return lowerStr;
	}
	return lowerStr; // No conversion for Japanese characters
}

static inline bool IsJapanese(const std::string& str) {
	// Check if the string contains any Japanese characters
	for (const auto& c : str) {
		if ((c >= 0x3040 && c <= 0x309F) || // Hiragana
			(c >= 0x30A0 && c <= 0x30FF) || // Katakana
			(c >= 0x4E00 && c <= 0x9FAF)) { // Kanji
			return true;
		}
	}
	return false;
}