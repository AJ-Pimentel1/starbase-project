# Starbase 1337 --- 30-Day Project Plan

**Planning window:** November 11--December 11, 2026\
**Estimated effort:** Approximately 85 hours\
**Target:** Complete the core assignment, validate the client/server
implementation, document it, and submit through the required Git
workflow.

------------------------------------------------------------------------

## 1. Scope and effort estimate

The assignment is substantial because it combines low-level networking,
a binary protocol, graph algorithms, concurrency, authentication, a
Python client, testing, and documentation.

  Area                        Rubric points
  ---------------------- ------------------
  Documentation                           5
  Server                                 55
  Client                                 25
  General requirements                   15
  **Core total**                    **100**
  Extra credit             **17 available**

A reasonable planning estimate is **60--90 hours for a solid core
submission**. Completing most extra-credit items may bring the total
closer to **90--120 hours**. This schedule budgets about **85 hours**,
prioritizing the core rubric and leaving extra credit conditional on
progress.

### Important implementation priorities

-   Implement the binary protocol exactly as specified, including
    big-endian serialization and the required packet layouts and
    opcodes.
-   Build reliable `send_all()` and `recv_all()` helpers early. TCP does
    not preserve application-level message boundaries, so one `send()`
    or `recv()` is not guaranteed to transfer an entire packet.
-   Keep the C server modular and use straightforward synchronization
    rather than overengineering.
-   Implement and test graph parsing and algorithms before integrating
    them into the network server.
-   Treat baseline multithreading as core work. The optional **10-thread
    pool** is a separate extra-credit item.
-   The assignment references `map_format.md`. Obtain and follow that
    format document before finalizing the map parser; do not guess the
    binary layout.

------------------------------------------------------------------------

## 2. Four-week schedule

### Phase 1 --- Graph foundation and repository setup

**November 11--17 \| 20 hours**

  ------------------------------------------------------------------------
  Date                  Task                                         Hours
  --------------------- --------------------- ----------------------------
  Nov 11                Set up repository,                               2
                        required directory    
                        structure, and        
                        Makefile targets      

  Nov 12                Implement binary map                             3
                        parser                

  Nov 13                Implement graph                                  3
                        representation and    
                        adjacency lists       

  Nov 14                Implement Dijkstra's                             3
                        shortest-path         
                        algorithm             

  Nov 15                Implement required                               3
                        graph operations      

  Nov 16                Write graph unit                                 3
                        tests                 

  Nov 17                Buffer handling,                                 3
                        debugging, and        
                        integration           

                        **Phase total**                             **20**
  ------------------------------------------------------------------------

**Deliverable by November 17**

-   Graph representation and required graph operations are implemented.
-   Map parsing follows the supplied map-format specification.
-   The project builds with `-Wall -Werror`.
-   Graph logic has basic tests.

### Phase 2 --- C server, protocol, authentication, and concurrency

**November 18--24 \| 22 hours**

  ------------------------------------------------------------------------
  Date                  Task                                         Hours
  --------------------- --------------------- ----------------------------
  Nov 18                Implement TCP socket                             3
                        setup and connection  
                        handling              

  Nov 19                Implement packet                                 4
                        parsing and           
                        serialization         

  Nov 20                Implement response                               3
                        packets and error     
                        codes                 

  Nov 21                Implement                                        4
                        authentication,       
                        SHA-512 password      
                        handling, and user    
                        management            

  Nov 22                Implement sessions                               3
                        and permissions       

  Nov 23                Implement                                        3
                        pthread-based         
                        concurrent client     
                        handling              

  Nov 24                Integration and                                  2
                        protocol debugging    

                        **Phase total**                             **22**
  ------------------------------------------------------------------------

**Deliverable by November 24**

-   The server accepts client connections.
-   Registration and login work.
-   Sessions and permissions are enforced.
-   Multiple clients can connect concurrently.
-   Requests and responses conform to the supplied protocol.

### Phase 3 --- Commands and Python client

**November 25--December 1 \| 22 hours**

  Date     Task                                                      Hours
  -------- ------------------------------------------------------ --------
  Nov 25   Implement required user commands                              3
  Nov 26   Implement graph-related commands                              3
  Nov 27   Implement list and search commands                            2
  Nov 28   Implement route command and output                            3
  Nov 29   Build Python socket client and serialization logic            4
  Nov 30   Implement Python CLI, commands, and password prompts          4
  Dec 1    End-to-end client/server testing                              3
           **Phase total**                                          **22**

**Deliverable by December 1**

-   All required commands work end-to-end.
-   The Python client communicates with the C server.
-   Route results are returned and displayed correctly.
-   IPv4 and IPv6 behavior is tested.
-   Error handling is in place.

### Phase 4 --- Hardening, documentation, and submission

