# ADR 0002: Core Framework Architecture Pattern

*   **Status:** Proposed
*   **Date:** 2026-05-29
*   **Authors:** Michael Repplinger

## Context
The `clegmed` library requires a highly deterministic, modular, and maintainable pipeline architecture to process and stream medical data flows (e.g., routing telemetry data or log transformations).
We need a design that separates distinct processing steps and allows flexible recongfiguration without tight coupling of business logic.

## Decision
We decide to implement the **Pipes & Filters Architecture Pattern** as the structural backbone of the framework:
1. **Filter** will encapsulate an independent processing unit (transformation, validation).
2. **Pipes** act as `InputPipes` or `OutputPipes` to receive data from a preceding filter or send data to a succeeding filter. 
3. **BindingObject** will be introduced as first-class citizens to encapsulate the connection topology between the `OuputPipe` and `InputPipe` of two connected Filters, enabling compile-time or runtime configuration validation.

## Consequences & Risk Evaluation 
*   **Positive (Benefits):**
    *   *High Testability:* Each Filter can be verified independently with GoogleTest via isolated inputs and outputs.
    *   *Reusability:* Filters can be re-arranged for different medical device setups without changing their internal code.
    *   *Strict Separation of Concerns:* Reduces the risk of unintended side effects across subsystems.
*   **Negative / Risks (Mitigations):**
    *   *Performance Overhead:* Passing large medical payloads through multiple pipes could trigger deep data copying.
    *   *Mitigation Strategy:* The C++ implementation must strictly enforce **Value Semantics with Move Semantics (`std::move`)** and **Pass-by-Reference (`const &`)** to guarantee zero-allocation and zero-copy data passing in the core runtime loop.
