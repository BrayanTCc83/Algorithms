#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/global.h"
#include "../headers/sort.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_integers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "Invalid number of integers: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    time_t start, end;

    int* numbers = read_fixed_integers(count);
    if (!numbers) {
        fprintf(stderr, "Insuficient memory to reserve for %d numbers.", count);
        return EXIT_FAILURE;
    }

    start = current_millis();
    cocktail_shaker_sort(count, numbers);
    end = current_millis();

    free(numbers);
    printf("%lld %lld\n", start, end);
    return EXIT_SUCCESS;
}