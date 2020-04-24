# 获取国内IP、域名

不想自己生成的可以直接下载
* 路由表文件：https://github.com/HMBSbige/Text_Translation/tree/master/chnroute
* ShadowsocksR 文件：https://github.com/HMBSbige/Text_Translation/tree/master/ShadowsocksR
* 国内域名表：https://raw.githubusercontent.com/HMBSbige/Text_Translation/master/chndomains.txt
* ShadowsocksR 安卓客户端用 ACL 文件：https://raw.githubusercontent.com/HMBSbige/Text_Translation/master/ShadowsocksR/whitelist.acl

## .Net Core 版本

### 所需文件：

* [delegated-apnic-latest](https://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest) 
* (可选) [china_ip_list.txt](https://raw.githubusercontent.com/17mon/china_ip_list/master/china_ip_list.txt)
* [accelerated-domains.china.conf](https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/accelerated-domains.china.conf)
* (可选) [apple.china.conf](https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/apple.china.conf)
* (可选) [google.china.conf](https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/google.china.conf)
* (可选) 自定义域名白名单 whitelist.txt

### 模式
#### 本地
`--local` 运行

加载根目录下的文件，并生成

#### 网络
从网络下载并加载根目录下的 `whitelist.txt`，并生成


### 输出文件
输出在 ```output``` 目录下

### 生成所需平台程序
```
dotnet publish -c release -r $RID
```
例如：
```
dotnet publish -c release -r win-x64
dotnet publish -c release -r linux-x64
dotnet publish -c release -r osx-x64
```
具体RID：https://docs.microsoft.com/zh-cn/dotnet/core/rid-catalog


## 输出文件用处
__chn_ip.txt__：

直接放进 ShadowsocksR 目录内,代理规则选择"绕过局域网和大陆"

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

国内域名

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

## 自用脚本
```
aria2c -x10 -s10 --https-proxy="https://127.0.0.1:23333" --allow-overwrite=true -Z https://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/accelerated-domains.china.conf
getcnip
pause
```

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
