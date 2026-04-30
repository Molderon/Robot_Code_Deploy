# Project: SaveRoom 🤖 
> **The Ultra-Lightweight - Version Control Daemon**

## ⚡ Tool Objectived
**SaveRoom** is a terminal-centric version control designed for the gritty reality of everyday troubleshooting software and hardware deployments. It’s built for the developer who needs to push code patches to a swarm of robots, servers or embedded "toasters" without the haevy duty CI/CD pipelines or container registries.

Running as a **Linux Daemon**, RCD ensures your machines are always running the right version and easy to maintain, even if the network is flaky or the hardware is ... experimental.
---

## 🧪 C++26: Learning Objects

- **Compile‑time reflections (`^^`)** – Automatically generate ZeroMQ serialization for telemetry data. No more manual `to_json` boilerplate.
- **`std::expected`** – Explicit error handling without exceptions. Every deployment step returns either a success value or a clear error.
- **`std::filesystem`** – Atomic symlink swaps for instant rollbacks. If a new build fails its health check, the daemon flips back to the last known good version.
- **`std::execution` (senders/receivers)** – Dispatch code updates to 50+ robots concurrently without spawning 50 threads. Memory footprint stays under 5 MB.
- **C++ contracts (`[[pre]]`, `[[post]]`)** – Guarantee safety at compile time: a binary won’t run if its checksum fails or it won’t fit in flash.
- **Structured bindings** – Unpack complex message frames elegantly:  
  `auto [status, payload, crc] = parse_packet(buffer);`
- **Coroutines** – The daemon suspends itself while waiting for network I/O, waking only when a message arrives. Near‑zero CPU usage when idle.
- **Ranges v3:** - Intuitive way to work with ranges of elements, simplifying tasks like filtering, mapping, and transforming data.
---

## 🏗️ Project Architecture
The file structure & build process:

```text
SaveRoom/
├── Installer/              # Installation and Config .py
├── .config/                # Daemon environment variables & network IDs
├── build/                  # Artifact directory (ignored by VCS)
├── docs/                   # The "Funky Manual" & "Installation".
├── include/
│   ├── network/            # ZeroMQ (cppzmq) wrappers
│   └── utils/              # Magic-wormhole bridges
│
├── src/
│   ├── daemon/             # Linux service loop (The "Heartbeat")
│   ├── cli/                # 'SafeRoom' terminal tool
│   └── Source/             
│       ├── Drvier/         # Driver Code
│       ├── State Machine/  # Topological Sorting / Priority Queue 
│       ├── Periphery/ 
│       ├── NetCode/        # Neworking
│       ├── Terminal/
│       ├── Parser/         # Analyses argc and file formatings
│       ├── Evoker/         # Perform builds/setups
│       └── Err/
│
├── Cargo/                  # Packages to deploy  
├── Anthenas/               # Encrypted Server/Client Rorster 
├── tests/                  # C++ Contracts validation suite
├── ErrLog/                 # FailLogs
└── CMakeLists.txt          # Target: CXX_STANDARD 26
```
<br>

## 📦 Dependencies
SaveRoom is using only two battle‑tested libraries:

| Library          | License | Purpose |
|------------------|---------|---------|
| **cppzmq**       | MIT     | Asynchronous messaging (heartbeats, commands) |
| **magic-wormhole** | MIT   | Secure ad‑hoc file & key exchange |

### cppzmq (ZeroMQ)
The high‑speed nervous system of RCD. It handles reconnections, message queues, and multiple messaging patterns automatically. Perfect for sending lightweight heartbeats or broadcasting commands to your fleet.

### magic-wormhole
Enables secure file transfer using a simple human‑readable code (e.g., `4‑purple‑dinosaur`). Ideal for one‑off debug pushes or initial key exchange, even through NATs or firewalls.

> **Note:** Both libraries are called via lightweight C++ wrappers – no heavy frameworks like Boost are allowed.

---


## 🔧 Build & Run
1. Dependencies: `cppzmq` and `magic-wormhole` (Will be installed if not native).
2. Configure: edit `.config` with your network ID and environment variables.
3. Build with CMake:
  ```bash
   > ./Install.py
  ```
> Start the daemon: `sudo ./saferoom`  
   Use the CLI tool: `Saferoom hit <target> <packet> <build tool> <special flags as str>`

---

## ❤︎ Contributing
I welcome issues and pull requests! Please ensure your code:
- Follows the lightweight philosophy
- Passes all contract tests (`tests/`)
- Uses C++26 features appropriately

---

## 📄 License
MIT – use it freely in your own robot army.
```
MIT License

Copyright (c) 2026 Molderon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.```