---
title: Day 6
date: 2026-04-07
topic: Busy Day and Light Review
---

# Day 6 - Busy Day and Light Review

Today was a busy day, so I could not spend proper time learning new networking topics. I only did a light review of the `Tcp_epoll_server.c` file and cleaned up the file by removing comments.

Even though I did not study deeply today, I still stayed connected to the project and looked again at the structure of an event-driven TCP server using `epoll`.

Most of my time today went to other responsibilities, so this was not a full learning session like the previous days. Still, I did not want to completely stop the routine, so I opened the project, looked through the server code, and tried to keep the important ideas fresh in my mind.

While reading the file, I noticed that even small cleanup work can help me understand the code better. Removing comments made me focus more directly on the actual functions, the event loop, and the relationship between accepting connections and reading client data.

---

## What I Did Today

1. I opened the `Tcp_epoll_server.c` file.
1. I reviewed the general structure of the server.
1. I removed comments from the file to make it cleaner.
1. I checked the code flow again at a high level.

---

## What Changed Today

- The main visible change was cleaning `Tcp_epoll_server.c` by removing the comments.
- No major new feature or new networking code was added today.
- This was more of a maintenance and review day than a full study day.

---

## Visual Summary (Picture)

```mermaid
flowchart LR
    A["Busy schedule"] --> B["Short coding time"]
    B --> C["Opened Tcp_epoll_server.c"]
    C --> D["Reviewed epoll server flow"]
    D --> E["Removed comments"]
    E --> F["Cleaner file and quick revision"]
    F --> G["No deep new study today"]
```

This picture shows that Day 6 was a continuity day: limited time, light review, and small cleanup.

---

## Study Note

I was busy today, so I could not learn these sections in depth.

I could not give enough time to fully study new parts, examples, or theory today. This was mainly a review and cleanup day, not a strong progress day.

---

## Quick Reminder

- `epoll` is used to watch many file descriptors efficiently.
- The server listens for new connections on the main socket.
- Client sockets are added to the `epoll` instance.
- When data is ready, the server reads it and sends a response back.

---

## Reflection

Not every study day has to be a big one. Today was limited, but I still kept the learning routine alive by reviewing the code and making a small cleanup. Tomorrow I can continue with a deeper study session.

One useful thing I learned from today is that consistency still matters, even on busy days. A short review is better than doing nothing, because it helps me stay familiar with the code and makes it easier to continue next time.
