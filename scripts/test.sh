#!/bin/sh

for file in examples/*.oni; do
  echo "Testing: $file"
  echo "--------------"
  ./bin/oni "$file"
  exit_code=$?

  if [ $exit_code -ne 0 ]; then
    echo "❌ Error while testing: $file (exit code: $exit_code)"
    exit $exit_code
  fi

  echo
done

echo "✅ All tests passed."
