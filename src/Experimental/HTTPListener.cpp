//
// Created by Tuna Cici on 14.09.2023
//

#include <iostream>

/* Big thanks to https://github.com/yhirose/cpp-httplib */
#include "HTTPLib.hpp"

int
main(int argc, char** argv)
{
        httplib::Server svr;

        svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
                res.set_content("Hello World!", "text/plain");
        });

        std::cout << "Server is running on port 8080" << std::endl;

        svr.listen("0.0.0.0", 8080);
}
