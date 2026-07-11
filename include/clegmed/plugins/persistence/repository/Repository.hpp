
#pragma once
#include <ranges>

#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {


    template <typename Repository, typename Entity>
    concept RepositoryInterface = requires(
        Repository& repository,
        const utils::EntityId_t<Entity>& entity_id,
        Entity&& entity
    ) {
        { repository.init() }                               -> std::same_as<void>;
        { repository.remove(entity_id) }                    -> std::same_as<void>;
        { repository.removeAll() }                          -> std::same_as<void>;
        { repository.getAll() }                             -> std::ranges::range;
        { repository.get(entity_id) }                       -> std::same_as<utils::EntityHandle<Entity>>;
        { repository.add(std::forward<Entity>(entity)) }    -> std::same_as<void>;
        { repository.update(entity) }                       -> std::same_as<void>;
    };

    template <typename RepositoryType, typename Entity>
    concept isRepository =
        utils::isEntity<Entity> &&
        RepositoryInterface<RepositoryType, Entity>;
};
