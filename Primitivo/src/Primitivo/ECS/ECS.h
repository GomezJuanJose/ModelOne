#pragma once


#include <bitset>
#include <typeindex>
#include <set>
#include <deque>


//TODO Use an array more than a vector? or a more efficient data structure, slotmap?



namespace Primitivo {

	const unsigned int MAX_COMPONENTS = 32;

	typedef std::bitset<MAX_COMPONENTS> Signature;



	class Entity {
	public:
		Entity(int id) : m_Id(id) {}
		inline int GetId() const { return m_Id; }

		Entity& operator =(const Entity& other) = default;
		bool operator ==(const Entity& other) const { return m_Id == other.m_Id; }
		bool operator !=(const Entity& other) const { return m_Id != other.m_Id; }
		bool operator >(const Entity& other) const { return m_Id > other.m_Id; }
		bool operator <(const Entity& other) const { return m_Id < other.m_Id; }

	private:
		int m_Id; //TODO Search a better type of id (unsigned id, size_t)
	};





	struct BaseComponent {
	protected:
		static int nextId;//TODO Search a better type of id (unsigned id, size_t)
	};

	template <typename T>
	class Component : public BaseComponent{
	public:
		static int GetId() {
			static int id = nextId++;
			return id;
		}
	};





	class System {
	public:
		System() = default;
		~System() = default;

		void AddEntity(Entity entity);
		void RemoveEntity(Entity entity);

		inline std::vector<Entity> GetEntities() const { return m_Entities; }
		inline const Signature& GetSignature() const { return m_Signature; }

		template <typename T> void RequireComponent();

	private:
		Signature m_Signature;
		std::vector<Entity> m_Entities;
	};

	template <typename T>
	void System::RequireComponent() {
		const int componentId = Component<T>::GetId();
		m_Signature.set(componentId);
	}





	class BasePool {
	public:
		virtual ~BasePool() {};
		virtual void TryToRemove(int entityId) = 0;
	};

	template<typename T>
	class Pool : public BasePool {

	public:
		Pool(int capacity = 100) {
			m_Size = 0;
			m_Data.resize(capacity);
		}

		virtual ~Pool() = default;

		bool IsEmpty() const{
			return m_Size == 0;
		}

		int GetSize() const {
			return m_Size;
		}

		void Resize(int n) {
			m_Data.resize(n);
		}

		void Clear() {
			m_Data.clear();
			m_Size = 0;
		}

		void Set(int entityId, T object) {
			if (m_EntityToIndex.find(entityId) != m_EntityToIndex.end()) {
				int index = m_EntityToIndex[entityId];
				m_Data[index] = object;
			}
			else {
				int index = m_Size;
				m_EntityToIndex.emplace(entityId, index);
				m_IndexToEntity.emplace(index, entityId);
				if (index >= m_Data.size()) {
					m_Data.resize(m_Size * 2);
				}
				m_Data[index] = object;
				m_Size++;
			}
		}

		void Remove(int entityId) {
			int indexOfRemoved = m_EntityToIndex[entityId];
			int indexOfLast = m_Size - 1;
			m_Data[indexOfRemoved] = m_Data[indexOfLast];

			int entityIdOfLastElement = m_IndexToEntity[indexOfLast];
			m_EntityToIndex[entityIdOfLastElement] = indexOfRemoved;
			m_IndexToEntity[indexOfRemoved] = entityIdOfLastElement;

			m_EntityToIndex.erase(entityId);
			m_IndexToEntity.erase(indexOfLast);

			m_Size--;
		}

		void TryToRemove(int entityId) override {
			if (m_EntityToIndex.find(entityId) != m_EntityToIndex.end()) {
				Remove(entityId);
			}
		}

		bool HasEntityToIndex(int entityId) {
			return m_EntityToIndex.count(entityId) == 0;
		}

		T& Get(int entityId) {
			if (m_EntityToIndex.find(entityId) == m_EntityToIndex.end()) {
				T result = T();
				result.is_valid = false;
				return result;//Probar con OPTIONAL
			}

			int index = m_EntityToIndex.at(entityId);
			return static_cast<T&>(m_Data[index]);
		}

