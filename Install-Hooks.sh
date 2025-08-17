#!/bin/sh

HOOKS_DIR=".githooks"
TARGET_DIR=".git/hooks"

if [ ! -d "$TARGET_DIR" ]; then
    echo "ERROR: .git folder not found. Are you in a git repo?"
    exit 1
fi

for hook in pre-commit pre-push; do
    if [ -f "$HOOKS_DIR/$hook" ]; then
        cp "$HOOKS_DIR/$hook" "$TARGET_DIR/$hook"
        chmod +x "$TARGET_DIR/$hook"
        echo "Installed $hook hook."
    fi
done

echo "All hooks installed successfully."
