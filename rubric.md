# Starbase 1337 Rubric


**Student Name**:

## Documentation

| Area  | Requirement   | Description   | Point Value | Points Earned | Comments |
|---------------|---------------------------------------|---------------------------------------------------------------------------------------|-------------|---------------|----------|
| Documentation | User Manual   | Project includes a README.md serving as a comprehensive user manual.   | 2   |       |  |
| Documentation | Code Commenting       | Code is well-commented, explaining the purpose and functionality clearly.  | 2   |       |       | |
| Documentation | Maintainers Manual | Project includes a reference manual (`refman.pdf`) that inludes documentation on every function, enum, class, and define.  | 1   |       |       | |

**Subtotal:** ___/5

## Server

| Area  | Requirement   | Description   | Point Value | Points Earned | Comments |
|---------------|---------------------------------------|---------------------------------------------------------------------------------------|-------------|---------------|----------|
| Robustness    | Connection Handling   | Server correctly manages multiple connections and requests at once | 3   |       |  |
| Function      | IP Versions | Correctly implements IPv4 and IPv6 sockets | 2  |       |  |
| Robustness    | Error Handling | Server has effective error handling and feedback mechanisms for server-side errors including memory management    | 10   |       |  |
| Design | Build Process | Successfully compiles and builds with specified `make` targets. | 1   |       |  |
| Function | Threading | Properly utilizes threading to process requests (with no race conditions) | 5 | |
| Function      | Stability and Performance | Server runs stably, efficiently handles tasks without crashing.  | 5  |       |  |
| Function      | User Commands | Implements all user related commands appropriately. | 4  |       |  |
| Function      | Graph Commands | Implements all graph updating commands appropriately (add/del location/link). | 4  |       |  |
| Function      | Backups | Backs up user credentials at shutdown | 3  |       |  |
| Function      | Path Commands | Implements `route` command appropriately, and correctly returns the shortest path | 6  |       |  |
| Function      | List Commands | Implements `list` and `search` commands appropriately | 6  |       |  |
| Function      | Communication Protocol | Correctly follows the request/response packet formats for communication.      | 6   |       |  |

**Subtotal:** ___/55

## Client

| Area  | Requirement   | Description   | Point Value | Points Earned | Comments |
|---------------|---------------------------------------|---------------------------------------------------------------------------------------|-------------|---------------|----------|
| Robustness    | Connection Management | Client effectively manages connection to the server, including initiation and termination. | 4   |       |  |
| Robustness    | Error Handling | Client has comprehensive handling and feedback for client-side errors.      | 6   |       |  |
| Function      | Command Implementation | Implements all client-side commands as specified. | 10  |       |  |
| Function      | Communication Protocol | Correctly follows the request/response packet formats for communication.      | 5   |       |  |

**Subtotal:** __/25

## General

| Area  | Requirement   | Description   | Point Value | Points Earned | Comments |
|---------------|---------------------------------------|---------------------------------------------------------------------------------------|-------------|---------------|----------|
| Style | Barr-c and google style python | Adheres to style guidelines      | 4  |       |  |
| General | Discretionary | Adheres to best practices and any specified tasks not explicitly mentioned.      | 10  |       |  |
| Design | Directory Structure | Adheres to the specified directory structure. | 1   |       |  |

**Subtotal:** __/15


## Extra Credit

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

Note: Extra credit can only apply to projects that have earned a passing score from previous sections

**Subtotal:** __/17

**Final Score:** ___

**Grader Name**:

**Additional Comments**:
