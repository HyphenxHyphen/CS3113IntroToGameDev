#include "Stack.h"
#include "EntityController.h"

Stack::Stack(EntityController* host, EntityController* source) { 
	m_host = host; 
	m_source = source; 
}

Stack::~Stack() { 
	m_host->deleteStack(this); 
}

void Stack::applyStack(int count) {
	std::cout << "Applying " << count << " of " << m_name << std::endl;
	m_count += count;
	onApply(count);
}

void Stack::removeStack(int count) {
	std::cout << "Removing " << count << " of " << m_name << std::endl;
	int maxRemove = count;
	if (m_count < count) {
		maxRemove = m_count;
	}
	m_count -= maxRemove;
	onRemove(maxRemove);
}

void StackRedHarvest::onApply(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("accDelta", m_host->getStat("accDelta") + 0.3f * m_host->getStat("accScore") * m_host->getStat("accMul"));
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") + 0.3f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackRedHarvest::onRemove(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("accDelta", m_host->getStat("accDelta") - 0.3f * m_host->getStat("accScore") * m_host->getStat("accMul"));
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") - 0.3f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackRedHarvest::onTurnEnd() {
	removeStack(1);
}

void StackRainbowHarvest::onApply(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("accDelta", m_host->getStat("accDelta") + 0.1f * m_host->getStat("accScore") * m_host->getStat("accMul"));
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") + 0.1f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackRainbowHarvest::onRemove(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("accDelta", m_host->getStat("accDelta") - 0.1f * m_host->getStat("accScore") * m_host->getStat("accMul"));
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") - 0.1f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackRainbowHarvest::onTurnEnd() {
	removeStack(1);
}

void StackNapalmSap::onTurnEnd() {
	if (m_host->getStackCount("BURNING BODY") > 0) {
		m_host->healDamage(5.0f * m_count);
	}
	else {
		m_host->takeDamage(5.0f * m_count);
	}
}

void StackStunned::onTurnStart() {
	m_host->setAnimState(TURNEND);
}

void StackStunned::onTurnEnd() {
	removeStack(1);
}

void StackBurningBody::onTurnEnd() {
	removeStack(1);
}

void StackArmor::onApply(int count) {
	if (Stack* doff = m_host->getStack<StackDoff>()) {
		if (doff->m_count > 0) {
			for (int i = 0; i < count; i++) {
				m_host->setStat("dmgDelta", m_host->getStat("dmgDelta") + 0.01f * doff->m_count);
			}
			m_host->updateStats();
		}
	}
}

void StackArmor::onRemove(int count) {
	if (Stack * doff = m_host->getStack<StackDoff>()) {
		if (doff->m_count > 0) {
			for (int i = 0; i < count; i++) {
				m_host->setStat("dmgDelta", m_host->getStat("dmgDelta") - 0.01f * doff->m_count);
			}
			m_host->updateStats();
		}
	}
}

void StackPlantDefense::onApply(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("defDelta", m_host->getStat("defDelta") + 0.2f * m_host->getStat("defScore") * m_host->getStat("defMul"));
	}
	m_host->updateStats();
}

void StackPlantDefense::onRemove(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("defDelta", m_host->getStat("defDelta") - 0.2f * m_host->getStat("defScore") * m_host->getStat("defMul"));
	}
	m_host->updateStats();
}

void StackPlantDefense::onTurnStart() {
	removeStack(1);
}

void StackDoff::onApply(int count) {
	if (Stack* block = m_host->getStack<StackArmor>()) {
		if (block->m_count > 0) {
			for (int i = 0; i < block->m_count; i++) {
				m_host->setStat("dmgDelta", m_host->getStat("dmgDelta") + 0.01f * count);
			}
			m_host->updateStats();
		}
	}
}

void StackDoff::onRemove(int count) {
	if (Stack * block = m_host->getStack<StackArmor>()) {
		if (block->m_count > 0) {
			for (int i = 0; i < block->m_count; i++) {
				m_host->setStat("dmgDelta", m_host->getStat("dmgDelta") - 0.01f * count);
			}
			m_host->updateStats();
		}
	}
}

void StackGrowDefense::onApply(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("defDelta", m_host->getStat("defDelta") + 0.3f * m_host->getStat("defScore") * m_host->getStat("defMul"));
	}
	m_host->updateStats();
}

void StackGrowDefense::onRemove(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("defDelta", m_host->getStat("defDelta") - 0.3f * m_host->getStat("defScore") * m_host->getStat("defMul"));
	}
	m_host->updateStats();
}

void StackGrowDefense::onTurnStart() {
	removeStack(1);
}

void StackPoisoned::onTurnStart() {
	m_host->takeDamage(m_host->getStat("hpMax") * 0.1f * m_count);
}

void StackWriggle::onApply(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") + 0.1f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackWriggle::onRemove(int count) {
	for (int i = 0; i < count; i++) {
		m_host->setStat("spdDelta", m_host->getStat("spdDelta") - 0.1f * m_host->getStat("spdScore") * m_host->getStat("spdMul"));
	}
	m_host->updateStats();
}

void StackWriggle::onTurnEnd() {
	removeStack(1);
}