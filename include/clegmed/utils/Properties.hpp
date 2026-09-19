#pragma once
#include <filesystem>
#include <expected>
#include <type_traits>

#define TOML_EXCEPTIONS 0 //NOSONAR
#include <toml++/toml.hpp>

namespace clegmed::utils {
    class Properties;

    template <typename T>
    concept DeserializableFromProperties =
        requires(const Properties& properties, T& target) { target.fromProperties(properties); } ||
        requires(const Properties& properties, T& target) { fromProperties(properties, target);} ||
        requires(const Properties& properties, T& target) { T::fromProperties(properties, target);};

    class Properties {
    public:
        enum class LoadError {
            FileNotFound,
            ParseError
        };

        explicit Properties(toml::v3::table tbl) : m_table(std::move(tbl)) {}
        explicit Properties() : m_table( toml::v3::table()) {  }


        [[nodiscard]]
        static auto fromFile(const std::filesystem::path& path) -> std::expected<Properties, LoadError>
        {
            if (!std::filesystem::exists(path)) {
                return std::unexpected(LoadError::FileNotFound);
            }

            auto result = toml::parse_file(path.string());
            if (!result) {
                return std::unexpected(LoadError::ParseError);
            }

            return Properties{ std::move(result).table() };
        }

        template <typename T>
        [[nodiscard]]
        auto get(const std::string_view path) const -> T {
            //Handle base types
            if constexpr (std::is_fundamental_v<T> || std::is_same_v<T, std::string>) {
                return m_table.at_path(path).value_or(T{});
            } else {
                static_assert(DeserializableFromProperties<T>, "Typ T must implement a 'fromProperties' method with supported signatures in DeserializableFromProperties.");

                //Handle complex data structures
                T target{};
                if (auto* node = m_table.at_path(path).as_table()) {
                    if constexpr (
                        Properties sub_properties{ *node };
                        requires { target.fromProperties(sub_properties); })
                    {
                        target.fromProperties(sub_properties);
                    } else if constexpr (requires { fromProperties(sub_properties, target); }) {
                        fromProperties(sub_properties, target);
                    } else if constexpr (requires { T::fromProperties(sub_properties, target); }) {
                        T::fromProperties(sub_properties, target);
                    }
                }
                return target;
            }
        }

        [[nodiscard]]
        auto subProperties(std::string_view path) const -> std::optional<Properties> {
            if (auto* node = m_table.at_path(path).as_table()) {
                return Properties{ *node };
            }
            return std::nullopt;
        }
    private:
        toml::v3::table m_table;
    };

    using PropertiesPtr = std::unique_ptr<Properties>;

}