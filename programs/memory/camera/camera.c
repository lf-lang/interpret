#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include <flexpret_noc.h>

// Image size = 640x480, based on the datasheet of OMNIVISION OV7251 B&W,
// the camera used on the Mars helicopter ingenuity.
#define WIDTH       640
#define HEIGHT      480
#define PIXEL_MAX   255
#define BATCH_SIZE  32      // Each batch contains 32 pixels
#define FPS_VGA     120     // FPS for capturing VGA

typedef uint8_t pixel_t;    // each pixel is 8-bit grayscale for B&W camera

int image_receiver();
int image_processor();

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);

    switch(core_id) {
        case 0: image_receiver(FPS_VGA, 2); break;
        case 1: image_receiver(FPS_VGA, 3); break;
        case 2: image_processor();  break;
        case 3: image_processor();  break;
        default: _fp_print(666); //ERROR
    }
}

/**
 * @brief A function simulating reading a frame from a B&W camera.
 * 
 * A 640x480 image requires 307200 pixels.
 * Storing them in batches of 32 pixels
 * (i.e., 256 bits = 1 DRAM memory request)
 * results in 9600 WRITE memory requests.
 * 
 * @param image A pointer to the image to be stored in the DRAM.
 */
void receive_image(pixel_t *image) {
    uint64_t *batch = (uint64_t *)image;

    // Assign dummy values to each pixel. For simplicity,
    // we'll use a fixed pattern: 150 for grayscale.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x += BATCH_SIZE) {
            uint64_t batch_pixels[BATCH_SIZE / 8] = {0};

            for (int b = 0; b < BATCH_SIZE; b++) {
                pixel_t pixel_value = 150; // Dummy grayscale value

                // Set the b-th pixel in the batch
                int batch_index = b / 8;
                int pixel_index = b % 8;
                batch_pixels[batch_index] |= (uint64_t)pixel_value << (pixel_index * 8);
            }

            // Store the batch of pixels
            for (int b = 0; b < BATCH_SIZE / 8; b++) {
                batch[(y * WIDTH + x) / 8 + b] = batch_pixels[b];
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
        receive_image(image);

        // Delay before next capture.
        uint32_t period = 1000000000LL / fps;
        delay_for(period);

        // Send the pointer to downstream_worker.
        noc_send(downstream_worker, (uint32_t)image, TIMEOUT_FOREVER);
    }
    return 0;
}

int image_processor() {
    while (1) {
        uint32_t *data;

        // Receive a pointer from an upstream receiver.
        noc_receive(data, TIMEOUT_FOREVER);

        // Process the image by inverting color.
        pixel_t *image = (pixel_t*)data;
        invert_color(image);

        // Free the image.
        free(image);
    }
    return 0;
}
