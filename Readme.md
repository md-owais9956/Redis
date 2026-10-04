# Redis Lite

A Redis-inspired in-memory key-value database built from scratch to understand how systems like Redis work internally.

This project is focused on learning and implementing the core concepts behind an in-memory database, including networking, command parsing, data structures, persistence, concurrency, and client-server communication.

> **Status:** Work in Progress

## Overview

Redis is a high-performance in-memory data store commonly used as a database, cache, message broker, and more.

Instead of simply using Redis, this project aims to understand what happens under the hood by implementing a simplified version from scratch.

The goal is not to recreate every Redis feature, but to build the fundamental components step by step.

## Goals

- Understand how an in-memory database works
- Build a TCP client-server architecture
- Implement a Redis-like command protocol
- Implement core data structures
- Handle concurrent client connections
- Explore persistence mechanisms
- Understand request parsing and command execution
- Learn how high-performance systems are designed
- Gradually add more advanced database features

## Planned Features

### Core

- [ ] TCP server
- [ ] TCP client
- [ ] Client connection handling
- [ ] Command parser
- [ ] Command execution engine
- [ ] In-memory key-value store

### Basic Commands

- [ ] `PING`
- [ ] `SET`
- [ ] `GET`
- [ ] `DEL`
- [ ] `EXISTS`
- [ ] `KEYS`
- [ ] `EXPIRE`
- [ ] `TTL`

### Data Structures

- [ ] Strings
- [ ] Lists
- [ ] Sets
- [ ] Hashes
- [ ] Sorted Sets

### Persistence

- [ ] Snapshot-based persistence
- [ ] Append-only log
- [ ] Database recovery
- [ ] Data serialization

### Advanced Features

- [ ] Multiple clients
- [ ] Concurrent request handling
- [ ] Transactions
- [ ] Pub/Sub
- [ ] Blocking operations
- [ ] Authentication
- [ ] Configuration system
- [ ] Graceful shutdown

### Performance

- [ ] Benchmarking
- [ ] Memory usage analysis
- [ ] Concurrent client benchmarks
- [ ] Command latency measurements
- [ ] Profiling and optimization

## Architecture

The project will follow a simple client-server architecture:

```text
                    ┌─────────────────┐
                    │      Client     │
                    └────────┬────────┘
                             │
                             │ TCP
                             ▼
                    ┌─────────────────┐
                    │   TCP Server    │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │ Command Parser  │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │ Command Engine  │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │  Data Store     │
                    │                 │
                    │  Key → Value    │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │   Persistence   │
                    └─────────────────┘
```

## Example

Once the basic server is implemented, the interaction will look something like:

```text
SET name Owais
+OK

GET name
"Owais"

DEL name
:1

GET name
(nil)
```

## Project Structure

The structure may evolve as the project grows.

```text
redis-from-scratch/
│
├── src/
│   ├── server/
│   ├── client/
│   ├── parser/
│   ├── commands/
│   ├── storage/
│   ├── data_structures/
│   └── persistence/
│
├── tests/
│
├── docs/
│
├── benchmarks/
│
├── examples/
│
├── README.md
└── LICENSE
```

## Development Roadmap

The project will be developed incrementally.

### Phase 1 — TCP Server

Build a basic TCP server capable of accepting client connections.

### Phase 2 — Command Protocol

Implement request parsing and responses.

Initial commands:

```text
PING
SET
GET
DEL
```

### Phase 3 — In-Memory Storage

Implement the internal key-value store.

```text
Key        Value
-------------------
name       Owais
language   C++
project    Redis
```

### Phase 4 — Data Structures

Add support for Redis-like collections:

```text
String
List
Set
Hash
Sorted Set
```

### Phase 5 — Expiration

Add key expiration and TTL support.

```text
SET session abc123 EX 60
TTL session
```

### Phase 6 — Persistence

Implement mechanisms for saving data to disk and recovering it after a restart.

### Phase 7 — Concurrency

Improve the server to handle multiple clients efficiently.

### Phase 8 — Optimization

Benchmark the implementation and identify performance bottlenecks.

## Learning Objectives

Through this project, I aim to understand:

- TCP networking
- Client-server architecture
- Serialization and deserialization
- Parsing protocols
- Hash tables
- Memory management
- Concurrency
- Threading
- Synchronization
- Persistence
- File I/O
- Database architecture
- Performance optimization
- Benchmarking
- System design

## Why Build Redis From Scratch?

Using a database teaches you how to use a database.

Building one teaches you how a database works.

This project is an attempt to move beyond APIs and abstractions and understand the engineering decisions behind an in-memory data store.

## Current Progress

| Component | Status |
|---|---|
| Project Setup | ⬜ |
| TCP Server | ⬜ |
| Client | ⬜ |
| Protocol Parser | ⬜ |
| `PING` | ⬜ |
| `SET` | ⬜ |
| `GET` | ⬜ |
| `DEL` | ⬜ |
| Expiration | ⬜ |
| Lists | ⬜ |
| Sets | ⬜ |
| Hashes | ⬜ |
| Persistence | ⬜ |
| Concurrency | ⬜ |
| Pub/Sub | ⬜ |
| Benchmarking | ⬜ |

## Disclaimer

This project is an educational implementation inspired by Redis.

It is not intended to be a drop-in replacement for production Redis.

Redis is an independent project and remains the work of its respective contributors.

## Future Ideas

Some features I may explore after the core implementation:

- Event-driven architecture
- Non-blocking I/O
- Memory-efficient data structures
- Replication
- Sharding
- Leader-follower architecture
- Distributed caching
- Cluster communication
- Fault tolerance
- Monitoring and metrics

##
