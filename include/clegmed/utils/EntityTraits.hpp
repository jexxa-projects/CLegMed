
#pragma once
#include <algorithm>
#include <concepts>
#include "OptionalReference.hpp"
namespace clegmed::utils {
    template<typename T>
    inline constexpr bool always_false = false;

    template <typename T, typename Inner>
    concept IsOptionalOf = std::same_as<T, std::optional<Inner>>;


    template<typename T>
    concept hasDefaultIdMethod = requires(const T& a) {
        { a.aggregateId() };
    };

    template<typename T>
    struct EntityTraits {
        /**
         * @brief Extrahiert die ID.
         * Versucht zuerst aggregate_id(), ansonsten harter Fehler.
         */
        static auto getId(const T& obj) {
            if constexpr (hasDefaultIdMethod<T>) {
                return obj.aggregateId();
            } else {
                static_assert(always_false<T>,
                    "Error: EntityTraits not specialized for this type AND "
                    "no 'aggregate_id()' method found in the class. "
                    "Please specialize clegmed::utils::EntityTraits<T>.");
            }
        }
    };

    template <typename T>
    concept isEntity =
        std::move_constructible<T> &&
        !std::copy_constructible<T> &&
        requires(const T& a) {
        { EntityTraits<T>::getId(a) };
        };


    template<typename Entity>
    using EntityHandle = optional<Entity&>;

    template<typename Entity>
    using EntityCollection = std::vector<std::reference_wrapper<Entity>>;

    template<typename T>
    using EntityId_t = std::decay_t<decltype(EntityTraits<T>::getId(std::declval<T>()))>;


}
