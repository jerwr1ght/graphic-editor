#include "Memento.h"

Memento::Memento(json originator_state) {
	state = originator_state;
}

json Memento::get_state() {
	return state;
}