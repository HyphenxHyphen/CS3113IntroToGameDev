#include "RotateLeftButton.h"

void RotateLeftButton::press() {
	if (!m_turnOrderDisplay->getTurnOrder()->at(0)->isAnt()) { return; }
	if ((*m_usingMove) != nullptr) {
		(*m_usingMove)->getBG()->setTexture(0);
		(*m_usingMove) = nullptr;
	}
	HexController::rotateHexes(m_allyHexData, false, 1);
	m_turnOrderDisplay->endTurn();
}