#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

//signatue
//using a bitset(1's & 0's) to keep track of which components an entity has, and also helps keep track of which entities a system is interested in.

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int nextId;
};

//used to assign a unique id to a component type
template <typename T>
class Component: public IComponent {
	//returns the unique id of Component<T>
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

class Entity {
private:
	int id;

public:
	Entity(int id) : id(id) {};
	Entity(const Entity& entity) = default;
	int GetId() const;

	Entity& operator =(const Entity& other) = default;
	bool operator == (const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }
	bool operator >(const Entity& other) const { return id >= other.id; }
	
};

//SYSTEM
//the system processes entities that contain a specific signature

class System {
private:
	Signature componentSignature;
	std::vector<Entity>  entities;

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	//defines the component type that the entities mus have to be considered by the system
	template <typename TComponent> void RequireComponent();
};

class Registry {

};

template<typename TComponent>
void System::RequireComponent(){
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}


#endif