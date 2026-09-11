#pragma once
#include <expected>
#include <filesystem>
#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace clegmed::utils {
    class Properties {
    public:
        enum class LoadError {
            FileNotFound,
            ParseError
        };

        explicit Properties(toml::v3::table tbl) : m_table(std::move(tbl)) {}


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
        auto get(std::string_view path) const -> T {
            //Handle base types
            if constexpr (std::is_fundamental_v<T> || std::is_same_v<T, std::string>) {
                return m_table.at_path(path).value_or(T{});
            } else {
                //Handle complex data structures
                T target{};
                if (auto* node = m_table.at_path(path).as_table()) {
                    Properties sub_properties{ *node };
                    fromProperties(sub_properties, target);
                }
                return target;
            }
        }

    private:
        toml::v3::table m_table;
    };


}