#pragma once

#include "core/Entity.h"

#include <cassert>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

namespace engine::core
{
// Basis zonder type, zodat de Registry van elke pool een component kan verwijderen.
class IComponentPool
{
public:
    IComponentPool() = default;
    virtual ~IComponentPool() = default;
    IComponentPool(const IComponentPool&) = delete;
    IComponentPool& operator=(const IComponentPool&) = delete;
    IComponentPool(IComponentPool&&) = delete;
    IComponentPool& operator=(IComponentPool&&) = delete;

    virtual void remove(Entity entity) = 0;
};

// Sparse set: sparse[entity] is de index in de aaneengesloten components-array.
template <typename T>
class ComponentPool final : public IComponentPool
{
public:
    template <typename... Args>
    T& add(Entity entity, Args&&... args)
    {
        assert(!contains(entity));
        if (entity >= sparse.size())
            sparse.resize(entity + 1, NONE);

        sparse[entity] = components.size();
        entities.push_back(entity);
        components.push_back(T{std::forward<Args>(args)...});
        return components.back();
    }

    // Swap-and-pop: het laatste element neemt de vrije plek in, zodat de array aaneengesloten blijft.
    void remove(Entity entity) override
    {
        if (!contains(entity))
            return;

        const std::size_t index = sparse[entity];
        const Entity last = entities.back();
        components[index] = std::move(components.back());
        entities[index] = last;
        sparse[last] = index;

        components.pop_back();
        entities.pop_back();
        sparse[entity] = NONE;
    }

    [[nodiscard]] bool contains(Entity entity) const
    {
        return entity < sparse.size() && sparse[entity] != NONE;
    }

    [[nodiscard]] T& get(Entity entity)
    {
        return components[sparse[entity]];
    }

    [[nodiscard]] const std::vector<Entity>& entityList() const
    {
        return entities;
    }

    [[nodiscard]] std::size_t size() const
    {
        return components.size();
    }

private:
    static constexpr std::size_t NONE = std::numeric_limits<std::size_t>::max();

    std::vector<std::size_t> sparse;
    std::vector<Entity> entities; // entities[i] hoort bij components[i]
    std::vector<T> components;
};
} // namespace engine::core
