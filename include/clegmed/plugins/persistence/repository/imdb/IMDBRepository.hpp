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
        void init() {}

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
            m_storage.emplace(utils::EntityTraits<Entity>::getId(std::move(entity)), std::move(entity));
        }

        [[nodiscard]] auto get(const id_type& id)  {
            using HandleType = utils::EntityHandle_t<Entity>;
            std::shared_lock lock(m_mutex);

            auto it = m_storage.find(id);
            if (it != m_storage.end()) {
                return std::optional<HandleType>(std::in_place, std::move(lock), it->second);
            }

            return std::optional<HandleType>(std::nullopt);
        }

        [[nodiscard]] auto getAll() const {
            std::unique_lock lock(m_mutex);

            return m_storage | std::views::keys | std::ranges::to<std::vector<id_type>>();
        }


        void update(utils::EntityHandle_t<Entity>&& entity_handle) {
            (*entity_handle)->lock().unlock();
        }

    };
}