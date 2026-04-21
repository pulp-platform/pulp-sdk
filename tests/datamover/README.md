# Datamover Test

This directory contains a test application for the HWPE Datamover.
Tests are defined in `datamover_tests.csv`. For each test, a Python golden model
generates the expected input/output data on-the-fly, which is then compiled into
the test binary and verified against the hardware output.

## How it works

1. **Golden model** (`datamover_golden_model.py`) generates a `data.h` header
   containing input stimuli and expected output for the configured operation.
2. **Test binary** (`src/datamover_test.c`) includes `data.h`, allocates TCDM
   buffers, triggers the Datamover operation via the Konark HAL
   (`konark/hal_datamover.c`, `konark/hal_hwpe.c`), and compares the result
   against the golden output. It prints `[DM-OK]` and `Test success` on success.
3. **Test runner** (`run_datamover_tests.sh`) iterates the CSV, generates golden
   data, builds, simulates, and checks the log for `[DM-OK]` and `Test success`.

The test runner uses `datamover_tests.csv` by default. To switch to a different
list, override `TESTS_CSV` on the command line:

```bash
TESTS_CSV=tests/datamover/datamover_tests.csv ./tests/datamover/run_datamover_tests.sh
```

## Test configuration (`datamover_tests.csv`)

Each row in `datamover_tests.csv` defines one test. The header row documents all
columns:

| Column | Description |
|---|---|
| `name` | Unique test name (no spaces) |
| `mode` | Datamover mode: `0`=copy, `1`=transpose, (`2`=CIM layout)\*, (`3`=CIM layout + transpose)\*, `4`=unfold, `5`=fold |
| `transp_mode` | Transpose element width: `1`, `2`, or `4` (ignored for modes 0, 2, 4, 5) |
| `cim_mode` | `0`=row-major → CIM-layout, `1`=CIM-layout → row-major (modes 2, 3) |
| `row_tile_size` | CIM row tile size in elements (modes 2, 3) |
| `C`, `M`, `N` | Tensor dimensions (channels, rows, columns) |
| `count` | `1`=counting stimuli (deterministic, for debugging), `0`=random stimuli |

\**CIM layout conversion and transposition not yet completely implemented. CIM layout conversion works for 64-element aligned matrices.*

To add a new test, append a row to the CSV.

## Quick start

To build and run the tests you need the PULP SDK and toolchain set up.
Set up the path to GVSoC and source the SDK environment before running any commands:

```bash
export PATH=<path_to_your_gvsoc>/gvsoc/install/bin:$PATH
source configs/siracusa.sh
```

### Manual run

1. Generate `data.h` (example: byte-wise transpose of a 64×64 matrix):
   ```bash
   python3 tests/datamover/datamover_golden_model.py \
       --datamover-mode 1 --transp-mode 1 \
       --size-c 1 --size-m 64 --size-n 64
   ```
   The header is written to `tests/datamover/data/data.h` by default.
   For unfold/fold modes (`--datamover-mode 4` or `5`), supply `--patch-size <P>`
   (default: `4`) to set the patch side-length.

2. Build the test binary:
   ```bash
   riscv make -C tests/datamover clean all
   ```

3. Run the simulation:
   ```bash
   riscv make -C tests/datamover run
   ```
   Logs are printed to stdout. A passing test prints `[DM-OK]` and `Test success`. Activate traces for more information during execution:
   ```bash
   riscv make -C tests/datamover run runner_args="--trace=datamover"
   ```

### Automated run

The script `run_datamover_tests.sh` runs all tests in `datamover_tests.csv`
sequentially. It can be called from anywhere; logs are written per-test to
`tests/datamover/logs/`.

```bash
# Run all tests
./tests/datamover/run_datamover_tests.sh

# Run a single named test
./tests/datamover/run_datamover_tests.sh --test 64x64_copy

# Resume from a named test (runs that test and all subsequent ones)
./tests/datamover/run_datamover_tests.sh --resume 64x64_transp1

# Enable GVSoC traces for the datamover
./tests/datamover/run_datamover_tests.sh --trace datamover
```

`--test` and `--resume` are mutually exclusive.

The script exits immediately on any failure and prints which test failed along
with the path to its log file.

## Recompiling the GVSoC Datamover model

The Datamover RTL model lives inside your GVSoC installation at:

```
gvsoc/pulp/pulp/datamover
```

After editing the model, recompile GVSoC from the repository root:

```bash
cd <path_to_your_gvsoc>
source sourceme.sh
make all TARGETS=siracusa
```
