#!/usr/bin/env bash
# Copyright 2026 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Authors: Cyrill Durrer <cdurrer@iis.ee.ethz.ch>

# Generates golden data on-the-fly via datamover_golden_model.py, then
# builds and runs datamover_test for each configuration.
# Usage: run from tests/datamover_test/:
#   ./run_datamover_tests.sh [--test <name>] [--resume <name>]
#   --test <name>     run only the named test case from datamover_tests.csv
#   --resume <name>   skip all tests before <name> and run from there to the end

set -u
set -e
set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GOLDEN_MODEL="$SCRIPT_DIR/datamover_golden_model.py"
LOG_DIR="$SCRIPT_DIR/logs"
GENERATED_HEADER="$SCRIPT_DIR/data/data.h"
TESTS_CSV="${TESTS_CSV:-$SCRIPT_DIR/datamover_tests.csv}"

mkdir -p "$LOG_DIR"

# ── Parse arguments ──────────────────────────────────────────────────────────
FILTER_TEST=""
RESUME_FROM=""
while [[ $# -gt 0 ]]; do
  case "$1" in
    --test)   FILTER_TEST="$2";  shift 2 ;;
    --resume) RESUME_FROM="$2"; shift 2 ;;
    *) echo "Unknown argument: $1"; echo "Usage: $0 [--test <name>] [--resume <name>]"; exit 1 ;;
  esac
done

if [[ -n "$FILTER_TEST" && -n "$RESUME_FROM" ]]; then
  echo "Error: --test and --resume are mutually exclusive"
  exit 1
fi

# ── Load test configurations from CSV ────────────────────────────────────────
mapfile -t ALL_TESTS < <(tail -n +2 "$TESTS_CSV" | grep -v '^[[:space:]]*$')

if [[ -n "$FILTER_TEST" ]]; then
  TESTS=()
  for entry in "${ALL_TESTS[@]}"; do
    entry_name="$(echo "${entry%%,*}" | tr -d ' ')"
    [[ "$entry_name" == "$FILTER_TEST" ]] && TESTS+=("$entry")
  done
  if [[ ${#TESTS[@]} -eq 0 ]]; then
    echo "Error: test '$FILTER_TEST' not found in $TESTS_CSV"
    exit 1
  fi
elif [[ -n "$RESUME_FROM" ]]; then
  TESTS=()
  found=0
  for entry in "${ALL_TESTS[@]}"; do
    entry_name="$(echo "${entry%%,*}" | tr -d ' ')"
    [[ "$entry_name" == "$RESUME_FROM" ]] && found=1
    [[ $found -eq 1 ]] && TESTS+=("$entry")
  done
  if [[ ${#TESTS[@]} -eq 0 ]]; then
    echo "Error: test '$RESUME_FROM' not found in $TESTS_CSV"
    exit 1
  fi
  echo "Resuming from: $RESUME_FROM"
else
  TESTS=("${ALL_TESTS[@]}")
fi

# ── Run tests ────────────────────────────────────────────────────────────────
pass=0

for entry in "${TESTS[@]}"; do
  IFS=',' read -r name mode transp_mode cim_mode tile_size size_c size_m size_n count <<< "$entry"
  # Trim whitespace from all fields
  read -r name      <<< "$name"
  read -r mode      <<< "$mode"
  read -r transp_mode <<< "$transp_mode"
  read -r cim_mode  <<< "$cim_mode"
  read -r tile_size <<< "$tile_size"
  read -r size_c    <<< "$size_c"
  read -r size_m    <<< "$size_m"
  read -r size_n    <<< "$size_n"
  read -r count     <<< "$count"
  gm_args="--datamover-mode $mode --transp-mode $transp_mode --cim-mode $cim_mode --row-tile-size $tile_size --size-c $size_c --size-m $size_m --size-n $size_n"
  [[ "$count" == "1" ]] && gm_args="$gm_args --count"

  echo ""
  echo "════════════════════════════════════════════════════"
  echo "  TEST : $name  (mode=$mode C=$size_c M=$size_m N=$size_n)"
  echo "════════════════════════════════════════════════════"

  # 1. Generate golden header
  # shellcheck disable=SC2086
  python3 "$GOLDEN_MODEL" $gm_args --output "$GENERATED_HEADER"

  # 2. Build
  build_log="$LOG_DIR/${name}.build.log"
  if ! riscv make -C "$SCRIPT_DIR" clean all 2>&1 | tee "$build_log"; then
    echo "FAILED: $name (build failed). Build log: $build_log"
    exit 2
  fi

  # 3. Simulate
  log_file="$LOG_DIR/${name}.log"
  if ! riscv make -C "$SCRIPT_DIR" run 2>&1 | tee "$log_file"; then
    echo "FAILED: $name (simulation failed). Log: $log_file"
    exit 3
  fi

  # 4. Check result
  if ! grep -q "\[DM-OK\]" "$log_file"; then
    echo "FAILED: $name (no [DM-OK] in log). Log: $log_file"
    exit 4
  fi
  if ! grep -q "Test success" "$log_file"; then
    echo "FAILED: $name (no 'Test success' in log). Log: $log_file"
    exit 5
  fi

  echo "PASSED: $name"
  pass=$(( pass + 1 ))
done

echo ""
echo "════════════════════════════════════════════════════"
echo "  All $pass / ${#TESTS[@]} tests passed"
echo "════════════════════════════════════════════════════"
