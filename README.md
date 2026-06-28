# clegmed (v0.1.2)

A modern, highly optimized C++23 shared library designed for pipes and 
filters. This project aligns with established regulatory software 
development lifecycle processes.

It serves as the C++ pendant to [JLegMed](https://github.com/jexxa-projects/JLegMed) (Java Legacy Mediator), 
specifically tailored for resource-constrained IoT, embedded systems, and real-time environments.

---

## 🩺 Regulatory Compliance & Quality Standards

*   **Software Risk Classification:** Medium-Risk Profile (Standard Target)
*   **Design Paradigm:** Defensive Programming, Object Ownership Semantics, Zero-Allocation Core.
*   **Traceability Requirement:** Every functional code modification must be tied to a requirement ID defined in `REQUIREMENTS.md`. Commits are automatically verified via a local `commit-msg` Git-Hook.

---

## 🛠️ Tech Stack & Tooling

To ensure deterministic behavior and meet qualification standards, the project utilizes a highly controlled toolchain:

*   **Language Standard:** C++23 (Enforced via CMake)
*   **Build System:** CMake 4.1+ (Target-based architecture)
*   **Compiler:** Apple Clang 21.0.0 / GCC 13+ 
*   **Unit Testing Framework:** GoogleTest v1.17.0 (Integrated as SOUP via `FetchContent`)

---
## 🛠️ Quickstart
### Hello World Example
```C++     
int main(int argc, char** argv) {
    using namespace clegmed::shortcuts;

    auto flowgraph = FlowGraph{}
        .every(2s)
        .from(emit("Hello World"))
        .then(passThrough())
        .consumeWith(logInfo());

    auto clegmed = CLegMed(argc, argv, std::move(flowgraph));
    clegmed.run();
}
```

---

## 📁 Repository Structure

The project follows a mirrored directory layout for optimal separation of public interfaces, inner logic, and validation suites:

```text
    clegmed/
    ├── CMakeLists.txt        # Central target-based build configuration
    ├── REQUIREMENTS.md       # Software Requirements Specification (SRS)
    ├── include/              # Public API Headers
    │   └── clegmed/
    │       └── utils/        # Utility functions for core- and plugins
    │       └── core/         # Core subsystem definitions
    │       └── plugins/      # Plugins definitions
(   ├── src/ )                # Internal Implementation (Source) for future plugins that require external dependencies or cpp files 
(   │   └── plugins/ )        # Plugins implementation
    └── tests/                # Verification Suite (Mirrored)
        └── core/             # GoogleTest cases for the core subsystem
```

---

## 🚀 Building and Running Tests

### Prerequisites
Ensure you have CMake (4.1+) and a C++23 compliant compiler installed on your system.

### Local Development Build
We enforce an **out-of-source build** pattern to keep the repository root pristine:

```bash
# 1. Create and enter the build directory
mkdir -p build && cd build

# 2. Configure the project and fetch dependencies (GoogleTest)
cmake ..

# 3. Compile the shared library and the test runner
cmake --build .
```

### Executing the Verification Suite
Tests are automatically registered with CMake's test automation runner (**CTest**):

```bash
# Run all tests using CTest
ctest --output-on-failure
```

---

## 🔒 Git Workflow & Commit Guidelines

To comply with regulatory requirements, all commit messages must follow the *Conventional Commits* standard and include a tracking ID matching the `REQUIREMENTS.md`.

**Format:** `<type>(<scope>): <description> [<TRACKING-ID>]`

*   **Feature Commit Example:**  
    `feat(core): implement localized engine initialization [REQ-002]`
*   **Maintenance Commit Example:**  
    `chore(cmake): bump build version to 0.1.0 [CHORE-000]`

If your message does not contain a valid `[REQ-XXXX]`, `[BUG-XXXX]`, or `[CHORE-XXXX]` token, the local Git-Hook will reject the commit.

---

## Release
* Configure release mode
  ```shell
   cmake -B build -DCMAKE_BUILD_TYPE=Release
  ```

* Compile
  ```shell
   make --build build --config Release
  ```

* Create archive
  ```shell
    cd build
    cpack -C Release
  ```
