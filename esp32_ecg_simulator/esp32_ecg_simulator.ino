/**
 * ESP32 ECG Simulator to test Edge Impulse model
 *
 * Simulates ECG signals based on the MIT-BIH dataset (187 points, 125Hz)
 * to test the heartbeat classification model.
 *
 * It has 10 samples of each class, randomly selected by pressing 'A'
 *
 * Author: Generated for ianaborda project
 */

#include <ECG-Anomaly-Detecion_inferencing.h>

// ============================================================================
// CONFIGURATION
// ============================================================================

#define SAMPLE_RATE_HZ    125
#define NUM_SAMPLES       187
#define SAMPLES_PER_CLASS 10
#define NUM_CLASSES       5

// ============================================================================
// SAMPLES FROM THE MIT-BIH DATASET - 10 samples of each class
// ============================================================================

#include "ecg_samples_10each.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

float ecg_buffer[NUM_SAMPLES];
int current_class = 0;
int current_sample_idx = 0;

const char* class_names[] = {"Normal", "Supraventricular", "Ventricular", "Fusion", "Unknown"};

// ============================================================================
// EDGE IMPULSE FUNCTIONS
// ============================================================================

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, ecg_buffer + offset, length * sizeof(float));
    return 0;
}

void run_inference() {
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    signal.get_data = &raw_feature_get_data;

    ei_impulse_result_t result = {0};
    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);

    if (err != EI_IMPULSE_OK) {
        Serial.print("Inference error: ");
        Serial.println(err);
        return;
    }

    Serial.println("\n========================================");
    Serial.print("Real class: ");
    Serial.print(class_names[current_class]);
    Serial.print(" (sample ");
    Serial.print(current_sample_idx);
    Serial.println(")");
    Serial.println("----------------------------------------");
    Serial.println("Classification results:");

    float max_value = 0;
    int max_index = 0;

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        Serial.print("  ");
        Serial.print(result.classification[ix].label);
        Serial.print(": ");
        Serial.print(result.classification[ix].value * 100, 1);
        Serial.println("%");

        if (result.classification[ix].value > max_value) {
            max_value = result.classification[ix].value;
            max_index = ix;
        }
    }

    Serial.println("----------------------------------------");
    Serial.print("Prediction: ");
    Serial.print(result.classification[max_index].label);
    Serial.print(" (");
    Serial.print(max_value * 100, 1);
    Serial.print("%) ");

    // Check if correct
    const char* predicted = result.classification[max_index].label;
    bool correct = (strcmp(predicted, class_names[current_class]) == 0);
    if (correct) {
        Serial.println("-> CORRECT!");
    } else {
        Serial.println("-> WRONG!");
    }

    Serial.print("Time: ");
    Serial.print(result.timing.dsp + result.timing.classification);
    Serial.println(" ms");
    Serial.println("========================================\n");
}

void load_sample(int class_idx, int sample_idx) {
    current_class = class_idx;
    current_sample_idx = sample_idx;

    const float* source;
    switch(class_idx) {
        case 0: source = ecg_normal[sample_idx]; break;
        case 1: source = ecg_supraventricular[sample_idx]; break;
        case 2: source = ecg_ventricular[sample_idx]; break;
        case 3: source = ecg_fusion[sample_idx]; break;
        case 4: source = ecg_unknown[sample_idx]; break;
        default: source = ecg_normal[0]; break;
    }

    for (int i = 0; i < NUM_SAMPLES; i++) {
        ecg_buffer[i] = pgm_read_float(&source[i]);
    }
}

void load_random_sample() {
    int class_idx = random(0, NUM_CLASSES);
    int sample_idx = random(0, SAMPLES_PER_CLASS);
    load_sample(class_idx, sample_idx);
}

// ============================================================================
// SETUP AND LOOP
// ============================================================================

void setup() {
    Serial.begin(115200);
    while (!Serial);

    randomSeed(analogRead(0));

    Serial.println();
    Serial.println("========================================");
    Serial.println("   ESP32 ECG Simulator v2");
    Serial.println("   10 samples per class");
    Serial.println("========================================");
    Serial.println();

    Serial.print("Model: ");
    Serial.println(EI_CLASSIFIER_PROJECT_NAME);
    Serial.print("Window size: ");
    Serial.print(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    Serial.println(" samples");
    Serial.println();

    Serial.println("Commands:");
    Serial.println("  A - Test RANDOM sample");
    Serial.println("  0-9 - Test specific Normal sample");
    Serial.println("  T - Test ALL 50 samples");
    Serial.println();
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();

        if (cmd == 'a' || cmd == 'A') {
            Serial.println("\n>> Testing RANDOM sample...");
            load_random_sample();
            run_inference();
        }
        else if (cmd == 't' || cmd == 'T') {
            Serial.println("\n>> Testing ALL 50 samples...\n");
            int correct = 0;
            int total = 0;

            for (int c = 0; c < NUM_CLASSES; c++) {
                for (int s = 0; s < SAMPLES_PER_CLASS; s++) {
                    load_sample(c, s);

                    // Run silent inference
                    signal_t signal;
                    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
                    signal.get_data = &raw_feature_get_data;
                    ei_impulse_result_t result = {0};
                    run_classifier(&signal, &result, false);

                    // Find prediction
                    float max_val = 0;
                    int max_idx = 0;
                    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                        if (result.classification[ix].value > max_val) {
                            max_val = result.classification[ix].value;
                            max_idx = ix;
                        }
                    }

                    const char* predicted = result.classification[max_idx].label;
                    bool is_correct = (strcmp(predicted, class_names[c]) == 0);
                    if (is_correct) correct++;
                    total++;

                    Serial.print(class_names[c]);
                    Serial.print("[");
                    Serial.print(s);
                    Serial.print("] -> ");
                    Serial.print(predicted);
                    Serial.print(" (");
                    Serial.print(max_val * 100, 0);
                    Serial.print("%) ");
                    Serial.println(is_correct ? "OK" : "ERROR");

                    delay(100);
                }
            }

            Serial.println("\n========================================");
            Serial.print("Accuracy: ");
            Serial.print(correct);
            Serial.print("/");
            Serial.print(total);
            Serial.print(" = ");
            Serial.print((float)correct / total * 100, 1);
            Serial.println("%");
            Serial.println("========================================\n");
        }
        else if (cmd >= '0' && cmd <= '9') {
            int sample_idx = cmd - '0';
            Serial.print("\n>> Testing Normal sample ");
            Serial.println(sample_idx);
            load_sample(0, sample_idx);
            run_inference();
        }
    }
}
