
#pragma once
#include <optional>
#include <ranges>
#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {
    template <typename RepositoryType, typename EntityId, typename Entity>
    concept isRepository =
            utils::hasEntityTraits<Entity> &&
        requires(RepositoryType& repository, EntityId&& aggregate_id, Entity&& aggregate) {
            { repository.init() }                       -> std::same_as<void>;
            { repository.update(aggregate) }            -> std::same_as<void>;
            { repository.remove(aggregate_id) }         -> std::same_as<void>;
            { repository.removeAll() }                  -> std::same_as<void>;
            { repository.add(std::move(aggregate)) }    -> std::same_as<void>;
            { repository.get( aggregate_id )}           -> std::same_as<std::optional<EntityHandle>>;
            { repository.get()}                         -> std::ranges::range;
        };
}
