
#pragma once
#include <optional>
#include <ranges>

#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {


    template <typename RepositoryType, typename Entity>
    concept isRepository =
        utils::isEntity<Entity> &&
        requires(
            RepositoryType& repository,
            const utils::EntityId_t<Entity>& entity_id,
            Entity&& entity,
            utils::EntityHandle_t<Entity>&& handle
        )
    {
        { repository.init() }                           -> std::same_as<void>;
        { repository.remove(entity_id) }                -> std::same_as<void>;
        { repository.removeAll() }                      -> std::same_as<void>;
        { repository.getAll() }                         -> std::ranges::range;
        { repository.get(entity_id) }                   -> utils::IsOptionalOf<utils::EntityHandle_t<Entity>>;
        { repository.add(std::declval<Entity&&>()) }    -> std::same_as<void>;
        { repository.update(handle) }                   -> std::same_as<void>;
    };
}
