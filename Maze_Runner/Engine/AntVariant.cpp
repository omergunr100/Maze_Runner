#include "AntVariant.h"

int AntVariant::counter = 0;
bool AntVariant::Ended = false;
int AntVariant::unique = 1;

AntVariant::AntVariant()
	:m_id(Empty),
	m_color{0,0,0},
	m_steps(0),
	m_unique(unique++)
{
}

AntVariant::AntVariant(const SpecialId & id)
	:m_id(id),
	m_steps(1),
	m_unique(unique++)
{
	if (m_id == Runner) {
		counter++;
		m_color = { 34, 139, 34 };
	}
	else if (m_id == Wall) {
		counter++;
		m_color = { 128, 128, 128 };
	}
	else if (m_id == Start) {
		counter++;
		m_color = {255, 1, 1};
	}
	else if (m_id == End) {
		counter++;
		m_color = { 1, 255, 1 };
	}
	else if (m_id == ChosenTrail) {
		m_color = { 1, 128, 255 };
	}
	else if (m_id == Trail) {
		counter++;
		m_color = { 231, 230, 230};
	}
	else {
		m_color = { 0,0,0 };
	}
	m_toStart = nullptr;
}

AntVariant::AntVariant(const AntVariant & other)
{
	m_id = other.m_id;
	m_color = other.m_color;
	m_steps = other.m_steps;
	m_unique = other.m_unique;
	m_toStart = other.m_toStart;
}

AntVariant::AntVariant(const SpecialId & id, const Color & color, const int & steps)
	:m_id(id),
	m_color(color),
	m_steps(steps),
	m_unique(unique++)
{
}

AntVariant::~AntVariant()
{
}

void AntVariant::SetId(const int& newID)
{
	m_id = SpecialId(newID);
}

int AntVariant::GetId()
{
	return m_id;
}

int AntVariant::GetUnique()
{
	return m_unique;
}

Color AntVariant::GetColor()
{
	return m_color;
}

AntVariant AntVariant::SpreadRunner()
{
	AntVariant ret = AntVariant(*this);
	ret.m_toStart = std::shared_ptr<AntVariant>(this);
	ret.m_unique = ++unique;
	ret.m_steps++;
	counter++;
	return ret;
}

AntVariant AntVariant::GetAsTrail()
{
	AntVariant trail = AntVariant(*this);
	trail.m_toStart = m_toStart;
	trail.m_id = Trail;
	trail.m_color = { 231, 230, 230 };
	return trail;
}

AntVariant AntVariant::GetAsChosen()
{
	AntVariant chosen = AntVariant(*this);
	chosen.m_toStart = m_toStart;
	chosen.m_id = ChosenTrail;
	chosen.m_color = { 1, 128, 255 };
	return chosen;
}

AntVariant AntVariant::GetRunner()
{
	return AntVariant(AntVariant::SpecialId::Runner);
}

AntVariant AntVariant::GetEmpty()
{
	return AntVariant(AntVariant::SpecialId::Empty);
}

AntVariant AntVariant::GetWall()
{
	return AntVariant(AntVariant::SpecialId::Wall);
}

std::vector<AntVariant> AntVariant::PlayTurn(const std::vector<AntVariant>& neighbors)
{
	// Just in case I will forget to handle it from the calling side
	std::vector<AntVariant> resultVect;
	resultVect.resize(9);

	// Get odds for each move
	for (int i = 0; i < resultVect.size(); i++) {
		// For the betterment of the one true path
		if (neighbors[i].m_id == Empty) {
			resultVect[i] = SpreadRunner();
		}
		else if (neighbors[i].m_unique == m_unique) {
			resultVect[i] = GetAsTrail();
		}
		else if (neighbors[i].m_id == Runner || neighbors[i].m_id == Trail) {
			if (neighbors[i].m_steps > m_steps + 1)
				resultVect[i] = SpreadRunner();
			else
				resultVect[i] = neighbors[i];
		}
		else if (neighbors[i].m_id == Wall || neighbors[i].m_id == PlaceHolder || neighbors[i].m_id == Start) {
			resultVect[i] = neighbors[i];
		}
		else if (neighbors[i].m_id == End) {
			resultVect[i] = neighbors[i];
		}
		else {
			resultVect[i] = neighbors[i];
		}
	}
	

	return resultVect;
}

std::vector<AntVariant> AntVariant::ChooseTrail(const std::vector<AntVariant>& neighbors)
{
	std::vector<AntVariant> resultVect = neighbors;
	bool start = false;
	int leastSteps = INT_MAX;
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i].m_id == Start) {
			start = true;
			break;
		}
		else if (neighbors[i].m_unique == m_unique || neighbors[i].m_id == ChosenTrail || neighbors[i].m_id == End || neighbors[i].m_id == UsedTrail) {
			continue;
		}
		else if(leastSteps > neighbors[i].m_steps){
			leastSteps = neighbors[i].m_steps;
		}
	}
	
	if (start)
		return neighbors;
	
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i].m_toStart == nullptr) {
			for (int i = 0; i < neighbors.size(); i++) {
				if (neighbors[i].m_unique == m_unique || neighbors[i].m_id == ChosenTrail || neighbors[i].m_id == UsedTrail) {
					continue;
				}
				else if (neighbors[i].m_steps == leastSteps) {
					resultVect[i] = resultVect[i].GetAsChosen();
					break;
				}
			}

			return resultVect;
		}
	}

	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i].m_unique == m_toStart->m_unique)
			resultVect[i] = resultVect[i].GetAsChosen();
	}

	return resultVect;
}
