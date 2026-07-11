#pragma once

#include <map>
#include <shared_mutex>
#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {
    template<typename Entity>
    class IMDBRepository {
        using id_type = utils::EntityId_t<Entity>;

        mutable std::shared_mutex m_mutex;
        std::map<id_type, Entity> m_storage;

    public:
        void init() {
            //We do not need to do anything here
        }

        void remove(const id_type& id) {
            std::unique_lock lock(m_mutex);

            m_storage.erase(id);
        }

        void removeAll() {
            std::unique_lock lock(m_mutex);

            m_storage.clear();
        }


        void add(Entity&& entity) {
            std::unique_lock lock(m_mutex);
            // extract ID before move to avoid UB
            auto id = utils::EntityTraits<Entity>::getId(entity);

            // try_emplace fügt NUR ein, wenn die ID noch nicht existiert
            auto [it, inserted] = m_storage.try_emplace(id, std::move(entity));

            if (!inserted) {
                throw std::invalid_argument("Entity with given ID alread exists!");
            }
        }

        [[nodiscard]] utils::EntityHandle<Entity> get(const id_type& id) {
            std::shared_lock lock(m_mutex);
            if (auto it = m_storage.find(id); it != m_storage.end()) {
                return utils::EntityHandle<Entity>(it->second);
            }
            return {};
        }

        [[nodiscard]] utils::EntityCollection<Entity> getAll() {
            std::shared_lock lock(m_mutex);

            return m_storage
                | std::views::values
                | std::views::transform([](Entity& entity) { return std::ref(entity); })
                | std::ranges::to<utils::EntityCollection<Entity>>();
        }


        void update(const Entity& _) {
            // Nothing to be done - just to fulfill the concept
        }

    };
}