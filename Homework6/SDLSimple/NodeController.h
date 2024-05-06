#pragma once
#include "Button.h"

enum NodeType { EVENT, TRADER, TREASURE, TUTOR, BATTLE, ELITE, BOSS, SCRAP, NODENONE };

class NodeController : public Button {
private:
	std::vector<GameObject*> m_nextNodes;
	NodeType m_nodeType = NODENONE;
	NodeType* m_chosenNode = nullptr;
	std::vector<GameObject*>* m_activeNodes = {};
	float m_seconds = 0.0f;

public:
	NodeController(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera, NodeType* chosenNode, std::vector<GameObject*>* activeNodes) :Button(parent, allButtons, image, camera) { m_chosenNode = chosenNode; m_activeNodes = activeNodes; };
	void update(float deltaTime) override;
	void press() override;

	// ————— GETTERS ————— //
	std::vector<GameObject*> const getNextNodes() const { return m_nextNodes; };
	bool const hasNextNode(GameObject* nextNode) const { return std::find(m_nextNodes.begin(), m_nextNodes.end(), nextNode) != m_nextNodes.end(); };
	NodeType const getNodeType() const { return m_nodeType; };

	// ————— SETTERS ————— //
	void const addNextNode(GameObject* nextNode);
	void const setNodeType(NodeType nodeType) { m_nodeType = nodeType; };
};

