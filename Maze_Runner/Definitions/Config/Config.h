#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

struct Config {
private:
	static std::map<std::string, std::string> config;
public:
	Config() = delete;
	Config(const Config& other) = delete;
	~Config() = delete;

	static int Parse();

	static const char* GetAsChar(const char* id);
	static std::string GetAsString(const char* id);
	static int GetAsInteger(const char* id);
	static float GetAsFloat(const char* id);
	static bool GetAsBool(const char* id);
	/*
	struct General {
		static const char* configPath;
		static std::ofstream log;
		static const char* imageFolder;

		General() = delete;
		General(const General& other) = delete;
		~General() = delete;
	};
	
	struct AntWeights{
		// Base stats increases tied to strongDirection
		static const float spreadBias;
		static const float aggressivenessBias;
		static const float strengthBias;

		// Colors
		static const bool mix; // Whether to mix the colors of nearby cells each turn
		static const int ownWeightMult; // Weight multiplier of the cell's own color in the mixing

		// Stat decrease/increase based on actions
		static const float spreadDecOnSuccess;
		static const float spreadIncOnFail;

		AntWeights() = delete;
		AntWeights(const AntWeights& other) = delete;
		~AntWeights() = delete;
	};
	
	struct GameParameters {
		static const int width;
		static const int height;
		static const int players;
		static const int turns;
		static const int scaleWidth;
		static const int scaleHeight;

		GameParameters() = delete;
		GameParameters(const GameParameters& other) = delete;
		~GameParameters() = delete;
	};
	*/
};