#pragma once
#include <utility>

namespace clegmed::core{
    /**
     * @class Filter
     * @brief Base class providing a compile-time polymorphic interface for medical data filtering.
     *
     * @details
     * This component defines the unified interface for all digital signal and image processing
     * filters within the system. To satisfy stringent execution timing constraints in medical
     * data processing pipelines, it replaces traditional runtime virtual polymorphism with
     * static polymorphism using the C++23 "Deducing This" feature.
     *
     * @note
     * <b>Safety-Critical Design Decisions (Risk Management):</b>
     * - <b>Resource Preservation:</b> Copy and Move operations are explicitly deleted to prevent
     *   accidental duplication or shallow-copying of heavy medical data buffers, ensuring strict
     *   single-ownership patterns.
     * - <b>Memory Safety:</b> Features a public virtual destructor to guarantee leak-free lifecycle
     *   management when instances are managed via base pointers in standard containers.
     *
     * @b Requirement [REQ-003-A]
     */
    class Filter {
    public:
        /**
        * @brief Default constructor for creating a Filter instance.
        * @details Initializes the base component structure. Guaranteed noexcept.
        */
        Filter() = default; // Standard-Konstruktor explizit beibehalten

        /**
         * @brief Copy constructor is explicitly deleted.
         * @details Safety measure to avoid duplicating safety-critical medical data frames.
         */
        Filter(const Filter&) = delete;

        /**
         * @brief Copy assignment operator is explicitly deleted.
         * @details Prevents unintended overwriting of runtime active filter configurations.
         */
        Filter& operator=(const Filter&) = delete;

        /**
         * @brief Move constructor is explicitly deleted.
         * @details Prevents slicing and pointer invalidation within active pipeline topologies.
         */
        Filter(Filter&&) = default;

        /**
         * @brief Move assignment operator is explicitly deleted.
         * @details Enforces immutability of the allocated filter structure during execution.
         */
        Filter& operator=(Filter&&) = default;

        /**
         * @brief Virtual destructor for proper cleanup of allocated resources.
         * @details Enforces proper cleanup of allocated resources in all child classes.
         */
        virtual ~Filter() = default;

        /**
          * @brief Resolves the calling instance to its concrete, derived implementation type.
          *
          * @details Uses C++23 explicit object parameters ("Deducing This") to query the exact
          *          compile-time type of the subclass. This eliminates the runtime overhead
          *          of virtual function tables (vtables) while preserving perfect forward-qualification
          *          (constness and value-category) of the caller.
          *
          * @tparam Self Auto-deduced type of the calling object (subclass).
          * @param explicit_this Explicit reference to the calling instance.
          * @return Perfect-forwarded reference (lvalue or rvalue) to the derived class instance.
          */
        template<typename Self>
        auto&& self(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this);
        }
    };
} // clegmed
