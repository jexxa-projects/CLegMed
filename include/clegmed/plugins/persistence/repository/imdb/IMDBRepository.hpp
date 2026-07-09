#pragma once

#include <map>
#include <shared_mutex>
#include "clegmed/utils/EntityTraits.hpp"

// TODO - Verify locking modell. since we will also support DBs, we will finally have Last Write Wins situation.
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
            m_storage.emplace(id, std::move(entity));
        }

        [[nodiscard]] auto get(const id_type& id) {
            using HandleType = utils::EntityHandle_t<Entity>;

            std::shared_lock lock(m_mutex);
            if (auto it = m_storage.find(id); it != m_storage.end()) {
                return std::optional<HandleType>(std::in_place, std::move(lock), it->second);
            }
            return std::nullopt;
        }

        [[nodiscard]] auto getAll() const {
            std::shared_lock lock(m_mutex);

            return m_storage | std::views::keys | std::ranges::to<std::vector<id_type>>();
        }


        void update(utils::EntityHandle_t<Entity> _) {
            // Nothing to be done - just to fulfill the concept
        }

    };
}