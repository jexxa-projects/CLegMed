# Software Requirements Specification (SRS) - clegmed v0.1.0

This document serves as the single source of truth for software requirements, according to IEC 62304. All functional code changes must trace back to an ID in this document.

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

---

## 🔒 [CHORE-001] Repository Governance & Compliance Tools

*   **[CHORE-001-A] Documentation**  
    The repository shall maintain an up-to-date README detailing the safety class, project layout, and building instructions.
*   **[CHORE-001-B] Commit Verification**  
    A local `commit-msg` Git-hook shall enforce the presence of a valid tracking token (`[REQ-XXXX]`, `[BUG-XXXX]`, or `[CHORE-XXXX]`) before any commit is accepted.
*   **[CHORE-001-C] Requirements Engineering & Lifecycle Tracking**  
    The project shall maintain a single source of truth for software requirements within this document, allowing manual lifecycle tracking via status tags (e.g., `[Draft]`, `[Approved]`, `[Verified]`).