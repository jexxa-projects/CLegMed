
#pragma once
#include <algorithm>
#include <concepts>
#include <shared_mutex>

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

    template<typename T, typename LockType>
    class EntityHandle {
        LockType lock_;
        T& entity_;

    public:
        EntityHandle(LockType&& lock, T& entity) noexcept
            : lock_(std::move(lock)), entity_(entity) {}

        T* operator->() { return &entity_; }
        const T* operator->() const { return &entity_; }

        T& get() { return entity_; }
        LockType& lock() { return lock_; }

        // Move only
        EntityHandle(const EntityHandle&) = delete;
        EntityHandle(EntityHandle&&) = default;
    };



    template <typename T>
    concept isEntity =
        std::move_constructible<T> &&
        !std::copy_constructible<T> &&
        requires(const T& a) {
        { EntityTraits<T>::getId(a) };
        };

    template<typename T>
    using EntityId_t = std::decay_t<decltype(EntityTraits<T>::getId(std::declval<T>()))>;

    template<typename T>
    using EntityView_t = EntityHandle<const T, std::shared_lock<std::shared_mutex>>;

    template<typename T>
    using EntityHandle_t = EntityHandle<T, std::shared_lock<std::shared_mutex>>;


}
