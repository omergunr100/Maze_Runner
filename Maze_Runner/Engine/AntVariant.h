#pragma once
#include "../Definitions/Config/Config.h"
#include "../Definitions/Image/Color.h"
#include "../Definitions/Random/Randomness.h"
#include <iostream>
#include <vector>

struct Weights {
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
};

class AntVariant {
public:

	static int counter;
	static int unique;
	static bool Ended;

	enum SpecialId {
		Start = -1, End = -2, Empty = -3, Runner = -4, Wall = -5, PlaceHolder = -6, Trail = -10, ChosenTrail = -11, UsedTrail = -12
	};
	static int LastId;

private:
	int m_unique;
	SpecialId m_id;
	Color m_color;
	int m_steps;
	std::shared_ptr<AntVariant> m_toStart;

public:
	AntVariant();
	AntVariant(const SpecialId& id);
	AntVariant(const AntVariant& other);
	AntVariant(const SpecialId& id, const Color& color, const int& steps);
	~AntVariant();

	void SetId(const int& newID);
	int GetId();
	int GetUnique();
	Color GetColor();
	AntVariant SpreadRunner();
	AntVariant GetAsTrail();
	AntVariant GetAsChosen();
	static AntVariant GetRunner();
	static AntVariant GetEmpty();
	static AntVariant GetWall();
	std::vector<AntVariant> PlayTurn(const std::vector<AntVariant>& neighbors);
	std::vector<AntVariant> ChooseTrail(const std::vector<AntVariant>& neighbors);
};
