#include <gtest/gtest.h>
#include <type_traits>
#include "clegmed/core/Filter.hpp"

using namespace clegmed::core;

class MockFilter : public Filter {
public:
    int value = 0;

    void modifyValue(const int v) { value = v; }
    [[nodiscard]] int getValue() const { return value; }
};

// --- 1. COMPILE-TIME TESTS (IEC 62304 Traceability: REQ-003-A) ---

// Testet, ob das Kopieren und Verschieben auf Compiler-Ebene blockiert ist
static_assert(!std::is_copy_constructible_v<Filter>, "REQ-003-A Violation: Filter must not be copy constructible!");
static_assert(!std::is_copy_assignable_v<Filter>,    "REQ-003-A Violation: Filter must not be copy assignable!");
static_assert(std::is_move_constructible_v<Filter>, "REQ-003-A Violation: Filter must not be move constructible!");
static_assert(std::is_move_assignable_v<Filter>,    "REQ-003-A Violation: Filter must not be move assignable!");

// Testet, ob der Destruktor virtuell ist (wichtig für polymorphes Löschen)
static_assert(std::has_virtual_destructor_v<Filter>, "REQ-003-A Violation: Filter must have a virtual destructor!");


// --- 2. RUNTIME TESTS (GoogleTest Framework) ---

class FilterTest : public testing::Test {
protected:
    template <typename Self>
    auto&& getFilter(this Self&& explicit_this) {
        // Bei Klassen-Attributen nutzt man std::forward auf das OBJEKT, nicht auf das Attribut selbst!
        return std::forward<Self>(explicit_this).m_filter;
    }

private:
    MockFilter m_filter;
};

// Testet, ob self() auf einem non-const Objekt die veränderbare abgeleitete Klasse liefert
TEST_F(FilterTest, SelfReturnsMutableDerivedReference) {
    // Überprüfung des exakten Typs via decltype
    static_assert(std::is_same_v<decltype(getFilter().self()), MockFilter&>,
                  "self() on non-const object must return MockFilter&");

    // Funktionaler Test: Modifikation über das von self() zurückgelieferte Objekt
    getFilter().self().modifyValue(42);
    EXPECT_EQ(getFilter().getValue(), 42);
}

// Testet, ob self() auf einem const Objekt die schreibgeschützte abgeleitete Klasse liefert
TEST_F(FilterTest, SelfReturnsConstDerivedReference) {
    //Arrange
    const MockFilter& objectUnderTest = getFilter();

    //Act
    getFilter().modifyValue(100);

    //Assert
    static_assert(std::is_same_v<decltype(objectUnderTest.self()), const MockFilter&>,
                  "self() on const object must return const MockFilter&");
    EXPECT_EQ(objectUnderTest.self().getValue(), 100);
}

// Testet das sichere Löschen über den Basisklassen-Pointer (Virtual Destructor Check)
TEST_F(FilterTest, SafeDeletionViaBasePointer) {
    //Arrange
    bool derived_destroyed = false;
    class DestructorCheckFilter : public Filter {
        bool* flag;
    public:
        explicit DestructorCheckFilter(bool* f) : flag(f) {}
        ~DestructorCheckFilter() override { *flag = true; }
    };
    std::unique_ptr<Filter> objectUnderTest = std::make_unique<DestructorCheckFilter>(&derived_destroyed);

    //Act
    objectUnderTest.reset();

    //Assert
    EXPECT_TRUE(derived_destroyed);
}
