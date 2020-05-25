#define ASIO_STANDALONE 1
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include "CapFile.hpp"
#include "FileSet.hpp"
#include "verify.hpp"
#include "Message.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> Client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

std::shared_ptr<ink::FileSet> fileSetPtr;

void on_open(Client *c, websocketpp::connection_hdl hdl) {
    c->send(hdl, fileSetPtr->greeting(), websocketpp::frame::opcode::binary);
}


void on_message(Client *c, websocketpp::connection_hdl hdl, message_ptr msg) {
    auto payload = msg->get_payload();
    auto one_size = payload.size();
    static auto total_size = 0;
    auto message = ink::Message(payload);
    total_size += one_size;
    std::cerr << "on_message called with hdl: " << hdl.lock().get()  << " and size: "
    << std::to_string(one_size) << " total: " << std::to_string(total_size) << std::endl;
    if (payload[1] == '\x08') {
        std::cerr << "received header"
        // << ink::escapes(payload)
        <<  std::endl;
    } else if (payload[1] == '\x01') {
        // std::cerr << "received transaction" << std::endl;
        VERIFY(fileSetPtr);
        fileSetPtr->receive(message);
    } else {
        throw std::runtime_error("unexpected message");
    }
}

void client_main(const std::string& uri = "ws://localhost:9002") {
    Client client;
    fileSetPtr = std::make_shared<ink::FileSet>("./db.ink");

    try {

        client.set_access_channels(websocketpp::log::alevel::none);

        // Set logging to be pretty verbose (everything except message payloads)
        // c.set_access_channels(websocketpp::log::alevel::all);
        // c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        client.init_asio();

        // Register our message handler
        client.set_message_handler(bind(&on_message, &client, ::_1, ::_2));
        client.set_open_handler(bind(&on_open, &client, ::_1));

        websocketpp::lib::error_code ec;
        Client::connection_ptr con = client.get_connection(uri, ec);
        if (ec) {
            std::cerr << "could not create connection because: " << ec.message() << std::endl;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        client.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        client.run();
    } catch (websocketpp::exception const &e) {
        std::cerr << e.what() << std::endl;
    }
}
