#include "plpch.h"
#include "ECS.h"

#include "Primitivo/Core/Core.h"

namespace Primitivo{

	int BaseComponent::nextId = 0;



	void System::AddEntity(Entity entity) {
		m_Entities.push_back(entity);
	}


	void System::RemoveEntity(Entity entity) {
		for (std::vector<Entity>::iterator it = m_Entities.begin(); it != m_Entities.end();) {
			if (entity.GetId() == it->GetId()) {
				m_Entities.erase(it);
				return;
			}

			++it;
		}
	}





	void Coordinator::Update() {
		for (auto entity : m_EntitiesToBeKilled) {
			m_EntityComponentSignatures[entity.GetId()].reset();

			UpdateQueryList(entity);

			for (std::shared_ptr<BasePool> pool : m_ComponentPools) {
				if (pool) {
					pool->TryToRemove(entity.GetId());
				}
			}

			m_FreeIds.push_back(entity.GetId());

			RemoveEntityFromGroup(entity);
		}
		m_EntitiesToBeKilled.clear();

		for (auto entity : m_EntitiesToBeModified) {
			UpdateQueryList(entity);
		}
		m_EntitiesToBeModified.clear();
	}

	Entity Coordinator::CreateEntity() {
		int entityId;

		if (m_FreeIds.empty()) {
			entityId = m_NumEntities++;

			if (entityId >= m_EntityComponentSignatures.size()) {
				m_EntityComponentSignatures.resize(entityId + 1);
			}
		}
		else {
			entityId = m_FreeIds.front();
			m_FreeIds.pop_front();
		}

		Entity entity(entityId);
		return entity;
	}

	void Coordinator::KillEntity(Entity entity) {
		m_EntitiesToBeKilled.insert(entity);
	}

	std::set<Entity> Coordinator::QueryEntitiesBySignature(Signature signature) {
		if (m_QueryEntities.find(signature) != m_QueryEntities.end()) {
			return m_QueryEntities.at(signature);
		}
		else {
			return {};
		}
	}

	Signature Coordinator::RegisterQueryRequirement() {
		Signature returnSignature = m_Signature;

		m_QueryEntities.insert({ returnSignature, {} });

		m_Signature.reset();
		return returnSignature;
	}

	void Coordinator::GroupEntity(Entity entity, const std::string& group) {
		m_EntitiesPerGroup.emplace(group, std::set<Entity>());
		m_EntitiesPerGroup[group].emplace(entity);

		m_GroupPerEntities.emplace(entity.GetId(), group);
	}

	bool Coordinator::EntityBelongsToGroup(Entity entity, const std::string& group) const {
		return m_GroupPerEntities.at(entity.GetId()).compare(group) == 0;					//Not handles if the users inserts a wrong group name that was not previously added because is O(N)
	}

	std::set<Entity> Coordinator::GetEntitiesByGroup(const std::string& group) const {
		return m_EntitiesPerGroup.at(group);												//Not handles if the users inserts a wrong group name that was not proviously added because is O(N)
	}

	void Coordinator::RemoveEntityFromGroup(Entity entity) {
		auto groupedEntity = m_GroupPerEntities.find(entity.GetId());
		
		if (groupedEntity != m_GroupPerEntities.end()) {
			auto group = m_EntitiesPerGroup.find(groupedEntity->second);
			if (group != m_EntitiesPerGroup.end()) {
				auto foundEntity = group->second.find(entity);
				if (foundEntity != group->second.end()) {
					group->second.erase(foundEntity);
				}
			}
			m_GroupPerEntities.erase(groupedEntity);
		}
	}



	void Coordinator::UpdateQueryList(Entity entity) {
		for (const auto& [query,entities] : m_QueryEntities) {
			Signature entitySignature = m_EntityComponentSignatures[entity.GetId()];
			
			bool compatibleQuery = (entitySignature & query) == query;
			auto it = entities.find(entity);
			if (!compatibleQuery && it != entities.end()) {
				m_QueryEntities.at(query).erase(it);
			}
			else if (compatibleQuery) {
				m_QueryEntities.at(query).insert(entity);
			}
		}
	}
}