		T& operator[](unsigned int index) {
			return m_Data[index];
		}

	private:
		int m_Size;
		std::vector<T> m_Data;

		std::unordered_map<int, int> m_EntityToIndex;
		std::unordered_map<int, int> m_IndexToEntity;
	};

	class Coordinator {
	public:
		Coordinator() = default;

		void Update();

		Entity CreateEntity();
		void KillEntity(Entity entity);

		std::set<Entity> QueryEntitiesBySignature(Signature signature);
		//Query was split into two methods (one to construct the signature of the query and another to add it to the map andretrieve it) to avoid exponential increse time builds due to templates(because a query can be done with differents components and different order)
		Signature RegisterQueryRequirement();
		template<typename T> void AddQueryRequirement();

		template<typename T, typename ...TArgs>	void AddComponent(Entity entity, TArgs&& ...args);
		template<typename T> void RemoveComponent(Entity entity);
		template<typename T> bool HasComponent(Entity entity) const;
		template<typename T> T& GetComponent(Entity entity) const;

		void GroupEntity(Entity entity, const std::string& group);
		bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
		std::set<Entity> GetEntitiesByGroup(const std::string& group) const;
		void RemoveEntityFromGroup(Entity entity);

	private:
		void UpdateQueryList(Entity entity);

	private:
		int m_NumEntities = 0;

		Signature m_Signature;
		std::unordered_map<Signature, std::set<Entity>> m_QueryEntities;

		std::vector<std::shared_ptr<BasePool>> m_ComponentPools;

		std::vector<Signature> m_EntityComponentSignatures;
		std::unordered_map<std::type_index, System*> m_Systems;

		std::set<Entity> m_EntitiesToBeModified;
		std::set<Entity> m_EntitiesToBeKilled;

		std::deque<int> m_FreeIds;

		std::unordered_map<std::string, std::set<Entity>> m_EntitiesPerGroup;
		std::unordered_map<int, std::string> m_GroupPerEntities;

	};

	template<typename T>
	void Coordinator::AddQueryRequirement() {
		const int componentId = Component<T>::GetId();
		m_Signature.set(componentId);
	}

	template<typename T, typename ...TArgs>
	void Coordinator::AddComponent(Entity entity, TArgs&& ...args) {
		const int componentId = Component<T>::GetId();
		const int entityId = entity.GetId();

		if (componentId >= m_ComponentPools.size()) {
			m_ComponentPools.resize(componentId + 1, nullptr);
		}

		if (!m_ComponentPools[componentId]) {
			std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
			m_ComponentPools[componentId] = newComponentPool;
		}

		std::shared_ptr<Pool<T>> componentPool = std::static_pointer_cast<Pool<T>>(m_ComponentPools[componentId]);

		T newComponent(std::forward<TArgs>(args)...);
		componentPool->Set(entityId, newComponent);


		m_EntityComponentSignatures[entityId].set(componentId);

		m_EntitiesToBeModified.insert(Entity(entityId));
	}

	
	

	template<typename T>
	void Coordinator::RemoveComponent(Entity entity) {
		const int componentId = Component<T>::GetId();
		const int entityId = entity.GetId();

		std::shared_ptr<Pool<T>> componentPool = std::static_pointer_cast<Pool<T>>(m_ComponentPools[componentId]);
		componentPool->Remove(entityId);


		m_EntityComponentSignatures[entityId].set(componentId, false);
		
		m_EntitiesToBeModified.insert(Entity(entityId));
	}

	template<typename T>
	bool Coordinator::HasComponent(Entity entity) const{
		const int componentId = Component<T>::GetId();
		const int entityId = entity.GetId();

		return m_EntityComponentSignatures[entityId].test(componentId);
	}

	template<typename T>
	T& Coordinator::GetComponent(Entity entity) const{
		const int componentId = Component<T>::GetId();
		const int entityId = entity.GetId();

		auto componentPool = std::static_pointer_cast<Pool<T>>(m_ComponentPools[componentId]);
		return componentPool->Get(entityId);
	}
}