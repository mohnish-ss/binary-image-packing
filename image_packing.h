#ifndef IMAGE_PACKING_H
#define IMAGE_PACKING_H

#include <stddef.h> // size_t for pixel counts and buffer sizes.
#include <stdint.h> // uint8_t for individual bytes.

// Support images with up to 4096 pixels, requiring at most 512 packed bytes.
#define BINARY_IMAGE_MAX_PIXELS 4096U
#define BINARY_IMAGE_MAX_BYTES ((BINARY_IMAGE_MAX_PIXELS + 7U) / 8U)

// Packing and unpacking return one of these result codes.
typedef enum
{
    BINARY_IMAGE_OK = 0,
    BINARY_IMAGE_INVALID_SIZE = -1,          // Pixel count is zero or exceeds the limit.
    BINARY_IMAGE_INVALID_PIXEL = -2,         // A pixel is neither '0' nor '1'.
    BINARY_IMAGE_NULL_POINTER = -3,          // A required pointer is NULL.
    BINARY_IMAGE_INSUFFICIENT_CAPACITY = -4  // The output buffer is too small.
} BinaryImageStatus;

// Return the number of bytes needed, rounding up for a partial final byte.
// This helper calculates storage size; it does not validate the pixel count.
size_t binary_image_packed_size(size_t pixel_count);

// Pack pixel_count character bytes ('0' or '1') into bits, highest bit first.
// packed_capacity is the size of the output buffer in bytes.
// Unused bits in the last byte are zero. On success, bytes_written receives
// the packed length. On error, packed and bytes_written remain unchanged.
// Input and output buffers must not overlap, and all pointers must be non-NULL.
BinaryImageStatus binary_image_pack(const uint8_t *pixels,
                                    size_t pixel_count,
                                    uint8_t *packed,
                                    size_t packed_capacity,
                                    size_t *bytes_written);

// Restore pixel_count character bytes ('0' or '1') from the packed bits.
// packed must contain at least binary_image_packed_size(pixel_count) bytes.
// pixel_capacity is the output buffer size and must be at least pixel_count.
// The output is not null-terminated. On error, pixels remains unchanged.
// Input and output buffers must not overlap, and both pointers must be non-NULL.
BinaryImageStatus binary_image_unpack(const uint8_t *packed,
                                      size_t pixel_count,
                                      uint8_t *pixels,
                                      size_t pixel_capacity);

#endif
