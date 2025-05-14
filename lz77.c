#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// LZ77 parameters - can be adjusted for different use cases
#define WINDOW_SIZE 4096     // Size of the sliding window
#define LOOKAHEAD_SIZE 32    // Size of the lookahead buffer
#define MIN_MATCH_LENGTH 3   // Minimum match length to encode

// Token format:
// - If length == 0: literal byte follows
// - If length > 0: (offset, length) pair represents a back-reference

typedef struct {
    uint16_t offset;   // Offset in the sliding window
    uint8_t length;    // Match length (0 for literal)
    uint8_t literal;   // Literal byte (used when length = 0)
} LZ77Token;

// Utility function to find the longest match in the window
static void find_longest_match(
    const uint8_t *data,
    size_t data_len,
    size_t current_pos,
    uint16_t *offset,
    uint8_t *length
) {
    *offset = 0;
    *length = 0;

    // If we don't have enough data for a minimum match, don't bother searching
    if (current_pos + MIN_MATCH_LENGTH > data_len) {
        return;
    }

    // Calculate the search boundaries
    size_t window_start = (current_pos > WINDOW_SIZE) ? (current_pos - WINDOW_SIZE) : 0;
    size_t max_look_ahead = (current_pos + LOOKAHEAD_SIZE < data_len) ? 
                            LOOKAHEAD_SIZE : (data_len - current_pos);

    // Search for the longest match in the window
    for (size_t i = window_start; i < current_pos; i++) {
        // Determine match length
        size_t j = 0;
        while (j < max_look_ahead && 
               data[current_pos + j] == data[i + j]) {
            j++;
        }

        // Update if we found a longer match
        if (j >= MIN_MATCH_LENGTH && j > *length) {
            *offset = (uint16_t)(current_pos - i);
            *length = (j > 255) ? 255 : (uint8_t)j; // Limit length to 255 (8 bits)
        }
    }
}

// Compresses the input data using LZ77 algorithm
// Returns the size of the compressed data
size_t lz77_compress(
    const uint8_t *input,
    size_t input_size,
    uint8_t *output,
    size_t output_capacity
) {
    if (!input || !output || input_size == 0 || output_capacity == 0) {
        return 0;
    }
	printf("[COMPRESS] Compressing %zu bytes\n", input_size);

    size_t in_pos = 0;       // Current position in input buffer
    size_t out_pos = 0;      // Current position in output buffer
    LZ77Token token;

    while (1) {
		printf("[COMPRESS] loop\n");
// in_pos < input_size && out_pos + 4 <= output_capacity
		if (!(in_pos < input_size)) {
			printf("[COMPRESS] Breaking1\n");
			break;
		}
		if (!(out_pos + 4 <= output_capacity)) {
			printf("[COMPRESS] Breaking2\n");
			break;
		}
        // Find the longest match at the current position
        find_longest_match(input, input_size, in_pos, &token.offset, &token.length);
		// printf("Found match at %zu, offset %u, length %u\n", in_pos, token.offset, token.length);

        // If no match found or match too short, encode a literal
        if (token.length < MIN_MATCH_LENGTH) {
            token.length = 0;
            token.literal = input[in_pos];
            in_pos++;
        } else {
            // Skip the matched bytes
            in_pos += token.length;
        }

		// abcabcdeabc
		// abc {off: -3, len: 3} de {off: -8, len: 6}

        // Encode the token
        // First byte indicates if it's literal (high bit 0) or match (high bit 1)
        if (token.length == 0) {
			printf("[COMPRESS] Found literal at %zu, literal %02X\n", in_pos, token.literal);
            // Literal: 0xxxxxxx
            output[out_pos++] = token.literal & 0x7F;
        } else {
			printf("[COMPRESS] Found match at %zu, offset %u, length %u\n", in_pos, token.offset, token.length);
            // Match: 1lllllll (length - MIN_MATCH_LENGTH + 1)
            output[out_pos++] = 0x80 | ((token.length - MIN_MATCH_LENGTH + 1) & 0x7F);
            
            // Encode offset (16 bits, big endian)
            output[out_pos++] = (token.offset >> 8) & 0xFF;
            output[out_pos++] = token.offset & 0xFF;
        }
    }

    // Return the size of the compressed data
    return out_pos;
}

