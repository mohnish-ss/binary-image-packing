#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "image_packing.h"

static void assert_round_trip(const char *pixels)
{
    uint8_t packed[BINARY_IMAGE_MAX_BYTES];
    uint8_t unpacked[BINARY_IMAGE_MAX_PIXELS];
    const size_t pixel_count = strlen(pixels);
    size_t bytes_written = 0U;

    assert(binary_image_pack((const uint8_t *)pixels,
                             pixel_count,
                             packed,
                             sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_OK);
    assert(bytes_written == binary_image_packed_size(pixel_count));
    assert(binary_image_unpack(packed, pixel_count, unpacked, sizeof(unpacked)) ==
           BINARY_IMAGE_OK);
    assert(memcmp(unpacked, pixels, pixel_count) == 0);
}

static void test_boundaries_and_bit_order(void)
{
    uint8_t packed[BINARY_IMAGE_MAX_BYTES];
    size_t bytes_written = 0U;

    assert_round_trip("0");
    assert_round_trip("0000000");
    assert_round_trip("00000000");
    assert_round_trip("000000000");
    assert_round_trip("111111111111111");
    assert_round_trip("1010101010101010");

    assert(binary_image_pack((const uint8_t *)"10101010",
                             8U,
                             packed,
                             sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_OK);
    assert(bytes_written == 1U);
    assert(packed[0] == 0xAAu);

    assert(binary_image_pack((const uint8_t *)"000000000",
                             9U,
                             packed,
                             sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_OK);
    assert(bytes_written == 2U);
    assert(packed[0] == 0x00u);
    assert(packed[1] == 0x00u);
}

static void test_validation(void)
{
    uint8_t packed[BINARY_IMAGE_MAX_BYTES] = {0};
    uint8_t unpacked[BINARY_IMAGE_MAX_PIXELS];
    size_t bytes_written = 0U;

    assert(binary_image_pack(NULL, 1U, packed, sizeof(packed), &bytes_written) ==
           BINARY_IMAGE_NULL_POINTER);
    assert(binary_image_pack((const uint8_t *)"0", 1U, NULL, 1U, &bytes_written) ==
           BINARY_IMAGE_NULL_POINTER);
    assert(binary_image_pack((const uint8_t *)"0", 1U, packed, sizeof(packed), NULL) ==
           BINARY_IMAGE_NULL_POINTER);
    assert(binary_image_pack((const uint8_t *)"", 0U, packed, sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_INVALID_SIZE);
    assert(binary_image_pack((const uint8_t *)"2", 1U, packed, sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_INVALID_PIXEL);
    assert(binary_image_pack((const uint8_t *)"000000000", 9U, packed, 1U,
                             &bytes_written) == BINARY_IMAGE_INSUFFICIENT_CAPACITY);
    assert(binary_image_unpack(packed, 1U, unpacked, 0U) ==
           BINARY_IMAGE_INSUFFICIENT_CAPACITY);
    assert(binary_image_unpack(NULL, 1U, unpacked, sizeof(unpacked)) ==
           BINARY_IMAGE_NULL_POINTER);
}

static void test_maximum_image(void)
{
    uint8_t pixels[BINARY_IMAGE_MAX_PIXELS];
    uint8_t packed[BINARY_IMAGE_MAX_BYTES];
    uint8_t unpacked[BINARY_IMAGE_MAX_PIXELS];
    size_t bytes_written = 0U;

    memset(pixels, '1', sizeof(pixels));
    assert(binary_image_pack(pixels,
                             sizeof(pixels),
                             packed,
                             sizeof(packed),
                             &bytes_written) == BINARY_IMAGE_OK);
    assert(bytes_written == BINARY_IMAGE_MAX_BYTES);
    assert(binary_image_unpack(packed, sizeof(pixels), unpacked, sizeof(unpacked)) ==
           BINARY_IMAGE_OK);
    assert(memcmp(unpacked, pixels, sizeof(pixels)) == 0);
}

int main(void)
{
    test_boundaries_and_bit_order();
    test_validation();
    test_maximum_image();
    puts("All image packing tests passed.");
    return 0;
}
