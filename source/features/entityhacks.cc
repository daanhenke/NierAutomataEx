#include "mod.hh"
#include "misc/console.hh"

#include <vector>
#include <algorithm>

#include "imgui.h"

std::vector<Entity*> handledEntities;
std::vector<Entity*> boostedEnemies;

void OnNewEntity(Entity* entity)
{
	//ConsoleWriteColor(FOREGROUND_RED, "New entity: %llx\n", entity);

	auto object_id = *entity->GetObjectIdPtr();
	if (object_id >= 0x20000 && object_id <= 0x30000)
	{
		if (rand() % 5 == 0 || true)
		{
			ConsoleWrite("Boosting an enemy %x @ %llx!\n", object_id, entity);
			boostedEnemies.push_back(entity);
		}
	}
}

void OnTickEntity(Entity* entity)
{
	auto object_id = *entity->GetObjectIdPtr();

	// Check if entity is an enemy
	if (object_id >= 0x20000 && object_id <= 0x30000)
	{
		auto player = GetPlayerEntity();

		// Enemy scaling
		if (player != nullptr && gConfig.EnableLevelScaling)
		{
			// Store current level & target level
			auto playerLvl = player->GetLevel();
			auto entLvl = entity->GetLevel();

			if (entLvl < playerLvl)
			{
				entity->SetLevel(playerLvl);
			}
		}

		if (std::count(boostedEnemies.begin(), boostedEnemies.end(), entity))
		{
			auto modelInfo = *entity->GetModelInfoPtr();
			if (modelInfo != nullptr && entity->IsAliveAndHasEntityInfo())
			{
				modelInfo->tint[0] = 64.f;
			}
		}
	}
}

void OnRemoveEntity(Entity* entity)
{
	//ConsoleWriteColor(FOREGROUND_RED, "Removed entity: %llx\n", entity);

	// If it has been boosted remove it from the list
	if (std::count(boostedEnemies.begin(), boostedEnemies.end(), entity))
	{
		ConsoleWriteColor(FOREGROUND_RED, "Unboosting entity: %llx\n", entity);
		boostedEnemies.erase(std::remove(boostedEnemies.begin(), boostedEnemies.end(), entity));
	}
}

void ApplyEntityMods()
{
	// Force player character in most? situations
	static int* pCurrentPlayerId = GetGameObject<int>("NextLoadPlayerIndex");

	if (gConfig.EnablePlayerForcing)
	{
		*pCurrentPlayerId = gConfig.PlayerForcingValue;
	}

	// Get a list of all entities in the EntityInfoList
	std::vector<Entity*> currentEntities;
	for (int i = 0; i < gEntityList->item_count; i++)
	{
		auto entry = gEntityList->items[i];
		auto info = entry.info;

		if (info == nullptr) continue;
		auto entity = info->entity;
		if (entity == nullptr) continue;

		currentEntities.push_back(entity);
	}

	// Figure out which entities are new and which have been removed
	std::vector<Entity*> removedEntities;
	for (auto handledEnt : handledEntities)
	{
		if (std::count(currentEntities.begin(), currentEntities.end(), handledEnt))
		{
			// Entity is already known about
			currentEntities.erase(std::remove(currentEntities.begin(), currentEntities.end(), handledEnt));
			OnTickEntity(handledEnt);
		}
		else
		{
			// Entity has been removed
			removedEntities.push_back(handledEnt);
		}
	}

	// We removed all known entities from the list, so all remaining ones are new
	for (auto currEnt : currentEntities)
	{
		handledEntities.push_back(currEnt);
		OnNewEntity(currEnt);
		OnTickEntity(currEnt);
	}

	// We saved the removed entities to a seperate list, handle them now
	for (auto remEnt : removedEntities)
	{
		handledEntities.erase(std::remove(handledEntities.begin(), handledEntities.end(), remEnt));
		OnRemoveEntity(remEnt);
	}
}