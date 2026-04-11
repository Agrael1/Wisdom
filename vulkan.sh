#!/bin/bash

# 1. Configuration - Update version here if needed
VULKAN_VER="1.4.341.1"
INSTALL_DIR="$HOME/.vulkan_sdk"
FILENAME="vulkansdk-linux-x86_64-$VULKAN_VER.tar.xz"
DOWNLOAD_URL="https://sdk.lunarg.com/sdk/download/$VULKAN_VER/linux/$FILENAME"

echo "--- Starting Vulkan SDK $VULKAN_VER Installation ---"

# 2. Create directory and download
mkdir -p "$INSTALL_DIR"
cd "$INSTALL_DIR" || exit

if [ ! -f "$FILENAME" ]; then
  echo "Downloading Vulkan SDK (.tar.xz)..."
  wget "$DOWNLOAD_URL" -O "$FILENAME"
else
  echo "Archive already exists, skipping download."
fi

# 3. Extract (using -xf which auto-detects or handles xz)
echo "Extracting $FILENAME..."
if tar -xf "$FILENAME"; then
  echo "Extraction successful."
else
  echo "Error: Extraction failed. Make sure 'xz-utils' is installed."
  exit 1
fi

# 4. Define the path to the 'x86_64' folder inside the extracted SDK
# Note: The tarball extracts into a folder named after the version
SDK_ROOT="$INSTALL_DIR/$VULKAN_VER/x86_64"

if [ -d "$SDK_ROOT" ]; then
  echo "--- Installation Complete! ---"
  echo ""
  echo "To 'hook it up' permanently, add these lines to your ~/.bashrc or ~/.zshrc:"
  echo "----------------------------------------------------------------"
  echo "export VULKAN_SDK=$SDK_ROOT"
  echo "export PATH=\$VULKAN_SDK/bin:\$PATH"
  echo "export LD_LIBRARY_PATH=\$VULKAN_SDK/lib:\$LD_LIBRARY_PATH"
  echo "export VK_LAYER_PATH=\$VULKAN_SDK/etc/vulkan/explicit_layer.d"
  echo "----------------------------------------------------------------"
  echo "Then run: source ~/.bashrc"
else
  echo "Error: Extraction failed or directory structure is unexpected."
fi
