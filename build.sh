# Configure build system
cmake -DMAIN_SRC="$MAIN" -G "MinGW Makefiles" .

# Build the project
cmake --build .

if [ $? -ne 0 ]; then
  echo "Build failed! Exiting..."
  exit 1
fi

# Run executable
exec ./bin/app $1