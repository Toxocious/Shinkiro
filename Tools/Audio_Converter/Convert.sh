#!/bin/bash

# ------------ CONFIG ------------

ALLOWED_FORMATS=("mp3" "wav" "ogg" "flac" "aac" "m4a" "opus" "alac" "wma" "aiff")

# ------------ FUNCTIONS ------------

wait_for_jobs() {
    while [ "$(jobs -r | wc -l)" -ge "$MAX_JOBS" ]; do
        sleep 0.5
    done
}

# ------------ ARG PARSING ------------

# Detect --dry-run flag anywhere and remove it from args
DRY_RUN=false

ARGS=()
for arg in "$@"; do
    if [ "$arg" = "--dry-run" ]; then
        DRY_RUN=true
    else
        ARGS+=("$arg")
    fi
done

if [ "${#ARGS[@]}" -lt 2 ] || [ "${#ARGS[@]}" -gt 3 ]; then
    echo "Usage: $0 <input_extension> <output_extension> [max_concurrent_threads] [--dry-run]"
    echo "Example: $0 ogg wav 8 --dry-run"
    echo
    echo -n "Supported formats: "

    printf '%s' "${ALLOWED_FORMATS[0]}"
    for ((i=1; i<${#ALLOWED_FORMATS[@]}; i++)); do
        printf ', %s' "${ALLOWED_FORMATS[i]}"
    done

    echo
    exit 1
fi

# ------------ VALIDATION CHECKING ------------

INPUT_EXT=$(echo "${ARGS[0]}" | tr '[:upper:]' '[:lower:]')
OUTPUT_EXT=$(echo "${ARGS[1]}" | tr '[:upper:]' '[:lower:]')

if [[ ! " ${ALLOWED_FORMATS[*]} " =~ " $INPUT_EXT " ]]; then
    echo "Error: Input format '$INPUT_EXT' is not supported."
    exit 1
fi

if [[ ! " ${ALLOWED_FORMATS[*]} " =~ " $OUTPUT_EXT " ]]; then
    echo "Error: Output format '$OUTPUT_EXT' is not supported."
    exit 1
fi

if [ "${#ARGS[@]}" -eq 3 ]; then
    THREADS_ARG="${ARGS[2]}"

    if ! [[ "$THREADS_ARG" =~ ^[0-9]+$ ]]; then
        echo "Error: max_concurrent_threads must be a positive integer."
        exit 1
    fi

    if [ "$THREADS_ARG" -lt 1 ] || [ "$THREADS_ARG" -gt 64 ]; then
        echo "Error: max_concurrent_threads must be between 1 and 64."
        exit 1
    fi

    MAX_JOBS="$THREADS_ARG"
else
    if command -v nproc &>/dev/null; then
        core_count=$(nproc)
    elif command -v sysctl &>/dev/null; then
        core_count=$(sysctl -n hw.ncpu)
    else
        core_count=4
    fi

    MAX_JOBS=$(( (core_count + 1) / 2 ))

    if [ "$MAX_JOBS" -lt 1 ]; then MAX_JOBS=1; fi
    if [ "$MAX_JOBS" -gt 64 ]; then MAX_JOBS=64; fi

    echo "Auto-detected $core_count core(s); using $MAX_JOBS thread(s) for conversion."
fi

# ------------ SCRIPT ROOT DIR ------------

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

INPUT_DIR="$SCRIPT_DIR/Input"
OUTPUT_DIR="$SCRIPT_DIR/Output"

if [ ! -d "$INPUT_DIR" ]; then
    echo "Input directory '$INPUT_DIR' does not exist."
    exit 1
fi
mkdir -p "$OUTPUT_DIR"

# ------------ FILES TO PROCESS ------------

mapfile -t files < <(find "$INPUT_DIR" -maxdepth 1 -type f -iname "*.${INPUT_EXT}")

file_count=${#files[@]}

if [ "$file_count" -eq 0 ]; then
    echo "No *.${INPUT_EXT} files found in $INPUT_DIR"
    exit 0
fi

echo "Found $file_count *.${INPUT_EXT} files to convert to ${OUTPUT_EXT}."

# ------------ MAIN LOOP ------------

success_count=0
fail_count=0
i=1

results_file="$(mktemp)"
LOG_DIR="$SCRIPT_DIR/Logs"
mkdir -p "$LOG_DIR"

log_file="$LOG_DIR/Conversion_$(date +%Y%m%d_%H%M%S).log"

echo "Conversion started at $(date)" >> "$log_file"
echo "" >> "$log_file"

for file in "${files[@]}"; do
    filename=$(basename "$file")
    base="${filename%.*}"
    output_file="$OUTPUT_DIR/${base}.${OUTPUT_EXT}"

    echo "[$(( (i * 100) / file_count ))%] Processing file $i of $file_count: $filename"

    if [ "$DRY_RUN" = true ]; then
        ORANGE='\033[38;5;208m'
        RESET='\033[0m'

        rel_input="./input/$filename"
        rel_output="./output/${base}.${OUTPUT_EXT}"

        echo -e "${ORANGE}[Dry Run]${RESET} Would run: ffmpeg -y -i \"$rel_input\" \"$rel_output\" -loglevel error -hide_banner"
        echo "[Dry Run] Log file would be: $log_file"
    else
        wait_for_jobs
        {
            ffmpeg -y -i "$file" "$output_file" -loglevel error -hide_banner

            exit_code=$?
            if [ $exit_code -eq 0 ]; then
                echo "SUCCESS" >> "$results_file"
                echo " - Conversion succeeded for '$filename' at $(date)"
            else
                echo "FAIL" >> "$results_file"
                echo " - Conversion FAILED for '$filename' with exit code $exit_code at $(date)"
            fi
        } >> "$log_file" 2>&1 &
    fi

    ((i++))
done

if [ "$DRY_RUN" = false ]; then
    wait

    success_count=$(grep -c '^SUCCESS$' "$results_file" || true)
    fail_count=$(grep -c '^FAIL$' "$results_file" || true)
    rm -f "$results_file"

    echo "" >> "$log_file"
    echo
    echo "All $i conversions complete at $(date)" >> "$log_file"
    echo "Logs saved to $log_file"

    echo
    echo "=========================="
    echo "Total files processed : $((i - 1))"
    echo "Successful conversions: $success_count${RESET}"
    echo "Failed conversions    : $fail_count${RESET}"
    echo "Log file location     : $log_file"
    echo "=========================="
else
    echo "" >> "$log_file"
    echo "[Dry Run] No conversions performed."
fi

{
    echo ""
    echo "=========================="
    echo "Total files processed : $((i - 1))"
    echo "Successful conversions: $success_count"
    echo "Failed conversions    : $fail_count"
    echo "=========================="
} >> "$log_file"
