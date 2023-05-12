#pragma once
#include "nlohmann/json.hpp"
using namespace nlohmann;

class Memento
{
private:
	json state;
	
public:
	Memento(json originator_state);
	json get_state();
};

