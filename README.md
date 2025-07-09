# OSIM - Organizational Simulation and Internal Management System

Welcome! This project was developed as a semester project for the CS-1004 Object Oriented Programming course at FAST NUCES, Islamabad.



![OSIM Working Screenshot](https://i.postimg.cc/8cWs7407/osim.jpg)


## Overview:
OSIM simulates an internal organizational environment where users with different roles (Junior, Employee,
Manager, Director, Executive) interact through tasks, messages, and resources. All access is
strictly governed by Role-Based Access Control (RBAC), clearance levels, and centralized
policy enforcement. Key activities include task delegation, secure messaging, and audit logging,
all with runtime accountability and secure operation tracking.
This project reinforces core Object-Oriented Programming (OOP) concepts including
inheritance, polymorphism, file handling, memory management, and access control. The
simulation also includes RBAC enforcement, audit logs, timeout monitoring, credential security,
and role-sensitive communication.

## Features
- **Visuals:**
  - GUI-style dashboard using ASCII art and box formatting
  - Color-coded output for better readability using values from `constants.h`
  - Custom `TEXT -> ASCII` converter built in that announces the name of the person (YES ANY ALPHANUMERIC NAME!)
  
- **OOP Principles:**
  - Abstraction, Encapsulation, Polymorphism, Inheritance
  - Operator Overloading, Friend Functions
  - DMA (Manual Memory Management i.e no STL or smart pointers)
  
- **Security:**
  - Use of hashing for senstive user credentials (password, forgot pass questions)
  - Encrypted Messaging using XOR cipher
  - All access control refer `PolicyEngine` no hardcoded checks
  - OTP Feature implemented along with checks for weak passwords
  - Manual `getErrFreeString()` and `getErrFreeInt()` functions to ensure no invalid input or any kind of overflow is possible (i think??)
  
- **Task System:**
  - Authorized users can create, assign, delegate, and expire tasks
  - `TimeManager` automatically expires tasks after TTL
  - Priority system has been implemented (High, Medium, Low)
  

- **Other Features**
  - `PerformanceSystem` adds or subtracts from a user's total score based on his performance
  - `Notifications` sends system wide notifications to users based on events
  - `AuditLogger` keeps track of all senstive system events and user actions
  
- **Clean Code:**
  - Everything data related is saved in `data/` directory so no data loss upon restart.
  - `constants.h` holds values for colors, scores, priorities etc that can be easily modified
  - Separate class and header files in `src/` and `include/` directories for better organization


## How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/abdul-m01z/OSIM
    ```
2. Navigate to the project directory:
    ```bash
    cd OSIM
    ```
3. Compile the code using a C++ compiler:
   ```bash
   g++ main.cpp src/*.cpp -o osim
   ```
4. Run the executable:
   ```bash
    ./osim
    ```

## Notes
- The ASCII art styling and colors dont seem to work on some windows terminals so try running in WSL or Linux for best experience
- The code is currently not commented although easy to understand but still I might add them later
- The structure for the data files is documented in `docs/datailes_format.txt`
- And as always bugs are possible

Thanks for visiting the repository! If you have any questions or suggestions feel free to contact me or open an issue.