# Software Requirements Specification (SRS) - clegmed v0.1.2

This document serves as the single source of truth for software requirements. All functional code changes must trace back to an ID in this document.


| Requirement ID  | Title                                                         | Status       | Verified By                                                          | Commit / Reference                                                  |
|:----------------|:--------------------------------------------------------------|:-------------|:---------------------------------------------------------------------|:--------------------------------------------------------------------|
| **[REQ-001]**   | Core Infrastructure                                           | **VERIFIED** | `hello_world_test`                                                   | [dfdbb25](https://github.com/jexxa-projects/CLegMed/commit/dfdbb25) |
| **[CHORE-001]** | Governance Tools                                              | **VERIFIED** | Code Review & Git                                                    | Visual verification of README, Git-Hook & CHANGELOG                 |
| **[REQ-002]**   | Modular Data Streaming Pipeline                               | **VERIFIED** | ADR-0002                                                             | [5a8f850](https://github.com/jexxa-projects/CLegMed/commit/5a8f850) |                                                                   |
| **[CHORE-002]** | Architecture Decision Records                                 | **VERIFIED** | Code Review                                                          | [5a8f850](https://github.com/jexxa-projects/CLegMed/commit/5a8f850) |
| **[CHORE-003]** | CMake Structure                                               | **VERIFIED** | Code Review                                                          | [6bf80c8](https://github.com/jexxa-projects/CLegMed/commit/6bf80c8) |
| **[REQ-003]**   | Component Specifications (Derived from ADR 0002)              | **VERIFIED** | tests in `tests/plugins/generic/`                                    |                                                                     |
| **[REQ-004]**   | Set of generic filters for testing purposes                   | **VERIFIED** | tests in `tests/plugins/generic/`                                    |                                                                     |
| **[REQ-005]**   | FlowGraph class to manage filters                             | **VERIFIED** | tests in `GenericFlowGraphTest` and `FlowGraphTest`                  |                                                                     |
| **[REQ-006]**   | Managing class to manage all FlowGraphs within an application | **VERIFIED** | tests in `CLegMedTest`                                               |                                                                     |
| **[REQ-007]**   | Logging support                                               | **VERIFIED** | tests in `LoggerTest`, `GenericProcessorTest`, `GenericConsumerTest` |                                                                     |


---

## 🏗️ [REQ-001] Core Infrastructure & Verification Setup

*   **[REQ-001-A] Target Namespace**  
    The library shall provide core classes encapsulated within the `clegmed::core` namespace.
*   **[REQ-001-B] Verification Method**  
    The engine shall provide a `hello` method returning a deterministic verification string for tag-1 validation.
*   **[REQ-001-C] Automated Testing Framework**  
    The project shall integrate GoogleTest (v1.17.0) via CMake FetchContent and register the executable using CMake's CTest suite.
    *   **Regulatory Status (Tool Qualification):** GoogleTest is used exclusively as an off-device verification tool. Its source code is strictly separated from the production target (`clegmed` library) and is only linked into the test executable.
    *   **Risk Evaluation:** No GoogleTest code is deployed onto the final medical device. It cannot introduce operational runtime risks to the patient. Its impact is limited to verification confidence.

## 🧩 [REQ-002] Modular Data Streaming Pipeline (Functional Requirement)
*   **Description:** The framework shall provide a mechanism to process, transform, and route medical data flows in a highly modular and deterministic manner.
*   **Decoupling:** Individual processing steps must be isolated from each other so they can be reconfigured without tight coupling of the underlying business logic.


## 🧩 [REQ-003] Component Specifications (Derived from ADR 0001)

*   **[REQ-003-A] Filter Components**  
    The framework shall provide processing components ("Filter") that encapsulate a specific data operation. Different filter types will be offered based on the number of intput/output pipes, e.g., `SoureFilter`, `SinkFilter`, `PorcessingFilter`.
*   **[REQ-003-B] Pipe Components**  
    The framework shall provide `InputPipes` and `OutputPipes` to receive data from a preceding filter or send data to a succeeding filter.
*   **[REQ-003-C] Binding Object**  
    The connection topology between an `OuputPipe` and `InputPipe` shall be represented as a first-class citizen (`BindingObject`).

## 🧩 [REQ-004] Set of generic filters for testing purposes
*   **[REQ-004-A] Generic Filter**  
    The framework shall provide a set of generic filters that can be used for testing purposes.
*   **Description:**  
    The generic filters will include basic operations such as identity, passthrough, and no-op filters, which can be used to verify the framework's functionality without the need for complex custom filters.
    These plugins should reside in a separate namespace (`clegmed::plugins::generic`) to enable a clean separation of core functionality and plugins.

## 🧩 [REQ-005] FlowGraph class to manage filters 
*   **[REQ-005-A] FlowGraph**  
    The framework shall provide a concept to build and manage filters within a FlowGraph
*   **Description:**
    The FlowGraph class shall provide a fluent API to set up the final flow graph with explicit semantic from(<PRODUCER>), then(<PROCESSOR>), consumeWith(<CONSUMER>) 
*   **[REQ-005-B] Producer specific methods**  
    The flow graph class shall provide methods for different producer types, such as every(), await() and repeat()

## 🧩 [REQ-006] Managing class to manage all FlowGraphs within an application 
*   **[REQ-006-A] CLegMed**  
    The framework shall provide a concept to build and manage all FlowGraphs of an application, especially to start and stop them
*   **[REQ-006-B] CLegMed - run()**  
    CLegMed shall provide a convenience method that starts all registered flow graphs and blocks main-method until 
    the application is terminated by standard signals (see [REQ-006-C])
*   **[REQ-006-C] CLegMed - Standard Signal handling**  
    CLegMed shall provide a meaningful approach for handling standard signals such as: 
    SIGINT (CTRL-C), SIGTERM (Signal 15) and SIGHUP (Signal 1)
*   **[REQ-006-C] CLegMed - Shortcuts**  
    CLegMed shall provide a shortcuts namespace that uses the required namespaced for convenience purpose 

## 🧩 [REQ-007] Simple Logging support
* **[REQ-007-A] Log Levels**
  The framework shall provide logging for distinct severity levels: 
  * INFO, WARN, and ERROR.Colored Output: Log messages must be 
    outputted in color to easily distinguish message types (e.g., error logs in red).
  * Timestamps: Every log entry must include a standard-formatted timestamp.
* **[REQ-007-B] Log Filter**
  * Provide consumer und processor filter for logging

---

## 🔒 [CHORE-001] Repository Governance & Compliance Tools

*   **[CHORE-001-A] Documentation**  
    The repository shall maintain an up-to-date README detailing the safety class, project layout, and building instructions.
*   **[CHORE-001-B] Commit Verification**  
    A local `commit-msg` Git-hook shall enforce the presence of a valid tracking token (`[REQ-XXXX]`, `[BUG-XXXX]`, or `[CHORE-XXXX]`) before any commit is accepted.
*   **[CHORE-001-C] Requirements Engineering & Lifecycle Tracking**  
    The project shall maintain a single source of truth for software requirements within this document, allowing manual lifecycle tracking via status tags (e.g., `[Draft]`, `[Approved]`, `[Verified]`).

## 🔒 [CHORE-002] Architecture Decision Logging
*   **[CHORE-002-A] Architecture Decision Logging**  
    The project shall document all architecturally significant decisions using Architecture Decision Records (ADRs) to ensure long-term rationale tracking and compliance with IEC 62304 architecture design controls.

## 🔒 [CHORE-003] Build & Release
*   **[CHORE-003-A] CMake Build System**  
    The project shall use CMake as its build system so that each target can be built independently. Especially, the build system should support 
    the separation of core and plugins to facilitate modular development and testing.
*   **[CHORE-003-B] Standard compliant (OS-Source)**
    The project shall provide a dedicated cmake-directory for header only components. The core package shall provide utils, 
    core and generic plugins.
