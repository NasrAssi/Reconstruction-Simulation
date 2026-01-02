# SPLand Reconstruction Simulation

## Project Description

This project is a C++ simulation system for managing and evaluating reconstruction plans in the fictional land of SPLand. The simulation allows users to create settlements, define reconstruction plans with various selection policies, add facilities, and run simulation steps to evaluate how different strategies impact reconstruction outcomes.

## Features

- **Settlement Management**: Create and manage settlements of different types (Village, City, Metropolis)
- **Facility Construction**: Build various facilities including libraries, hospitals, factories, parks, and more
- **Multiple Selection Policies**:
  - **Naive (nve)**: Selects facilities in sequential order
  - **Balanced (bal)**: Selects facilities to minimize score differences
  - **Economy (eco)**: Prioritizes economy-category facilities
  - **Sustainability (env)**: Prioritizes environment-category facilities
- **Simulation Control**: Step through simulation, backup state, and restore previous states
- **Score Tracking**: Track Life Quality, Economy, and Environment scores for each plan

## Project Structure

```
├── src/
│   ├── main.cpp
│   ├── Simulation.cpp
│   ├── Plan.cpp
│   ├── Settlement.cpp
│   ├── Facility.cpp
│   ├── SelectionPolicy.cpp
│   ├── Action.cpp
│   └── Auxiliary.cpp
├── include/
│   ├── Simulation.h
│   ├── Plan.h
│   ├── Settlement.h
│   ├── Facility.h
│   ├── SelectionPolicy.h
│   ├── Action.h
│   └── Auxiliary.h
├── bin/
│   └── (compiled files)
├── makefile
└── README.md
```

## Compilation

To compile the project, run:

```bash
make
```

This will compile all source files and create the executable `simulation` in the `bin/` directory.

### Compiler Flags

The project uses the following compiler flags:
- `-g` - Debug information
- `-Wall` - Enable all warnings
- `-Weffc++` - Effective C++ warnings
- `-std=c++11` - C++11 standard
- `-Iinclude` - Include directory

## Running the Simulation

```bash
./bin/simulation <config_file_path>
```

Example:
```bash
./bin/simulation config_file.txt
```

## Configuration File Format

The configuration file defines the initial state of the simulation:

```
# Settlements
settlement <name> <type>
# type: 0=Village, 1=City, 2=Metropolis

# Facilities  
facility <name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
# category: 0=Life Quality, 1=Economy, 2=Environment

# Plans
plan <settlement_name> <selection_policy>
# policy: nve, bal, eco, env
```

### Example Configuration

```
settlement MyVillage 0
settlement MyCity 1
facility Library 0 3 3 2 2
facility Factory 1 5 2 5 1
plan MyVillage nve
plan MyCity eco
```

## Available Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| Step | `step <num>` | Advance simulation by num steps |
| Plan Status | `planStatus <id>` | Display plan information |
| Add Plan | `plan <settlement> <policy>` | Create new reconstruction plan |
| Add Settlement | `settlement <name> <type>` | Create new settlement |
| Add Facility | `facility <name> <cat> <price> <lq> <eco> <env>` | Add facility type |
| Change Policy | `changePolicy <id> <policy>` | Change plan's selection policy |
| Log | `log` | Print all executed actions |
| Backup | `backup` | Save simulation state |
| Restore | `restore` | Restore saved state |
| Close | `close` | End simulation and display results |

## Settlement Types

| Type | Value | Construction Limit |
|------|-------|-------------------|
| Village | 0 | 1 facility |
| City | 1 | 2 facilities |
| Metropolis | 2 | 3 facilities |

## Selection Policies

- **nve (Naive)**: Cycles through facilities sequentially
- **bal (Balanced)**: Minimizes difference between max and min scores
- **eco (Economy)**: Selects only Economy-category facilities
- **env (Sustainability)**: Selects only Environment-category facilities

## Memory Management

The project implements the Rule of 5 for proper resource management:
- Copy Constructor
- Copy Assignment Operator
- Move Constructor
- Move Assignment Operator
- Destructor

Memory leak testing with Valgrind:
```bash
valgrind --leak-check=full --show-reachable=yes ./bin/simulation config_file.txt
```

Expected output:
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```
