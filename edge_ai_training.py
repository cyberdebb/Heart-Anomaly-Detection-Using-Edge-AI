# Lightweight model for ESP32 - ECG Classification
# Paste into Edge Impulse: Impulse Design -> Learning Block -> Expert Mode

import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import (
    Dense, InputLayer, Dropout, Conv1D, Flatten,
    MaxPooling1D, BatchNormalization, ReLU, Reshape
)
from tensorflow.keras.optimizers.legacy import Adam

# ============================================================================
# HYPERPARAMETERS
# ============================================================================
EPOCHS = args.epochs or 40
LEARNING_RATE = args.learning_rate or 0.001
BATCH_SIZE = args.batch_size or 64
ENSURE_DETERMINISM = args.ensure_determinism

# ============================================================================
# DATA PREPARATION
# ============================================================================
if not ENSURE_DETERMINISM:
    train_dataset = train_dataset.shuffle(buffer_size=BATCH_SIZE * 4)

train_dataset = train_dataset.batch(BATCH_SIZE, drop_remainder=False)
validation_dataset = validation_dataset.batch(BATCH_SIZE, drop_remainder=False)

# ============================================================================
# LITE ARCHITECTURE - Optimized for ESP32
# ============================================================================
# Fewer parameters, lower RAM usage
# Estimate: ~15-25KB of Flash, ~5-10KB of RAM

model = Sequential()

# Input
model.add(InputLayer(input_shape=(input_length,)))
model.add(Reshape((input_length, 1)))

# Block 1: Small Conv
model.add(Conv1D(16, kernel_size=7, padding='same'))
model.add(BatchNormalization())
model.add(ReLU())
model.add(MaxPooling1D(pool_size=4))

# Block 2
model.add(Conv1D(32, kernel_size=5, padding='same'))
model.add(BatchNormalization())
model.add(ReLU())
model.add(MaxPooling1D(pool_size=4))

# Block 3
model.add(Conv1D(32, kernel_size=3, padding='same'))
model.add(ReLU())
model.add(MaxPooling1D(pool_size=2))

# Compact classifier
model.add(Flatten())
model.add(Dense(32, activation='relu'))
model.add(Dropout(0.3))
model.add(Dense(classes, name='y_pred', activation='softmax'))

# ============================================================================
# COMPILATION AND TRAINING
# ============================================================================
opt = Adam(learning_rate=LEARNING_RATE)

callbacks.append(BatchLoggerCallback(
    BATCH_SIZE,
    train_sample_count,
    epochs=EPOCHS,
    ensure_determinism=ENSURE_DETERMINISM
))

callbacks.append(tf.keras.callbacks.EarlyStopping(
    monitor='val_accuracy',
    patience=8,
    restore_best_weights=True
))

model.compile(
    loss='categorical_crossentropy',
    optimizer=opt,
    metrics=['accuracy']
)

model.fit(
    train_dataset,
    epochs=EPOCHS,
    validation_data=validation_dataset,
    verbose=2,
    callbacks=callbacks
)

disable_per_channel_quantization = False