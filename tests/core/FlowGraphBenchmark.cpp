#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include "clegmed/core/Consumer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Producer.hpp"

static void BM_FlowGraphThroughput(benchmark::State& state) {
    // 1. Arrange
    using namespace clegmed::core;

    std::vector<std::string> data_storage;
    // Speicher vorreservieren, um Reallozierungen während des Tests zu minimieren
    data_storage.reserve(10000);

    auto producer_strategy = [] { return "Hello"; };
    auto processor_strategy = [](const std::string &input) { return input + " World"; };
    auto consumer_strategy = [&data_storage](const std::string &data) {
        data_storage.push_back(data);
    };

    auto producer = make_producer(producer_strategy);
    auto processor = make_processor(processor_strategy);
    auto consumer = make_consumer(consumer_strategy);

    producer.outputPipe().connect(processor.inputPipe());
    processor.outputPipe().connect(consumer.inputPipe());

    int64_t items_processed = 0;

    // 2. Act
    for ([[maybe_unused]]auto _ : state) {
        producer.produce();
        items_processed++;

        // Verhindert Speicherüberlauf: Speicher regelmäßig leeren, ohne Kapazität zu verlieren
        if (data_storage.size() >= 5000) {
            // Dem Compiler mitteilen, dass die Daten bis hierhin wichtig waren
            benchmark::DoNotOptimize(data_storage);
            data_storage.clear();
        }
    }

    // 3. Durchsatz-Metriken an das Framework melden
    state.SetItemsProcessed(items_processed);
}

// Benchmark registrieren
BENCHMARK(BM_FlowGraphThroughput);

// Generiert die main()-Methode für Google Benchmark
BENCHMARK_MAIN();
