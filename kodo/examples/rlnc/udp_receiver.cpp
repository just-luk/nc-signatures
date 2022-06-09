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
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <kodo/finite_field.hpp>

#include <kodo/block/decoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>

const char* my_port = "8080";

#define MAXBUFLEN 1400

// helper function to get the address that was sent from
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// helper function to receive data from a udp socket
std::vector<uint8_t> receive_from_udp_socket()
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    struct sockaddr_storage their_addr;
    int numbytes;

    // Create the buffer to store the data
    std::vector<uint8_t> buf(MAXBUFLEN);
    socklen_t addr_len;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    // Listen on any address, port 8080
    if ((rv = getaddrinfo(nullptr, my_port, &hints, &servinfo)) != 0)
    {
        std::cout << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        exit(1);
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1)
        {
            std::cout << "receive_from_udp_socket: socket" << std::endl
                      << strerror(sockfd) << std::endl;
            continue;
        }

        if (int errorcode = bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            std::cout << "receive_from_udp_socket: bind" << std::endl
                      << strerror(errorcode) << std::endl;
            continue;
        }

        break;
    }

    // if we enter this if statement, it means we didn't get bound
    if (p == nullptr)
    {
        std::cout << "receive_from_udp_socket: failed to bind socket"
                  << std::endl;
        exit(2);
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(servinfo);

    addr_len = sizeof(their_addr);
    // Receive data
    if ((numbytes = recvfrom(sockfd, buf.data(), MAXBUFLEN, 0,
                             (struct sockaddr*)&their_addr, &addr_len)) == -1)
    {
        std::cout << "recvfrom: failed to receive bytes" << std::endl
                  << strerror(numbytes) << std::endl;
        exit(1);
    }

    // Close the socket
    close(sockfd);

    // Return the received data
    return buf;
}

/// @example rlnc/udp_receiver.cpp
///
/// This example shows how to listen on a UDP socket, receive data, and decode
/// the data with the kodo::block::decoder. Currently only usable on Linux.
int main()
{
    // We will send 100 blocks of 15 symbols.
    std::size_t blocks_to_send = 100;

    // The packet headers contain a bool that indicates if the packet is
    // systematic (source symbol, uncoded) or not, and a uint32_t that either
    // indicates:
    //
    // 1. The index of the packet if it is systematic.
    // 2. The seed of the coefficients used in the encoding if it is encoded
    // (i.e not systematic).
    const std::size_t header_size = sizeof(bool) + sizeof(uint32_t);

    for (std::size_t i = 0; i < blocks_to_send; ++i)
    {

        std::cout << "Receiving Block number " << i + 1 << std::endl;
        // Create a new socket
        // Seed the random number generator to get different random data
        srand(static_cast<uint32_t>(time(0)));

        // Pick the finite field to use for the encoding and decoding.
        auto field = kodo::finite_field::binary8;

        // Create an encoder and decoder.
        kodo::block::decoder decoder(field);

        // Pick the number of symbols to encode/decode.
        auto symbols = 10;

        // Pick the size of each symbol in bytes
        auto symbol_bytes = 1400 - header_size;

        // Configure the coders, the encoder and decoder must be configured
        // identically to be compatible.
        decoder.configure(symbols, symbol_bytes);

        // Create generator generator.
        kodo::block::generator::random_uniform generator(field);

        // The generator must similarly be configured based on the
        // encoder/decoder.
        generator.configure(decoder.symbols());

        // Allocate some storage for the coefficients.
        std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

        // Define a data buffer where the symbols should be decoded
        std::vector<uint8_t> data_out(decoder.block_bytes());
        decoder.set_symbols_storage(data_out.data());

        // Lose packets with 10% probability
        auto loss_propability = 10;

        // We will receive 15 packets, make sure that this number is equal to
        // symbols + encoded symbols in udp_sender.cpp
        std::size_t packets_to_receive = 15;

        // No packets from this block have been received yet
        std::size_t packets = 0;

        // Stop early if the decoder is complete
        while (!decoder.is_complete())
        {
            // If the sender starts sending a new block, we have failed to
            // decode. The program terminates
            if (packets == packets_to_receive)
            {
                std::cout
                    << "Failed to decode the data. Perhaps up the number of "
                       "encoded symbols or lower the loss_probability"
                    << std::endl;
                return 1;
            }

            // Receive a packet from the sender
            std::vector<uint8_t> packet = receive_from_udp_socket();

            // increment the number of packets received for this block
            packets += 1;

            // If the packet is uncoded (source symbol) this bool will be true,
            // otherwise false.
            bool is_systematic = packet[0];

            if (is_systematic)
            {
                // Apply some packet loss, will ignore the packet if it is
                // 'lost'
                if (rand() % 100 < loss_propability)
                {
                    std::cout << "Lost packet" << std::endl;
                    continue;
                }
                std::cout << "Systematic packet" << std::endl;

                // Unpack the index from the header.
                uint32_t index =
                    *reinterpret_cast<uint32_t*>(packet.data() + sizeof(bool));

                // Decode the packet
                decoder.decode_systematic_symbol(packet.data() + header_size,
                                                 index);
            }
            // If the packet is encoded (non-source symbol)
            else
            {
                // Apply some packet loss, will ignore the packet if it is
                // 'lost'
                if (rand() % 100 < loss_propability)
                {
                    std::cout << "Lost packet" << std::endl;
                    continue;
                }

                std::cout << "Encoded packet" << std::endl;

                // Unpack the seed from the header.
                uint32_t seed =
                    *reinterpret_cast<uint32_t*>(packet.data() + sizeof(bool));

                // Set the seed of the coefficients generator
                generator.set_seed(seed);

                // Generate the coefficients into the coefficients buffer
                generator.generate(coefficients.data());

                // Decode the packet
                decoder.decode_symbol(packet.data() + header_size,
                                      coefficients.data());
            }
        }

        // Just for fun, print out if we sent too many extra packets.
        std::size_t unnecessary_symbols = packets_to_receive - packets;

        for (std::size_t i = 0; i < unnecessary_symbols; ++i)
        {
            std::vector<uint8_t> packet = receive_from_udp_socket();
        }

        std::cout << "Decoding of block finished" << std::endl
                  << "Unnecessary symbols: " << unnecessary_symbols
                  << std::endl;
    }

    std::cout << "Received all " << blocks_to_send << " blocks" << std::endl;

    return 0;
}
