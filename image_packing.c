#include "image_packing.h"

#include <string.h>

// Number of bits in one byte.
#define ONE_BYTE 8

size_t binary_image_packed_size(size_t pixel_count) {
    // Round up so any leftover pixels get their own byte (9 pixels need 2 bytes).
    return (pixel_count + ONE_BYTE - 1) / ONE_BYTE;
}

// Check every pixel before packing so invalid input leaves the output unchanged.
static BinaryImageStatus validate_pixels(const uint8_t *pixels, size_t pixel_count) {
    for (size_t index = 0; index < pixel_count; ++index) {
        if (pixels[index] != '0' && pixels[index] != '1') {
            return BINARY_IMAGE_INVALID_PIXEL;
        }
    }
    return BINARY_IMAGE_OK;
}

BinaryImageStatus binary_image_pack(const uint8_t *pixels, size_t pixel_count, uint8_t *packed,
                                    size_t packed_capacity, size_t *bytes_written) {
    // Reject missing pointers, invalid image sizes, and buffers that are too small.
    if (pixels == NULL || packed == NULL || bytes_written == NULL) {
        return BINARY_IMAGE_NULL_POINTER;
    }
    if (pixel_count == 0U || pixel_count > BINARY_IMAGE_MAX_PIXELS) {
        return BINARY_IMAGE_INVALID_SIZE;
    }

    const size_t required_bytes = binary_image_packed_size(pixel_count);
    if (packed_capacity < required_bytes) {
        return BINARY_IMAGE_INSUFFICIENT_CAPACITY;
    }

    const BinaryImageStatus validation = validate_pixels(pixels, pixel_count);
    if (validation != BINARY_IMAGE_OK) {
        return validation;
    }

    // Start with all bits cleared, including unused bits in the last byte.
    memset(packed, 0, required_bytes);

    for (size_t index = 0; index < pixel_count; ++index) {
        if (pixels[index] == '1') {
            // Fill each byte from its highest bit to its lowest bit.
            // With 8-bit bytes, pixel 0 uses bit 7 and pixel 8 uses bit 7 of byte 1.
            const size_t byte_index = index / ONE_BYTE;
            const size_t bit_index = ONE_BYTE - 1 - index % ONE_BYTE;

            // Shift a single 1 into place, then OR it into the byte.
            packed[byte_index] |= (1U << bit_index);
        }
    }

    // Report the output length only after packing succeeds.
    *bytes_written = required_bytes;
    return BINARY_IMAGE_OK;
}

BinaryImageStatus binary_image_unpack(const uint8_t *packed, size_t pixel_count, uint8_t *pixels,
                                      size_t pixel_capacity) {
    // The caller supplies pixel_count because the last byte may contain padding.
    if (packed == NULL || pixels == NULL) {
        return BINARY_IMAGE_NULL_POINTER;
    }
    if (pixel_count == 0U || pixel_count > BINARY_IMAGE_MAX_PIXELS) {
        return BINARY_IMAGE_INVALID_SIZE;
    }
    if (pixel_capacity < pixel_count) {
        return BINARY_IMAGE_INSUFFICIENT_CAPACITY;
    }

    for (size_t index = 0; index < pixel_count; ++index) {
        // Locate the same byte and bit used when packing this pixel.
        const size_t byte_index = index / ONE_BYTE;
        const size_t bit_index = ONE_BYTE - 1 - index % ONE_BYTE;

        // AND with a single-bit mask to check whether that bit is set.
        // Restore character bytes ('0' or '1'), rather than numeric 0 or 1.
        if ((packed[byte_index] & (1U << bit_index)) != 0) {
            pixels[index] = '1';
        } else {
            pixels[index] = '0';
        }
    }

    return BINARY_IMAGE_OK;
}
