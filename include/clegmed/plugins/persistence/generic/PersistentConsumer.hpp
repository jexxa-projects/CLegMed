
#pragma once
#include "../repository/Repository.hpp"

namespace clegmed::plugins::persistence {
    template<typename RepositoryType, typename Entity>
    [[nodiscard]] auto persistentStore(RepositoryType& repository) {

        auto lambda_strategy = [&repository](Entity&& entity) {
            if (auto id = utils::EntityTraits<Entity>::getId(entity); repository.get(id).has_value()) {
                repository.update(entity);
            } else {
                repository.add(std::move(entity));
            }
        };

        return core::make_consumer(lambda_strategy);
    }
}
