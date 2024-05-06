#include "HexController.h"

void HexController::rotateHexes(HexData* hexData, bool clockwise, int amount) {
	for (unsigned int _ = 0; _ < amount; _++) {
		if (clockwise) {
			EntityController* tempOne = hexData->m_hexes.at(5)->getOccupant();
			for (int i = 5; i > 0; i--) {
				EntityController* temp = hexData->m_hexes.at(i - 1)->getOccupant();
				if (temp != nullptr) {
					temp->setHex(i);
					temp->setPosition({ hexData->m_hexes.at(i)->getPosition().x, hexData->m_hexes.at(i)->getPosition().y + 0.7f, 1.0f });
					//std::cout << "Hex " << i << " is now holding " << (hexData->m_hexes.at(i)->getOccupant()->getName()) << " and is alive " << (hexData->m_hexes.at(i)->getOccupant()->getParent()->isAlive()) << std::endl;
				}
				hexData->m_hexes.at(i)->setOccupant(temp);
			}
			if (tempOne != nullptr) {
				tempOne->setHex(0);
				tempOne->setPosition({ hexData->m_hexes.at(0)->getPosition().x, hexData->m_hexes.at(0)->getPosition().y + 0.7f, 1.0f });
			}
			hexData->m_hexes.at(0)->setOccupant(tempOne);

			hexData->m_rotationDelta += 1;
			if (hexData->m_rotationDelta == 6) {
				hexData->m_rotationDelta = 0;
				hexData->m_rotations += 1;
			}
		}
		else {
			EntityController* tempOne = hexData->m_hexes.at(0)->getOccupant();
			for (int i = 0; i < 5; i++) {
				EntityController* temp = hexData->m_hexes.at(i + 1)->getOccupant();
				if (temp != nullptr) {
					temp->setHex(i);
					temp->setPosition({ hexData->m_hexes.at(i)->getPosition().x, hexData->m_hexes.at(i)->getPosition().y + 0.7f, 1.0f });
				}
				hexData->m_hexes.at(i)->setOccupant(temp);
			}
			if (tempOne != nullptr) {
				tempOne->setHex(5);
				tempOne->setPosition({ hexData->m_hexes.at(5)->getPosition().x, hexData->m_hexes.at(5)->getPosition().y + 0.7f, 1.0f });
			}
			hexData->m_hexes.at(5)->setOccupant(tempOne);

			hexData->m_rotationDelta -= 1;
			if (hexData->m_rotationDelta == -6) {
				hexData->m_rotationDelta = 0;
				hexData->m_rotations += 1;
			}
		}
	}
}