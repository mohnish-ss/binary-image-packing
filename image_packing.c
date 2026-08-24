#include "image_packing.h"

#include <limits.h>
#include <string.h>

size_t binary_image_packed_size(size_t pixel_count)
{
    return (pixel_count + (size_t)CHAR_BIT - 1U) / (size_t)CHAR_BIT;
}

static BinaryImageStatus validate_pixels(const uint8_t *pixels, size_t pixel_count)
{
    for (size_t index = 0; index < pixel_count; ++index)
    {
        if (pixels[index] != (uint8_t)'0' && pixels[index] != (uint8_t)'1')
        {
            return BINARY_IMAGE_INVALID_PIXEL;
        }
    }
    return BINARY_IMAGE_OK;
}

BinaryImageStatus binary_image_pack(const uint8_t *pixels,
                                    size_t pixel_count,
                                    uint8_t *packed,
                                    size_t packed_capacity,
                                    size_t *bytes_written)
{
    if (pixels == NULL || packed == NULL || bytes_written == NULL)
    {
        return BINARY_IMAGE_NULL_POINTER;
    }
    if (pixel_count == 0U || pixel_count > BINARY_IMAGE_MAX_PIXELS)
    {
        return BINARY_IMAGE_INVALID_SIZE;
    }

    const size_t required_bytes = binary_image_packed_size(pixel_count);
    if (packed_capacity < required_bytes)
    {
        return BINARY_IMAGE_INSUFFICIENT_CAPACITY;
    }

    const BinaryImageStatus validation = validate_pixels(pixels, pixel_count);
    if (validation != BINARY_IMAGE_OK)
    {
        return validation;
    }

    memset(packed, 0, required_bytes);

    for (size_t index = 0; index < pixel_count; ++index)
    {
        if (pixels[index] == (uint8_t)'1')
        {
            const size_t byte_index = index / (size_t)CHAR_BIT;
            const unsigned bit_index =
                (unsigned)((size_t)CHAR_BIT - 1U - index % (size_t)CHAR_BIT);
            packed[byte_index] |= (uint8_t)(UINT8_C(1) << bit_index);
        }
    }

    *bytes_written = required_bytes;
    return BINARY_IMAGE_OK;
}

BinaryImageStatus binary_image_unpack(const uint8_t *packed,
                                      size_t pixel_count,
                                      uint8_t *pixels,
                                      size_t pixel_capacity)
{
    if (packed == NULL || pixels == NULL)
    {
        return BINARY_IMAGE_NULL_POINTER;
    }
    if (pixel_count == 0U || pixel_count > BINARY_IMAGE_MAX_PIXELS)
    {
        return BINARY_IMAGE_INVALID_SIZE;
    }
    if (pixel_capacity < pixel_count)
    {
        return BINARY_IMAGE_INSUFFICIENT_CAPACITY;
    }

    for (size_t index = 0; index < pixel_count; ++index)
    {
        const size_t byte_index = index / (size_t)CHAR_BIT;
        const unsigned bit_index =
            (unsigned)((size_t)CHAR_BIT - 1U - index % (size_t)CHAR_BIT);
        pixels[index] =
            (packed[byte_index] & (uint8_t)(UINT8_C(1) << bit_index)) != 0U
                ? (uint8_t)'1'
                : (uint8_t)'0';
    }

    return BINARY_IMAGE_OK;
}
