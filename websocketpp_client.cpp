#define ASIO_STANDALONE 1
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

void on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg) {
    static bool greeting_sent = false;
    auto payload = msg->get_payload();
    auto one_size = payload.size();
    static auto total_size = 0;
    total_size += one_size;
    std::cout << "on_message called with hdl: " << hdl.lock().get()  << " and size: "
    << std::to_string(one_size) << " total: " << std::to_string(total_size) << std::endl;

    if (not greeting_sent) {
        c->send(hdl, "\x92\x08\x80", websocketpp::frame::opcode::binary);
        greeting_sent = true;
    }
}

void client_main(std::string uri = "ws://localhost:9002") {
    // Create a client endpoint
    client c;

    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();
    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    }
}