**December 2--10 \| 21 hours**

  ------------------------------------------------------------------------
  Date                  Task                                         Hours
  --------------------- --------------------- ----------------------------
  Dec 2                 Memory management and                            3
                        error-handling review 

  Dec 3                 Race-condition                                   3
                        review, thread-safety 
                        fixes, and stress     
                        testing               

  Dec 4                 Graceful shutdown and                            2
                        backup/save behavior  

  Dec 5                 TLS 1.3 extra-credit                             4
                        implementation, if    
                        core work is on       
                        schedule              

  Dec 6                 Expand automated                                 3
                        tests                 

  Dec 7                 Docker and CI/CD                                 2
                        extra-credit work     

  Dec 8                 README and                                       2
                        reference-manual PDF  

  Dec 9                 Logging and final                                1
                        fixes                 

  Dec 10                Clean build,                                     1
                        regression test, and  
                        submission            
                        preparation           

                        **Phase total**                             **21**
  ------------------------------------------------------------------------

**Deliverable by December 10**

-   Server shutdown is graceful and required save prompts/backup
    behavior are handled.
-   Thread safety and memory handling have been reviewed.
-   Required documentation is complete.
-   A clean build and regression test pass.
-   Submission is ready.

### December 11 --- Submission

Do not plan new features for submission day.

1.  Run `make clean && make all`.
2.  Run `make test`.
3.  Verify the project builds in a clean environment.
4.  Verify the required directory structure and documentation.
5.  Push the completed work to the `develop` branch.
6.  Open a merge request from `develop` to `main`.
7.  Add the instructor as the required reviewer.

------------------------------------------------------------------------

## 3. Weekly workload

The plan averages roughly **20--22 hours per week**.

A workable distribution is:

-   **Monday--Friday:** approximately 2--3 hours per day.
-   **Saturday:** approximately 4 hours.
-   **Sunday:** approximately 2--3 hours.

The daily allocations in the schedule are targets, not rigid
requirements. Move work between days as needed, but preserve the phase
deliverables.

------------------------------------------------------------------------

## 4. Milestone tracker

Use this tracker to assess whether the project is on schedule.

-   [ ] **November 17 --- Graph foundation complete:** parser, graph
    representation, algorithms, and basic tests.
-   [ ] **November 24 --- Server foundation complete:** protocol,
    authentication, sessions, permissions, and concurrent connections.
-   [ ] **December 1 --- End-to-end functionality complete:** required
    commands and Python client work against the server.
-   [ ] **December 10 --- Hardening complete:** tests, documentation,
    clean build, and submission preparation.
-   [ ] **December 11 --- Submitted:** `develop` branch pushed and merge
    request opened to `main` with the instructor as reviewer.

------------------------------------------------------------------------

## 5. Extra-credit strategy

There are 17 extra-credit points available:

  Extra-credit item         Points
  ----------------------- --------
  Tests                          3
  CI/CD                          1
  Graph flowchart                1
  Docker                         1
  Windows compatibility          3
  TLS 1.3                        5
  10-thread pool                 2
  Timestamp logging              1
  **Total**                 **17**

Prioritize extra credit only after the core implementation is stable.

Lower-cost additions such as timestamp logging, a graph flowchart,
Docker, and CI/CD may be attractive if the core work is complete. TLS
1.3 and Windows compatibility can require more debugging and should not
jeopardize the required functionality. The 10-thread pool is optional;
it does not replace the baseline requirement to support concurrent
clients.

### If the schedule slips

If the project is behind by **December 1**, stop pursuing optional
features and focus on the core rubric. In particular:

1.  Finish required server functionality.
2.  Finish the Python client and required commands.
3.  Validate protocol compliance and end-to-end behavior.
4.  Complete documentation and submission requirements.
5.  Drop TLS or other extra-credit work before sacrificing core
    functionality.

------------------------------------------------------------------------

## 6. Definition of done

Before submission, verify the following:

### Build and project structure

-   [ ] Required directory structure is present.
-   [ ] Required Makefile targets exist: `all`, `program`, `clean`,
    `test`, `docs`, and `setup`.
-   [ ] `make clean && make all` succeeds.
-   [ ] The project compiles with the required warning/error settings.

### Server and protocol

-   [ ] TCP server accepts connections.
-   [ ] Packet parsing and serialization match the supplied protocol.
-   [ ] Big-endian fields are handled correctly.
-   [ ] `send_all()` and `recv_all()` handle partial transfers.
-   [ ] IPv4 and IPv6 are supported.
-   [ ] Multiple clients can be handled concurrently.
-   [ ] Authentication uses the required SHA-512 handling.
-   [ ] Session timeout is 10 minutes.
-   [ ] Permissions and error responses are correct.
-   [ ] SIGINT triggers graceful shutdown and required save behavior.

### Graph and client

-   [ ] Map parser matches `map_format.md`.
-   [ ] Required graph operations work.
-   [ ] Dijkstra shortest-route functionality works.
-   [ ] Python 3.10 client connects and serializes requests correctly.
-   [ ] All required client commands work end-to-end.

### Documentation and submission

-   [ ] README is complete.
-   [ ] Reference manual PDF is generated.
-   [ ] `make test` succeeds.
-   [ ] Repository is pushed to `develop`.
-   [ ] Merge request to `main` is open.
-   [ ] Instructor is assigned as reviewer.

------------------------------------------------------------------------

## 7. Working principle

**Get the complete core system working before polishing optional
features.** The biggest integration risks are the binary protocol, TCP
message handling, concurrency, and the boundary between the C server and
Python client. Test those boundaries early and repeatedly.
