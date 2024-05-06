#include "NodeController.h"

void NodeController::press() {
	if (std::find(m_activeNodes->begin(), m_activeNodes->end(), m_parent) == m_activeNodes->end()) { return; }
	*m_chosenNode = m_nodeType;
	m_activeNodes->clear();
	*m_activeNodes = m_nextNodes;
}

void const NodeController::addNextNode(GameObject* nextNode) {
	m_nextNodes.push_back(nextNode);

	// Sort for easy processing
	for (unsigned int i = 1; i < m_nextNodes.size(); i++) {
		int j = i;
		while (j > 0 && (m_nextNodes[j]->getPosition().x < m_nextNodes[j - 1]->getPosition().x)) {
			std::swap(m_nextNodes[j], m_nextNodes[j - 1]);
			j--;
		}
	}
};

void NodeController::update(float deltatime) {
	if (std::find(m_activeNodes->begin(), m_activeNodes->end(), m_parent) == m_activeNodes->end()) { return; }
	m_seconds += deltatime * 100.0f;
	m_parent->setScaleRelative({ 0.05f * sin(glm::radians(m_seconds)) + 0.75f,  0.05f * sin(glm::radians(m_seconds)) + 0.75f, 1.0f });
}