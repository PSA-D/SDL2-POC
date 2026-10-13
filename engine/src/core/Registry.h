#pragma once

#include "core/ComponentPool.h"
#include "core/Entity.h"

#include <cstddef>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::core
{
// Beheert entities en per componenttype één ComponentPool.
// Regel: tijdens each() niets aanmaken of verwijderen. Gebruik destroyLater() en roep na de lus flush() aan.
class Registry
{
public:
    Entity create()
    {
        if (freeEntities.empty())
            return nextEntity++;

        const Entity entity = freeEntities.back();
        freeEntities.pop_back();
        return entity;
    }

    // Eén keer per entity aanroepen; pas bij flush() wordt hij echt verwijderd.
    void destroyLater(Entity entity)
    {
        pendingDestroy.push_back(entity);
    }

    void flush()
    {
        for (const Entity entity : pendingDestroy)
        {
            for (const auto& [type, pool] : pools)
                pool->remove(entity);
            freeEntities.push_back(entity);
        }
        pendingDestroy.clear();
    }

    void clear()
    {
        pools.clear();
        pendingDestroy.clear();
        freeEntities.clear();
        nextEntity = 0;
    }

    template <typename T, typename... Args>
    T& add(Entity entity, Args&&... args)
    {
        auto& pool = pools[typeid(T)];
        if (!pool)
            pool = std::make_unique<ComponentPool<T>>();
        return findPool<T>()->add(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    T& get(Entity entity)
    {
        return findPool<T>()->get(entity);
    }

    template <typename T>
    [[nodiscard]] bool has(Entity entity) const
    {
        const auto* pool = findPool<T>();
        return pool != nullptr && pool->contains(entity);
    }

    template <typename T>
    [[nodiscard]] std::size_t count() const
    {
        const auto* pool = findPool<T>();
        return pool != nullptr ? pool->size() : 0;
    }

    // Roept func(entity, First&, Rest&...) aan voor elke entity die al deze componenten heeft.
    template <typename First, typename... Rest, typename Func>
    void each(Func func)
    {
        auto* first = findPool<First>();
        if (first == nullptr)
            return;

        for (const Entity entity : first->entityList())
        {
            if ((has<Rest>(entity) && ...))
                func(entity, first->get(entity), get<Rest>(entity)...);
        }
    }

private:
    template <typename T>
    ComponentPool<T>* findPool() const
    {
        const auto it = pools.find(typeid(T));
        if (it == pools.end())
            return nullptr;
        // De sleutel typeid(T) garandeert dat dit een ComponentPool<T> is.
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        return static_cast<ComponentPool<T>*>(it->second.get());
    }

    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;
    std::vector<Entity> pendingDestroy;
    std::vector<Entity> freeEntities; // ids van verwijderde entities worden hergebruikt
    Entity nextEntity{0};
};
} // namespace engine::core
