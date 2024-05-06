#include "RotateRightButton.h"

void RotateRightButton::press() {
	if (!m_turnOrderDisplay->getTurnOrder()->at(0)->isAnt()) { return; }
	if ((*m_usingMove) != nullptr) {
		(*m_usingMove)->getBG()->setTexture(0);
		(*m_usingMove) = nullptr;
	}
	HexController::rotateHexes(m_allyHexData, true, 1);
	m_turnOrderDisplay->endTurn();
}