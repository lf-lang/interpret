#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_noc.h>

// Image size = 13 MB, based on the datasheet of SONY IMX214,
// the camera used on the Mars helicopter ingenuity.
#define WIDTH       4224
#define HEIGHT      3200
#define PIXEL_MAX   255

typedef unsigned short pixel_t; // Assuming 16-bit pixel depth.

int image_receiver();
int image_processor();

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);

    switch(core_id) {
        case 0: image_receiver(30, 2); break;
        case 1: image_receiver(60, 3); break;
        case 2: image_processor();  break;
        case 3: image_processor();  break;
        default: _fp_print(666); //ERROR
    }
}

/**
 * @brief A function simulating reading a frame from a camera.
 * 
 * @param image 
 */
void read_image(pixel_t *image) {
    // Assign dummy values to each pixel. For simplicity,
    // we'll use a fixed pattern: 100 for R, 200 for G, and 150 for B.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            pixel_t *pixel = image + (y * WIDTH + x);  // Compute the address of the pixel in the one-dimensional array.

            if ((x % 2) == (y % 2)) {
                // Green pixel
                *pixel = 200;
            } else if (x % 2) {
                // Red pixel
                *pixel = 100;
            } else {
                // Blue pixel
                *pixel = 150;
            }
        }
    }
}

/**
 * @brief A function simulating processing a frame from a camera.
 * 
 * @param image 
 */
void invert_color(pixel_t *image) {
    // Assign dummy values to each pixel. For simplicity,
    // we'll use a fixed pattern: 100 for R, 200 for G, and 150 for B.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            pixel_t *pixel = image + (y * WIDTH + x);  // Compute the address of the pixel in the one-dimensional array.
            // Invert the value of each pixel.
            *pixel = PIXEL_MAX - *pixel;
        }
    }
}

int image_receiver(uint32_t fps, uint32_t downstream_worker) {
    while (1) {
        // Allocate the memory for an image.
        pixel_t *image = malloc(WIDTH * HEIGHT * sizeof(pixel_t));
        if (!image) {
            _fp_print(666);
            return 1;
        }

        // Fill the image with a dummy Bayer pattern.
        read_image(image);

        // Delay before next capture.
        uint32_t period = 1000000000LL / fps;
        delay_for(period);

        // Send the pointer to core 2.
        noc_send(downstream_worker, (uint32_t)image, TIMEOUT_FOREVER);
    }
    return 0;
}

int image_processor() {
    while (1) {
        uint32_t *data;

        // Receive a pointer from core 0 and process the image.
        noc_receive(data, TIMEOUT_FOREVER);

        // ... Process the image.
        pixel_t *image = (pixel_t*) data;
        invert_color(image);

        // Free the image.
        free(image);
    }
    return 0;
}
