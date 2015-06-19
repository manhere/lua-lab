# lua-lab
Automatically exported from code.google.com/p/lua-lab

Lua 编程语言内核的实验性扩展项目。
增加或修改一些代价低、实用性强的一些功能。
由于目前实验条件和时间所限，暂时只面向 Windows 平台开发并提供 Visual C++ 的工程文件，但修改的代码会尽可能考虑到跨平台。
项目的一些描述信息暂时使用简体中文描述，以后考虑增加英文描述。


原则

- 基于官方最新版本, 如果功能与本项目修改的功能有重复或冲突, 以官方版本为主
- 尽量做到向官方版兼容, 尽量不改动 VM 部分
- 尽量做到向后兼容
- 尽量在有充足理由的情况下做增加和修改
- 尽量减少代码的改动


特性

- 编译 Lua 源文件时忽略 UTF-8 的 BOM
- 支持 UTF-8 字符集及部分本地字符集中非单字节字符(每个字节均在 0x80-0xFE 范围)作为标识符
- 支持 continue 语句, 作用和 C 相同, 可用在 for, while, repeat 循环中使用
- 支持 break N 和 continue N 语句, 可跳出 N 层循环
- 支持 != 运算符, 作用和 ~= 相同
- string.format 中的 %s 支持输入非字符串变量, 此时将自动获取其 tostring 元方法的结果
- (Win32平台) string 表增加 mb2wc 及 wc2mb 两个函数, 可用于字符串的字符集转换
- 增加 buffer 扩展库, 用于操作可修改的字节数组, 以弥补 string 类型的某些不足


开发者

- dwing, dwing(at)163(dot)com


参考

- Lua 官方版本: http://lua.org
- 部分修改参考自: http://lua-users.org/wiki/LuaPowerPatches
