#!/bin/bash

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# 检测操作系统类型
OS=$(uname)

# 根据操作系统选择正确的 premake5 路径和项目类型
if [ "$OS" = "Darwin" ]; then
    PREMAKE_PATH="$PROJECT_ROOT/Walnut/vendor/bin/premake/macOS/premake5"
    PROJECT_TYPE="xcode4"
elif [ "$OS" = "Linux" ]; then
    PREMAKE_PATH="$PROJECT_ROOT/Walnut/vendor/bin/premake/Linux/premake5"
    PROJECT_TYPE="gmake2"
else
    echo "不支持的操作系统: $OS"
    exit 1
fi

# 检查 premake5 是否存在
if [ ! -f "$PREMAKE_PATH" ]; then
    echo "错误: 找不到 premake5 可执行文件: $PREMAKE_PATH"
    exit 1
fi

# 确保 premake5 有执行权限
chmod +x "$PREMAKE_PATH"

cd "$PROJECT_ROOT"
"$PREMAKE_PATH" --cc=clang --file=Build-Client.lua "$PROJECT_TYPE"
"$PREMAKE_PATH" --cc=clang --file=Build-Server.lua "$PROJECT_TYPE"
