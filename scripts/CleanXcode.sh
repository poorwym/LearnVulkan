#!/bin/bash

# 清理Xcode相关的缓存文件脚本
# 这个脚本会删除所有Xcode项目文件和缓存，包括:
# - .xcodeproj 目录
# - .xcworkspace 目录
# - DerivedData 目录
# - .build 目录
# - UserInterfaceState.xcuserstate 文件
# - .DS_Store 文件

echo "开始清理Xcode相关文件..."

# 从当前目录开始递归查找和删除Xcode项目文件
find . -type d -name "*.xcodeproj" -exec rm -rf {} +
find . -type d -name "*.xcworkspace" -exec rm -rf {} +
find . -type d -name "DerivedData" -exec rm -rf {} +
find . -type d -name ".build" -exec rm -rf {} +
find . -name "*.xcuserstate" -delete
find . -name ".DS_Store" -delete

# 如果用户的主目录中存在Xcode的DerivedData和缓存目录，也可以选择清理
if [ -d ~/Library/Developer/Xcode/DerivedData ]; then
  echo "是否清理系统的DerivedData目录? [y/N]"
  read -r response
  if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    rm -rf ~/Library/Developer/Xcode/DerivedData
    echo "已清理系统DerivedData目录"
  fi
fi

if [ -d ~/Library/Caches/com.apple.dt.Xcode ]; then
  echo "是否清理Xcode缓存? [y/N]"
  read -r response
  if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    rm -rf ~/Library/Caches/com.apple.dt.Xcode
    echo "已清理Xcode缓存"
  fi
fi

echo "Xcode相关文件清理完成!" 