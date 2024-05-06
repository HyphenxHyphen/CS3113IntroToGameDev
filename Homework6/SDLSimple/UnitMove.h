#pragma once
#include "Component.h"

enum UnitMoveRange { SHORT, MID, LONG, SELF, ALLY };

struct UnitMoveInfo {
	UnitMoveInfo(float moveId, std::string moveName, UnitMoveRange moveRange, std::string moveDesc) {
		m_moveId = moveId;
		m_moveName = moveName;
		m_moveRange = moveRange;
		m_moveDesc = moveDesc;
	}

	float m_moveId = 0;
	std::string m_moveName = "Name";
	UnitMoveRange m_moveRange = SHORT;
	std::string m_moveDesc = "Description";
};

class EntityController;

class UnitMove : public Component {
private:
	EntityController* m_ec = nullptr;
public:
	UnitMove(GameObject* parent, EntityController* ec) :Component(parent) { m_ec = ec; };
	static bool targetUnitMove(UnitMoveInfo move, EntityController* user, EntityController* target);
	static void useUnitMove(int moveId, EntityController* user, EntityController* target);
	static UnitMoveInfo getUnitMoveInfo(int moveId);
};

