
#pragma once
#include <optional>
#include <ranges>
#include <shared_mutex>

#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {


    template <typename RepositoryType, typename Entity>

    concept isRepository =
            utils::isEntity<Entity> &&
        requires(
            RepositoryType& repository,
            const utils::EntityId_t<Entity>& entity_id,
            Entity&& entity,
            utils::EntityHandle_t<Entity> entity_handle)
    {
            { repository.init() }                   -> std::same_as<void>;
            { repository.update( std::move(entity_handle) ) }  -> std::same_as<void>;
            { repository.remove(entity_id) }        -> std::same_as<void>;
            { repository.removeAll() }              -> std::same_as<void>;
            { repository.add(std::move(entity)) }   -> std::same_as<void>;
            { repository.get( entity_id )}          -> std::same_as<std::optional<utils::EntityHandle_t<Entity>>>;
            { repository.getAll()}                  -> std::ranges::range;
        };
}
