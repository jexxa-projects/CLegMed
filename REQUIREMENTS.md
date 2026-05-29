# Software Requirements Specification (SRS) - clegmed v0.1.0

This document serves as the single source of truth for software requirements, according to IEC 62304. All functional code changes must trace back to an ID in this document.

# Software Requirements Specification (SRS) - clegmed v0.1.0


| Requirement ID  | Title                           | Status       | Verified By        | Commit / Reference                                                  |
|:----------------|:--------------------------------|:-------------|:-------------------|:--------------------------------------------------------------------|
| **[REQ-001]**   | Core Infrastructure             | **VERIFIED** | `hello_world_test` | [dfdbb25](https://github.com/jexxa-projects/CLegMed/commit/dfdbb25) |
| **[CHORE-001]** | Governance Tools                | **VERIFIED** | Code Review & Git  | Visual verification of README, Git-Hook & CHANGELOG                 |
| **[REQ-002]**   | Modular Data Streaming Pipeline | **APPROVED** | *Pending*          |                                                                     |
| **[CHORE-002]** | Architecture Decision Records   | **DRAFT**    | *Pending*          |                                                                     |



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
