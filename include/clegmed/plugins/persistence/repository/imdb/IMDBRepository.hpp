#pragma once

#include <map>
#include <shared_mutex>
#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {
    template<typename Entity>
    class IMDBRepository {
        using id_type = std::decay_t<decltype(utils::EntityTraits<Entity>::get_id(std::declval<const Entity&>()))>;

        mutable std::shared_mutex m_mutex;
        std::map<id_type, Entity> m_storage;

    public:
        void init() {}
        void update(utils::EntityHandle<Entity, std::shared_mutex>& entity) {
            //Nothing TODO
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
            m_storage.emplace(utils::EntityTraits<Entity>::get_id(std::move(entity)), std::move(entity));
        }

        [[nodiscard]] auto get(const id_type& id) const {
            std::shared_lock lock(m_mutex);

            auto it = m_storage.find(id);

            using HandleType = utils::EntityHandle<const Entity, std::shared_lock<std::shared_mutex>>;

            if (it != m_storage.end()) {
                return std::optional<HandleType>(std::in_place, std::move(lock), it->second);
            }

            return std::optional<HandleType>(std::nullopt);
        }
    };
}