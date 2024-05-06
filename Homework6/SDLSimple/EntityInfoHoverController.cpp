#include "EntityInfoHoverController.h"

void EntityStatsHoverController::update(float deltaTime) {
	if (isHovered()) {
		m_statsDisplay->setEntityController(m_ec);
		m_stacksDisplay->setEntityController(m_ec);

		if (*m_usingMove == nullptr) {
			for (auto moveDisplay : m_moveDisplays) {
				moveDisplay->setEntityController(m_ec);
			}
		}
	}
}

void EntityStatsHoverController::press() {
	//EntityMoveDisplayController* moveDisplay = (*m_usingMove);
	if ((*m_usingMove) != nullptr) {
		if (UnitMove::targetUnitMove((*m_usingMove)->getMove(), (*m_usingMove)->getEntityController(), m_ec)) {
			UnitMove::useUnitMove((*m_usingMove)->getMove().m_moveId, (*m_usingMove)->getEntityController(), m_ec);
		}
		(*m_usingMove)->getBG()->setTexture(0);
		(*m_usingMove) = nullptr;
	}
}