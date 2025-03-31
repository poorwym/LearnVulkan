# 脚本说明

本目录包含以下脚本：

## CleanXcode.sh

清理所有Xcode相关的项目文件和缓存。这个脚本将会：

- 删除所有 `.xcodeproj` 目录
- 删除所有 `.xcworkspace` 目录
- 删除所有 `DerivedData` 目录
- 删除所有 `.build` 目录
- 删除所有 `UserInterfaceState.xcuserstate` 文件
- 删除所有 `.DS_Store` 文件

此外，它还会询问是否要清理系统级的Xcode缓存目录。

### 使用方法

```bash
# 在项目根目录下运行
./scripts/CleanXcode.sh
```

## Setup.sh

项目的设置脚本，用于MacOS系统。

## Setup.bat

项目的设置脚本，用于Windows系统。 