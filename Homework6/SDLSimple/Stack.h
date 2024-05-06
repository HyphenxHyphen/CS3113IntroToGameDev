#pragma once
#include <string>
#include <vector>
#include <map>

class EntityController;

class Stack {
public:
	std::string m_name = "";
	int m_count = 0;
	bool m_buff = false;
	EntityController* m_host;
	EntityController* m_source;

	Stack(EntityController* host, EntityController* source);
	virtual ~Stack();

	void virtual onApply(int count) {};
	void virtual onRemove(int count) {};
	void virtual onTurnStart() {};
	void virtual onTurnEnd() {};
	//void virtual onTakeDamage() {};

	void applyStack(int count);
	void removeStack(int count);
};

class StackRedSpore : public Stack {
public:
	StackRedSpore(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "RED SPORE"; m_buff = true; applyStack(count); };
};

class StackRedHarvest : public Stack {
public:
	StackRedHarvest(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "RED HARVEST"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
	void onTurnEnd() override;
};

class StackGreenSpore : public Stack {
public:
	StackGreenSpore(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "GREEN SPORE"; m_buff = true; applyStack(count); };
};

class StackBlueSpore : public Stack {
public:
	StackBlueSpore(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "BLUE SPORE"; m_buff = true; applyStack(count); };
};

class StackRainbowHarvest : public Stack {
public:
	StackRainbowHarvest(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "RAINBOW HARVEST"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
	void onTurnEnd() override;
};

class StackNapalmSap : public Stack {
public:
	StackNapalmSap(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "NAPALM SAP"; m_buff = false; applyStack(count); };

	void onTurnEnd() override;
};

class StackStunned : public Stack {
public:
	StackStunned(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "STUNNED"; m_buff = false; applyStack(count); };

	void onTurnStart() override;
	void onTurnEnd() override;
};

class StackBurningBody : public Stack {
public:
	StackBurningBody(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "BURNING BODY"; m_buff = true; applyStack(count); };

	void onTurnEnd() override;
};

class StackArmor : public Stack {
public:
	StackArmor(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "ARMOR"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
};

class StackPlantDefense : public Stack {
public:
	StackPlantDefense(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "PLANT DEFENSE"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
	void onTurnStart() override;
};

class StackDoff : public Stack {
public:
	StackDoff(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "DOFF"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
};

class StackGrowth : public Stack {
public:
	StackGrowth(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "GROWTH"; m_buff = true; applyStack(count); };
};

class StackGrowDefense : public Stack {
public:
	StackGrowDefense(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "GROW DEFENSE"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
	void onTurnStart() override;
};

class StackPoisoned : public Stack {
public:
	StackPoisoned(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "POISONED"; m_buff = false; applyStack(count); };
	void onTurnStart() override;
};

class StackWriggle : public Stack {
public:
	StackWriggle(EntityController* host, EntityController* source, int count) :Stack(host, source) { m_name = "WRIGGLE"; m_buff = true; applyStack(count); };

	void onApply(int count) override;
	void onRemove(int count) override;
	void onTurnEnd() override;
};