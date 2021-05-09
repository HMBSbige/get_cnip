# 获取国内IP、域名

不想自己生成的可以直接下载
* 路由表文件：https://github.com/HMBSbige/Text_Translation/tree/master/chnroute
* ShadowsocksR 文件：https://github.com/HMBSbige/Text_Translation/tree/master/ShadowsocksR
* 国内域名表：https://raw.githubusercontent.com/HMBSbige/Text_Translation/master/chndomains.txt
* ShadowsocksR 安卓客户端用 ACL 文件：https://raw.githubusercontent.com/HMBSbige/Text_Translation/master/ShadowsocksR/whitelist.acl

## 输出文件用处
__chn_ip.txt__：

就是国内 CIDR v4 + v6

直接放在 ShadowsocksR 的根目录，代理规则选择 "绕过局域网和大陆" 或 "绕过局域网和非大陆"

__add.txt__：

使用以下bat添加路由表，与[cmroute.dll](https://github.com/HMBSbige/getcnIP/releases/download/1.0/cmroute.dll)同目录
```
@echo off

net session >nul 2>&1
if not %errorlevel% == 0 (
  echo 请使用右键 "已管理员身份运行" 此脚本
  pause
  exit 1
)

cd /d %~dp0
echo 导入路由表...
rundll32.exe cmroute.dll,SetRoutes /STATIC_FILE_NAME add.txt /DONT_REQUIRE_URL /IPHLPAPI_ACCESS_DENIED_OK
pause
```
__del.txt__：

使用以下bat删除路由表，与[cmroute.dll](https://github.com/HMBSbige/getcnIP/releases/download/1.0/cmroute.dll)同目录
```
@echo off

net session >nul 2>&1
if not %errorlevel% == 0 (
  echo 请使用右键 "已管理员身份运行" 此脚本
  pause
  exit 1
)

cd /d %~dp0
echo 移除路由表...
rundll32.exe cmroute.dll,SetRoutes /STATIC_FILE_NAME del.txt /DONT_REQUIRE_URL /IPHLPAPI_ACCESS_DENIED_OK
pause
```
注：重启系统后自动清空路由表

__chndomains.txt__：

国内域名列表

__ss_cnall.pac__：

ShadowsocksR 目录内的 pac.txt。

客户端设置为"PAC 模式"、代理规则选择"全局"，可绕过国内域名和IP，只代理国外的。

__ss_cnip.pac__：

ShadowsocksR 目录内的 pac.txt。

客户端设置为"PAC 模式"、代理规则选择"全局"，可绕过国内IP。

__ss_white.pac__：

ShadowsocksR 目录内的 pac.txt。

可绕过国内域名。

__ss_white_r.pac__：

ShadowsocksR 目录内的 pac.txt。

仅通过国内域名（国外访问大陆）。

__whitelist.acl__

ShadowsocksR 安卓客户端用 ACL 文件

可绕过国内域名和IP，只代理国外的。

## 关于cmroute.dll
我能找到的文档：https://technet.microsoft.com/zh-cn/library/dd672665(v=ws.10).aspx

但是文档实在太老，很多都不适用。而且不知道为什么win10提取的cmroute.dll不能用...

简单的来说，

* 基本格式：

> 命令 网络目标 MASK 网络掩码 网关 METRIC 跃点数 IF 接口

* 命令可以是:

	* ADD
	* DELETE
	* REMOVE_GATEWAY


* 网关 跃点数 接口一般都默认（default）

* 用`route print` 命令查看 __接口列表__

* METRIC __不可__ 省略，且不知道为什么跃点数只能是default，填数字无效。
