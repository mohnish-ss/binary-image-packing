# Binary Image Packing Library

A C library for packing one-byte-per-pixel binary image data into a one-bit-per-pixel representation. It uses bit masks and shifts to reduce storage by up to 8x and provides the inverse operation for reconstructing the original pixel bytes.

## Technical Highlights

- Packs ASCII `'0'` and `'1'` pixels in most-significant-bit-first order.
- Emits the final partial byte even when every remaining pixel is zero.
- Unpacks the representation back to the original byte-per-pixel format.
- Validates pointers, input length, pixel values, packed-buffer capacity, and unpacking destination capacity.
- Exercises boundary lengths, bit order, malformed input, maximum-size input, and round-trip behavior.

## Architecture

`image_packing.c` contains the capacity-aware packing and unpacking operations. `image_packing.h` defines the public API, size limits, and result codes. `tests/test_image_packing.c` provides assertion-based round-trip and validation tests. The default build produces the static library `libimagepacking.a`.

## Building

```bash
make
```

## Running

This repository provides a library rather than a standalone application. Include `image_packing.h` and link against `libimagepacking.a` from a C program. The included executable flow is the test suite documented below.

## Testing

Run the standard tests:

```bash
make test
```

Run the same test suite with AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
make asan
```

Remove generated files with `make clean`.

## Technical Concepts

- C
- Bit manipulation
- Bit masks and shifts
- Packed memory representation
- Bounds validation
- Memory efficiency
- Round-trip testing
- AddressSanitizer
- UndefinedBehaviorSanitizer
