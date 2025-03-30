# webserver

![GitHub repo size](https://img.shields.io/github/repo-size/redadoo/webserver)
![GitHub contributors](https://img.shields.io/github/contributors/redadoo/webserver)
![GitHub stars](https://img.shields.io/github/stars/redadoo/webserver?style=social)
![GitHub forks](https://img.shields.io/github/forks/redadoo/webserver?style=social)

## Table of Contents

- [About](#about)
- [Usage](#usage)
- [Ressources](#ressources)

## About

This project involves developing a simple HTTP web server from scratch in C++, compatible with the C++98 standard. The server is designed to handle HTTP requests, manage multiple client connections, and serve static web pages, utilizing efficient I/O mechanisms such as `select`, `poll`, and `epoll` for asynchronous processing. Additionally, the server implements a logging system that records all events, providing valuable insights into its operations. This project is part of the core curriculum at 42 and was created in collaboration with [@Fborroto](https://github.com/Fborroto) and [@ffursini](https://github.com/ffursini).

## Usage

1. Clone this repository:

   ```sh
   git clone https://github.com/redadoo/webserver.git
   ```
2. Navigate into the project directory:
   ```sh
   cd webserver/
   ```
3. Build and start the Docker containers:
   ```sh
   make run
   ```
4. Visit site on:
   [site](http://127.0.0.1:8080/)

5. test webserver with command like :
   ```sh
   siege -b -t 30s http://127.0.0.1:8080/
   ```

## ressources

[How nginx processes a request](http://nginx.org/en/docs/http/request_processing.html)

[How nginx server names works](http://nginx.org/en/docs/http/server_names.html)

[how nginx syntax works](http://nginx.org/en/docs/beginners_guide.html)

[I/O Multiplexing (select vs. poll vs. epoll/kqueue)](https://nima101.github.io/io_multiplexing)

[How to handle socket file asynchronously with epoll (in this case as TCP socket server).](https://stackoverflow.com/questions/66916835/c-confused-by-epoll-and-socket-fd-on-linux-systems-and-async-threads)

[Building a multi-client chat server with select and epoll](https://mecha-mind.medium.com/a-non-threaded-chat-server-in-c-53dadab8e8f3)

[Epoll: does it silently remove fds](https://stackoverflow.com/questions/46987302/epoll-does-it-silently-remove-fds)

[The method to epoll’s madness](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)

[What is the meaning of SO_REUSEADDR](https://stackoverflow.com/questions/3229860/what-is-the-meaning-of-so-reuseaddr-setsockopt-option-linux)

[HTTP Messages](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)