// Decompresses the LZ77 compressed data
// Returns the size of the decompressed data
size_t lz77_decompress(
    const uint8_t *input,
    size_t input_size,
    uint8_t *output,
    size_t output_capacity
) {
    if (!input || !output || input_size == 0 || output_capacity == 0) {
        return 0;
    }
	printf("Decompressing %zu bytes\n", input_size);

    size_t in_pos = 0;       // Current position in input buffer
    size_t out_pos = 0;      // Current position in output buffer
    uint8_t token_header;
    uint16_t offset;
    uint8_t length;

    while (1) {
		printf("[DECOMPRESS] loop\n");
		if (!(in_pos < input_size)) {
			printf("[DECOMPRESS] Breaking1\n");
			break;
		}
		if (!(out_pos < output_capacity)) {
			printf("[DECOMPRESS] Breaking2\n");
			break;
		}
        // Read token header
        token_header = input[in_pos++];
        
        // Check if it's a literal or a match
        if ((token_header & 0x80) == 0) {
			printf("[DECOMPRESS] Found literal at %zu: %02X\n", in_pos - 1, token_header & 0x7F);
            // Literal
            output[out_pos++] = token_header & 0x7F;
        } else {
            // Match - decode length and offset
            if (in_pos + 1 >= input_size) {
				printf("[DECOMPRESS] Breaking3\n");
                break; // Not enough data for offset
            }
 
            length = (token_header & 0x7F) + MIN_MATCH_LENGTH - 1;
            offset = ((uint16_t)input[in_pos] << 8) | input[in_pos + 1];
            in_pos += 2;
            
            // Invalid offset
            if (offset == 0 || offset > out_pos) {
				printf("[DECOMPRESS] Breaking4\n");
                break;
            }

			printf("[DECOMPRESS] Found match at %zu\n", in_pos - 3);
            
            // Copy matched bytes
            for (int i = 0; i < length && out_pos < output_capacity; i++) {
                output[out_pos] = output[out_pos - offset];
                out_pos++;
            }
        }
    }

    // Return the size of the decompressed data
    return out_pos;
}

// Utility function to print a buffer in hex format for debugging
void print_buffer_hex(const uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

// Test function to demonstrate compression and decompression
void test_lz77(const char *test_data, size_t input_size) {
    const uint8_t *input = (const uint8_t *)test_data;
    
    // Allocate buffers with extra space to be safe
    size_t max_compressed_size = input_size * 2;
    uint8_t *compressed = (uint8_t *)malloc(max_compressed_size);
    uint8_t *decompressed = (uint8_t *)malloc(input_size * 2);
    
	memset(compressed, 0, max_compressed_size);
	memset(decompressed, 0, input_size * 2);

    if (!compressed || !decompressed) {
        printf("Memory allocation failed\n");
        free(compressed);
        free(decompressed);
        return;
    }
    
    // Compress the data
    size_t compressed_size = lz77_compress(input, input_size, compressed, max_compressed_size);
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0f - (float)compressed_size / input_size) * 100.0f);
    
    // Decompress the data
    size_t decompressed_size = lz77_decompress(compressed, compressed_size, decompressed, input_size * 2);
    printf("Decompressed size: %zu bytes\n", decompressed_size);
    
    // Verify the decompressed data matches the original
    int match = (decompressed_size == input_size && 
                 memcmp(input, decompressed, input_size) == 0);
    printf("Decompression successful: %s\n", match ? "YES" : "NO");
    
    // If they don't match, print the first few bytes of each
    if (!match) {
        printf("Original bytes: ");
        print_buffer_hex(input, input_size > 32 ? 32 : input_size);
        printf("Decompressed bytes: ");
        print_buffer_hex(decompressed, decompressed_size > 32 ? 32 : decompressed_size);
    }
    
    free(compressed);
    free(decompressed);
}

static void	test_lz77s(const char *test_data) {
	size_t input_size = strlen(test_data);
	test_lz77(test_data, input_size);
}

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Example usage
int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Test 1: Simple repeated pattern\n");
		test_lz77s("ABABABABABABABABABABABABABABABABABABABABAB");
		
		printf("\nTest 2: Text with repetition\n");
		test_lz77s("The quick brown fox jumps over the lazy dog. "
				  "The quick brown fox jumps over the lazy dog. "
				  "The quick brown fox jumps over the lazy dog.");
		
		printf("\nTest 3: Random data with some repetition\n");
		test_lz77s("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"
				  "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*");
		return 0;
	}
	if (argc > 2) {
		for (int i = 2; i < argc; i++) {
			printf("Testing %s\n", argv[i]);
			test_lz77(argv[i], strlen(argv[i]));
		}
		return 0;
	}

	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		printf("Failed to open file: %s\n", argv[1]);
		return 1;
	}

	struct stat st;
	fstat(fd, &st);

	char* buffer = (char*)malloc(st.st_size);
	if (!buffer) {
		printf("Failed to allocate memory for file: %s\n", argv[1]);
		return 1;
	}
	read(fd, buffer, st.st_size);

	close(fd);

	test_lz77(buffer, st.st_size);

	free(buffer); 
    return 0;
}
