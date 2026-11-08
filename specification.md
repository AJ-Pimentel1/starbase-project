# Starbase 1337

## Introduction

In the year 2260, aboard the renowned Starbase 1337, you serve as a skilled engineer responsible for maintaining the backbone of interstellar commerce and navigation. Recently, an unforeseen encounter with a mysterious space anomaly – speculated to be the mischievous meddling of the [enigmatic Q](https://en.wikipedia.org/wiki/Q_(Star_Trek)) – has resulted in a catastrophic system failure. Many programs, including the central navigation program, crucial for plotting safe and efficient starship routes, has been erased, leaving only the raw map data in binary files.

Your urgent mission is to rebuild the Space Travel And Routing System (STARS) within \___ weeks. The clock is ticking as the galaxy teeters on the brink of chaos; without the STARS, the intricate network of galactic trade and travel faces imminent collapse. If the system isn't restored before galactic trade breaks down for more than \___ weeks, the consequences will be dire: critical supply lines to remote colonies will be severed, diplomatic missions will grind to a halt, and emergency response capabilities will be severely hampered.

Luckily, the documentation on the functionality of the program before the crash is still intact, and has a public [protocol API](#communication-protocol). You must rebuild the STARS server and provide a client for incoming ships to use. However, some travelers will still prefer to use their own client from before the crash. Because of this, your server must match the protocol exactly, any deviation could result in loss of goods and potentially life.

## Objectives

This project is primarily meant to measure the students Knowledge, Skills, and Abilities in:

- Multithreading
- Serialization
- Graph Theory
- Cryptography

## Requirements

The trainee has ___ business days to create a Server-Client program pair. The program must be able to run on Ubuntu 22.04 LTS without any external dependencies.

The Server:
- Will be written in C
- Adhere to the Barr-C Styleguide
- Will rely on the `openssl` library (`libssl-dev`) for any encryption/hashing mechanisms.
- Will rely on the `pthread` library for multithreading

The Client:
- Will be written in Python 3.10
- Adhere to the Google Python Styleguide
- Will rely on the python ssl library (`ssl`) for any encryption mechanisms

### User Manual

The User Manual must include at a minimum:

- Project compilation
- How to start the server
- How to communicate with the server from the client
- How to interact with the client
- Known issues

### Server

1. The server shall be designed to handle incoming messages and manage route requests in real-time.
2. It shall manage an interstellar map with locations and links between locations as well as keep track of users.
3. Routes will be calculated using a [shortest path algorithm](https://en.wikipedia.org/wiki/Shortest_path_problem).
4. Runs with `./bin/stars [options]`
5. Shuts down gracefully when `Ctrl+C` (SIGINT) is delivered. Prompting the user if they want to update the `map` and `users.csv` file before exiting, save them as a new file(s), or not save at all.
6. User credentials stored in the `users.csv` file will use SHA512 for hashing.
7. Compiles without warnings or errors with: `-Wall -Werror`
8. Can be configured with either IPv4 or IPv6 for communication
9. All serialized data (network traffic and binary file data) is assumed to be big-endian
10. The server shall manage and serve multiple connections at once.
11. Support `make` targets: `all`, `program`, `clean`, `test`, `docs`, `setup`
    - `all`: (Default) compiles `./bin/stars` 
    - `program`: compiles the program
    - `clean`: removes all compiled objects (i.e. `.o` files)
    - `test`: runs unit/functional tests
    - `docs`: produces any automatically generated documentation
    - `setup`: creates any necessary directories and creates a self-signed certificate/private key for the server (if needed)
12. Session IDs will be used to keep track of valid user sessions. The session id will timeout after 10 minutes of inactivity.

#### Server Startup flags

| Flag  | Description                                | Example               | Default Value     |
|-------|--------------------------------------------|-----------------------|-------------------|
| `-m`  | Path to the map file                       | `-m /path/to/map`     | `./maps/large_map`| 
| `-p`  | Port number to listen for connections      | `-p 8080`             | `8000`            |
| `-h`  | Prints a short usage text                  | `-h`                  | N/A               |
| `-u`  | Path to user file                          | `-u /path/to/userfile`| `./users.csv` |
| `-6`  | Listen with an IPv6 socket                 | `-6`                  | N/A              |


##### Notes

- The format of the `map` file can be found in [The map format section](#map-format)
- The format of the `users` file should be a .csv of the format `username,password_hash,is_admin`, where permission level is `1` for admin or `0` for user. Hashes will be based on the [SHA512sum](https://www.openssl.org/docs/man1.1.1/man3/SHA512.html). A newline will be used as the delimiter between entries, see [users.csv](users.csv) for an example.
- If the `users` file doesn't exist, the server will startup with the default admin login of `admin`:`password`.
- The server will use IPv4 by default if the `-6` flag is not specified

### Client

The client shall allow the following commands and send the appropriate packet:

| Command       | Description                                   | Usage                                  | Example                        |
|---------------|-----------------------------------------------|----------------------------------------|--------------------------------|
| `login`       | Authenticate a user against the server.       | `login <username>`    | `login picard` (followed by a password prompt) |
| `register`    | Register as a new user | `register <username>` | `register kirk` (Followed by password prompt for the new user's password) |
| `admin_add`    | Manually create a new admin user (admin only command) | `user_add <username>` | `admin_add spock` (Followed by password prompt for the new user's password) |
| `user_del`    | Delete a user/admin. (admin only command) | `user_del <username>`                  | `user_del kirk` |
| `user_pass`    | change the password of a user. users can only change their own passwords. Admins can change anyone's password. Including other admins | `user_pass <username>` (Followed by a password prompt for the new password) | `user_pass picard` |
| `list`        | List all available locations for routing. Or given a location, list the locations that can be traveled directly from there. Case sensitive  | `list <optional: location>` | `list` or `list Earth` |
| `search`      | search for all locations that **contain** the provided string, Case sensitive | `search <string>` | `search Beta` (may return "Beta Thoridar" and "Betazed") |
| `route`       | request the shortest route between two locations. | `route <source> <destination>` | `route Earth Vulcan` |
| `link_add`    | add a new link between two locations (admin only).     | `link_add <start> <dest> <weight>` | `link_add Earth Vulcan 2` |
| `link_del`    | remove a link between two locations (admin only).     | `link_del <start> <dest> <weight>` | `link_del Earth Vulcan 2` |
| `location_add`| Add a location (admin only).     | `location_add <name>`               | `location_add Romulus` |
| `location_del`| Delete a location (admin only).     | `location_del <name>`               | `location_del Romulus` |
| `help` | Get help. optionally get help for a specific command.     | `help <optional: command>`               | `help` or `help link_add` |
| `exit` | disconnect and close the program.     | `exit`               | `exit` |

The client shall provide appropriate feedback to the user based on the return code in the response packet. For example, a success or appropriate failure message.

Notes:

- `weight` fields are calculated as light years. Larger values mean longer distances. A weight of `0` is allowed. Negative weights are not allowed.
- All links are directional from source to destination.
- Admins **CAN** delete other admins.
- There CAN only be one link between nodes, if a new link is added, only keep the shortest of the two.
- Names of locations are alphanumeric + punctuation + spaces (1 to 32 characters in length) (i.e `Qo'nos` or `Mintaka III`).
- Usernames are alphanumeric (1 to 32 characters in length)
- Passwords are alphanumeric + punctuation + spaces (1 to 32 characters in length)
- When entering passwords, the client will prompt the user for a password [**without** echoing their password to the screen](https://docs.python.org/3/library/getpass.html)
- A succesful `route` request should provide the user with all of the stops and the total distance. For example to go from "Earth" to "Risa" using the "small_map" the client may display something like: `Earth -> Vulcan -> Risa; Total Distance: 6 light years`
- If a location is deleted and then re-added it does not recover it's previous links, locations are created with no links until they are added


#### Client Startup flags

| Flag | Description                                 | Example                 | Default Value         |
|------|---------------------------------------------|-------------------------|-----------------------|
| `-i` | IP address of the server                  | `-i 127.0.0.1`          | `127.0.0.1` (localhost)|
| `-p` | Port number the server is listening on      | `-p 1337`               | `1337`                |
| `-6` | Use IPv6 for communication                  | `-6`                    | N/A                  |

- IPv6 Example: `./src/client.py -i fe80::1ff:fe23:4567:890a -p 1234 -6`
- IPv4 Example: `./src/client.py -i 127.0.0.1 -p 1234`
- The client will default to IPv4 if the `-6` flag is not specified

Each command corresponds to specific functionality that the client can request from the server. The client interface should be designed to be user-friendly and intuitive, allowing for easy navigation and execution of these commands. The feedback mechanism should clearly inform the user of the outcomes of their actions, including successful operations, errors, or any required further input. 

### Directory Structure

```
.
|-- bin/                      (Compiled binaries)
|   `-- stars                 (Compiled server program)
|-- docs/                     (Documentation)
|-- include/                  (Public headers)
|-- maps/                     (STARS map files)
|-- Makefile                  (Makefile)
|-- README.md                 (User Manual)
|-- users.csv                 (Stored user credentials)
|-- src/                      (Server and client source code)
|   |-- client.py             (Client program)
|   |-- server.c              (Server program)
|   `-- ...                   (Additional source files)
`-- test/                     (Tests)
    |-- tests.c               (Test source files)
    `-- ...                   (Additional test files)
```

## Communication Protocol

See [protocol.md](protocol.md) for the protocol used in communication.

## Map Format

See [map_format.md](map_format.md) for a breakdown of the map binary format.

### Error Handling

The server and client must handle errors such as invalid requests, server unavailable, invalid routes, and unauthorized access attempts.

### Extra Credit

| Area | Requirement | Description | Point Value | Points Earned | Comments |
| --- | --- | --- | --- | --- | --- |
| Testing | Has automated tests (points based on thoroughness) | 3 | | |
| Testing | Uses CI/CD pipelines to automatically build, lint, and test | 1 | | |
| Data Visualization | Create a flowchart that shows all the nodes and how they connect | 1 | | |
| Portability | Create a Dockerfile that contains all dependencies and launches the server. It should be able to be built and launched with `docker build -t starbase_1337 .` and `docker run -p <host_port>:<container_port> starbase_1337` | 1 | | |
| Portability | Will also compile and run on Windows 10 | 3 | | |
| Security | Uses TLS v1.3 to communicate between server/client | 5 | | |
| Optimization | Uses a threadpool with 10 threads to process work | 2 | | |
| logging | Timestamped logging on the server to a logging file detailing any change to the map or users (specified with `-l log.txt`) | 1 | | |

## Turn-In Instructions

Final work shall be committed to a "develop" branch. Create a "merge request" to merge "develop" into "main" when ready for review, selecting the instructor as the reviewer.

## Some helpful resources:

- [Dijkstra's Shortest path Algorithm](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/)
- [SSL and TLS explained](https://youtu.be/j9QmMEWmcfo)
- [SSL in C](https://aticleworld.com/ssl-server-client-using-openssl-in-c/)
- [SSL in C example](https://stackoverflow.com/a/16328115)
- [SSL in Python](https://docs.python.org/3/library/ssl.html)
- [Simple TLS Server](https://wiki.openssl.org/index.php/Simple_TLS_Server)
- TLPI CH 61.1: Partial Reads and Writes on Steam Sockets
- TLPI CH 60.4: Other Concurrent Server Designs: Preforked and prethreaded servers
- [Threadpool in C](https://nachtimwald.com/2019/04/12/thread-pool-in-c/)
- [CI pipelines](https://docs.gitlab.com/ee/ci/)
- [docker 101](https://www.docker.com/101-tutorial/)
- [Windows sockets](https://learn.microsoft.com/en-us/windows/win32/winsock/using-winsock)
