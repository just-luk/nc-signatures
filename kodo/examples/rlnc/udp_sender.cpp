// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
//
// Licensees holding a valid commercial license may use this project
// in accordance with the standard license agreement terms provided
// with the Software (see accompanying file LICENSE.rst or
// https://www.steinwurf.com/license), unless otherwise different
// terms and conditions are agreed in writing between Licensee and
// Steinwurf ApS in which case the license will be regulated by that
// separate written agreement.
//
// License for Non-Commercial Usage
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <kodo/finite_field.hpp>

#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>

const char* server_port = "8080";

/// Helper function to send data to a udp socket
int send_to_udp_socket(const std::vector<uint8_t>& data)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // Get the server address, we will use the loopback address (localhost)
    if ((rv = getaddrinfo("127.0.0.1", server_port, &hints, &servinfo)))
    {
        std::cout << stderr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return 1;
    }

    // Setup the socket at the given address
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
        {
            std::cout << "talker: socket" << std::endl
                      << strerror(sockfd) << std::endl;

            continue;
        }

        break;
    }

    // Send the data
    int bytes_sent =
        sendto(sockfd, data.data(), data.size(), 0, p->ai_addr, p->ai_addrlen);

    if (bytes_sent == -1)
    {
        std::cout << "Failed to send data: " << strerror(errno) << std::endl;
        exit(2);
    }
    else if ((std::size_t)bytes_sent != data.size())
    {
        std::cout << "Failed to send all data: " << strerror(errno)
                  << std::endl;
        exit(3);
    }

    std::cout << "Sent " << bytes_sent << " bytes" << std::endl;

    // Close the socket and free the address
    freeaddrinfo(servinfo);
    close(sockfd);

    // Give the receiver time to receive the data
    usleep(10000);

    return 0;
}

/// @example rlnc/udp_sender.cpp
///
/// This example shows how to setup a UDP socket, encode data with a
/// kodo::block::encoder and send the data over the socket. Currently only
/// usable on Linux.
int main()
{
    // We will pack the header of the packets with info that is necessary at the
    // decoder, it will contain a bool that indicates if the packet is
    // systematic, i.e an uncoded symbol (source symbol) and a uint32_t that
    // indicates one of two things:
    //
    // 1. The index of the packet if it is systematic.
    // 2. The seed of the coefficients used in the encoding if it is encoded
    // (i.e not systematic).

    bool is_systematic;

    uint32_t seed;

    std::size_t header_size = sizeof(bool) + sizeof(uint32_t);

    // We will send a 100 blocks of 15 symbols each. Make sure that this equal
    // to the number of blocks to receive in udp_receiver.cpp
    std::size_t blocks_to_send = 100;

    for (std::size_t i = 0; i < blocks_to_send; ++i)
    {

        std::cout << "Sending Block number " << i + 1 << std::endl;
        // Seed the random number generator to get different random data
        srand(static_cast<uint32_t>(time(0)));

        // Pick the finite field to use for the encoding and decoding.
        auto field = kodo::finite_field::binary8;

        // Create an encoder and decoder.
        kodo::block::encoder encoder(field);

        // Pick the number of symbols to encode/decode.
        uint32_t symbols = 10;

        // Pick the number of encoded symbols to produce.
        uint32_t encoded_symbols = 5;

        // Pick the size of each symbol in bytes
        auto symbol_bytes = 1400 - header_size;

        // Configure the coders, the encoder and decoder must be configured
        // identically to be compatible.
        encoder.configure(symbols, symbol_bytes);

        // Create generator generator.
        kodo::block::generator::random_uniform generator(field);

        // The generator must similarly be configured based on the
        // encoder/decoder.
        generator.configure(encoder.symbols());

        // Allocate some storage for a symbol.
        std::vector<uint8_t> symbol(encoder.symbol_bytes() + header_size);

        // Allocate some storage for the coefficients.
        std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

        // Allocate some data to encode. In this case we make a buffer
        // with the same size as the encoder's block size (the max.
        // amount a single encoder can encode)
        std::vector<uint8_t> data_in(encoder.block_bytes());

        // Just for fun - fill data_in with random data
        std::generate(data_in.begin(), data_in.end(), rand);

        // Assign the data buffer to the encoder so that we may start
        // to produce encoded symbols from it
        encoder.set_symbols_storage(data_in.data());

        // Keep track of the systematic symbols
        uint32_t systematic_index = 0;

        // Keep track of the encoded symbols
        uint32_t encoded_symbols_count = 0;

        while (systematic_index + encoded_symbols_count <
               symbols + encoded_symbols)
        {
            // Send the systematic symbols first.
            if (encoder.rank() > systematic_index)
            {
                is_systematic = true;

                // Pack the header of the packet.
                memcpy(symbol.data(), &is_systematic, sizeof(bool));
                memcpy(symbol.data() + sizeof(bool), &systematic_index,
                       sizeof(uint32_t));

                std::cout << "Systematic Symbol" << std::endl;

                // Add the symbol data to the symbol buffer
                encoder.encode_systematic_symbol(symbol.data() + sizeof(bool) +
                                                     sizeof(uint32_t),
                                                 systematic_index);

                // Send the symbol
                send_to_udp_socket(symbol);

                systematic_index += 1;
            }
            // Then the encoded symbols.
            else
            {
                // The RLNC code is rateless, which means, in theory, that it
                // can generate an infinite number of coded packets.
                std::cout << "Encoded Symbol" << std::endl;

                // This is not a systematic symbol.
                is_systematic = false;

                // Draw a seed and seed the coefficients generator.
                seed = rand();
                generator.set_seed(seed);

                // Pack the header of the packet.
                memcpy(symbol.data(), &is_systematic, sizeof(bool));
                memcpy(symbol.data() + sizeof(bool), &seed, sizeof(uint32_t));

                // Generate the coefficients into the symbol buffer
                generator.generate(coefficients.data());

                // Encode a symbol into the symbol buffer (don't encode the
                // header)
                encoder.encode_symbol(symbol.data() + sizeof(bool) +
                                          sizeof(uint32_t),
                                      coefficients.data());

                // Send the symbol
                send_to_udp_socket(symbol);

                encoded_symbols_count += 1;
            }
        }
    }

    std::cout << "Sent all " << blocks_to_send << " blocks" << std::endl;

    return 0;
}