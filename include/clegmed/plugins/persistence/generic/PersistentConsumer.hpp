
#pragma once
#include "../repository/Repository.hpp"

namespace clegmed::plugins::persistence {


    template<typename RepositoryType>
    [[nodiscard]] auto persistentStore(RepositoryType& repository) {
        using Entity = extract_entity_t<RepositoryType>;

        auto lambda_strategy = [&repository](Entity entity) {
            if (auto id = utils::EntityTraits<Entity>::getId(entity); repository.get(id).has_value()) {
                repository.update(entity);
            } else {
                repository.add(std::move(entity));
            }
        };

        return core::make_consumer(lambda_strategy);
    }
}
