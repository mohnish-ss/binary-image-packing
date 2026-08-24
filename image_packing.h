#ifndef IMAGE_PACKING_H
#define IMAGE_PACKING_H

#include <stddef.h>
#include <stdint.h>

#define BINARY_IMAGE_MAX_PIXELS 4096U
#define BINARY_IMAGE_MAX_BYTES ((BINARY_IMAGE_MAX_PIXELS + 7U) / 8U)

typedef enum
{
    BINARY_IMAGE_OK = 0,
    BINARY_IMAGE_INVALID_SIZE = -1,
    BINARY_IMAGE_INVALID_PIXEL = -2,
    BINARY_IMAGE_NULL_POINTER = -3,
    BINARY_IMAGE_INSUFFICIENT_CAPACITY = -4
} BinaryImageStatus;

size_t binary_image_packed_size(size_t pixel_count);

BinaryImageStatus binary_image_pack(const uint8_t *pixels,
                                    size_t pixel_count,
                                    uint8_t *packed,
                                    size_t packed_capacity,
                                    size_t *bytes_written);

BinaryImageStatus binary_image_unpack(const uint8_t *packed,
                                      size_t pixel_count,
                                      uint8_t *pixels,
                                      size_t pixel_capacity);

#endif
